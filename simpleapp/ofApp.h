#pragma once
#include "ofMain.h"
#include "include/cef_app.h"
#include "simple_client.h"
#include <mutex>

class ofApp : public ofBaseApp {
public:
    explicit ofApp(CefMainArgs args);

    void setup() override;
    void update() override;
    void draw() override;
    void windowResized(int w, int h) override;
    void exit() override;

private:
    // CEF
    CefMainArgs mainArgs;
    CefRefPtr<SimpleClient> client;
    CefRefPtr<CefBrowser>   browser;
    ofTexture               cefTex;
    std::mutex              texMtx;
    std::thread             cefLoopThread;
    std::atomic<bool>       runCefLoop {true};

    // Video
    ofVideoPlayer           video;

    bool cefShutdown {false};
};