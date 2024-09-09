#ifndef _UPLOADTHREAD_H_
#define _UPLOADTHREAD_H_

#include "Thread.h"
#include "WebRequest.h"
#include <vector>

using namespace dd;
using namespace dd::thread;

namespace dd {
namespace network {

class UploadThread :
	public Thread
{
public:

	UploadThread(
        const std::string& serverHost, 
        unsigned short serverPort,
		HttpRequestType serverRequestType, 
        const std::string& serverRequestPath,
		const std::string& searchDirectory, 
        const std::string& searchExtension
    );

	void Run();

protected:
	void uploadFile(const std::string& fileName);

	std::string mServerHost;
	unsigned short mServerPort;
	HttpRequestType mServerRequestType;
	std::string mServerRequestPath;
	std::string mSearchDirectory;
	std::string mSearchExtension;
};

typedef std::vector<UploadThread*> UploadThreadVec;

}
}

#endif
