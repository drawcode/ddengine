#include "Vector.h"
#include "ISurface.h"
#include "IRenderingEngine.h"
#include "IApplicationEngine.h"
#include "ParametricSurface.h"

using namespace std;

using namespace dd;
using namespace dd::render;

namespace dd {
	
const string TextureFiles[] = {
	"UI_WorldMap.png",
	/*"Utopia4444.pvr",
	"Grasshopper565.pvr",
	"Luma8.png",
	"LumaAlpha8.png",
	"Rgb8.png",
	"Rgba8.png",
	"LetterA.png",
	"Astronomy.pvr", 
	"Utopia.png",
	"Utopia.pvr",*/
};
 
IApplicationEngine* CreateApplicationEngine(IRenderingEngine* renderingEngine)
{
    return new ApplicationEngine(renderingEngine);
}

ApplicationEngine::ApplicationEngine(IRenderingEngine* renderingEngine) :
    m_renderingEngine(renderingEngine),
    m_dragging(false),
    m_timestamp(0)
{
}

ApplicationEngine::~ApplicationEngine()
{
    delete m_renderingEngine;
}

void ApplicationEngine::Initialize(int width, int height)
{
	
    m_renderingEngine->Initialize(width, height);
	
	m_screenSize = ivec2(width, height);
    m_centerPoint = m_screenSize / 2;
	
    vector<ISurface*> surfaces(1);
    surfaces[0] = new Quad(2, 2);
    m_renderingEngine->RenderSurfaces(surfaces);
	delete surfaces[0];
}

void ApplicationEngine::Render() const
{
	vector<Visual> visuals(1);
    visuals[0].Color = vec3(1, 1, 1);
    visuals[0].LowerLeft = ivec2(-160, 0);
    visuals[0].ViewportSize = ivec2(m_screenSize.x*2, m_screenSize.y);
    visuals[0].Orientation = Quaternion();
    m_renderingEngine->RenderVisuals(visuals);
    m_renderingEngine->Render();
}

void ApplicationEngine::UpdateAnimation(float dt)
{
    m_timestamp += dt;
    m_renderingEngine->UpdateAnimation(m_timestamp);
}

void ApplicationEngine::OnFingerUp(ivec2 location)
{
    m_dragging = false;
}

void ApplicationEngine::OnFingerDown(ivec2 location)
{
    if (!m_dragging) {
        m_dragging = true;  
        m_dragStart = m_dragEnd = location.y;
    }
	printf("OnFingerDown x:%d y:%d", location.x, location.y);
}

void ApplicationEngine::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_dragEnd = newLocation.y;
}
	
void ApplicationEngine::OnRotate(DeviceOrientation orientation)
{
	m_renderingEngine->OnRotate(orientation);
}
	
void ApplicationEngine::LoadTexture()
{
	/*
	string filename = TextureFiles[m_textureIndex];
	string suffix = ".pvr";
	size_t i = filename.rfind(suffix);
	if (i != string::npos && i == (filename.length() - suffix.length()))
		m_renderingEngine->SetPvrTexture(filename);
	else
		m_renderingEngine->SetPngTexture(filename);
	 */
}

}
