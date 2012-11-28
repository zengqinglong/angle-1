//
// Copyright (c) 2012 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Renderer.h: Defines a back-end specific class that hides the details of the
// implementation-specific renderer.

#ifndef LIBGLESV2_RENDERER_RENDERER_H_
#define LIBGLESV2_RENDERER_RENDERER_H_

#define GL_APICALL
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#define EGLAPI
#include <EGL/egl.h>

#include "libGLESv2/Texture.h"
#include "libGLESv2/angletypes.h"

const int versionWindowsVista = MAKEWORD(0x00, 0x06);
const int versionWindows7 = MAKEWORD(0x01, 0x06);

// Return the version of the operating system in a format suitable for ordering
// comparison.
inline int getComparableOSVersion()
{
    DWORD version = GetVersion();
    int majorVersion = LOBYTE(LOWORD(version));
    int minorVersion = HIBYTE(LOWORD(version));
    return MAKEWORD(minorVersion, majorVersion);
}

namespace egl
{
class Display;
}

namespace rx
{
class TextureStorage2D;
class TextureStorageCubeMap;
class Blit;

struct ConfigDesc
{
    GLenum  renderTargetFormat;
    GLenum  depthStencilFormat;
    GLint   multiSample;
    bool    fastConfig;
};

class Renderer
{
  public:
    explicit Renderer(egl::Display *display) : mDisplay(display) {};
    virtual ~Renderer() {};

    virtual EGLint initialize() = 0;
    virtual bool resetDevice() = 0;

    virtual int generateConfigs(ConfigDesc **configDescList) = 0;
    virtual void deleteConfigs(ConfigDesc *configDescList) = 0;

    virtual void startScene() = 0;
    virtual void endScene() = 0;

    virtual void sync(bool block) = 0;

    virtual SwapChain *createSwapChain(HWND window, HANDLE shareHandle, GLenum backBufferFormat, GLenum depthBufferFormat) = 0;

    virtual void setSamplerState(gl::SamplerType type, int index, const gl::SamplerState &sampler) = 0;
    virtual void setTexture(gl::SamplerType type, int index, gl::Texture *texture) = 0;

    virtual void setRasterizerState(const gl::RasterizerState &rasterState, unsigned int depthSize) = 0;
    virtual void setBlendState(const gl::BlendState &blendState, const gl::Color &blendColor,
                               unsigned int sampleMask) = 0;
    virtual void setDepthStencilState(const gl::DepthStencilState &depthStencilState, bool frontFaceCCW,
                                      unsigned int stencilSize) = 0;

    virtual void setScissorRectangle(const gl::Rectangle& scissor, unsigned int renderTargetWidth,
                                     unsigned int renderTargetHeight) = 0;

    virtual void applyRenderTarget(gl::Framebuffer *frameBuffer) = 0;

    virtual void clear(GLbitfield mask, const gl::Color &colorClear, float depthClear, int stencilClear,
                       gl::Framebuffer *frameBuffer) = 0;

    // lost device
    virtual void markDeviceLost() = 0;
    virtual bool isDeviceLost() = 0;
    virtual bool testDeviceLost(bool notify) = 0;
    virtual bool testDeviceResettable() = 0;

    // Renderer capabilities
    virtual DWORD getAdapterVendor() const = 0;
    virtual const char *getAdapterDescription() const = 0;
    virtual GUID getAdapterIdentifier() const = 0;

    virtual bool getDXT1TextureSupport() = 0;
    virtual bool getDXT3TextureSupport() = 0;
    virtual bool getDXT5TextureSupport() = 0;
    virtual bool getEventQuerySupport() = 0;
    virtual bool getFloat32TextureSupport(bool *filtering, bool *renderable) = 0;
    virtual bool getFloat16TextureSupport(bool *filtering, bool *renderable) = 0;
    virtual bool getLuminanceTextureSupport() = 0;
    virtual bool getLuminanceAlphaTextureSupport() = 0;
    virtual bool getVertexTextureSupport() const = 0;
    virtual bool getNonPower2TextureSupport() const = 0;
    virtual bool getDepthTextureSupport() const = 0;
    virtual bool getOcclusionQuerySupport() const = 0;
    virtual bool getInstancingSupport() const = 0;
    virtual bool getTextureFilterAnisotropySupport() const = 0;
    virtual float getTextureMaxAnisotropy() const = 0;
    virtual bool getShareHandleSupport() const = 0;

    virtual bool getShaderModel3Support() const = 0;
    virtual float getMaxPointSize() const = 0;
    virtual int getMaxTextureWidth() const = 0;
    virtual int getMaxTextureHeight() const = 0;
    virtual bool get32BitIndexSupport() const = 0;
    virtual int getMinSwapInterval() const = 0;
    virtual int getMaxSwapInterval() const = 0;

    virtual GLsizei getMaxSupportedSamples() const = 0;

    virtual bool copyToRenderTarget(TextureStorage2D *dest, TextureStorage2D *source) = 0;
    virtual bool copyToRenderTarget(TextureStorageCubeMap *dest, TextureStorageCubeMap *source) = 0;

    virtual bool copyImage(gl::Framebuffer *framebuffer, const RECT &sourceRect, GLenum destFormat,
                           GLint xoffset, GLint yoffset, TextureStorage2D *storage, GLint level) = 0;
    virtual bool copyImage(gl::Framebuffer *framebuffer, const RECT &sourceRect, GLenum destFormat,
                           GLint xoffset, GLint yoffset, TextureStorageCubeMap *storage, GLenum target, GLint level) = 0;

  protected:
    egl::Display *mDisplay;

  private:
    DISALLOW_COPY_AND_ASSIGN(Renderer);

};

}
#endif // LIBGLESV2_RENDERER_RENDERER_H_