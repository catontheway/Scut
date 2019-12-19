using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ProtoBuf;
using ZyGames.Framework.Model;

namespace FrameworkUnitTest.Cache.Model
{
    [Serializable]
    [ProtoContract]
    [EntityTable(CacheType.Entity, MyDataConfigger.DbKey, AccessLevel = AccessLevel.ReadWrite)]
    public class Alliance : ShareEntity
    {
        [ProtoMember(1)]
        [EntityField(true)]
        public long AId { get; set; }


        [ProtoMember(3)]
        [EntityField]
        public int Members { get; set; }
        
    }
}
