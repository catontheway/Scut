--ZyReader.recordBegin()���ṹ���У���ʼ������������
--ZyReader.recordEnd()���ṹ��պϣ�����������������
--ZyReader.getBYTE()������unsigned char��byte��1�ֽڣ�������
--ZyReader.getWORD()������unsigned short��WORD��2�ֽڣ�������
--ZyReader.getDWORD()������unsigned int��DWORD��4�ֽڣ�������
--ZyReader.getFloat()������float������
--ZyReader.getDouble()������double������
--ZyReader.readString()������String������
--ZyReader.readInt64()������Int64������
--ZyReader.getInt()������Int32������
--ZyReader.readErrorMsg()������������Ϣ���ô�����Ϣ��Ҫ������Լ�����������������쳣������ҵ���߼����ִ���ʱ�򣬴�ӡ����Ϣ��
--ZyReader.getResult()����ȡ��ǰ��������״̬������˷��ش����룬�ô�������Ҫ��ͻ���Լ����
--ZyReader.getActionID ()����ȡ��ǰ�����ActionID��

ZyRequestParam = {param = {}}
function ZyRequestParam:getParamData(nTag)
    return ZyRequestParam.param[nTag]
end
ZyRequestTag = 0
ZyReader = ScutDataLogic.CNetReader:getInstance()
ZyWriter = ScutDataLogic.CNetWriter:getInstance()

eScutNetSuccess = 0
eScutNetError = 10000

function ZyReader.readString()
    local nLen = ZyReader:getInt()
    local strRet = nil
    if nLen ~= 0
    then
        local str = ScutDataLogic.CLuaString:new("")
        ZyReader:getString(str, nLen)
        strRet = string.format("%s", str:getCString())
        str:delete()
    end
    return strRet
end
function ZyReader:readInt64()
    return ScutDataLogic.CInt64:new_local(ZyReader:getCInt64())
end

function ZyReader.readErrorMsg()
    return string.format("%s", ZyReader:getErrMsg():getCString())
end

--����pScutScene Ϊ����ӿڷ����scene���󣬴˶���ΪCCScene���͡�
--����lpDataΪ�û��������ݣ������ݽ���ֱ�Ӵ��ء�
function ZyExecRequest(pScutScene, lpData, addressPath)
    ZyRequestTag = ZyRequestTag + 1
    if lpData then
        table.insert(ZyRequestParam.param, ZyRequestTag, lpData)
    end
    if addressPath == nil  then
        ScutDataLogic.CDataRequest:Instance():AsyncExecRequest(pScutScene, ZyWriter:generatePostData(), ZyRequestTag, nil)
    else
        local lenth=string.len(ZyWriter:generatePostData())
        ScutDataLogic.CDataRequest:Instance():AsyncExecTcpRequest(pScutScene,addressPath, ZyRequestTag, nil, ZyWriter:generatePostData(), lenth)
        ScutDataLogic.CNetWriter:resetData()
    end
    ScutDataLogic.CNetWriter:resetData()
end

