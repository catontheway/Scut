/****************************************************************************
Copyright (c) 2013-2015 scutgame.com

http://www.scutgame.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
using System;
using System.Data;
using System.Data.SqlClient;
using System.Linq;
using System.Threading;
using MySql.Data.MySqlClient;
using ZyGames.Framework.Common.Configuration;
using ZyGames.Framework.Common.Log;
using ZyGames.Framework.Common.Serialization;
using ZyGames.Framework.Common.Threading;
using ZyGames.Framework.Config;
using ZyGames.Framework.Profile;
using ZyGames.Framework.Redis;
using ZyGames.Framework.Common;
using ServiceStack.Redis;
using System.Collections.Generic;

namespace ZyGames.Framework.Data
{
    /// <summary>
    /// sql消息队列命令管理
    /// </summary>
    public abstract class SqlStatementManager
    {
        private static string SlaveMessageQueue;
        /// <summary>
        /// 同步到数据库的Sql队列, 存储格式List:SqlStatement对象
        /// </summary>
        public static readonly string SqlSyncQueueKey = "__QUEUE_SQL_SYNC";
        /// <summary>
        /// 同步到数据库的Sql出错队列，格式同SqlSyncQueueKey
        /// </summary>
        public static readonly string SqlSyncErrorQueueKey = "__QUEUE_SQL_SYNC_ERROR";
        /// <summary>
        /// 
        /// </summary>
        public static readonly string SqlSyncConnErrorQueueKey = "__QUEUE_SQL_SYNC_CONN_ERROR";
        private static Timer[] _queueWatchTimers;
        //private static SmartThreadPool _threadPools;
        private static int[] _isWatchWorking;
        private const int sqlSyncPackSize = 101;


        static SqlStatementManager()
        {
        }

        private static MessageQueueSection GetSection()
        {
            return ConfigManager.Configger.GetFirstOrAddConfig<MessageQueueSection>();
        }

        /// <summary>
        /// 是否使用异常队列
        /// </summary>
        public static bool IsUseSyncQueue
        {
            get { return _queueWatchTimers != null; }
        }

        /// <summary>
        /// 开启初始化监听处理
        /// </summary>
        public static void Start()
        {
            TraceLog.ReleaseWriteDebug("Sql write queue start init...");
            MessageQueueSection section = GetSection();
            SlaveMessageQueue = section.SlaveMessageQueue;
            if (_queueWatchTimers != null && _queueWatchTimers.Length != section.SqlSyncQueueNum)
            {
                foreach (var timer in _queueWatchTimers)
                {
                    try
                    {
                        timer.Dispose();
                    }
                    catch (Exception ex)
                    {
                        TraceLog.WriteError("Sql write queue stop error:{0}", ex);
                    }
                }
                _queueWatchTimers = null;
            }
            if (_queueWatchTimers == null)
            {
                _isWatchWorking = new int[section.SqlSyncQueueNum];
                _queueWatchTimers = new Timer[_isWatchWorking.Length];
                for (int i = 0; i < _queueWatchTimers.Length; i++)
                {
                    _queueWatchTimers[i] = new Timer(OnCheckSqlSyncQueue, i, 100, 100);
                }
                //_threadPools = new SmartThreadPool(180 * 1000, 100, 5);
                //_threadPools.Start();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="parameters"></param>
        /// <returns></returns>
        internal static SqlParam[] ConvertSqlParam(IDataParameter[] parameters)
        {
            SqlParam[] paramList = new SqlParam[parameters.Length];
            for (int i = 0; i < parameters.Length; i++)
            {
                var p = parameters[i];
                int size = 0;
                int dbType = (int)p.DbType;
                if (p is SqlParameter)
                {
                    size = ((SqlParameter)p).Size;
                    dbType = Convert.ToInt32(((SqlParameter)p).SqlDbType);
                }
                else if (p is MySqlParameter)
                {
                    size = ((MySqlParameter)p).Size;
                    dbType = Convert.ToInt32(((MySqlParameter)p).MySqlDbType);
                }
                paramList[i] = new SqlParam()
                {
                    ParamName = p.ParameterName,
                    DbTypeValue = dbType,
                    Size = size,
                    Value = new ProtoObject(p.Value)
                };
            }
            return paramList;
        }

        private static IDataParameter[] ToSqlParameter(DbBaseProvider dbProvider, SqlParam[] paramList)
        {
            IDataParameter[] list = new IDataParameter[paramList.Length];
            for (int i = 0; i < paramList.Length; i++)
            {
                SqlParam param = paramList[i];
                list[i] = dbProvider.CreateParameter(param.ParamName, param.DbTypeValue, param.Size, param.Value.Value);
            }
            return list;
        }

        /// <summary>
        /// 放到消息队列池中
        /// </summary>
        /// <param name="statement"></param>
        public static bool Put(SqlStatement statement)
        {
            bool result = false;
            try
            {
                if (!IsUseSyncQueue)
                {
                    return false;
                }
                string tableName = statement.Table;
                string key = GetSqlQueueKey(statement.IdentityID);
                byte[] value = ProtoBufUtils.Serialize(statement);
                RedisConnectionPool.Process(client =>
                {
                    client.LPush(key, value);
                    ProfileManager.PostSqlOfMessageQueueTimes(tableName, 1);
                });
                result = true;
            }
            catch (Exception ex)
            {
                TraceLog.WriteError("Sql update queue write error:{0}\r\n{1}", ex, JsonUtils.SerializeCustom(statement));
            }

            return result;
        }

        /// <summary>
        /// put error sql
        /// </summary>
        /// <param name="value"></param>
        /// <param name="key"></param>
        /// <returns></returns>
        private static bool PutError(byte[] value, string key = null)
        {
            bool result = false;
            try
            {
                RedisConnectionPool.Process(client => client.LPush(SlaveMessageQueue + (string.IsNullOrEmpty(key) ? SqlSyncErrorQueueKey : key), value));
                result = true;
            }
            catch (Exception ex)
            {
                TraceLog.WriteError("Sql error queue write error:{0}", ex);
            }
            return result;
        }

        /// <summary>
        /// Sql process queue
        /// </summary>
        /// <param name="identityId"></param>
        /// <returns></returns>
        internal static string GetSqlQueueKey(int identityId)
        {
            int count = _queueWatchTimers?.Length ?? 1;
            int index = Math.Abs(identityId) % count;
            string queueKey = string.Format("{0}{1}{2}",
                SlaveMessageQueue,
                SqlSyncQueueKey,
                count > 1 ? ":" + index : "");
            return queueKey;
        }

        private static void OnCheckSqlSyncQueue(object state)
        {
            int identity = (int)state;
            if (Interlocked.CompareExchange(ref _isWatchWorking[identity], 1, 0) == 0)
            {
                try
                {
                    string queueKey = GetSqlQueueKey(identity);
                    string workingKey = queueKey + "_temp";
                    bool result;
                    byte[][] bufferBytes = new byte[0][];
                    byte[][] valueBytes = new byte[0][];
                    byte[][] scoreBytes = new byte[0][];
                    do
                    {
                        result = false;
                        RedisConnectionPool.ProcessReadOnly(client =>
                        {
                            bool hasWorkingQueue = client.ContainsKey(workingKey);
                            bool hasNewWorkingQueue = client.ContainsKey(queueKey);

                            if (!hasWorkingQueue && !hasNewWorkingQueue)
                            {
                                return;
                            }
                            if (!hasWorkingQueue)
                            {
                                try
                                {
                                    client.Rename(queueKey, workingKey);
                                }
                                catch { }
                            }
                            bufferBytes = GetPackFromQueue(client, workingKey, sqlSyncPackSize);

                            if (bufferBytes.Length > 0)
                            {
                                result = true;
                            }
                        });
                        if (!result)
                        {
                            break;
                        }
                        DoProcessSqlSyncQueue(workingKey, queueKey, bufferBytes);
                    } while (true);
                }
                catch (Exception ex)
                {
                    TraceLog.WriteError("OnCheckSqlSyncQueue error:{0}", ex);
                }
                finally
                {
                    Interlocked.Exchange(ref _isWatchWorking[identity], 0);
                }
            }
        }

        private static byte[][] GetPackFromQueue(RedisClient client, string workingKey, int sqlSyncPackSize)
        {
            var result = new List<byte[]>();
            for (int i = 0; i < sqlSyncPackSize; i++)
            {
                var value = client.RPop(workingKey);
                if (value != null && value.Length > 0)
                {
                    result.Add(value);
                }
                else
                {
                    break;
                }
            }
            return result.ToArray();
        }

        private static void DoProcessSqlSyncQueue(string workingKey, string queueKey, byte[][] bufferBytes)
        {
            try
            {
                bool hasClear = false;
                int index = 0;
                foreach (var buffer in bufferBytes)
                {
                    index++;
                    DbBaseProvider dbProvider = null;
                    SqlStatement statement = null;
                    int result = 0;
                    try
                    {
                        statement = ProtoBufUtils.Deserialize<SqlStatement>(buffer);
                        dbProvider = DbConnectionProvider.CreateDbProvider("", statement.ProviderType,
                            statement.ConnectionString);
                        var paramList = ToSqlParameter(dbProvider, statement.Params);
                        result = dbProvider.ExecuteQuery(statement.CommandType, statement.CommandText, paramList);
                        TraceLog.WriteDebug($"ProcessSqlSyncQueue[{workingKey}]-{queueKey} excute result:{result} sql:{statement.ToString()}");
                    }
                    catch (DbConnectionException connError)
                    {
                        TraceLog.WriteSqlError("SqlSync Error:{0}\r\nSql>>\r\n{1}", connError,
                            statement != null ? statement.ToString() : "");
                        if (dbProvider != null)
                        {
                            //modify error: 40 - Could not open a connection to SQL Server
                            dbProvider.ClearAllPools();

                            //resend
                            var paramList = ToSqlParameter(dbProvider, statement.Params);
                            result = dbProvider.ExecuteQuery(statement.CommandType, statement.CommandText, paramList);
                        }
                        else
                        {
                            PutError(buffer, SqlSyncConnErrorQueueKey);
                        }
                    }
                    catch (Exception e)
                    {
                        TraceLog.WriteSqlError("SqlSync Error:{0}\r\nSql>>\r\n{1}", e,
                            statement != null ? statement.ToString() : "");
                        PutError(buffer);
                        if (!hasClear && dbProvider != null)
                        {
                            //modify error: 40 - Could not open a connection to SQL Server
                            hasClear = true;
                            dbProvider.ClearAllPools();
                        }
                    }
                    finally
                    {
                        if (result > 0)
                        {
                            ProfileManager.ProcessSqlOfMessageQueueTimes(statement != null ? statement.Table : null);
                        }
                        else
                        {
                            ProfileManager.ProcessFailSqlOfMessageQueueTimes(statement != null ? statement.Table : null, 1);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                TraceLog.WriteError("DoProcessSqlSyncQueue error:{0}", ex);
            }
        }

    }
}