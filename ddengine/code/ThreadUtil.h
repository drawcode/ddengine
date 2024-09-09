#ifndef _THREADUTIL_H
#define _THREADUTIL_H

#include "DDHeader.h"

#ifndef _WIN32
#include <pthread.h>
#endif

namespace dd {
namespace thread {

class ThreadUtil {
public:
    ThreadUtil(void);
    virtual ~ThreadUtil(void);

    static void WaitForSeconds(int seconds);
};

}
}

#endif
