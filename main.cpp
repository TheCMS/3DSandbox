
#include <iostream>
#include <WindowsHandler.h>

#include "GUILogic.h"
#include "ThreadsManager.h"
#include "ThreadsManagerTests.h"

#include <windows.h>
#include <functional>

#include "AppLogic.h"
#include "OGLLogic.h"
#include "Utils.h"

#include <unistd.h>

using namespace std;


WindowsHandler* g_pHnd=nullptr;
bool g_bExit=false;


int main(int argc, char *argv[])
{




    // QTUI* ui =new QTUI(argc,argv);

    // TestThreadsManager();

    ThreadsManager::Instance().enableThread(TID_OGL,true);
    ThreadsManager::Instance().enableThread(TID_UI,true);
    ThreadsManager::Instance().enableThread(TID_LOGIC,true);

    ThreadsManager::Instance().setThreadCiclicty(TID_OGL,16); // 60 fps
    ThreadsManager::Instance().setThreadCiclicty(TID_UI,16); // 60 fps
    ThreadsManager::Instance().setThreadCiclicty(TID_LOGIC,50); // 20 fps
    ThreadsManager::Instance().startThreads();




    // Open GL Context
    std::function<void()> oglFct = [&] {

        if (g_pHnd!=NULL)
        {
            g_pHnd->manualWndLoop();
            ThreadsManager::Instance().postOnThread(TID_OGL,oglFct);
        }

    };

    ThreadsManager::Instance().runOnThread(TID_OGL,
    [&]
    {

        g_pHnd = new WindowsHandler("OpenGL Sandbox",800,600,300);
        g_pHnd->setRenderCb(OGLLogic::RenderLoop);
        g_pHnd->createWindow();
        ThreadsManager::Instance().postOnThread(TID_OGL,oglFct);

    });



    // Logic

    AppLogic::GetInstance().setAppExitPtr(&g_bExit);

    std::function<void()> logicFct = [&] {

        AppLogic::GetInstance().updateAppLogic();
        ThreadsManager::Instance().postOnThread(TID_LOGIC,logicFct);
    };


    GUILogic::GetInstance().startGUI();

    // printf("loop ended!");
    GUILogic::GetInstance().closeGUI();
    OGLLogic::GetInstance().runOnOGLThread(
                []
    {
        printf("closing window !\n");
        g_pHnd->closeWindow();
    });

    WindowsHandler* ptr = g_pHnd;
    g_pHnd=NULL;
    g_pHnd->destroyWindow();


    ThreadsManager::Instance().stopThreads();

//     g_pHnd->closeWindow();
//     delete g_pHnd;

   return 0;
}
