#pragma once
#include "ofMain.h"
#include "include/cef_app.h"
#include "SimpleClient.h"
#include <mutex>
#include <vector>

class ofApp : public ofBaseApp {
public:
    ofApp(const CefMainArgs& args, CefRefPtr<CefApp> cefApp);

    void setup()             override;
    void update()            override;
    void draw()              override;
    void windowResized(int, int) override;
    void exit()              override;

private:
    // ---- CEF ----
    CefMainArgs               mainArgs;
    CefRefPtr<CefApp>         cefApp;     // holds MyCefApp
    CefRefPtr<SimpleClient>   client;
    CefRefPtr<CefBrowser>     browser;

    // pixel upload
    std::mutex                texMtx;
    std::vector<uint8_t>      staging;
    bool                      dirty = false;
    ofTexture                 cefTex;

    // ---- video ----
    ofVideoPlayer             video;

    bool                      shuttingDown = false;
};
