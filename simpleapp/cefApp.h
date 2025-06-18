#pragma once

#include "ofMain.h"
#include "ofApp.h"
#include "include/cef_app.h"
#include <atomic>

class MyCefApp : public CefApp, public CefBrowserProcessHandler {
 public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    void OnBeforeCommandLineProcessing(const CefString&, CefRefPtr<CefCommandLine> cmd) override {
        cmd->AppendSwitch("disable-gpu");
        cmd->AppendSwitch("disable-gpu-compositing");
        cmd->AppendSwitch("disable-software-rasterizer");
        cmd->AppendSwitch("enable-begin-frame-scheduling");
        cmd->AppendSwitch("enable-media-stream");
    }

    void OnScheduleMessagePumpWork(int64_t delay_ms) override {
        auto next = std::chrono::steady_clock::now() + std::chrono::milliseconds(delay_ms);
        next_pump_.store(next, std::memory_order_relaxed);
    }

    bool shouldPumpNow() const {
        auto now = std::chrono::steady_clock::now();
        auto next = next_pump_.load(std::memory_order_relaxed);
        return now >= next;
    }

    IMPLEMENT_REFCOUNTING(MyCefApp);

 private:
    std::atomic<std::chrono::steady_clock::time_point> next_pump_{
        std::chrono::steady_clock::now()
    };
};
