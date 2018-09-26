#ifndef _PROC_LOCKABLE_H_
#define _PROC_LOCKABLE_H_

#include <pthread.h>

#include "lockable.h"

// 包装了mutex
class ProcLockable : public Lockable
{
private:
	pthread_mutex_t _mutex;

public:
	ProcLockable();
	virtual ~ProcLockable();

	virtual void acquire();
	virtual void release();

	pthread_mutex_t &get_mutex();
};

#endif
