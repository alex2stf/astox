/*
 * FileSystem.h
 *
 *  Created on: Jul 30, 2014
 *      Author: alexandru
 */

#ifndef ASTX_THREAD_H_
#define ASTX_THREAD_H_

    #include "macros.h"
    #include "util.h"
 	 #include "types.h"
    #include <iostream>
    #include <string>

    #ifdef ASTOX_OS_WINDOWS
		#include <process.h>
		#include <stddef.h>
		#include <stdlib.h>
		#include <conio.h>
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
		#define stx_thread HANDLE
		#define stx_thread_return DWORD
		#define stx_thread_arg LPVOID
		#define stx_thread_id DWORD
		#define stx_thread_self GetCurrentThread
		#define stx_thread_mutex HANDLE
		#define stx_sleep(nms) Sleep(nms)
		#define stx_thread_detach(thread) if(thread!=NULL)CloseHandle(thread)
		#define stx_thread_cancel(thread) TerminateThread(thread,0)
		#define stx_thread_exit(thread) ExitThread(thread);
		#define stx_thread_mutex HANDLE
		#define stx_thread_mutex_init(pobject,pattr) (*pobject=CreateMutex(NULL,FALSE,NULL))
		#define stx_thread_mutex_lock(pobject) WaitForSingleObject(*pobject,INFINITE)
		#define stx_thread_mutex_unlock(pobject) ReleaseMutex(*pobject)
	#else
		#define stx_thread pthread_t
		#define stx_thread_arg void *
		#define stx_thread_id int
		#define stx_thread_return void *
		#define stx_thread_self pthread_self
		#define stx_thread_mutex pthread_mutex_t
		#define stx_sleep(nms) sleep((nms+500)/1000)
		#define stx_thread_detach(thread) pthread_detach(thread)
		#define stx_thread_cancel(thread) pthread_cancel(thread)
		#define stx_thread_exit(thread) pthread_exit(thread);
		#define stx_thread_mutex pthread_mutex_t
		#define stx_thread_mutex_init(pobject,pattr) pthread_mutex_init(pobject,pattr)
		#define stx_thread_mutex_lock(pobject) pthread_mutex_lock(pobject)
		#define stx_thread_mutex_unlock(pobject) pthread_mutex_unlock(pobject)
    #endif

	typedef stx_thread_return(stx_thread_function)(stx_thread_arg);

	typedef struct stx_thread_attr{
		int stack_size;
	} stx_thread_attr;

	typedef struct thread_data{
		stx_thread * thread;
		stx_thread_id thread_id;
		bool valid;
	} thread_data;



	thread_data stx_create_thread(stx_thread_function function,
			stx_thread_arg arg, stx_thread_attr * attr = NULL);

	bool stx_join_thread(const thread_data * td);


	class ThreadArg{
	private:
		int _id;
		Function * _callback;
		Value * _args;
	public:
		ThreadArg(int id, Function * callback, Value * args);
		int id();
		Value * args();
		Function * callback();

	};

	class ThreadFactory{
	private:
		vector<thread_data> vth;
		ThreadFactory();
		static ThreadFactory * _in;

		static stx_thread_return callback(stx_thread_arg threadarg);
		volatile int cnt;
	public:
		stx_thread_mutex cond_var_lock;
	


		void incr(){
			stx_thread_mutex_lock (&cond_var_lock);
			cnt++;
			stx_thread_mutex_unlock (&cond_var_lock);
		};

		void decr(){
			stx_thread_mutex_lock (&cond_var_lock);
			cnt--;
			stx_thread_mutex_unlock (&cond_var_lock);
		}

		void lock(){
			cnt++;
		}

		void unlock(){
			cnt--;
		}
		static ThreadFactory * instance();
		void make(Function *&callback, Value * args, stx_thread_attr * attr = NULL);
		void wait();
		void close(int id);
	};




}

#endif /* ASTX_THREAD_H */
