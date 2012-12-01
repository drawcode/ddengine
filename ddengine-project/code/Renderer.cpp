#include "Renderer.h"
namespace dd {

Renderer::Renderer(
    int width, 
    int height, 
    int xOffset, 
    int yOffset, 
    float widthScale, 
    float heightScale
): 
    mWidth(width),
    mHeight(height),
    mXOffset(xOffset),
    mYOffset(yOffset),
    mWidthScale(widthScale),
    mHeightScale(heightScale)
{
    //mClearColor[4];

    //mLightAmbientColor[4];
    //mLightDiffuseColor[4];
    //mLightSpecularColor[4];
    //mLightDirection[4];
}

Renderer::~Renderer(void) {
    if (mCamera) {
        mCamera = NULL;
        delete mCamera;
    }
}

// props

void Renderer::SetCamera(Camera *camera) {
    mCamera = camera;
}

Camera *Renderer::GetCamera(void) {
    return mCamera;
}

// action

void Renderer::BeginDraw(void) {
}

void Renderer::EndDraw(void) {
}

void Renderer::Flush(void) {
}

}
