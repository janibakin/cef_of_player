#include "ofMain.h"
#include "ofApp.h"
#include "include/cef_app.h"

//--------------------------------------------------
// Custom CefApp to inject command‑line switches *before*
// any CEF process (browser, renderer, GPU) starts.
//--------------------------------------------------
class MyCefApp : public CefApp {
public:
    void OnBeforeCommandLineProcessing(const CefString& process_type,
                                       CefRefPtr<CefCommandLine> command_line) override {
        command_line->AppendSwitch("disable-gpu");
        command_line->AppendSwitch("disable-gpu-compositing");
        command_line->AppendSwitch("disable-software-rasterizer");
        command_line->AppendSwitch("enable-begin-frame-scheduling");
        command_line->AppendSwitch("enable-media-stream");
    }
    IMPLEMENT_REFCOUNTING(MyCefApp);
};

int main(int argc, char** argv) {
    CefMainArgs cefArgs(argc, argv);
    CefRefPtr<MyCefApp> myApp = new MyCefApp();

    int code = CefExecuteProcess(cefArgs, myApp, nullptr);
    if (code >= 0) return code;

    ofGLFWWindowSettings settings;
    settings.setSize(1280, 720);
    ofCreateWindow(settings);

    return ofRunApp(std::make_shared<ofApp>(cefArgs));
}