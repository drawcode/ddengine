#ifndef _IRENDERINGENGINE_H_
#define _IRENDERINGENGINE_H_

#include "DDHeader.h"
#include "Vector.h"
#include "Box.h"
#include "ISurface.h"
#include "IResourceManager.h"
#include "PVRTTexture.h"

using namespace std;
using namespace dd;
using namespace dd::asset;
using namespace dd::render;

namespace dd {
namespace render {
namespace es1 {	
	
struct Drawable {
	GLuint VertexBuffer;
	GLuint IndexBuffer;
	int IndexCount;
};
	
// Physical orientation of a handheld device, equivalent to UIDeviceOrientation
enum DeviceOrientation {
    DeviceOrientationUnknown,
    DeviceOrientationPortrait,
    DeviceOrientationPortraitUpsideDown,
    DeviceOrientationLandscapeLeft,
    DeviceOrientationLandscapeRight,
    DeviceOrientationFaceUp,
    DeviceOrientationFaceDown,
};

struct IRenderingEngine {
    //virtual void Initialize() = 0;	
    virtual void Initialize(int width, int height) = 0;
    //virtual void Initialize(const vector<ISurface*>& surfaces) = 0;
    virtual void Render() const = 0;
	
    virtual void RenderSurfaces(const vector<ISurface*>& surfaces) = 0;
    virtual void RenderVisuals(const vector<Visual>& visuals) const = 0;
    virtual void RenderScene() const = 0;
    virtual void RenderTexture(const string& file) const = 0;
	virtual void OnRotate(DeviceOrientation orientation) = 0;
    //virtual void Render(const vector<Visual>& visuals) const = 0;
    virtual void SetPngTexture(const string& file) const = 0;
    virtual void SetPvrTexture(const string& file) const = 0;
    virtual void UpdateAnimation(float timeStep) = 0;
    virtual void Clear() const = 0;
    //virtual void Resize(GLsizei width, GLsizei height) = 0;
    virtual ~IRenderingEngine() {}
};

struct PVRTextureHeader {
	unsigned int HeaderSize;
	unsigned int Height;
	unsigned int Width;
	unsigned int MipMapCount;
	unsigned int Flags;
	unsigned int TextureDataSize;
	unsigned int BitCount;
	unsigned int RBitMask;
	unsigned int GBitMask;
	unsigned int BBitMask;
	unsigned int AlphaBitMask;
	unsigned int PVR;
	unsigned int NumSurfs;
};

enum PVRPixelType {
	OGL_RGBA_4444 = 0x10,
	OGL_RGBA_5551,
	OGL_RGBA_8888,
	OGL_RGB_565,
	OGL_RGB_555,
	OGL_RGB_888,
	OGL_I_8,
	OGL_AI_88,
	OGL_PVRTC2,
	OGL_PVRTC4,
	PVRTEX_PIXELTYPE = 0xff,
};

struct Framebuffers {
	GLuint Screen;
};

struct Renderbuffers {
	GLuint Screen;
};

struct Textures {
	GLuint Tile;
	GLuint Unified;
	GLuint Background;
	GLuint Body;
	GLuint Eyes;
};

struct Vertex {
	vec3 Position;
	vec2 TexCoord;
};

struct NoopSprite {
	box2 Body;
	box2 Eyes;
};
	
class RenderingEngine : public IRenderingEngine {
public:
	RenderingEngine();
	~RenderingEngine();
	//void Initialize();	
    void Initialize(int width, int height);
	void Render() const;
	void RenderScene() const;
	void RenderTexture(const string& file) const;	
	void RenderSurfaces(const vector<ISurface*>& surfaces);
	void RenderVisuals(const vector<Visual>& visuals) const;
	void OnRotate(DeviceOrientation orientation);
    void SetPngTexture(const string& name) const;
    void SetPvrTexture(const string& file) const;
	void UpdateAnimation(float timestamp);
	void Clear() const;
	
private:
	GLuint CreateTexture(const unsigned long* data);
	Textures m_textures;
	Renderbuffers m_renderbuffers;
	Framebuffers m_framebuffers;
    GLfloat m_rotationAngle;
    GLfloat m_currentAngle;
    GLfloat m_scale;
    ivec2 m_pivotPoint;
	
	//std::vector<NoopSprite> m_noopFrames;
	//std::vector<NoopSprite> m_unifiedFrames;
	size_t m_frameCount;
	size_t m_frameIndex;
	
	
    vector<Drawable> m_drawables;
    GLuint m_colorRenderbuffer;
    GLuint m_depthRenderbuffer;
    mat4 m_translation;
    IResourceManager *m_resourceManager;
    mutable float m_offset;
};

	
IRenderingEngine* CreateRenderingEngine();

}	
}
}
#endif


