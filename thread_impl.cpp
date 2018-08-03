#include "thread_impl.h"
#include "Utils.h"

#ifdef WIN32
#include <windows.h>
#endif


#ifdef WIN32


struct win_thread_t
{
    thread_holder_t* pParent;

    HANDLE  hMutex;
    DWORD   dwThreadId;
    HANDLE  hThread;

};

DWORD WINAPI ThreadEntry(LPVOID lpParam)
{


    if (lpParam==nullptr)
    {
        // weridly bad
        return 1;
    }
    win_thread_t& trInfo (*(win_thread_t*)lpParam);

    thread_holder_t& parent(*trInfo.pParent);
    DWORD dWaitResult;

    long long initialTime;
    long long delta;

    while (parent.m_bRunning)
    {
        initialTime = getCurrentTimeMillis();
        if (parent.m_vQueue.size()>0)
        {
            // aquire mutex



            switch (dWaitResult)
            {
                case WAIT_OBJECT_0:
                    for (unsigned int i=0;i<parent.m_vQueue.size();i++)
                    {
                        dWaitResult = WaitForSingleObject(trInfo.hMutex,INFINITE);
                        std::function<void()>&f (parent.m_vQueue[i]);

                        if (!ReleaseMutex(trInfo.hMutex))
                        {
                            // something rly rly bad happend

                        } else {

                        }
                        f();
                    }

                    parent.m_vQueue.clear();

                break;
                default:

                break;
            }
        }


        parent.opLock.lock();
        for (int i=0;i<parent.m_vPostQueue.size();i++)
        {
            parent.m_vQueue.push_back(parent.m_vPostQueue[i]);
        }
        parent.m_vPostQueue.clear();

        parent.opLock.unlock();

        // FPS limiter
        delta =parent.ciclicityMs - ( getCurrentTimeMillis() - initialTime );

        if (delta >0 )
        {
            Sleep(delta);
        }

    }

    return 0;
}

#endif

void thread_holder_t::post(threadFct_t fct)
{
    opLock.lock();

    m_vPostQueue.push_back(fct);
    opLock.unlock();
}

void thread_holder_t::enqueue(threadFct_t fct)
{
    post(fct);

//    opLock.lock();
//    if (pImpl==NULL)
//    {
//        // really early access for some reason
//        opLock.unlock();
//        return;
//    }
//#ifdef WIN32
//    win_thread_t* ptr = (win_thread_t*)pImpl;

//    if (tID == 1 )
//        printf("getting mutex in enq\n");
//    DWORD res = WaitForSingleObject(ptr->hMutex,INFINITE);
//    if (tID == 1 )
//        printf("getting mutex in enq done\n");
//    if (res==WAIT_OBJECT_0)
//    {

//        m_vQueue.push_back(fct);
//        if (!ReleaseMutex(ptr->hMutex))
//        {
//            // something really bad happenend
////            printf("bubu in enq !\n");
//        }
//         else {
//            if (tID == 1 )
//                printf("releasing mutex in enq\n");
//        }
//    } else {
//        // omgzor lock
//    }
//#endif
//    opLock.unlock();
}

void thread_holder_t::setCiclicityMs(unsigned int ms)
{
    ciclicityMs= ms;
}

// waiting
void thread_holder_t::stop(bool bBlocking)
{
    m_bRunning=false;
#ifdef WIN32
    if (bBlocking)
    {
        opLock.lock();
        win_thread_t* p = (win_thread_t*) pImpl;

        WaitForSingleObject(p->hThread,INFINITE);
        ReleaseMutex(p->hMutex);
        opLock.unlock();

    }
#endif
}


void thread_holder_t::start()
{
    m_bRunning=true;

    opLock.lock();
    // create the actual thread
    win_thread_t* p = (win_thread_t*) malloc(sizeof(win_thread_t));
    pImpl=p;
    p->pParent=this;

    p->hMutex = CreateMutex(NULL,FALSE,NULL);

    p->hThread = CreateThread(NULL,0,ThreadEntry,
                              p,0,&p->dwThreadId);

    if (p->hThread==NULL)
    {
        // bad bad bad
    }

    opLock.unlock();
}

void thread_holder_t::kill()
{
#ifdef WIN32
    win_thread_t* p = (win_thread_t*) pImpl;
    stop(true);

    opLock.lock();
    CloseHandle(p->hMutex);
    CloseHandle(p->hThread);
    free(p);
    pImpl=0;
    opLock.unlock();
#endif

}
