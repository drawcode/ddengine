#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "DDHeader.h"
#include "GameBase.h"
#include "Camera.h"

namespace dd
{

class Renderer 
{
public:
    Renderer(
        int width, 
        int height, 
        int xOffset, 
        int yOffset, 
        float widthScale, 
        float heightScale
    );
    virtual ~Renderer(void);

    int mWidth;
    int mHeight;

    int mXOffset;
    int mYOffset;
   
    float mWidthScale;
    float mHeightScale;

    float mClearColor[4];

    float mLightAmbientColor[4];
    float mLightDiffuseColor[4];
    float mLightSpecularColor[4];
    float mLightDirection[4];
    
    Camera *mCamera;

    // Camera
    virtual void SetCamera(Camera *camera);
    virtual Camera *GetCamera(void);

    // Display
    virtual void BeginDraw();
    virtual void EndDraw();
    virtual void Flush();

};

}

#endif
