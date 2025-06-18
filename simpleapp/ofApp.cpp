#include "ofApp.h"

ofApp::ofApp(CefMainArgs args) : mainArgs(args) {}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(0);

    //----- Init CEF off‑screen --------------------------------
    // (GPU switches are now injected from MyCefApp in main.cpp)
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    settings.multi_threaded_message_loop = false;

    CefInitialize(mainArgs, settings, nullptr, nullptr);
    cefLoopThread = std::thread([this]() {
        while (runCefLoop.load()) {
            CefDoMessageLoopWork();
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // ~100 fps
        }
    });

    int w = ofGetWidth();
    int h = ofGetHeight() / 2;   // top half for CEF

    client = new SimpleClient(w, h,
        [this](const void* buf, int bw, int bh)
        {
            std::scoped_lock lock(texMtx);
            if (!cefTex.isAllocated()) {
                std::cout << "[CEF] Allocating texture for " << bw << "x" << bh << std::endl;
                cefTex.allocate(bw, bh, GL_RGBA);
            }
            cefTex.loadData(static_cast<const unsigned char*>(buf), bw, bh, GL_BGRA);
        });

    CefWindowInfo winfo;
    winfo.SetAsWindowless(0);

    CefBrowserSettings bsettings;
    bsettings.windowless_frame_rate = 30;

    // std::string localHtml = ofToDataPath("bouncing_ball.html", true);
    // std::string url = "file://" + localHtml;
    std::string url {"https://shadertoy.com"};
    std::cout << "Loading URL: " << url << std::endl;
    browser = CefBrowserHost::CreateBrowserSync(winfo, client, url, bsettings, nullptr, nullptr);
    browser->GetHost()->WasResized();
    browser->GetHost()->Invalidate(PET_VIEW);


    //----- Load video -----------------------------------------
    if (!video.load("video.mp4"))
        ofLogError() << "Failed to load video.mp4";
    else {
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (cefShutdown) return;
    // CefDoMessageLoopWork();

    // CEF now runs its own message‑pump threads (multi_threaded = true)
    video.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    int w = ofGetWidth();
    int h = ofGetHeight();
    int halfH = h / 2;

    // Draw CEF buffer (upper half)
    {
        std::scoped_lock lock(texMtx);
        if (cefTex.isAllocated())
            cefTex.draw(0, 0, w, halfH);
    }

    // Draw video (original scale, centered bottom half)
    if (video.isLoaded()) {
        float vw = video.getWidth();
        float vh = video.getHeight();
        float scale = std::min(static_cast<float>(w) / vw, static_cast<float>(halfH) / vh);
        float dw = vw * scale;
        float dh = vh * scale;
        float dx = (w - dw) * 0.5f;
        float dy = halfH + (halfH - dh) * 0.5f;
        video.draw(dx, dy, dw, dh);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    if (browser) {
        browser->GetHost()->NotifyMoveOrResizeStarted();
        client->Resize(w, h/2);
        browser->GetHost()->WasResized();
    }
}

//--------------------------------------------------------------
void ofApp::exit()
{
    cefShutdown = true;

    runCefLoop.store(false);
    if (cefLoopThread.joinable()) cefLoopThread.join();
    
    if (video.isLoaded()) video.stop();

    if (browser) {
        browser->GetHost()->CloseBrowser(true);
        browser = nullptr;
    }

    CefShutdown();
}
