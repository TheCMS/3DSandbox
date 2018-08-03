#ifndef THREADSMANAGER_H
#define THREADSMANAGER_H

#include "AppManagerTypes.h"
#include "thread_impl.h"
#include <functional>



enum eThreadIDs
{
    TID_UI=0,
    TID_OGL,
    TID_LOGIC,
    TID_LOADER,
    TID_TOTAL
};

enum eThreadError
{
    TERR_SUCCESS=0,
    TERR_INVALID_TID,
    TERR_THREAD_NOT_STARTED,
    TERR_FAILED
};

class ThreadsManagerTest : public TObjectManager<ThreadsManagerTest, thread_holder_t>
{
friend class TSingleton<ThreadsManagerTest>;
protected:
    ThreadsManagerTest(){}
    ~ThreadsManagerTest(){}
};

class ThreadsManager : private TObjectManager<ThreadsManager,thread_holder_t>
{
friend class TSingleton<ThreadsManager>;

public:

    eThreadError startThreads();
    eThreadError runOnThread(eThreadIDs threadId, threadFct_t fct); // runs now
    eThreadError postOnThread(eThreadIDs threadId, threadFct_t fct);

    eThreadError setThreadCiclicty(eThreadIDs threadId, unsigned int ms);
    eThreadError enableThread(eThreadIDs threadId, bool bEnable );

    void stopThreads();

    static ThreadsManager& Instance();

private:

    std::vector<std::pair<unsigned int, bool> > m_vThreadsPolicies;
    void applyThreadsPolicies();
    void createDefaultPolicy();
    bool m_bThreadsStarted;
    bool m_bPolicyPopulated;
    ThreadsManager();
    ~ThreadsManager();

};




#endif // THREADSMANAGER_H
