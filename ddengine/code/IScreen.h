#ifndef _ISCENE_H_
#define _ISCENE_H_

#include "Vector.h"

using namespace dd;
using namespace dd::render;

namespace dd {
	
struct IScene {
    virtual void Initialize(int width, int height) = 0;
    virtual void Render() const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnFingerUp(ivec2 location) = 0;
    virtual void OnFingerDown(ivec2 location) = 0;
    virtual void OnFingerMove(ivec2 oldLocation, ivec2 newLocation) = 0;
    virtual ~IScene() {}
};

}
#endif



