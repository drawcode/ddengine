#ifndef _WEBREQUESTTHREADED_H
#define _WEBREQUESTTHREADED_H
#include "DDHeader.h"
#include "Thread.h"
#include "WebRequest.h"

using namespace dd;
using namespace dd::thread;

namespace dd {
namespace network {

class WebRequestThreaded : public Thread
{
public:
	WebRequestThreaded(WebRequest *p);
	~WebRequestThreaded();
	void Run();
private:
	WebRequest *GetWebRequest() const { return mWebRequest; }
	WebRequestThreaded(const WebRequestThreaded& obj) : mWebRequest(obj.GetWebRequest()) {}
	WebRequestThreaded& operator=(const WebRequestThreaded& ) { return *this; }
	WebRequest *mWebRequest;
};

}
}

#endif
