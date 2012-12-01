#ifndef __HEADER_H_
#define __HEADER_H_

#pragma warning (disable :4996)

#include "DDBuild.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <assert.h>
#include <stdexcept> // std::runtime_error
#include <sstream>
#include <vector>
#include <map>
//#include <errno.h>
#include <time.h>
//#include <cstdio>
#include <ctime>
//#include <stdlib.h>

#ifdef WIN32
#include <tchar.h>
#endif

#ifdef LINUX
#include <netdb.h>
#endif

#ifdef MACOSX
#include <netdb.h>
#endif

#ifdef IPHONE
#include <netdb.h>
#include <sys/time.h>
#endif

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#ifdef _IPHONE
#include <OpenGLES/EAGL.h>
#ifdef ES2
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#endif
#endif

#ifdef WIN32
#pragma comment (lib, "opengl32.lib")
//#pragma comment (lib, "glut32.lib")
#pragma comment (lib, "glu32.lib")
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#ifdef WGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GLES/egl.h>
#include <GLES/gl.h>
#endif
#endif

#define _DEBUG_PRINT(X)   /* X */

#define SEND_RQ(MSG) \
/*cout<<send_str;*/ \
	send(sock,MSG,strlen(MSG),0);

#define SEND_RQ_SECURE(MSG) \
/*cout<<send_str;*/ \
	SSL_write(sslSocket,MSG,strlen(MSG));

template <class T>
inline std::string to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

#endif