#ifndef _WEBREQUEST_H_
#define _WEBREQUEST_H_

#include "DDHeader.h"

using namespace std;

namespace dd {
namespace network {

enum HttpRequestType 
{ 
    HTTP_GET, 
    HTTP_POST, 
    HTTP_PUT,
    HTTP_DELETE,
	HTTPS_GET, 
    HTTPS_POST, 
    HTTPS_PUT,
    HTTPS_DELETE,
};

class WebRequest
{
public:
    
    int mReadSize;
    int mError;

    std::string mMessageHeader;
    std::vector<unsigned char> mMessageBody;
    std::string mErrorMsg;

public:
    WebRequest(void);
    virtual ~WebRequest(void);

    virtual std::string GetHeader(std::string header_name);
    virtual std::string GetMessageBodyString();
    virtual int Request
    (
        int request_type
        , std::string url_host
        , int url_port
        , std::string url_path
        , std::string data
    );
	
private:
	virtual void CloseSocket(int socket); // should not be called directly...
};

}
}

#endif
