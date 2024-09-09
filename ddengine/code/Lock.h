#ifndef _LOCK_H
#define _LOCK_H

#include "IMutex.h"

namespace dd {
namespace thread {

class IMutex;

class Lock
{
public:
	Lock(const IMutex&);
	~Lock();
private:
	const IMutex& mMutex;
};

}
}

#endif
