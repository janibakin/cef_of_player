#pragma once

#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include <functional>

class SimpleClient : public CefClient, public CefRenderHandler {
public:
    SimpleClient(int w, int h, std::function<void(const void*, int, int)> cb)
        : width(w), height(h), paintCallback(cb) {}

    CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
        rect = CefRect(0, 0, width, height);
    }

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type,
                 const CefRenderHandler::RectList& dirtyRects,
                 const void* buffer, int w, int h) override {
        std::cout << "OnPaint called with buffer size: " << w << "x" << h << std::endl;
        paintCallback(buffer, w, h);
    }

    void Resize(int w, int h) {
        width = w;
        height = h;
    }

private:
    int width, height;
    std::function<void(const void*, int, int)> paintCallback;

    IMPLEMENT_REFCOUNTING(SimpleClient);
};
