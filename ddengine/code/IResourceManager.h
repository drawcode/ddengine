#ifndef _IRESOURCEMANAGER_H_
#define _IRESOURCEMANAGER_H_

#include "DDHeader.h"
#include "ISurface.h";

using namespace std;

using namespace dd;
using namespace render;

namespace dd {
namespace asset {

struct IResourceManager {
    virtual string GetResourcePath() const = 0;
    virtual TextureDescription LoadPngImage(const string& filename) = 0;
    virtual TextureDescription LoadPvrImage(const string& filename) = 0;
    virtual TextureDescription LoadImage(const string& filename) = 0;
    virtual TextureDescription GenerateCircle() = 0;
    virtual void* GetImageData() = 0;
    virtual void UnloadImage() = 0;
    virtual ~IResourceManager() {}
};
	
/*
 class ResourceManager : public IResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	string GetResourcePath() const;
	TextureDescription LoadImage(const string& file);
	TextureDescription GenerateCircle();
	TextureDescription LoadPngImage(const string& file);
	TextureDescription LoadPvrImage(const string& file);
	void* GetImageData();
	void UnloadImage();

private:
#ifdef _IPHONE
	NSData* m_imageData;
#endif
	bool m_hasPvrHeader;
};
*/	

IResourceManager* CreateResourceManager();

}
}

#endif