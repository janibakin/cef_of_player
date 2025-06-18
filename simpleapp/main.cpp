#include "ofMain.h"
#include "ofApp.h"
#include "include/cef_app.h"

int main(int argc, char** argv) {
    // Prevent GStreamer from using unstable hardware decoders
    setenv("GST_PLUGIN_FEATURE_RANK", "nvh264dec:0", 1);
    
    CefMainArgs cef_args(argc, argv);
    int code = CefExecuteProcess(cef_args, nullptr, nullptr);
    if (code >= 0) return code;

    ofGLFWWindowSettings settings;
    settings.setSize(1024, 600);
    ofCreateWindow(settings);
    return ofRunApp(std::make_shared<ofApp>(cef_args));
}
