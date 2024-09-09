#ifndef _RENDERINGENGINEES1_H_
#define _RENDERINGENGINEES1_H_

#include "DDHeader.h"
#include "IRenderingEngine.h"
#include "IResourceManager.h"
#include "ISurface.h"
#include "Vector.h"
#include "Box.h"
#include "Matrix.h"

using namespace std;
using namespace dd;
using namespace dd::asset;
using namespace dd::render;
using namespace dd::render::es1;

namespace dd {
namespace render {	
namespace es1 {

IRenderingEngine* CreateRenderingEngine()
{
	return new RenderingEngine();
}
	
RenderingEngine::RenderingEngine()
{
    //glGenRenderbuffersOES(1, &m_renderbuffers.Screen);
    //glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.Screen);
    //Initialize();
	m_resourceManager = CreateResourceManager();
	
    m_offset = 0;
    glGenRenderbuffersOES(1, &m_colorRenderbuffer);
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
}
	
RenderingEngine::~RenderingEngine()
{
	//glGenRenderbuffersOES(1, &m_renderbuffers.Screen);
	//glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_renderbuffers.Screen);
	//Initialize();
    delete m_resourceManager;
}

void RenderingEngine::Initialize(int width, int height)
{
    m_pivotPoint = ivec2(width / 2, height / 2);

    Clear();   
}

void RenderingEngine::OnRotate(DeviceOrientation orientation)
{
    float angle = 0;
    
    switch (orientation) {
        case DeviceOrientationLandscapeLeft:
            angle = 270;
            break;

        case DeviceOrientationPortraitUpsideDown:
            angle = 180;
            break;

        case DeviceOrientationLandscapeRight:
            angle = 90;
            break;
    }
    
    m_currentAngle = angle;
}

    
void RenderingEngine::UpdateAnimation(float timestamp)
{
	/*
    float speed = 30;
    int timeindex = ((int) (timestamp * speed)) % (m_frameCount * 2);
    if (timeindex < m_frameCount)
    {
        m_frameIndex = timeindex;
    }
    else
    {
        timeindex -= m_frameCount;
        m_frameIndex = m_frameCount - timeindex - 1;
    }
	 */
}

void RenderingEngine::Render() const
{
    RenderScene();
}

void RenderingEngine::RenderScene() const
{
    std::string str = "UI_WorldMap.png";
    SetPngTexture(str);
}

void RenderingEngine::RenderTexture(const string& filename) const
{
    
}


void RenderingEngine::RenderVisuals(const vector<Visual>& visuals) const
{
	glClearColor(0.5f, 0.5f, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vector<Visual>::const_iterator visual = visuals.begin();
	for (int visualIndex = 0; visual != visuals.end(); ++visual, ++visualIndex) {
		
		// Set the viewport transform.
		ivec2 size = visual->ViewportSize;
		ivec2 lowerLeft = visual->LowerLeft;
		glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
		
		// Set the light position.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		// Set the model-view transform.
		mat4 rotation = visual->Orientation.ToMatrix();
		mat4 modelview = rotation * m_translation;
		glLoadMatrixf(modelview.Pointer());
		
		// Set the projection transform.
		float h = 4.0f * size.y / size.x;
		mat4 projection = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(projection.Pointer());
		
		// Set the diffuse color.
		vec3 color = visual->Color * 0.75f;
		vec4 diffuse(color.x, color.y, color.z, 1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse.Pointer());
		
		// Draw the surface.
		int stride = sizeof(vec3) +  sizeof(vec2);
		const GLvoid* texCoordOffset = (const GLvoid*) sizeof(vec3);
		const Drawable& drawable = m_drawables[visualIndex];
		glBindBuffer(GL_ARRAY_BUFFER, drawable.VertexBuffer);
		glVertexPointer(3, GL_FLOAT, stride, 0);
		glTexCoordPointer(2, GL_FLOAT, stride, texCoordOffset);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawable.IndexBuffer);
		glDrawElements(GL_TRIANGLES, drawable.IndexCount, GL_UNSIGNED_SHORT, 0);
	}
}
	
	
void RenderingEngine::RenderSurfaces(const vector<ISurface*>& surfaces)
{
	vector<ISurface*>::const_iterator surface;
	for (surface = surfaces.begin(); surface != surfaces.end(); ++surface) {
		
		// Create the VBO for the vertices.
		vector<float> vertices;
		unsigned char vertexFlags = VertexFlagsTexCoords;
		(*surface)->GenerateVertices(vertices, vertexFlags);
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,
					 vertices.size() * sizeof(vertices[0]),
					 &vertices[0],
					 GL_STATIC_DRAW);
		
		// Create a new VBO for the indices if needed.
		int indexCount = (*surface)->GetTriangleIndexCount();
		GLuint indexBuffer;
		if (!m_drawables.empty() && indexCount == m_drawables[0].IndexCount) {
			indexBuffer = m_drawables[0].IndexBuffer;
		} else {
			vector<GLushort> indices(indexCount);
			(*surface)->GenerateTriangleIndices(indices);
			glGenBuffers(1, &indexBuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
						 indexCount * sizeof(GLushort),
						 &indices[0],
						 GL_STATIC_DRAW);
		}
		
		Drawable drawable = { vertexBuffer, indexBuffer, indexCount};
		m_drawables.push_back(drawable);
	}
	
	// Extract width and height from the color buffer.
	int width, height;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
									GL_RENDERBUFFER_WIDTH_OES, &width);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
									GL_RENDERBUFFER_HEIGHT_OES, &height);
	
	// Create a depth buffer that has the same size as the color buffer.
	glGenRenderbuffersOES(1, &m_depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES,
							 width, height);
	
	// Create the framebuffer object.
	GLuint framebuffer;
	glGenFramebuffersOES(1, &framebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, framebuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
								 GL_RENDERBUFFER_OES, m_colorRenderbuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES,
								 GL_RENDERBUFFER_OES, m_depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, m_colorRenderbuffer);
	
	// Set up various GL state.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Set up the texture state.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// Set up the material properties.
	vec4 specular(0.5f, 0.5f, 0.5f, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular.Pointer());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0f);
	
	m_translation = mat4::Translate(0, 0, -7);
}


void RenderingEngine::Clear() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//void RenderingEngine1::OnFingerUp(ivec2 location)
//{
//    m_scale = 1.0f;
//}
//
//void RenderingEngine1::OnFingerDown(ivec2 location)
//{
//    m_scale = 1.5f;
//    OnFingerMove(location, location);
//}
//
//void RenderingEngine1::OnFingerMove(ivec2 previous, ivec2 location)
//{
//    vec2 direction = vec2(location - m_pivotPoint).Normalized();
//
//    // Flip the Y axis because pixel coords increase towards the bottom.
//    direction.y = -direction.y;
//
//    m_rotationAngle = std::acos(direction.y) * 180.0f / 3.14159f;
//    if (direction.x > 0)
//        m_rotationAngle = -m_rotationAngle;
//}

//GLvoid RenderingEngine::Resize(GLsizei width, GLsizei height) {
//	if (height==0)								
//		height=1;
//
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
//	glLoadIdentity(); // Reset The Projection Matrix
//
//	// Calculate The Aspect Ratio Of The Window
//	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
//
//	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
//	glLoadIdentity(); // Reset The Modelview Matrix
//}

GLuint RenderingEngine::CreateTexture(const unsigned long* data){

    PVRTextureHeader* header = (PVRTextureHeader*) data;
    GLsizei w = header->Width;
    GLsizei h = header->Height;
    const unsigned long* texels = data + header->HeaderSize / 4;

    GLuint name;
    glGenTextures(1, &name);
    glBindTexture(GL_TEXTURE_2D, name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    switch (header->Flags & PVRTEX_PIXELTYPE) {
        case OGL_I_8: {
            GLenum format = GL_ALPHA;
            GLenum type = GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        case OGL_AI_88: {
            GLenum format = GL_LUMINANCE_ALPHA;
            GLenum type = GL_UNSIGNED_BYTE;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        case OGL_RGB_565: {
            GLenum format = GL_RGB;
            GLenum type = GL_UNSIGNED_SHORT_5_6_5;
            glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, type, texels);
            break;
        }
        default: {
            w = h = 512;
            GLsizei size = max(32, w * h / 2);
            GLenum format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, size, data);
            break;
        }
    }

    return name;
}


void RenderingEngine::SetPngTexture(const string& filename) const
{
	TextureDescription description = m_resourceManager->LoadImage(filename);
	//TextureDescription description = m_resourceManager->GenerateCircle();
	
	GLenum format;
	switch (description.Format) {
		case TextureFormatGray:      format = GL_LUMINANCE;       break;
		case TextureFormatGrayAlpha: format = GL_LUMINANCE_ALPHA; break;
		case TextureFormatRgb:       format = GL_RGB;             break;
		case TextureFormatRgba:      format = GL_RGBA;            break;
	}
	
	GLenum type;
	switch (description.BitsPerComponent) {
		case 8: type = GL_UNSIGNED_BYTE; break;
		case 4:
			if (format == GL_RGBA) {
				type = GL_UNSIGNED_SHORT_4_4_4_4;
				break;
			}
			// intentionally fall through
		default:
			assert(!"Unsupported format.");
	}
	
	void* data = m_resourceManager->GetImageData();
	ivec2 size = description.Size;
	glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, data);
	m_resourceManager->UnloadImage();
}

void RenderingEngine::SetPvrTexture(const string& filename) const
{
	TextureDescription description = m_resourceManager->LoadPvrImage(filename);
	unsigned char* data = (unsigned char*) m_resourceManager->GetImageData();
	int width = description.Size.x;
	int height = description.Size.y;
	
	int bitsPerPixel;
	GLenum format;
	bool compressed = true;
	switch (description.Format) {
		case TextureFormatPvrtcRgba2:
			bitsPerPixel = 2;
			format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			break;
		case TextureFormatPvrtcRgb2:
			bitsPerPixel = 2;
			format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
			break;
		case TextureFormatPvrtcRgba4:
			bitsPerPixel = 4;
			format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
			break;
		case TextureFormatPvrtcRgb4:
			bitsPerPixel = 4;
			format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
			break;
		default:
			compressed = false;
			break;
	}
	
	if (compressed) {
		for (int level = 0; level < description.MipCount; ++level) {
			GLsizei size = max(32, width * height * bitsPerPixel / 8);
			glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, data);
			data += size;
			width >>= 1; height >>= 1;
		}
	} else {
		GLenum type;
		switch (description.Format) {
			case TextureFormatRgba:
				assert(description.BitsPerComponent == 4);
				format = GL_RGBA;
				type = GL_UNSIGNED_SHORT_4_4_4_4;
				bitsPerPixel = 16;
				break;
			case TextureFormat565:
				format = GL_RGB;
				type = GL_UNSIGNED_SHORT_5_6_5;
				bitsPerPixel = 16;
				break;
			case TextureFormat5551:
				format = GL_RGBA;
				type = GL_UNSIGNED_SHORT_5_5_5_1;
				bitsPerPixel = 16;
				break;
		}
		for (int level = 0; level < description.MipCount; ++level) {
			glTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, format, type, data);
			GLsizei size = width * height * bitsPerPixel / 8;
			data += size;
			width >>= 1; height >>= 1;
		}
	}
	
	m_resourceManager->UnloadImage();
}
   
}
}
}

#endif

// RENDER

    //// Load up the sprite coordinates:
    // m_frameCount = 0;//sizeof(BodyLayerBoxes) / sizeof(box2);
    // m_frameIndex = 0;
    //NoopSprite sprite = m_unifiedFrames[m_frameIndex];
    ////NoopSprite sprite = m_noopFrames[m_frameIndex];

    //float width = sprite.Body.width;
    //float height = sprite.Body.height;
    //float x = 160 - width / 2;
    //float y = 240 - height / 2;
    //
    //int bodyRectangle[] = {
    //    sprite.Body.x,
    //    sprite.Body.y,
    //    sprite.Body.width,
    //    sprite.Body.height };
    //
    //int eyesRectangle[] = {
    //    sprite.Eyes.x,
    //    sprite.Eyes.y,
    //    sprite.Eyes.width,
    //    sprite.Eyes.height };
    //
    //int backgroundRectangle[] = { 0, 0, 480, 320 };

    //glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_TEXTURE_2D);

    //// Draw background:
    //glColor4f(1, 1, 1, 1);
    //glDisable(GL_BLEND);
    //glBindTexture(GL_TEXTURE_2D, m_textures.Background);
    //glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, backgroundRectangle);
    //glDrawTexfOES(0, 0, 0, 320, 480);
    //
    //if (0) {

    //    // TWO PASSES WITH TWO TEXTURES

    //    // Enable Blending:
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //    // Draw Noop's body in a yellowish hue:
    //    glColor4f(1, 0.83f, 0.33f, 1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Body);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);
    //    glDrawTexfOES(x, y, 0, width, height);

    //    // Draw Noop's eyes in white:
    //    glColor4f(1, 1, 1, 1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Eyes);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);
    //    glDrawTexfOES(x, y, 0, width, height);

    //} else if (0) {
    //    
    //    // TWO PASSES WITH ONE TEXTURE

    //    glEnable(GL_BLEND);
    //    
    //    // Draw Noop's body:
    //    glColor4f(1, 0.83f, 0.33f, 1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Unified);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);
    //    glDrawTexfOES(x, y, 0, width, height);
    //    
    //    // Draw Noop's eyes:
    //    glColor4f(1, 1, 1, 1);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Unified);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, eyesRectangle);
    //    glDrawTexfOES(x, y, 0, width, height);
    //    
    //} else if (0) {
    //    
    //    // MULTITEXTURING WITH THE SAME TEXTURE APPLIED TO BOTH STAGES
    //    
    //    // This exposes a driver bug.  The driver does not account for
    //    // unique crop rectangles at each stage.
    //    
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //    glColor4f(1, 0.83f, 0.33f, 1);

    //    glActiveTexture(GL_TEXTURE0);
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Unified);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);

    //    glActiveTexture(GL_TEXTURE1);
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Unified);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, eyesRectangle);
    //    
    //    // Render Noop's body and eyes with one draw call:
    //    glDrawTexfOES(x, y, 0, width, height);
    //    
    //    // Disable texturing in both stages:
    //    glActiveTexture(GL_TEXTURE0);
    //    glDisable(GL_TEXTURE_2D);
    //    glActiveTexture(GL_TEXTURE1);
    //    glDisable(GL_TEXTURE_2D);
    //    
    //} else {
    //    
    //    // MULTITEXTURING WITH DIFFERENT TEXTURES APPLIED TO EACH STAGE,
    //    // BUT WITH THE CROP RECTANGLE BEING THE SAME ACROSS BOTH STAGES.
    //    
    //    glColor4f(1, 0.83f, 0.33f, 1);
    //    glEnable(GL_BLEND);

    //    glActiveTexture(GL_TEXTURE0);
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Body);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);
    //    
    //    glActiveTexture(GL_TEXTURE1);
    //    glEnable(GL_TEXTURE_2D);
    //    glBindTexture(GL_TEXTURE_2D, m_textures.Eyes);
    //    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, bodyRectangle);

    //    // Render Noop's body and eyes with one draw call:
    //    glDrawTexfOES(x, y, 0, width, height);

    //    // Disable texturing in both stages:
    //    glActiveTexture(GL_TEXTURE0);
    //    glDisable(GL_TEXTURE_2D);
    //    glActiveTexture(GL_TEXTURE1);
    //    glDisable(GL_TEXTURE_2D);

    //}

// INIT

 //m_noopFrames.resize(m_frameCount);
    //m_unifiedFrames.resize(m_frameCount);

    //const float* pBody = BodyLayerBoxes;
    //const float* pEyes = EyeLayerBoxes;
    //for (size_t i = 0; i < m_frameCount; ++i) {
    //    float l, t, r, b;
    //    
    //    l = *pBody++;
    //    t = *pBody++;
    //    r = *pBody++;
    //    b = *pBody++;
    //    m_noopFrames[i].Body = box2::FromLeftTopRightBottom(l, t, r, b);
    //    m_noopFrames[i].Body.FlipY(512);

    //    l = *pEyes++;
    //    t = *pEyes++;
    //    r = *pEyes++;
    //    b = *pEyes++;
    //    m_noopFrames[i].Eyes  = box2::FromLeftTopRightBottom(l, t, r, b);
    //    m_noopFrames[i].Eyes.FlipY(512);
    //}
    
    //pBody = UnifiedBodyLayerBoxes;
    //pEyes = UnifiedEyeLayerBoxes;
    //for (size_t i = 0; i < m_frameCount; ++i) {
    //    float l, t, r, b;
    //    
    //    l = *pBody++;
    //    t = *pBody++;
    //    r = *pBody++;
    //    b = *pBody++;
    //    m_unifiedFrames[i].Body = box2::FromLeftTopRightBottom(l, t, r, b);
    //    m_unifiedFrames[i].Body.FlipY(512);
    //    
    //    l = *pEyes++;
    //    t = *pEyes++;
    //    r = *pEyes++;
    //    b = *pEyes++;
    //    m_unifiedFrames[i].Eyes  = box2::FromLeftTopRightBottom(l, t, r, b);
    //    m_unifiedFrames[i].Eyes.FlipY(512);
    //}
    
    // Load up some textures:
    //m_textures.Tile = CreateTexture(Tile);
    //m_textures.Eyes = CreateTexture(EyesLayer);
    //m_textures.Body = CreateTexture(BodyLayer);
    //m_textures.Unified = CreateTexture(Unified);
    //m_textures.Background = CreateTexture(_Background_pvrtc);

    // Extract width and height from the color buffer:
    //ivec2 screenSize;
    //glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
    //                                GL_RENDERBUFFER_WIDTH_OES, &screenSize.x);
    //glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES,
    //                                GL_RENDERBUFFER_HEIGHT_OES, &screenSize.y);
    //
    //// Create the on-screen FBO:
    //glGenFramebuffersOES(1, &m_framebuffers.Screen);
    //glBindFramebufferOES(GL_FRAMEBUFFER_OES, m_framebuffers.Screen);
    //glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES,
    //                             GL_RENDERBUFFER_OES, m_renderbuffers.Screen);
    //
    //// Set up various GL state:
    //glViewport(0, 0, screenSize.x, screenSize.y);
    //glEnable(GL_TEXTURE_2D);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //
    //// Set up the second texture stage for typical alpha blending:
    //glActiveTexture(GL_TEXTURE1);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
    //glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC2_RGB, GL_TEXTURE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);
    //glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
    //glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_PREVIOUS);
    //glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
    //glActiveTexture(GL_TEXTURE0);