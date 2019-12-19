using FrameworkUnitTest.Cache;
using FrameworkUnitTest.Cache.Model;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using ZyGames.Framework.Cache.Generic;
using ZyGames.Framework.Common.Configuration;
using ZyGames.Framework.Common.Serialization;
using ZyGames.Framework.Data;
using ZyGames.Framework.Model;
using ZyGames.Framework.Redis;

namespace FrameworkUnitTest.Data
{
    [TestClass]
    public class DbProviderTest
    {
        [TestInitialize]
        public void Init()
        {
            var ser = new JsonCacheSerializer(Encoding.UTF8);
            ConfigManager.GetConfigger<MyDataConfigger>();
            RedisConnectionPool.Initialize(ser);
            DbConnectionProvider.Initialize();
            EntitySchemaSet.InitSchema(typeof(LoginLog), false, MyDataConfigger.DbKeys);
            EntitySchemaSet.LoadAssembly(typeof(SingleData).Assembly);
            var setting = new CacheSetting();
            CacheFactory.Initialize(setting, ser);
            EntitySchemaSet.StartCheckTableTimer();

        }

        [TestMethod]
        public void SendMutilDb()
        {
            var dbProvider = DbConnectionProvider.CreateDbProvider(MyDataConfigger.DbKey);
            var data = new DataTable();
            data.Columns.Add("id", typeof(long));
            data.Columns.Add("name", typeof(string));
            data.Columns.Add("sex", typeof(bool));
            data.Columns.Add("money", typeof(decimal));
            data.Columns.Add("create_time", typeof(DateTime));
            for (int i = 0; i < 10; i++)
            {
                var dr = data.NewRow();
                dr["id"] = i + 1;
                dr["name"] = "晨test_" + i;
                dr["sex"] = 1;
                dr["money"] = 45.23;
                dr["create_time"] = DateTime.Now;
                data.Rows.Add(dr);
            }
            DbBulkOption option = new DbBulkOption()
            {
            };
            var watch = Stopwatch.StartNew();
            var result = dbProvider.ExecuteBulkQuery("bulk_temp", data, option);
            Trace.WriteLine("result:" + result);
            WaitEnd(watch);
        }


        [TestMethod]
        public void SendToDb()
        {
            var watch = Stopwatch.StartNew();
            var r = new Random();
            var keys = new List<string>(MyDataConfigger.DbKeys);
            keys.Add(MyDataConfigger.DbKey);
            foreach (var key in keys)
            {
                var log = new LoginLog();
                log.UserId = (int)(r.NextDouble() * 10000);
                log.NickName = "cocos" + r.NextDouble();
                log.RoleLv = 1;
                log.RetailId = "000";
                log.CreateTime = DateTime.Now;

                DataSyncQueueManager.SendToDb(key, log);
            }
            WaitEnd(watch);
        }

        [TestMethod]
        public void SendToDb2()
        {
            var watch = Stopwatch.StartNew();
            var r = new Random();
            var keys = new List<string>(MyDataConfigger.DbKeys);
            for (int i = 0; i < 5; i++)
            {
                var data = new Alliance();
                data.AId = (int)(10000 + i);
                data.Members = (int)(r.NextDouble() * 1000);
                DataSyncQueueManager.SendToDb(MyDataConfigger.DbKey, data);
                data.Members = 1;
                data.TempTimeModify = DateTime.Now;
                DataSyncQueueManager.SendToDb(MyDataConfigger.DbKey, data);

            }
            WaitEnd(watch);
        }

        private void WaitEnd(Stopwatch watch, int ms = 2000)
        {
            Trace.WriteLine("Running time:" + watch.ElapsedMilliseconds + "ms.");
            do
            {
                Thread.Sleep(ms);
            } while (!DataSyncQueueManager.IsRunCompleted);
            //} while (true);
        }

    }
}
