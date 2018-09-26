#include "mwrunnable.h"

MWRunnable::~MWRunnable()
{
}

void* MWRunnable::thread_run(void* arg)
{
	return static_cast<MWRunnable*>(arg)->run();
}

void MWRunnable::start()
{
	pthread_create(& _thread_id, NULL, thread_run, this);
}

void MWRunnable::join()
{
	pthread_join(_thread_id, NULL);
}

