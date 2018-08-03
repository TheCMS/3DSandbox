#include "GUILogic.h"
#include "QtGui"
#include "functional"
#include "QTUI.h"
#include "ThreadsManager.h"
#include "QtConstructs.h"
#include "QDebug"


GUILogic::GUILogic()
{

}

GUILogic::~GUILogic()
{
    QtThreadChecker* p = (QtThreadChecker*) m_pThreadChecker;
    m_pThreadChecker = nullptr;
    delete p;

    QtThreadReceiver* ptr = (QtThreadReceiver*) m_pThreadReceiver;
    m_pThreadReceiver=nullptr;
    delete ptr;


    QTUI* ptr2 = (QTUI*) m_pUI;
    m_pUI = nullptr;
    delete ptr2;

}

void GUILogic::runOnGUIThread(std::function<void()> fct)
{
    // unsafe
    // mutex should be added
    if (m_pThreadChecker!=nullptr)
    {
        QtThreadChecker* p = (QtThreadChecker*) m_pThreadChecker;

        emit p->runOnOtherThread(fct);

    }

}

void GUILogic::sendGUIMessage(eGUIMessages msg, void *payload)
{
    switch (msg)
    {
        case GM_LOG:
        {

            runOnGUIThread(
                        [&,payload]
                {
                    char* str = (char*) payload;
                    QTUI* ptr = (QTUI*) this->m_pUI;
                    ptr->getHandler()->appendConsoleText(str);
                    free(str);
                }
                );
        }
        break;
        case GM_RENDER_MODE_CHANGED:
        {
            runOnGUIThread ([&, payload]
            {
                char* str = (char*) payload;
                QTUI* ptr = (QTUI*) this->m_pUI;
                ptr->getHandler()->updateRenderModeText(str);
                free(str);
            }
            );
        }
        break;
        case GM_CULL_MODE_CHANGED:
        {
            runOnGUIThread([&,payload]
            {
                char* str =(char*) payload;
                QTUI* ptr = (QTUI*) this->m_pUI;
                ptr->getHandler()->updateCullModeText(str);
                free(str);
            }
                        );
        }
        break;
        default:
        break;
    }

}

void GUILogic::startGUI()
{
    // printf("starg gui thread is %d\n",QThread::currentThreadId());
    qDebug()<<" start gui from "<<QThread::currentThreadId();
    QtThreadChecker* pChecker = new QtThreadChecker();
    m_pThreadChecker = pChecker;
    // GUI
// #define SEPARATE_GUI_THREAD
#ifdef SEPARATE_GUI_THREAD
    eThreadError err = ThreadsManager::Instance().runOnThread(
                TID_UI,[&,pChecker,this]()
                {

                    QTUI* ui = new QTUI(0,nullptr);
                    m_pUI=ui;

                    QtThreadReceiver* pReceiver = new QtThreadReceiver();

                    pChecker->setReceiver(pReceiver);
                    pChecker->linkObjects();
                    this->m_pThreadReceiver = pReceiver;
                    // ui->start(pChecker,pReceiver);
                    ui->start();

                }
    );

    (void)err;
#else

    QTUI* ui = new QTUI(0,nullptr);
    m_pUI=ui;
    QtThreadReceiver* pReceiver = new QtThreadReceiver();

    pChecker->setReceiver(pReceiver);
    pChecker->linkObjects();
    this->m_pThreadReceiver = pReceiver;
    ui->start();

#endif


}
void GUILogic::closeGUI()
{
    runOnGUIThread([this]{
        QTUI* ptr =(QTUI*) m_pUI;
        ptr->stop();
    });
}
