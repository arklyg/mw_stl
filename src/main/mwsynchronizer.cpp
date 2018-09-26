#include <mwsynchronizer.h>

MWEvent::MWEvent()
{
}

MWEvent::~MWEvent()
{
}

MWEvent::MWEvent(int id, void* const arg)
	: _id(id)
	, _arg(arg)
{
}

MWSynchronizer::MWSynchronizer()
{
	pthread_cond_init(& _cond, NULL);
}

MWSynchronizer::~MWSynchronizer()
{
	pthread_cond_destroy(& _cond);
}

MWEvent* MWSynchronizer::next(size_t nanoseconds)
{
	_lock.acquire();

	if (_list.empty())
	{
		if (0 == nanoseconds)
		{
			pthread_cond_wait(& _cond, & _lock.get_mutex());
		}
		else
		{
			timespec max_time;
			max_time.tv_sec = 0;
			max_time.tv_nsec = nanoseconds;
			pthread_cond_timedwait(& _cond, & _lock.get_mutex(), & max_time);

			if (_list.empty())
			{
				_lock.release();
				return NULL;
			}
		}
	}

	MWEvent* ret = _list.front();
	_list.pop_front();

	_lock.release();
	return ret;
}

void MWSynchronizer::broadcast(MWEvent* const event)
{
	_lock.acquire();
	_list.push_back(event);
	pthread_cond_broadcast(& _cond);
	_lock.release();
}
