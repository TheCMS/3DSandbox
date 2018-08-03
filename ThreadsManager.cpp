#include "ThreadsManager.h"
#include "thread_impl.h"
#include "stdio.h"

eThreadError ThreadsManager::startThreads()
{

    createPrecachePool(TID_TOTAL);
    for (unsigned int i=0;i<TID_TOTAL;i++)
    {
        thread_holder_t* ptr = getObjectForId(i);
        if (ptr==NULL)
        {
            return TERR_FAILED;
        }
        ptr->tID=i;
        ptr->m_bRunning=false;

        ptr->ciclicityMs = 50;
    }
    applyThreadsPolicies();
    m_bThreadsStarted=true;
    return TERR_SUCCESS;
}

eThreadError ThreadsManager::runOnThread(eThreadIDs threadId, threadFct_t fct)
{
    if (threadId>=0 && threadId< TID_TOTAL)
    {
        unsigned int tid = (unsigned int)threadId;
        thread_holder_t* ptr = getObjectForId(tid);
        if (ptr==NULL)
        {
            return TERR_FAILED;
        }
        if(!ptr->m_bRunning)
        {
            return TERR_THREAD_NOT_STARTED;
        }
        ptr->enqueue(fct);
        return TERR_SUCCESS;

    } else {
        return TERR_INVALID_TID;
    }
    return TERR_SUCCESS;
}

eThreadError ThreadsManager::postOnThread(eThreadIDs threadId, threadFct_t fct)
{
    if (threadId>=0 && threadId< TID_TOTAL)
    {
        unsigned int tid = (unsigned int)threadId;
        thread_holder_t* ptr = getObjectForId(tid);
        if (ptr==NULL)
        {
            return TERR_FAILED;
        }
        if(!ptr->m_bRunning)
        {
            return TERR_THREAD_NOT_STARTED;
        }
        ptr->post(fct);
        return TERR_SUCCESS;

    } else {
        return TERR_INVALID_TID;
    }
    return TERR_SUCCESS;
}

void ThreadsManager::createDefaultPolicy()
{
    m_bPolicyPopulated=true;
    for (unsigned int i=0;i<TID_TOTAL;i++)
    {
        std::pair<unsigned int, bool> pair = { 500, false};
        m_vThreadsPolicies.push_back(pair);
    }
}

eThreadError ThreadsManager::setThreadCiclicty(eThreadIDs threadId, unsigned int ms)
{
    if (threadId>=0 && threadId< TID_TOTAL)
    {
        unsigned int idx = (unsigned int) threadId;
        m_vThreadsPolicies[idx].first=ms;

    } else {
        return TERR_INVALID_TID;
    }
    return TERR_SUCCESS;
}

eThreadError ThreadsManager::enableThread(eThreadIDs threadId, bool bEnable )
{
    if (threadId>=0 && threadId< TID_TOTAL)
    {
        unsigned int idx = (unsigned int) threadId;
        m_vThreadsPolicies[idx].second=bEnable;

    } else {
        return TERR_INVALID_TID;
    }
    return TERR_SUCCESS;
}

void ThreadsManager::applyThreadsPolicies()
{
    for (unsigned int i=0;i<m_vThreadsPolicies.size();i++)
    {
        thread_holder_t* ptr = getObjectForId(i);
        ptr->setCiclicityMs(m_vThreadsPolicies[i].first);
        if (m_vThreadsPolicies[i].second)
        {
            ptr->start();
        }
    }

}

void ThreadsManager::stopThreads()
{

    m_bThreadsStarted=false;
    for (unsigned int i=0;i<TID_TOTAL;i++)
    {
        thread_holder_t* ptr = getObjectForId(i);
        ptr->kill();
    }
    destroyAll();
}

ThreadsManager& ThreadsManager::Instance()
{
    return GetInstance();
}

ThreadsManager::ThreadsManager():
    m_bThreadsStarted(false),
    m_bPolicyPopulated(false)
{
    createDefaultPolicy();
}

ThreadsManager::~ThreadsManager()
{

}



