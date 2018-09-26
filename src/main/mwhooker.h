#ifndef _MWSTL_MWHOOKER_H_
#define _MWSTL_MWHOOKER_H_

class MWHooker
{
public:
	virtual ~MWHooker();

	virtual void* callback(void* arg) = 0;
};

#endif 
