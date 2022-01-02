#ifndef ASTOX_THREAD_TEST_H
#define ASTOX_THREAD_TEST_H


#include "thread.h"
#include "staticvars.h"

using namespace astox;
using namespace astox::scriptengine;
using namespace astox::testing;
using namespace astox::ecma::methods;
using namespace std;




static int cnt = 0;

static inline stx_thread_return MyThreadFunction(stx_thread_arg threadarg) {
	ThreadArg *pOb = (ThreadArg*)threadarg;
	stx_thread current_thread = stx_thread_self();
	log_stack("\n\nwait id = %i\n\n", pOb->id());
//	log_stack("\n\nseparate thread %s\n\n", serializers::to_json(pOb).c_str());

	//CloseHandle(current_thread);

	return 0;
}


static inline Value * threadCallback(Value * args, const lexic &l){
   int wt = 1000 * args->toInt();
	stx_sleep(1000);
	cnt++;
	log_stack("\n\nseparate thread %s\n\n", serializers::to_json(args).c_str());

	if (args->toInt() == 4) {
		Function * f = Function::New(threadCallback, Syntax::JS);
		ThreadFactory::instance()->make(f, Number::New(10, Syntax::JS), NULL);
	}
	if (args->toInt() == 10) {
		printf("\n\nwait ---------------------------------- %i.....\n\n", wt);
	}
	else {

		printf("\n\nwait %i.....\n\n", wt);
	}
	return NULL;
}




static inline void thread_test() {


		stx_thread_attr attr;
		stx_thread_id thread_id;
		Object * obj = Object::New(Syntax::JS);
		obj->addKey("unu", 1);
		obj->addKey("doi", 2);
		//stx_thread_create(&threads[0], NULL, PrintHello, NULL);
		attr.stack_size = 0;



		for(int i = 0; i < 10; i++){
			Function * f = Function::New(threadCallback, Syntax::JS);
			ThreadFactory::instance()->make(f, Number::New(i,  Syntax::JS), &attr);
		}


		cout << "THREADS created";

		ThreadFactory::instance()->wait();
		cout << "Main: completed cnt =  :" << cnt;
		//stx_sleep(4000);

}

#endif
