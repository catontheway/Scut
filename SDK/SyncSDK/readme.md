SyncSDK
=================

SyncSDK ʹ��
----------------
<ul>
<li>����SyncSDKĿ¼����Ϸ��Ŀ��Client/luaĿ¼��
<li>��luaĿ¼�µ�mainapp.lua�ļ����޸�<br />
    a) ����ScutDataSync.lua�ļ���require("SyncSDK.ScutDataSync")<br />
    b) ��table����strSubDirs����"SyncSDK"����Ŀ¼<br />
    c) ע�������push�ص���CCDirector:sharedDirector():RegisterSocketPushHandler("ScutDataSync.PushReceiverCallback")<br />
    d) �ļ���β����ע��Sync֪ͨ�����ص������磺ScutDataSync.registerSceneCallback(ScutEntityListener.NotifySceneLayer)<br />
</ul>