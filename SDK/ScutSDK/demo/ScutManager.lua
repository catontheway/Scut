-------------------------------------------------------------------
--�ļ���ScutManager.lua
--���ߣ�wangsheng
--���ڣ�2015��6��30��
--�������������ӿڹ�����
-------------------------------------------------------------------
require "NetHelper"

function OnHandleData(pScene, nTag, nNetRet, lpData, lpExternal)
    ScutManager:execCallback(nTag, nNetRet, lpData,lpExternal)
end

ScutManager = class("ScutManager")

local loadingCache = {}--loading����
local callbackCache = {}--�ص�����
local callTag = 0--���õ�id����
ScutManager.url = "192.168.2.98:9001"--��������ַ

--��ʼ��
function ScutManager:init()
    ScutDataLogic.CNetWriter:setUrl(ScutManager.url)
    
    cc.Director:getInstance():getScheduler():scheduleScriptFunc(function()
        ScutDataLogic.CDataRequest:Instance():PeekLUAData()
    end, 0, false)
end

--ִ������ص�������
function ScutManager:execCallback(nTag, nNetState, lpData,lpExternal)
    local actionID = ZyReader:getActionID()
    local lpExternalData = lpExternalData or 0
    local userData = ZyRequestParam:getParamData(lpData)
    if 2 == nNetState then
        local reader = ScutDataLogic.CDataRequest:Instance()
        local bValue = reader:LuaHandlePushDataWithInt(lpData)
        if callbackCache[nTag] then
            callbackCache[nTag]()
            callbackCache[nTag]=nil
        end
        self:netDecodeEnd(nTag)
    end
end
--���ýӿ�
--callBack�ص�����
--url��ַ
--lpDataΪ�û��������ݣ������ݽ���ֱ�Ӵ��ء�
function ScutManager:doHandler(callBack,url,lpData)
    ZyExecRequest(cc.Director:getInstance():getRunningScene(),lpData,url or ScutManager.url)
    if callBack then
        callbackCache[ZyRequestTag] = callBack
    end
    --�������
--    local loading = ClassManager.createInstance("LoadingLayer")
--    loadingCache[ZyRequestTag] = loading
--    UIManager.push(loading)
end
--ִ�����
function ScutManager:netDecodeEnd(nTag)
    --������ϣ�����loading �Ƴ�loading
--    local loading = loadingCache.nTag
--    if loading then
--    	UIManager.pop(loading:getId())
--    	loadingCache.nTag = nil
--    end
end

ScutManager:init()