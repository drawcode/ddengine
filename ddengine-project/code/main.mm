#include "BBHeader.h"
#include "BBWebRequest.h"
#include "BBWebRequestThreaded.h"
#include "BBThread.h"
#include "BBMutex.h"

using namespace bb;

int main(int argc, char *argv[])
{        
    WebRequest *webreq1 = new WebRequest();
    WebRequest *webreq2 = new WebRequest();

    printf("STARTED THREADS");

    unsigned int i = 0;
    for (i = 0;i < 10; i++)
    {
        WebRequestThreaded *webthread1 = new WebRequestThreaded(webreq1);
        WebRequestThreaded *webthread2 = new WebRequestThreaded(webreq2);
    }

    printf("COMPLETED THREADS");

    //std::clock_t start;
    //double diff;

    //start = std::clock();
    //for ( int i = 0; i < 1000000; i++ )
    //{
    //    diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;

    //    std::cout<<"printf: "<< diff <<'\n';
    //}
 
    //printf("Running request...");

    ////boxreq->Request(HTTPS_GET, "live.2xlgames.com", 443, "/", "");
    //webreq->Request(HTTPS_GET, "live.2xlgames.com", 443, "/", "");

    //printf("Completed request...");

    //printf(webreq->mMessageBody.c_str());

    //printf("tester");

    std::cin.get();

    return 0;
}