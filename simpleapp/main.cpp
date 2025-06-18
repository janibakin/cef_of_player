#include "ofMain.h"
#include "ofApp.h"
#include "include/cef_app.h"
#include "cefApp.h"

//--------------------------------------------------------------
int main(int argc, char** argv)
{
    CefMainArgs cefArgs(argc, argv);
    CefRefPtr<MyCefApp> cefApp = new MyCefApp();

    // sub-process?
    int code = CefExecuteProcess(cefArgs, cefApp, nullptr);
    if (code >= 0) return code;

    // create the window
    ofGLFWWindowSettings win;  win.setSize(1280, 720);
    ofCreateWindow(win);

    // run the OF app – pass both CefMainArgs *and* cefApp
    return ofRunApp(std::make_shared<ofApp>(cefArgs, cefApp));
}
