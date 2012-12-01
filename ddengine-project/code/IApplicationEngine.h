#ifndef _IAPPLICATIONENGINE_H_
#define _IAPPLICATIONENGINE_H_

#include "Vector.h"
#include "IRenderingEngine.h"

using namespace dd;
using namespace dd::render;
using namespace dd::render::es1;

namespace dd {
	
struct IApplicationEngine {
    virtual void Initialize(int width, int height) = 0;
    virtual void Render() const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void OnFingerUp(ivec2 location) = 0;
    virtual void OnFingerDown(ivec2 location) = 0;
    virtual void OnFingerMove(ivec2 oldLocation, ivec2 newLocation) = 0;
	virtual void OnRotate(DeviceOrientation orientation) = 0;
    virtual ~IApplicationEngine() {}
};
	
	
class ApplicationEngine : public IApplicationEngine {
public:
	ApplicationEngine(IRenderingEngine* renderingEngine);
	~ApplicationEngine();
	void Initialize(int width, int height);
	void OnFingerUp(ivec2 location);
	void OnFingerDown(ivec2 location);
	void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
	void OnRotate(DeviceOrientation orientation);
	void Render() const;
	void UpdateAnimation(float dt);
private:
    void LoadTexture();
	float m_timestamp;
	int m_dragStart;
	int m_dragEnd;
	bool m_dragging;
	
    ivec2 m_screenSize;
    ivec2 m_centerPoint;
    float m_timer;
	IRenderingEngine* m_renderingEngine;
};

IApplicationEngine* CreateApplicationEngine(IRenderingEngine*);

}
#endif



