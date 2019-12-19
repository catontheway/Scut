------------------------------------------------------------------
-- mainapp.lua
-- Author     : LZW
-- Version    : 1.0.0.0
-- Date       : 2014-1-20
-- Description:
------------------------------------------------------------------


local strModuleName = "mainapp"
CCLuaLog("Module ".. strModuleName.. " loaded.")
strModuleName = nil

function PushReceiverCallback(pScutScene, lpExternalData)
    testScene.netCallback(pScutScene, lpExternalData, true)
end

local function ScutMain()
    ------------------------------------------------------------------
    -- ���� ��ʼ���������� ��ʼ ����
    ------------------------------------------------------------------
    
    local strRootDir = ScutDataLogic.CFileHelper:getPath("lua");
    local strTmpPkgPath = package.path;
    
    local strSubDirs =
    {
      
        "lib",
       
        
        -- �ڴ�����µ�Ŀ¼
    };
    
    -- ���������ļ���
    for key, value in ipairs(strSubDirs) do
        local strOld = strTmpPkgPath;
        if(1 == key) then
            strTmpPkgPath = string.format("%s/%s/?.lua%s", strRootDir, value, strOld);
        else
            strTmpPkgPath = string.format("%s/%s/?.lua;%s", strRootDir, value, strOld);
        end
        strOld = nil;
    end
    
    package.path = string.format("%s/?.lua;%s", strRootDir, strTmpPkgPath);
    strTmpPkgPath = nil;

    ------------------------------------------------------------------
    -- ���� ��ʼ���������� ���� ����
    ------------------------------------------------------------------
    
    -- require�����ڻ���������ʼ��֮�󣬱����ļ��Ҳ������������
    require("lib.lib")
    require("lib.ScutScene")
    require("lib.FrameManager")
    require("testScene")
    g_frame_mgr = FrameManager:new()
    g_frame_mgr:init()

    function OnHandleData(pScene, nTag, nNetRet, pData)
        pScene = tolua.cast(pScene, "CCScene")
        g_scenes[pScene]:execCallback(nTag, nNetRet, pData)
    end

    math.randomseed(os.time());
    __NETWORK__=true
    ------------------------------------------------------------------
    -- ���� Э���������ע�� ��ʼ ����
    ------------------------------------------------------------------
    
    function processCommonData(lpScene)
    return true;
    end
    
    function netDecodeEnd(pScutScene, nTag)

    end

    --ע�������push�ص�
   ScutExt:getInstance():RegisterSocketPushHandler("PushReceiverCallback")
   ScutScene:registerNetDecodeEnd("netDecodeEnd");
    ------------------------------------------------------------------
    -- ���� Э���������ע�� ���� ����
    ------------------------------------------------------------------

end

function __G__TRACKBACK__(msg)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(msg) .. "\n")
    print(debug.traceback())
    print("----------------------------------------")
end

local function main()
    ScutMain()
    testScene.init()
end

xpcall(main, __G__TRACKBACK__)