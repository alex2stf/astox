/*
 * thread.cpp
 *
 *  Created on: 7 Jul 2017
 *      Author: alex
 */

#include "astox.h"

namespace astox{
	//ThreadFactory * ThreadFactory::_in = new ThreadFactory();

	ThreadFactory::ThreadFactory(){
		log_stack("construct");
//		#ifdef _PTHREAD_H
//				cond_var_lock =  PTHREAD_MUTEX_INITIALIZER;
//		#endif
		stx_thread_mutex_init(&cond_var_lock, NULL);
		cnt = 0;
	};

	//ThreadFactory * ThreadFactory::instance(){
	//	return _in;
	//};

	ThreadArg::ThreadArg(int id, Function * callback, Value * args){
		_id = id;
		_args = args;
		_callback = callback;

	}

	int ThreadArg::id(){
		return _id;
	}

	Value * ThreadArg::args(){
		return _args;
	};


	Function * ThreadArg::callback(){
		return _callback;
	};


	stx_thread_return ThreadFactory::callback(stx_thread_arg threadarg){
		ThreadArg * ta = (ThreadArg*)threadarg;
		log_stack("\n\nwait id = %i\n\n", ta->id());
		Function * f = ta->callback();
		f->call(ta->args(), f);

		//ThreadFactory::instance()->decr();
		stx_thread_exit(NULL);
		return 0;
	}

	void ThreadFactory::close(int i){
		printf("close %i from %i \n", i, (int)vth.size());
		vth.at(i).valid = false;
	};

	void ThreadFactory::make(Function *&callback, Value * args, stx_thread_attr * attr) {
		//ThreadFactory::instance()->incr();
		ThreadArg * tfarg = new ThreadArg((int)vth.size(), callback, args);
		thread_data t = stx_create_thread(ThreadFactory::callback, (stx_thread_arg)tfarg, attr);
		stx_thread * st = t.thread;
		stx_thread_detach(*st);

	}

	void ThreadFactory::wait(){
		int n = 0;
		while(cnt){
            n++;
        };
	};

	thread_data stx_create_thread(stx_thread_function function,
				stx_thread_arg arg, stx_thread_attr * atr){

			thread_data td;
			td.valid = true;
			stx_thread thread;
	#ifdef ASTOX_OS_WINDOWS
			stx_thread_id thread_id;
			int ssz = 0;
			if (atr != NULL) {
				ssz = atr->stack_size;
			}
			thread = CreateThread(
					NULL,                   // default security attributes
					ssz,                    // use default stack size
					reinterpret_cast<LPTHREAD_START_ROUTINE>(function),               // thread function name
					arg,         			// argument
					0,                      // use default creation flags
					&thread_id);            // returns the thread identifier
			 if (thread == NULL){
				 td.valid = false;
				 return td;
			 }

			 td.thread_id = thread_id;
	#else
	 #include <pthread.h>
			pthread_attr_t pat;
			//if(atr != NULL){
//				pthread_attr_init(&pat);
//				pthread_attr_setdetachstate(&pat, PTHREAD_CREATE_JOINABLE);
//				pthread_attr_setstacksize(&pat, atr->stack_size);
//				pthread_attr_setschedpolicy(&pat, PTHREAD_EXPLICIT_SCHED);
//				pthread_attr_setguardsize(&pat, 20);
//				pthread_attr_setscope(&pat, 2);
//				pthread_attr_setinheritsched(&pat, 3);

			//}

            int r = pthread_create(&thread, NULL, function, arg);


			pthread_attr_destroy(&pat);
			td.thread_id = r;
			if(r){
				td.valid = false;
				return td;
			}

//			if(pthread_join(thread, NULL)){
//				printf("ERR JOIN");
//			}

	#endif
			td.thread = &thread;
			return td;
		};



	bool stx_join_thread(const thread_data * td){
#ifdef ASTOX_OS_WINDOWS
		DWORD result = WaitForSingleObject(td->thread, 990);

		if (result == WAIT_OBJECT_0) {
		   return false;
		}
		else {
			return true;
		}
#else
		stx_thread * t = td->thread;
		pthread_join(*t, NULL);
        return true; //TODO return if joined success
#endif

	};
}



