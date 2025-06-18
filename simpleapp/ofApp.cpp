#include "ofApp.h"
#include "cefApp.h"   // forward-declare or include the same header used in main

//--------------------------------------------------------------
ofApp::ofApp(const CefMainArgs& a, CefRefPtr<CefApp> ca)
: mainArgs(a), cefApp(std::move(ca)) {}

//--------------------------------------------------------------
void ofApp::setup()
{
    ofBackground(27, 27, 27);

    // ----- CEF init (single-thread + external pump) ----------
    CefSettings s;
    s.no_sandbox                   = true;
    s.windowless_rendering_enabled = true;
    s.multi_threaded_message_loop  = false;
    s.external_message_pump        = true;

    CefInitialize(mainArgs, s, cefApp, nullptr);

    // ----- create off-screen browser -------------------------
    int w = ofGetWidth();
    int h = ofGetHeight() / 2;          // top half

    client = new SimpleClient(
        w, h,
        [this](const void* buf, int bw, int bh)
        {
            std::lock_guard<std::mutex> lk(texMtx);
            staging.assign(static_cast<const uint8_t*>(buf),
                           static_cast<const uint8_t*>(buf) + bw*bh*4);
            dirty = true;
        });

    CefWindowInfo winfo;  winfo.SetAsWindowless(0);
    CefBrowserSettings bset;  bset.windowless_frame_rate = 60;

    std::string localHtml = ofToDataPath("index.html", true);
    std::string url = "file://" + localHtml;
    browser = CefBrowserHost::CreateBrowserSync(winfo, client, url, bset, nullptr, nullptr);

    // ----- video --------------------------------------------
    if(video.load("video.mp4")){
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    } else {
        ofLogError() << "Failed to load video.mp4";
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(shuttingDown) return;

    // pump when CEF requested it
    auto* app = static_cast<MyCefApp*>(cefApp.get());
    if(app->shouldPumpNow())
        CefDoMessageLoopWork();

    video.update();

    // upload BGRA → GL texture (main thread)
    if (dirty) {
        std::lock_guard<std::mutex> lk(texMtx);
        if(!cefTex.isAllocated())
            cefTex.allocate(client->getWidth(), client->getHeight(), GL_RGBA);
        cefTex.loadData(staging.data(), client->getWidth(), client->getHeight(), GL_BGRA);
        dirty = false;
    }
}

void ofApp::draw()
{
    int w = ofGetWidth(), h = ofGetHeight(), half = h / 2;

    ofSetColor(27, 27, 47);
    ofDrawRectangle(0, 0, w, h);

    {
        int padding = 10;

        ofSetColor(255);
        ofDrawRectangle(padding - 4, padding - 4, w - 2 * padding + 8, half - 2 * padding + 8);

        std::lock_guard<std::mutex> lk(texMtx);
        if (cefTex.isAllocated()) {
            ofSetColor(255);
            cefTex.draw(padding, padding, w - 2 * padding, half - 2 * padding);
        }
    }

    // --- draw video (bottom half) ---
    if (video.isLoaded()) {
        ofRectangle src(0, 0, video.getWidth(), video.getHeight());
        ofRectangle dst(0, half, w, half);
        src.scaleTo(dst, OF_SCALEMODE_FIT);
        ofSetColor(255);
        video.draw(src);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w,int h)
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
    shuttingDown = true;

    if (video.isLoaded()) {
        video.stop();
        video.close();
    }
    if (browser) {
        browser->GetHost()->CloseBrowser(true);
        browser = nullptr;
    }
    CefShutdown();
}
