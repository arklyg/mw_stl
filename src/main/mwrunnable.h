#ifndef _MWSTL_RUNNABLE_H_
#define _MWSTL_RUNNABLE_H_

#include <pthread.h>

class MWRunnable
{
protected:
	pthread_t _thread_id;

private:
	static void* thread_run(void* arg);

protected:
	virtual void* run() = 0;

public:
	virtual ~MWRunnable();

	void start();
	void join();
};

#endif
