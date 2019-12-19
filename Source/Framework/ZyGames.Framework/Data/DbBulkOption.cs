using MySql.Data.MySqlClient;
using System;
using System.Collections.Generic;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZyGames.Framework.Data
{
    /// <summary>
    /// 批量插入配置项
    /// </summary>
    public class DbBulkOption
    {
        /// <summary>
        /// 分批次提交大小
        /// </summary>
        public int? BashSize { get; set; }

        /// <summary>
        /// 超时时间
        /// </summary>
        public int? Timeout { get; set; }

        /// <summary>
        /// Sql复制选项
        /// </summary>
        public SqlBulkCopyOptions CopyOptions { get; set; } = SqlBulkCopyOptions.Default;

        /// <summary>
        /// MySql临时文件
        /// </summary>
        public string TempPath { get; set; }

        /// <summary>
        /// 日期格式
        /// </summary>
        public string DateTimeFormat { get; set; } = "yyyy-MM-dd HH:mm:ss";

        /// <summary>
        /// 列值处理
        /// </summary>
        public Action<StreamWriter, string, Type, object> ColumnValueFormat { get; set; }

        /// <summary>
        /// 跳过行数
        /// </summary>
        public int? NumberOfLinesToSkip { get; set; }
        /// <summary>
        /// 单行列分隔符, ','
        /// </summary>
        public string FieldTerminator { get; set; } = ",";

        /// <summary>
        /// 多行换行符, /r/n
        /// </summary>
        public string LineTerminator { get; set; } = Environment.NewLine;

        /// <summary>
        /// 行的前缀
        /// </summary>
        public string LinePrefix { get; set; }

        /// <summary>
        /// MySql安全符
        /// </summary>
        public char? FieldQuotationCharacter { get; set; } = '"';

        /// <summary>
        /// 空字符
        /// </summary>
        public char? EscapeCharacter { get; set; } = '"';

        /// <summary>
        /// MySql复制选项
        /// </summary>
        public MySqlBulkLoaderConflictOption? ConflictOption { get; set; }

        /// <summary>
        /// 列名转换
        /// </summary>
        public IDictionary<string, string> ColumnsMapping { get; set; }

        /// <summary>
        /// Sql执行结果
        /// </summary>
        public event Action<long> RowsCopied;

        internal void OnRowsCopied(long rows)
        {
            RowsCopied?.Invoke(rows);
        }

        internal string GetColumnMapping(string column)
        {
            string value;
            if (ColumnsMapping != null && ColumnsMapping.TryGetValue(column, out value))
            {
                return value;
            }
            return column;
        }
    }
}
