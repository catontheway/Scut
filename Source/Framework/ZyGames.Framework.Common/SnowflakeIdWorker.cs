using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ZyGames.Framework.Common
{
    class Base36Converter
    {
        private static string _charList = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        /// <summary>
        /// The character set for encoding. Defaults to upper-case alphanumerics 0-9, A-Z.
        /// </summary>
        public static string CharList
        {
            get
            {
                return _charList;
            }
            set
            {
                _charList = value;
            }
        }

        public static string Convert(string number, int fromBase, int toBase)
        {
            // var digits = "0123456789abcdefghijklmnopqrstuvwxyz";
            int length = number.Length;
            string result = string.Empty;


            char[] newCharArray = number.ToCharArray();
            int newlen;

            List<int> nibbles = new List<int>();

            foreach (char c in newCharArray)
            {
                nibbles.Add((int)c);
            }

            do
            {
                int value = 0;
                newlen = 0;

                for (int i = 0; i < length; ++i)
                {
                    value = value * fromBase + nibbles[i];

                    if (value >= toBase)
                    {
                        if (newlen == nibbles.Count)
                        {
                            nibbles.Add(0);
                        }

                        nibbles[newlen++] = value / toBase;
                        value %= toBase;
                    }
                    else if (newlen > 0)
                    {
                        if (newlen == nibbles.Count)
                        {
                            nibbles.Add(0);
                        }

                        nibbles[newlen++] = 0;
                    }
                }

                length = newlen;
                result = CharList[value] + result;
            } while (newlen != 0);

            return result;
        }

        public static string FromHex(string hex)
        {
            return Base36Converter.Convert(hex, 16, 36);
        }
        public static string FromGuid(Guid guid)
        {
            return Base36Converter.Convert(guid.ToString("N"), 16, 36);
        }
        public static string FromInt32(long int32)
        {
            return Base36Converter.Convert(int32.ToString(), 10, 36);
        }
        public static string FromInt64(long int64)
        {
            return Base36Converter.Convert(int64.ToString(), 10, 36);
        }
        /// <summary>
        /// Encode the given number into a Base36 string
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static string Encode(long input)
        {
            if (input < 0) throw new ArgumentOutOfRangeException("input", input, "input cannot be negative");

            char[] clistarr = CharList.ToCharArray();
            Stack<char> result = new Stack<char>();

            while (input != 0)
            {
                result.Push(clistarr[input % 36]);
                input /= 36;
            }

            return new string(result.ToArray());
        }

        /// <summary>
        /// Decode the Base36 Encoded string into a number
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        public static long Decode(string input)
        {
            char[] newCharArray = input.ToCharArray();
            Array.Reverse(newCharArray);
            long result = 0;
            int pos = 0;

            foreach (char c in newCharArray)
            {
                result += CharList.IndexOf(c) * (long)Math.Pow(36, pos);
                pos++;
            }

            return result;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    /// <see cref="https://github.com/lun3322/SnowflakeId"/>
    public class SnowflakeIdWorker
    {
        /// <summary>
        /// 开始时间截 (2015-01-01)
        /// </summary>
        private const long Twepoch = 1420041600000L;

        /// <summary>
        /// 机器id所占的位数
        /// </summary>
        private static int WorkerIdBits => 5;

        /// <summary>
        /// 数据标识id所占的位数
        /// </summary>
        private static int DatacenterIdBits => 5;

        /// <summary>
        /// 支持的最大机器id，结果是31 (这个移位算法可以很快的计算出几位二进制数所能表示的最大十进制数)
        /// </summary>
        private readonly long _maxWorkerId = -1L ^ (-1L << WorkerIdBits);

        /// <summary>
        /// 支持的最大数据标识id，结果是31
        /// </summary>
        private readonly long _maxDatacenterId = -1L ^ (-1L << DatacenterIdBits);

        /// <summary>
        /// 序列在id中占的位数
        /// </summary>
        private static int SequenceBits => 12;

        /// <summary>
        /// 机器ID向左移12位
        /// </summary>
        private readonly int _workerIdShift = SequenceBits;

        /// <summary>
        /// 数据标识id向左移17位(12+5)
        /// </summary>
        private readonly int _datacenterIdShift = SequenceBits + WorkerIdBits;

        /// <summary>
        /// 时间截向左移22位(5+5+12)
        /// </summary>
        private readonly int _timestampLeftShift = SequenceBits + WorkerIdBits + DatacenterIdBits;

        /// <summary>
        /// 生成序列的掩码，这里为4095 (0b111111111111=0xfff=4095)
        /// </summary>
        private readonly long _sequenceMask = -1L ^ (-1L << SequenceBits);

        /// <summary>
        /// 毫秒内序列(0~4095)
        /// </summary>
        private long _sequence;

        /// <summary>
        /// 上次生成ID的时间截
        /// </summary>
        private long _lastTimestamp = -1L;

        private long WorkerId { get; }
        private long DataCenterId { get; }

        private static readonly object LockObj = new object();

        public SnowflakeIdWorker(long workerId, long datacenterId)
        {
            if (workerId > _maxWorkerId || workerId < 0)
            {
                throw new ArgumentException($"worker Id can't be greater than {_maxWorkerId} or less than 0");
            }
            if (datacenterId > _maxDatacenterId || datacenterId < 0)
            {
                throw new ArgumentException($"datacenter Id can't be greater than {_maxDatacenterId} or less than 0");
            }

            WorkerId = workerId;
            DataCenterId = datacenterId;
        }

        /// <summary>
        /// 获得下一个ID (该方法是线程安全的)
        /// </summary>
        /// <returns></returns>
        public string Next(string delimiter)
        {
            lock (LockObj)
            {
                string id = Base36Converter.Encode(GenerateId());
                if (!string.IsNullOrEmpty(delimiter))
                {
                    id = id.Insert(4, delimiter);
                    id = id.Insert(9, delimiter);
                }
                return id;
            }
        }

        /// <summary>
        /// 获得下一个ID (该方法是线程安全的)
        /// </summary>
        /// <returns></returns>
        public long NextId()
        {
            lock (LockObj)
            {
                return GenerateId();
            }
        }

        /// <summary>
        /// 获得下一个ID
        /// </summary>
        /// <returns></returns>
        private long GenerateId()
        {
            var timestamp = TimeGen();

            //如果当前时间小于上一次ID生成的时间戳，说明系统时钟回退过这个时候应当抛出异常
            if (timestamp < _lastTimestamp)
            {
                throw new Exception($"Clock moved backwards.  Refusing to generate id for {_lastTimestamp - timestamp} milliseconds");
            }

            //如果是同一时间生成的，则进行毫秒内序列
            if (_lastTimestamp == timestamp)
            {
                _sequence = (_sequence + 1) & _sequenceMask;
                //毫秒内序列溢出
                if (_sequence == 0)
                {
                    //阻塞到下一个毫秒,获得新的时间戳
                    timestamp = TilNextMillis(_lastTimestamp);
                }
            }
            //时间戳改变，毫秒内序列重置
            else
            {
                _sequence = 0L;
            }

            //上次生成ID的时间截
            _lastTimestamp = timestamp;

            //移位并通过或运算拼到一起组成64位的ID
            return ((timestamp - Twepoch) << _timestampLeftShift)
                   | (DataCenterId << _datacenterIdShift)
                   | (WorkerId << _workerIdShift)
                   | _sequence;
        }

        /// <summary>
        /// 返回以毫秒为单位的当前时间
        /// </summary>
        /// <returns></returns>
        private long TimeGen()
        {
            var ts = DateTime.UtcNow - new DateTime(1970, 1, 1, 0, 0, 0, 0);
            return Convert.ToInt64(ts.TotalMilliseconds);
        }

        /// <summary>
        /// 阻塞到下一个毫秒，直到获得新的时间戳
        /// </summary>
        /// <param name="lastTimes">上次生成ID的时间截</param>
        /// <returns>当前时间戳</returns>
        private long TilNextMillis(long lastTimes)
        {
            var timestamp = TimeGen();
            while (timestamp <= lastTimes)
            {
                timestamp = TimeGen();
            }
            return timestamp;
        }
    }
}
