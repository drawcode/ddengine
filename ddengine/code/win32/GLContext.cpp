#include "GLContext.h"
namespace dd
{

GLContext::GLContext()
{
    Reset();
}

GLContext::~GLContext()
{
    Purge();
}

void GLContext::Init(HWND hWnd)
{
    // remember the window handle (HWND)
    mhWnd = hWnd;

    // get the device context (DC)
    mhDC = GetDC( mhWnd );

    // set the pixel format for the DC
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                  PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int format = ChoosePixelFormat( mhDC, &pfd );
    SetPixelFormat( mhDC, format, &pfd );

    // create the render context (RC)
    mhRC = wglCreateContext( mhDC );

    // make it the current render context
    wglMakeCurrent( mhDC, mhRC );
}

void GLContext::Purge()
{
    if ( mhRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( mhRC );
    }
    if ( mhWnd && mhDC )
    {
        ReleaseDC( mhWnd, mhDC );
    }
    Reset();
}

void GLContext::Reset()
{
    mhWnd = NULL;
    mhDC = NULL;
    mhRC = NULL;
}

}