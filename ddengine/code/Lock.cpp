#include "IMutex.h"
#include "Lock.h"

namespace dd {
namespace thread {

Lock::Lock(const IMutex& m) : mMutex(m)
{
	mMutex.Lock();
}

Lock::~Lock()
{
	mMutex.Unlock();
}

}
}