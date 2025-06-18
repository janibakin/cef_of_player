#include "ofApp.h"

ofApp::ofApp(CefMainArgs args) : mainArgs(args) {}

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofBackground(0);

    // --- Initialize CEF ---
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    settings.log_severity = LOGSEVERITY_DISABLE;
    CefInitialize(mainArgs, settings, nullptr, nullptr);

    int w = ofGetWidth();
    int h = ofGetHeight() / 2; // use top half for browser

    client = new SimpleClient(w, h, [this](const void* buf, int bw, int bh) {
        std::scoped_lock lock(texMutex);
        if (!tex.isAllocated()) tex.allocate(bw, bh, GL_RGBA);
        tex.loadData((const unsigned char*)buf, bw, bh, GL_BGRA);
    });

    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 15; // lower FPS to reduce load

    std::string localPath = ofToDataPath("bouncing_ball.html", true);
    std::string url = "file://" + localPath;

    browser = CefBrowserHost::CreateBrowserSync(window_info, client, url, browser_settings, nullptr, nullptr);

    // --- Load video (defer play to update()) ---
    if (video.load("video.mp4")) {
        video.setLoopState(OF_LOOP_NORMAL);
    } else {
        ofLogError() << "Failed to load video file: video.mp4";
    }

    cefShutdown = false;
}

void ofApp::update() {
    if (cefShutdown) return;

    CefDoMessageLoopWork();

    if (!video.isPlaying() && video.isLoaded()) {
        video.play();
    }
    video.update();
}

void ofApp::draw() {
    if (cefShutdown) return;

    int w = ofGetWidth();
    int h = ofGetHeight();

    {
        std::scoped_lock lock(texMutex);
        if (tex.isAllocated()) tex.draw(0, 0, w, h / 2);
    }

    if (video.isLoaded()) {
        float videoW = video.getWidth();
        float videoH = video.getHeight();
        float targetW = w;
        float targetH = h / 2;
        float scale = std::min(targetW / videoW, targetH / videoH);
        float drawW = videoW * scale;
        float drawH = videoH * scale;
        float drawX = (targetW - drawW) * 0.5f;
        float drawY = h / 2 + (targetH - drawH) * 0.5f;
        video.draw(drawX, drawY, drawW, drawH);
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
    cefShutdown = true;

    if (video.isLoaded()) {
        video.stop();
    }

    if (browser) {
        browser->GetHost()->CloseBrowser(true);
        browser = nullptr;
    }

    CefShutdown();
}
