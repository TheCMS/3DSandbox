#ifdef WIN32

#ifndef WINWINDOW_H
#define WINWINDOW_H

#include <windows.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <string>

typedef void (*renderCb_t)();

struct wndInfo
{
    renderCb_t renderCb;
    int w;
    int h;
    int x;
    int y;
    std::string wndName;

    // OS Specific
    HDC dc; // replaced by nativeDisplay
    PIXELFORMATDESCRIPTOR pfd; // unused
    HGLRC renderCtx; //  unused


    HINSTANCE parentWindow;
    HWND renderWindow;

    // OGL Specific
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    EGLNativeDisplayType nativeDisplay;

    bool bClosing;

};

class WinWindow
{
public:
    WinWindow(int x, int y,int w, int h, std::string name, renderCb_t cb=NULL);

    void setRenderCb(renderCb_t rCb);
    void create(HINSTANCE parent=NULL);
    void close();
    void destroy();

    void manualLoop();

    void loop();

private:
    wndInfo info;
};

#endif // WINWINDOW_H

#endif // win 32
