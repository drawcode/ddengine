#ifndef _IMUTEX_H
#define _IMUTEX_H

namespace dd {
namespace thread {

class IMutex
{
public:
	virtual ~IMutex() {}
	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};

}
}

#endif 
