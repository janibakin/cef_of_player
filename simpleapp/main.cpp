#include "ofMain.h"
#include "ofApp.h"
#include "include/cef_app.h"
#include "cefApp.h"

int main(int argc, char** argv) {
    CefMainArgs cefArgs(argc, argv);
    CefRefPtr<MyCefApp> cefApp = new MyCefApp();

    int code = CefExecuteProcess(cefArgs, cefApp, nullptr);
    if (code >= 0) return code;

    ofGLFWWindowSettings win;
    win.setSize(1280, 720);
    ofCreateWindow(win);

    return ofRunApp(std::make_shared<ofApp>(cefArgs, cefApp));
}
