#include "Thread.h"



using namespace std;


namespace astox {
    int Thread::_numCpu = 0;

    Thread::Thread() {

    }

    MutexState Thread::mutexUnlock(MutexId mutex) {
#ifdef ASTOX_OS_WINDOWS
        return ReleaseMutex(*mutex);
#else
        return pthread_mutex_unlock(mutex);
#endif
    }

    MutexState Thread::mutexLock(MutexId mutex) {
#ifdef ASTOX_OS_WINDOWS
        return WaitForSingleObject(*mutex, INFINITE);
#else
        return pthread_mutex_lock(mutex);
#endif
    }

    void Thread::mutexDestroy(MutexId mutex) {
#ifdef ASTOX_OS_WINDOWS
        if (*mutex != NULL) {
            CloseHandle(*mutex);
        }
#else
        pthread_mutex_destroy(mutex);
#endif
    }

    void Thread::mutexInit(MutexId ghMutex) {
#ifdef ASTOX_OS_WINDOWS
        *ghMutex = CreateMutex(NULL, FALSE, NULL);
        if (ghMutex == NULL) {
            printf("CreateMutex error: %d\n", GetLastError());
        }
#else
        if (pthread_mutex_init(ghMutex, NULL) != 0) {
            printf("\n mutex init failed\n");
        }
#endif
    }

    void Thread::exitCurrent(ThreadExitCode exitCode) {
#ifdef ASTOX_OS_WINDOWS
        ExitThread(exitCode);
#else
        pthread_exit(exitCode);
#endif
    };

    void Thread::closeAll() {
        for (unsigned int i = 0; i < handles.size(); i++) {
            closeThread(handles.at(i));
        }
    }

    void Thread::closeThread(ThreadId id, int statusCode) {
#ifdef ASTOX_OS_WINDOWS
        CloseHandle(id);
#else
        pthread_kill(id, statusCode);
#endif
    }

    void Thread::detachThread(ThreadId id) {
#ifdef ASTOX_OS_WINDOWS
        CloseHandle(id);
#else
        pthread_detach(id);
#endif
    };

    int Thread::createThread(ThreadFunction method, ThreadArgs args) {
        ThreadId hThread;
        int response = 0;
#ifdef ASTOX_OS_WINDOWS
        DWORD dwThreadId;
        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) method, args, 0, &dwThreadId);
        if (hThread == NULL) {
            printf("CreateThread error: %d\n", GetLastError());
            response = 1;
        } else {
            handles.push_back(hThread);
        }
        return handles.size();
#else
        response = pthread_create(&(hThread), NULL, method, args);

        if (response != 0) {
            printf("\ncan't create thread :[%s]", strerror(response));
            return -1;
        } else {
            handles.push_back(hThread);
        }
        return response;
#endif
    }

    void Thread::joinThread(ThreadId id) {
#ifdef ASTOX_OS_WINDOWS
        WaitForSingleObject(id, INFINITE);
#else
        pthread_join(id, NULL);
#endif
    };

    void Thread::joinAll() {
        for (unsigned int i = 0; i < handles.size(); i++) {
            joinThread(handles.at(i));
        }
    }

    int Thread::getNumCPU() {
        if (_numCpu > 0) {
            return _numCpu;
        }

#ifdef WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        _numCpu = sysinfo.dwNumberOfProcessors;
#elif defined ASTOX_OS_MAC
        int nm[2];
        size_t len = 4;
        uint32_t count;

        nm[0] = CTL_HW;
        nm[1] = HW_AVAILCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);

        if (count < 1) {
            nm[1] = HW_NCPU;
            sysctl(nm, 2, &count, &len, NULL, 0);
            if (count < 1) {
                count = 1;
            }
        }
        _numCpu = count;
#else
        _numCpu = sysconf(_SC_NPROCESSORS_ONLN);
#endif
        return _numCpu;
    }




}//exit namespace











