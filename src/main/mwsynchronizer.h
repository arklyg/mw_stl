#ifndef _MW_SYNCHRONIZER_H_
#define _MW_SYNCHRONIZER_H_

#include <list>

#include "proclockable.h"

using namespace std;

class MWEvent
{
public:
	size_t _id;
	void* _arg;

public:
	MWEvent();
	~MWEvent();

	MWEvent(int event_id, void* const arg);
};

class MWSynchronizer
{
private:
	ProcLockable _lock;
	pthread_cond_t _cond;
	list<MWEvent*> _list;

public:
	MWSynchronizer();
	~MWSynchronizer();

	MWEvent* next(size_t nanoseconds = 0);
	void broadcast(MWEvent* const event);
};

#endif

