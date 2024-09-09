#ifndef _MUTEX_H
#define _MUTEX_H

#include "DDHeader.h"
#include "IMutex.h"
#ifndef _WIN32
#include <pthread.h>
#endif

namespace dd {
namespace thread {

class Mutex : public IMutex
{
public:
	Mutex();
	~Mutex();
	virtual void Lock() const;
	virtual void Unlock() const;
private:
#ifdef _WIN32
	HANDLE mMutex;
#else
	mutable pthread_mutex_t mMutex;
#endif
};

}
}

#endif
