#include "proclockable.h"

ProcLockable::ProcLockable()
{
	pthread_mutex_init(& _mutex, NULL);
}

ProcLockable::~ProcLockable()
{
	pthread_mutex_destroy(& _mutex);
}

void ProcLockable::acquire()
{
	pthread_mutex_lock(& _mutex);
}

void ProcLockable::release()
{
	pthread_mutex_unlock(& _mutex);
}

pthread_mutex_t &ProcLockable::get_mutex()
{
	return _mutex;
}
