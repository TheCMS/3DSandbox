#include "AppLogic.h"

#include "ThreadsManager.h"


AppLogic::AppLogic():
    appExit(nullptr)
{

}
AppLogic::~AppLogic()
{

}

void AppLogic::setAppExitPtr(bool* exitRef)
{
    appExit = exitRef;
}

void AppLogic::sendLogicMessage(eLogicMessages msg, void* payload)
{
    (void)payload;
    switch (msg)
    {
        case LM_EXIT:
        runOnAppThread([=]{
                if (appExit!=nullptr)
                {
                    *appExit=true;
                }
            });
        break;
        default:
            // invalid message;
        break;
    }
}

void AppLogic::updateAppLogic()
{
    // nothing right now
}
void AppLogic::runOnAppThread(std::function<void()> fct)
{
    ThreadsManager::Instance().runOnThread(TID_LOGIC,fct);
}
