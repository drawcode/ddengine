#ifndef _IANIMATIONENGINE_H_
#define _IANIMATIONENGINE_H_

#include "DDHeader.h"
#include "Vector.h"

namespace dd {
namespace render {

struct IAnimationEngine {
    virtual void Initialize() = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void Clear() = 0;
    virtual ~IAnimationEngine() {}
};

}
}

#endif