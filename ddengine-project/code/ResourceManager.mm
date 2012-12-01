#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include "DDHeader.h"
#include "IRenderingEngine.h"
#include "IResourceManager.h"
#include "ISurface.h"
#include "PVRTTexture.h"

#ifdef _IPHONE
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#endif

using namespace std;
using namespace dd;
using namespace dd::asset;
using namespace dd::render;

namespace dd {
namespace asset {

class ResourceManager : public IResourceManager {
public:
    ResourceManager()
    {
#ifdef _IPHONE
        m_imageData = 0;
#endif
    }
    string GetResourcePath() const
    {
#ifdef _IPHONE
        NSString* bundlePath =[[NSBundle mainBundle] resourcePath];
        return [bundlePath UTF8String];
#endif
    }
    TextureDescription LoadImage(const string& file)
    {
        TextureDescription description;
#ifdef _IPHONE
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
                
        description.Size.x = CGImageGetWidth(uiImage.CGImage);
        description.Size.y = CGImageGetHeight(uiImage.CGImage);
        description.BitsPerComponent = 8;
        description.Format = TextureFormatRgba;
        description.MipCount = 1;
        m_hasPvrHeader = false;

        int bpp = description.BitsPerComponent / 2;
        int byteCount = description.Size.x * description.Size.y * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
            description.Size.x,
            description.Size.y,
            description.BitsPerComponent,
            bpp * description.Size.x,
            colorSpace,
            bitmapInfo);
        CGColorSpaceRelease(colorSpace);
        CGRect rect = CGRectMake(0, 0, description.Size.x, description.Size.y);
        CGContextDrawImage(context, rect, uiImage.CGImage);
        CGContextRelease(context);
        
        m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
#endif
        return description;
    }
    TextureDescription GenerateCircle()
    {
        TextureDescription description;
        description.Size = ivec2(256, 256);
        description.BitsPerComponent = 8;
        description.Format = TextureFormatRgba;
#ifdef _IPHONE
        int bpp = description.BitsPerComponent / 2;
        int byteCount = description.Size.x * description.Size.y * bpp;
        unsigned char* data = (unsigned char*) calloc(byteCount, 1);
        
        CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
        CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big;
        CGContextRef context = CGBitmapContextCreate(data,
                                                     description.Size.x,
                                                     description.Size.y,
                                                     description.BitsPerComponent,
                                                     bpp * description.Size.x,
                                                     colorSpace,
                                                     bitmapInfo);
        CGColorSpaceRelease(colorSpace);
                
        CGRect rect = CGRectMake(5, 5, 246, 246);
        CGContextSetRGBFillColor(context, 0, 0, 1, 1);
        CGContextFillEllipseInRect(context, rect);

        CGContextRelease(context);
        
        m_imageData = [NSData dataWithBytesNoCopy:data length:byteCount freeWhenDone:YES];
#endif
        return description;
    }
    TextureDescription LoadPngImage(const string& file)
    {
        TextureDescription description;
#ifdef _IPHONE
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];

        NSLog(@"Loading PNG image %s...", fullPath);

        UIImage* uiImage = [UIImage imageWithContentsOfFile:fullPath];
        CGImageRef cgImage = uiImage.CGImage;

        CFDataRef dataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
        m_imageData = [NSData dataWithData:(NSData*) dataRef];
        CFRelease(dataRef);
        
        m_hasPvrHeader = false;

        description.Size.x = CGImageGetWidth(cgImage);
        description.Size.y = CGImageGetHeight(cgImage);
        bool hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
        switch (CGColorSpaceGetModel(colorSpace)) {
            case kCGColorSpaceModelMonochrome:
                description.Format = hasAlpha ? TextureFormatGrayAlpha : TextureFormatGray;
                break;
            case kCGColorSpaceModelRGB:
                description.Format = hasAlpha ? TextureFormatRgba : TextureFormatRgb;
                break;
            default:
                assert(!"Unsupported color space.");
                break;
        }
        description.BitsPerComponent = CGImageGetBitsPerComponent(cgImage);
#endif
        return description;
    }
    TextureDescription LoadPvrImage(const string& file)
    {
        TextureDescription description;
#ifdef _IPHONE
        NSString* basePath = [NSString stringWithUTF8String:file.c_str()];
        NSString* resourcePath = [[NSBundle mainBundle] resourcePath];
        NSString* fullPath = [resourcePath stringByAppendingPathComponent:basePath];
        
        m_imageData = [NSData dataWithContentsOfFile:fullPath];
        m_hasPvrHeader = true;
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        bool hasAlpha = header->dwAlphaBitMask ? true : false;

        switch (header->dwpfFlags & PVRTEX_PIXELTYPE) {
            case OGL_RGB_565:
                description.Format = TextureFormat565;
                break;
            case OGL_RGBA_5551:
                description.Format = TextureFormat5551;
                break;
            case OGL_RGBA_4444:
                description.Format = TextureFormatRgba;
                description.BitsPerComponent = 4;
                break;
            case OGL_PVRTC2:    
                description.Format = hasAlpha ? TextureFormatPvrtcRgba2 :
                                                TextureFormatPvrtcRgb2;
                break;
            case OGL_PVRTC4:
                description.Format = hasAlpha ? TextureFormatPvrtcRgba4 :
                                                TextureFormatPvrtcRgb4;
                break;
            default:
                assert(!"Unsupported PVR image.");
                break;
        }
        
        description.Size.x = header->dwWidth;
        description.Size.y = header->dwHeight;
        description.MipCount = header->dwMipMapCount;
#endif
        return description;
    }
    void* GetImageData()
    {
#ifdef _IPHONE
        if (!m_hasPvrHeader)
            return (void*) [m_imageData bytes];
        
        PVR_Texture_Header* header = (PVR_Texture_Header*) [m_imageData bytes];
        char* data = (char*) [m_imageData bytes];
        unsigned int headerSize = header->dwHeaderSize;
        return data + headerSize;
#endif
    }
    void UnloadImage()
    {
#ifdef _IPHONE
        m_imageData = 0;
#endif
    }
private:
#ifdef _IPHONE
    NSData* m_imageData;
#endif
    bool m_hasPvrHeader;
};

IResourceManager* CreateResourceManager()
{
    return new ResourceManager();
}

}
}

#endif
