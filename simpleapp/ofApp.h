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
    void exit() override;
    void windowResized(int w, int h) override;

private:
    CefMainArgs mainArgs;
    CefRefPtr<SimpleClient> client;
    CefRefPtr<CefBrowser> browser;
    ofTexture tex;
    std::mutex texMutex;
    ofVideoPlayer video;
    bool cefShutdown {false};
};
