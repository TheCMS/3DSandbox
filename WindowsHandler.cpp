#include "WindowsHandler.h"

#include <stdio.h>

#ifdef WIN32
#include "WinWindow.h"
#endif





WindowsHandler::WindowsHandler(const char* wndName, int w, int h,int x, int y):
    m_szName(wndName),
    m_w(w),
    m_h(h),
    m_x(x),
    m_y(y),
    m_renderCb(NULL),
    m_pImpl(NULL)
{

#ifdef WIN32
    WinWindow* p = new WinWindow(m_x,m_y,m_w,m_h,m_szName,m_renderCb);
    m_pImpl=p;
#endif
}

WindowsHandler::~WindowsHandler()
{
#ifdef WIN32
    WinWindow* p=(WinWindow*)m_pImpl;
    delete p;
    m_pImpl=NULL;
#endif

}


void WindowsHandler::createWindow()
{

#ifdef WIN32

    WinWindow* p = (WinWindow*)m_pImpl;
    p->create();
    m_bActive=true;

#endif

}

void WindowsHandler::closeWindow()
{
#ifdef WIN32
    m_bActive=false;
    WinWindow* p = (WinWindow*)m_pImpl;
    p->close();

#endif

}

void WindowsHandler::startWndLoop()
{
#ifdef WIN32
     WinWindow* p = (WinWindow*)m_pImpl;
     p->loop();
#endif

}
void WindowsHandler::setRenderCb(glRenderCb cb)
{
    m_renderCb=cb;
#ifdef WIN32
    WinWindow* p = (WinWindow*)m_pImpl;
    p->setRenderCb(cb);
#endif
}


void WindowsHandler::manualWndLoop()
{

#ifdef WIN32
    WinWindow* p = (WinWindow*)m_pImpl;
    if (m_bActive)
    {
        p->manualLoop();
    }
#endif
}

void WindowsHandler::destroyWindow()
{

#ifdef WIN32
   WinWindow* p = (WinWindow*)m_pImpl;
   p->destroy();
   delete p;
#endif

}

