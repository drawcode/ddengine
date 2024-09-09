#include "DDHeader.h"
#include "main.h"

// Global Windows Variables
HWND gHWnd;
HINSTANCE gHInst;

unsigned int gTouchID;
HGLRC gHRC;
DWORD gWindowStyle;
bool gFullscreen;

// This is the windowsize that the renderer will use
Vector2 gWindowSize;

// This is the size of the full window
Vector4 gClientRect;

// This is the size of the full viewport (not the same as the client rect
// for windowed mode
Vector4 gViewRect;


// Forward declarations
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ReadCommandLine(char **configFile, POINT *pos, POINT *size);


//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application. 
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    POINT windowSize;
    RECT clientRect;
    RECT viewRect;


    //// Set the directory mount points for the game.
    //char mountPoint[MAX_PATH] = "data";

    //XLChangeProfileMountPoint(mountPoint);
    //XLChangeMountPoint(mountPoint);

    //XLChangeTexFolder("tex_wgl");
    //XLChangeMatFolder("mat");

    //// Enumerate our displays.  This will come in handy later.
    //XLDisplayEnumerationPC displayEnumerator;

    //// First read our settings ini file
    //XLFileParameterBlock block;
    //char buf[MAX_PATH];
    //_snprintf(buf, sizeof(buf), "%s%s", gDIRECTORY_APP_START, DISPLAY_FILENAME);

    //if(block.OpenFile(buf))
    //{
    //    if(block.ReadParameterBlock("Resolution"))
    //    {
    //        clientRect.top  = 0;
    //        clientRect.left = 0;

    //        block.GetParameter("width", (int *)&clientRect.right); 
    //        block.GetParameter("height", (int *)&clientRect.bottom);
    //        block.GetParameter("fullscreen", (bool *)&gFullscreen);
    //    }
    //}

    // Register the window class
    gHInst = hInst;
    HBRUSH hBrush = CreateSolidBrush(0xFF0000); 

    WNDCLASSEX wc = 
    { 
        sizeof(WNDCLASSEX), 
        0, 
        MsgProc, 
        0L, 
        0L, 
        hInst, 
        NULL, 
        LoadCursor(NULL, IDC_ARROW),
        hBrush, 
        "DDEngine", 
        "DDEngine", 
        NULL 
    };

    RegisterClassEx(&wc);

    char* configFile = NULL;

    POINT pos =
    {
        CW_USEDEFAULT, 10
    };

    if(gFullscreen)
    {
        //displayEnumerator.ChangeDisplayMode(clientRect);

        RECT desktop;
        // Get a handle to the desktop window
        const HWND hDesktop = GetDesktopWindow();
        // Get the size of screen to the variable desktop
        GetWindowRect(hDesktop, &desktop);

        clientRect = viewRect = desktop;

        gWindowStyle = WS_POPUP | WS_VISIBLE | WS_MAXIMIZE ;
    }
    else
    {
        viewRect = clientRect;

        gWindowStyle = WS_OVERLAPPED | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ;
        viewRect.right -= 6;
        viewRect.bottom -= 40;
    }

    //AdjustWindowRect(&clientRect, gWindowStyle, false);
    // Create the application's window
    gHWnd = CreateWindow
    ( 
        "DDEngine", 
        "DDEngine", 
        gWindowStyle, 
        pos.x, pos.y,
        clientRect.right - clientRect.left, 
        clientRect.bottom - clientRect.top,
        NULL, NULL, hInst, NULL 
    );

    HDC hDC = GetDC(gHWnd);

    wglSwapIntervalEXT(1);
    SetForegroundWindow(gHWnd);

    //Instantiate the game (Each game is responsible for defining AllocGame in its game specific code)
    XLATVGame_Alloc();
    
    // Initialize the renderer's width and height.
    //gpRenderer->mHeight = 1024;
    //gpRenderer->mWidth  = 768;

    //gpRenderer->mXOffset = (viewRect.right - XL_UI_RESOLUTION_X)/2;
    //gpRenderer->mYOffset = (viewRect.bottom - XL_UI_RESOLUTION_Y)/2;

    windowSize.x = XL_UI_RESOLUTION_X;
    windowSize.y = XL_UI_RESOLUTION_Y;

    //gpRenderer->mHeightScale = (float)windowSize.x/(float)gpRenderer->mHeight;
    //gpRenderer->mWidthScale  = (float)windowSize.y/(float)gpRenderer->mWidth;

    // We use this flag to tell the renderer we're in the UI state and it should use the windowSize 
    //gpRenderer->mUIState = true;

    //gpGame->Create();									

    //gpGame->mMovieFinished = true;

    gWindowSize = Vector2((float)windowSize.x, (float)windowSize.y);
    gClientRect = Vector4((float)clientRect.left, (float)clientRect.top, (float)clientRect.right, (float)clientRect.bottom);
    gViewRect   = Vector4((float)viewRect.left, (float)viewRect.top, (float)viewRect.right, (float)viewRect.bottom);

    // Enter the message loop
    bool gotMsg   = false;
    bool exitGame = false;

    MSG  msg;
    msg.message = WM_NULL;

    while(   msg.message != WM_QUIT
          && !exitGame)
    {
        // Use PeekMessage() so we can use idle time to execute the game loop
        gotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

        if(gotMsg)
        {
            // Translate and dispatch the message
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //if(gpGame->mPlayMovie)
        //{
        //    //Play the movie
        //    //gpGame->mpAudioVideoPlayer->Play(gpGame->mMovieName);

        //    gpGame->mPlayMovie = false;
        //}

        //// Start playing audio if requested.
        //if(gpGame->mPlayAudio)
        //{
        //    //Play the audio track
        //    gpGame->mpAudioVideoPlayer->Play(gpGame->mAudioName);

        //    gpGame->mPlayAudio = false;
        //    gpGame->mAudioPlaying  = true;
        //}

        //if(gpGame->mAudioPlaying)
        //{
        //    // Set the volume
        //    gpGame->mpAudioVideoPlayer->SetVolume(gpGame->mAudioVolume);
        //    gpGame->mAudioFinished = false;
        ////    
        //}

        //if(gpGame->mStopAudio)
        //{
        //    //Stop playing the audio
        //    gpGame->mpAudioVideoPlayer->Stop();
        //    gpGame->mStopAudio = false; 
        //    gpGame->mAudioFinished = true;
        //    gpGame->mAudioPlaying  = false;
        //}

        //gpGame->HandleTickEvent();
        //gpGame->HandleDrawEvent();
        SwapBuffers(hDC);
    }

    // Destroy the game
    //if(gpGame)
    //{
    //    delete gpGame;
    //}

    // Clean up windows
    UnregisterClass("DDEngine", NULL);

    //Exit
    return 0;
}

//-----------------------------------------------------------------------------
// Window message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    float mouseXPos = 0;
    float mouseYPos = 0;
    HDC hDC = NULL;

    switch(msg)
    {
        case WM_ACTIVATEAPP:
        {
            //gpGame->SetPaused(wParam != TRUE);
        }

        case WM_PAINT:
        {
            PAINTSTRUCT ps; 
            HDC hDC = BeginPaint(hWnd, &ps); 
            SetBkColor(hDC, 0xFF0000); 
            SetTextColor(hDC, 0xFFFFFF);
            RECT rect;
            GetClientRect(hWnd, &rect); 

            EndPaint(hWnd, &ps);
            return 0;
        }

        case WM_CREATE:
        {
            hDC = GetDC(hWnd);	            //get the device context for window
            SetupPixelFormat(hDC);

            // the HRC does need to be global because no one else stores it for us
            gHRC = wglCreateContext(hDC);	//create rendering context
            wglMakeCurrent(hDC,gHRC);	    //make rendering context current
            break;
        }

        case WM_DESTROY:
        {
            hDC = GetDC(hWnd);	            //get the device context for window
            wglMakeCurrent(hDC,NULL);	    //deselect rendering context
            wglDeleteContext(gHRC);		    //delete rendering context
            PostQuitMessage(0);
            break;
        }

        case WM_LBUTTONDOWN:
        {
            mouseXPos = (float)LOWORD(lParam);
            mouseYPos = (float)HIWORD(lParam);
           /* if(gpRenderer->mUIState)
            {
                gpGame->HandleTouchBeginEvent((void *)&gTouchID, (gWindowSize.y - mouseYPos + gpRenderer->mYOffset)/(gpRenderer->mWidthScale), (mouseXPos - gpRenderer->mXOffset)/(gpRenderer->mHeightScale));
            }
            else
            {
                gpGame->HandleTouchBeginEvent((void *)&gTouchID, (gViewRect.w - mouseYPos)/(gpRenderer->mWidthScale), mouseXPos/(gpRenderer->mHeightScale));
            }*/
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            mouseXPos = (float)LOWORD(lParam);
            mouseYPos = (float)HIWORD(lParam);

            // Are these coords the outer edge of our box?
            if(   mouseXPos < XL_WINDOW_EDGE_THRESHOLD
               || mouseYPos < XL_WINDOW_EDGE_THRESHOLD
               || mouseXPos > gWindowSize.x - XL_WINDOW_EDGE_THRESHOLD
               || mouseYPos > gWindowSize.y - XL_WINDOW_EDGE_THRESHOLD)
            {
                //gpGame->HandleTouchEndEvent((void *)&gTouchID);
                gTouchID = ((gTouchID++)%0xFFFFFFFF);
            }
            else
            {
                /*if(gpRenderer->mUIState)
                {
                    gpGame->HandleTouchMoveEvent((void *)&gTouchID, (gWindowSize.y - mouseYPos + gpRenderer->mYOffset)/(gpRenderer->mWidthScale), (mouseXPos - gpRenderer->mXOffset)/(gpRenderer->mHeightScale));
                }
                else
                {
                    gpGame->HandleTouchMoveEvent((void *)&gTouchID, (gViewRect.w - mouseYPos)/(gpRenderer->mWidthScale), mouseXPos/(gpRenderer->mHeightScale));
                }*/
            }
            
            return 0;
        }

        case WM_LBUTTONUP:
        {
            mouseXPos = (float)LOWORD(lParam);
            mouseYPos = (float)HIWORD(lParam);
            //gpGame->HandleTouchEndEvent((void *)&gTouchID);
            gTouchID = ((gTouchID++)%0xFFFFFFFF);
            return 0;
        }

        case WM_KEYDOWN:
        {
            switch((int)wParam)
            {
                case VK_ESCAPE:
                {   
                    PostQuitMessage(0);
                    break;
                }
            }
        }
    }


    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void ReadCommandLine(char **configFile, POINT *pos, POINT *size)
{
    int argCt = 1;
    int argVal;

    if(__argc <= argCt) 
    {
        return;
    }

    // Check for a given game configuration file
    if(isalpha(*__argv[argCt]))
    {
        *configFile = __argv[argCt++];
    }

    int digCt = 0;

    // Read window size and position values
    while(__argc > argCt && isdigit(*__argv[argCt]))
    {
        argVal = atoi(__argv[argCt]);
        digCt++;

        switch (digCt)
        {
            // arg[2] application window x pos  (-1 = CW_USEDEFAULT)
            // arg[3] application window y pos  (-1 = CW_USEDEFAULT)
            // arg[4] application window width  (-1 = CW_USEDEFAULT; 0 = full screen)
            // arg[5] application window height (-1 = CW_USEDEFAULT)
        case 1:
            pos->x = argVal != -1 ? argVal : CW_USEDEFAULT;   
            break;

        case 2:
            pos->y = argVal != -1 ? argVal : CW_USEDEFAULT;    
            break;

        case 3:
            size->x = argVal != -1 ? argVal : CW_USEDEFAULT;    
            break;

        case 4:
            size->y = argVal != -1 ? argVal : CW_USEDEFAULT;    
            break;
        }

        argCt++;
    }
}
