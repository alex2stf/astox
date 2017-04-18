/*
 * FileSystem.h
 *
 *  Created on: Jul 30, 2014
 *      Author: alexandru
 */

#ifndef ASTX_THREAD_H_
    #define ASTX_THREAD_H_

    #include "Macros.h"
    #include "Util.h"
    #include <iostream>
    #include <string>

    #ifdef ASTOX_OS_WINDOWS

    #elif  defined ASTOX_OS_MAC
        #include <sys/param.h>
        #include <sys/sysctl.h>
    #else
        #include <unistd.h>
        #include <pthread.h>//required by ubuntu
        #include <signal.h>//still ubuntu
    #endif


namespace astox {

    #ifdef ASTOX_OS_WINDOWS
        #define ThreadArgs LPVOID
        #define ThreadCallback DWORD WINAPI
        #define MutexLock HANDLE
        #define MutexId HANDLE *
        #define ThreadId HANDLE
        #define MUTEX_OK WAIT_OBJECT_0
        #define MUTEX_ABANDONED WAIT_ABANDONED
        #define MutexState DWORD
    typedef DWORD(WINAPI * ThreadFunction)(LPVOID lpParameter);
        #define ThreadExitCode __in DWORD
    #else
        #define MutexLock pthread_mutex_t
        #define MutexId pthread_mutex_t *
        #define ThreadId pthread_t
        #define MutexState int
        #define ThreadArgs void*
        #define ThreadCallback void*
    typedef void *(ThreadFunction) (void*);
        #define ThreadExitCode void *
        #define MUTEX_OK 0
        #define MUTEX_ABANDONED 1
    #endif

    class Thread {
    private:
        std::vector <ThreadId> handles;
        static int _numCpu;

    public:
        Thread();
        static void mutexDestroy(MutexId mutex);
        static MutexState mutexUnlock(MutexId mutex);
        static MutexState mutexLock(MutexId mutex);
        static void mutexInit(MutexId ghMutex);
        static void joinThread(ThreadId thread);
        static int getNumCPU();
        static void exitCurrent(ThreadExitCode exitCode);
        static void closeThread(ThreadId id, int statusCode = 0);

        static void detachThread(ThreadId id);

        int createThread(ThreadFunction method, ThreadArgs args = NULL);
        void joinAll();
        void closeAll();

    };



}

#endif /* ASTX_THREAD_H */
