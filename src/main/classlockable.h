#ifndef _CLASS_LOCKABLE_H_
#define _CLASS_LOCKABLE_H_

#include <map>
#include <string>
#include <pthread.h>

#include "lockable.h"

using namespace std;

// 用于同一个类的所有对象的同步, 用于此对象外的被同步
// 内部同步建议使用ProcLockable
class ClassLockable : public Lockable
{
private:
	static pthread_mutex_t _map_mutex;
	static map<string, pthread_mutex_t*> _mutex_map;

private:
	pthread_mutex_t* get_mutex();
	pthread_mutex_t* add_mutex();

public:
	virtual void acquire();
	virtual void release();
};


#endif
