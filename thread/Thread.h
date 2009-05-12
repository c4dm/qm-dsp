/* -*- c-basic-offset: 4 indent-tabs-mode: nil -*-  vi:set ts=8 sts=4 sw=4: */

/*
    QM DSP Library

    Centre for Digital Music, Queen Mary, University of London.
    This file copyright Chris Cannam, used with permission.
*/

#ifndef _THREAD_H_
#define _THREAD_H_

#ifdef _WIN32
#include <windows.h>
#else /* !_WIN32 */
#ifdef USE_PTHREADS
#include <pthread.h>
#endif /* USE_PTHREADS */
#endif /* !_WIN32 */

#include <string>

//#define DEBUG_THREAD 1
//#define DEBUG_MUTEX 1
//#define DEBUG_CONDITION 1

class Thread
{
public:
#ifdef _WIN32
    typedef HANDLE Id;
#else
#ifdef USE_PTHREADS
    typedef pthread_t Id;
#endif
#endif

    Thread();
    virtual ~Thread();

    Id id();

    void start();
    void wait();

    static bool threadingAvailable();

protected:
    virtual void run() = 0;

private:
#ifdef _WIN32
    HANDLE m_id;
    bool m_extant;
    static DWORD WINAPI staticRun(LPVOID lpParam);
#else
#ifdef USE_PTHREADS
    pthread_t m_id;
    bool m_extant;
    static void *staticRun(void *);
#endif
#endif
};

class Mutex
{
public:
    Mutex();
    ~Mutex();

    void lock();
    void unlock();
    bool trylock();

private:
#ifdef _WIN32
    HANDLE m_mutex;
#ifndef NO_THREAD_CHECKS
    DWORD m_lockedBy;
#endif
#else
#ifdef USE_PTHREADS
    pthread_mutex_t m_mutex;
#ifndef NO_THREAD_CHECKS
    pthread_t m_lockedBy;
    bool m_locked;
#endif
#endif
#endif
};

class MutexLocker
{
public:
    MutexLocker(Mutex *);
    ~MutexLocker();

private:
    Mutex *m_mutex;
};

class Condition
{
public:
    Condition(std::string name);
    ~Condition();

    //!!! NO -- reproducing more conventional lock/wait
    
    // To wait on a condition, either simply call wait(), or call
    // lock() and then wait() (perhaps testing some state in between).
    // To signal a condition, call signal().

    // Although any thread may signal on a given condition, only one
    // thread should ever wait on any given condition object --
    // otherwise there will be a race conditions in the logic that
    // avoids the thread code having to track whether the condition's
    // mutex is locked or not.  If that is your requirement, this
    // Condition wrapper is not for you.
    void lock();
    void unlock();
    void wait(int us = 0);

    void signal();
    
private:

#ifdef _WIN32
    HANDLE m_mutex;
    HANDLE m_condition;
    bool m_locked;
#else
#ifdef USE_PTHREADS
    pthread_mutex_t m_mutex;
    pthread_cond_t m_condition;
    bool m_locked;
#endif
#endif
#ifdef DEBUG_CONDITION
    std::string m_name;
#endif
};

#endif
