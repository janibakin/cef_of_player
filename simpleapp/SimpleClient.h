#pragma once
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include <functional>

class SimpleClient : public CefClient,
                     public CefRenderHandler {
public:
    using PaintCB = std::function<void(const void*, int, int)>;

    SimpleClient(int w, int h, PaintCB cb)
    : width(w), height(h), paintCallback(std::move(cb)) {}

    // CefClient
    CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

    // CefRenderHandler
    void GetViewRect(CefRefPtr<CefBrowser>, CefRect& r) override {
        r = CefRect(0,0,width,height);
    }

    void OnPaint(CefRefPtr<CefBrowser>, PaintElementType,
                 const RectList&, const void* buffer, int w, int h) override {
        paintCallback(buffer, w, h);        // copy only – no OpenGL!
    }

    void Resize(int w,int h){ width=w; height=h; }
    int  getWidth()  const { return width; }
    int  getHeight() const { return height; }

private:
    int      width, height;
    PaintCB  paintCallback;
    IMPLEMENT_REFCOUNTING(SimpleClient);
};
