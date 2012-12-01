#ifndef _THREAD_H
#define _THREAD_H

#include "DDHeader.h"

#ifndef _WIN32
#include <pthread.h>
#endif

namespace dd {
namespace thread {

#ifdef _WIN32
//typedef unsigned threadfunc_t;
typedef LPTHREAD_START_ROUTINE threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX __stdcall
#else
typedef void * threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX
#endif

class Thread
{
public:
	Thread(bool release = true);
	virtual ~Thread();
    
	static threadfunc_t STDPREFIX StartThread(threadparam_t);
    
    void Start();
	virtual void Run() = 0;
#ifdef _WIN32
	HANDLE GetThread() { return mThread; }
	unsigned GetThreadId() { return mThreadId; }
#else
	pthread_t GetThread() { return mThread; }
#endif
	bool IsRunning();
	void SetRunning(bool x);
	bool IsReleased();
	void SetRelease(bool x);
	bool DeleteOnExit();
	void SetDeleteOnExit(bool x = true);
	bool IsDestructor();
protected:
#ifdef _WIN32
	HANDLE mThread;
	unsigned mThreadId;
#else
	pthread_t mThread;
#endif
private:
	Thread(const Thread& ) {}
	Thread& operator=(const Thread& ) { return *this; }
	bool mRunning;
	bool mRelease;
	bool mDeleteOnExit;
	bool mDestructor;
};

}
}

#endif 
