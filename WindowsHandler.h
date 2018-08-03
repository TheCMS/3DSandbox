#ifndef WINDOWSHANDLER_H
#define WINDOWSHANDLER_H

#include <string>


// abstraction over the window with the openGL context

typedef void (*glRenderCb)();

class WindowsHandler final
{
public:
    WindowsHandler(const char* wdnName, int w, int h, int x=0, int y=0);
    ~WindowsHandler();

    void createWindow();
    void closeWindow();

    void setRenderCb(glRenderCb cb);
    void startWndLoop();

    void manualWndLoop();

    void destroyWindow();

private:

    bool m_bActive;
    std::string m_szName;
    int m_w;
    int m_h;
    int m_x;
    int m_y;

    glRenderCb m_renderCb;
    void* m_pImpl;  // type erassure

};

#endif // WINDOWSHANDLER_H
