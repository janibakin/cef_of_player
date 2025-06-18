#include "ofApp.h"
#include "cefApp.h"

ofApp::ofApp(const CefMainArgs& a, CefRefPtr<CefApp> ca)
    : mainArgs(a), cefApp(std::move(ca)) {}

void ofApp::setup() {
    ofBackground(27, 27, 27);

    CefSettings s;
    s.no_sandbox = true;
    s.windowless_rendering_enabled = true;
    s.multi_threaded_message_loop = false;
    s.external_message_pump = true;
    CefInitialize(mainArgs, s, cefApp, nullptr);

    int w = ofGetWidth();
    int h = ofGetHeight() / 2;

    client = new SimpleClient(w, h, [this](const void* buf, int bw, int bh) {
        std::lock_guard<std::mutex> lk(texMtx);
        staging.assign(static_cast<const uint8_t*>(buf),
                       static_cast<const uint8_t*>(buf) + bw * bh * 4);
        dirty = true;
    });

    CefWindowInfo winfo;
    winfo.SetAsWindowless(0);
    CefBrowserSettings bset;
    bset.windowless_frame_rate = 60;

    std::string localHtml = ofToDataPath("index.html", true);
    std::string url = "file://" + localHtml;
    browser = CefBrowserHost::CreateBrowserSync(winfo, client, url, bset, nullptr, nullptr);

    if (video.load("video.mp4")) {
        video.setLoopState(OF_LOOP_NORMAL);
        video.play();
    } else {
        ofLogError() << "Failed to load video.mp4";
    }
}

void ofApp::update() {
    if (shuttingDown) return;

    auto* app = static_cast<MyCefApp*>(cefApp.get());
    if (app->shouldPumpNow()) {
        CefDoMessageLoopWork();
    }

    video.update();

    if (dirty) {
        std::lock_guard<std::mutex> lk(texMtx);

        const int w = client->getWidth();
        const int h = client->getHeight();

        if (!cefTex.isAllocated() || cefTex.getWidth() != w || cefTex.getHeight() != h) {
            cefTex.allocate(w, h, GL_RGBA);
        }

        if (staging.size() >= static_cast<size_t>(w * h * 4)) {
            cefTex.loadData(staging.data(), w, h, GL_BGRA);
        } else {
            ofLogError() << "Staging buffer too small! Skipping loadData.";
        }

        dirty = false;
    }
}

void ofApp::draw() {
    int w = ofGetWidth();
    int h = ofGetHeight();
    int half = h / 2;

    ofSetColor(27, 27, 47);
    ofDrawRectangle(0, 0, w, h);

    int padding = 10;
    ofSetColor(255);
    ofDrawRectangle(padding - 4, padding - 4, w - 2 * padding + 8, half - 2 * padding + 8);

    std::lock_guard<std::mutex> lk(texMtx);
    if (cefTex.isAllocated()) {
        ofSetColor(255);
        cefTex.draw(padding, padding, w - 2 * padding, half - 2 * padding);
    }

    if (video.isLoaded()) {
        ofRectangle src(0, 0, video.getWidth(), video.getHeight());
        ofRectangle dst(0, half, w, half);
        src.scaleTo(dst, OF_SCALEMODE_FIT);
        ofSetColor(255);
        video.draw(src);
    }
}

void ofApp::windowResized(int w, int h) {
    if (browser) {
        browser->GetHost()->NotifyMoveOrResizeStarted();
        client->Resize(w, h / 2);
        browser->GetHost()->WasResized();
    }
}

void ofApp::exit() {
    shuttingDown = true;

    if (video.isLoaded()) {
        video.stop();
        video.close();
    }

    if (browser) {
        browser->GetHost()->CloseBrowser(true);
        browser = nullptr;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    CefShutdown();
}