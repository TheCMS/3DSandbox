#ifndef THREAD_IMPL_H
#define THREAD_IMPL_H

#include <vector>
#include <functional>
#include <mutex>

class ThreadsManager;

typedef std::function<void()> threadFct_t;

struct thread_holder_t
{


    unsigned int tID;
    unsigned int ciclicityMs;
    void* pImpl;

    void enqueue(threadFct_t);
    void post(threadFct_t);
    void setCiclicityMs(unsigned int ms);
    void stop(bool bBlocking);

    std::vector<threadFct_t> m_vQueue;
    std::vector<threadFct_t> m_vPostQueue;

    bool m_bRunning;

    void start();
    void kill();

    std::mutex opLock;


};

#endif // THREAD_IMPL_H
