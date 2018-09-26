#ifndef _LOCKABLE_H_
#define _LOCKABLE_H_

class Lockable
{
public:
    virtual ~Lockable();

    virtual void acquire() = 0;
    virtual void release() = 0;
};


#endif
