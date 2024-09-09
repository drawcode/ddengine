#ifndef _GLCONTEXT_H_
#define _GLCONTEXT_H_

#include "DDHeader.h"
#include "GameBase.h"
#include "GLContext.h"

namespace dd
{

class GLContext
{
public:

    GLContext(void);
    virtual ~GLContext();

    virtual void Init(HWND hWnd);
    virtual void Purge();

private:

    void Reset();

    HWND mhWnd;
    HDC mhDC;
    HGLRC mhRC;

};

}

#endif
