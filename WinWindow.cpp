#ifdef WIN32

#include "WinWindow.h"
#include "AppLogic.h"

// CMS to be upgraded
wndInfo* g_pInfo=NULL;


void defaultRenderLoop()
{

    glClearColor(1.0,0.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFlush();

}


//
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{

    wndInfo* pInfo = (wndInfo*)GetWindowLongPtr(hWnd,GWLP_USERDATA);


    if (pInfo==NULL)
    {
        return DefWindowProc(hWnd,message,wParam,lParam);

    }

    switch (message)
    {

        case WM_PAINT:

            if (pInfo->renderCb!=NULL)
            {
                pInfo->renderCb();
            }

            eglSwapBuffers(pInfo->display,pInfo->surface);

        break;
        case WM_SIZE:

            glViewport(0,0,LOWORD(lParam),HIWORD(lParam));
            pInfo->w=LOWORD(lParam);
            pInfo->h=HIWORD(lParam);
            break;
        case WM_CLOSE:
            {
                if (!pInfo->bClosing)
                {

                    ReleaseDC(pInfo->renderWindow,pInfo->dc);
#ifdef OGL10
                    wglMakeCurrent(NULL,NULL);
                    wglDeleteContext(pInfo->renderCtx);
#endif
                    eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                    eglDestroyContext(pInfo->display, pInfo->context);
                    eglDestroySurface(pInfo->display, pInfo->surface);
                    eglTerminate(pInfo->display);
                    // eglDestroyContext(ctx.eglDisplay, ctx.eglContext);
                    // eglDestroySurface(ctx.eglDisplay, ctx.eglSurface);
                    // eglTerminate(ctx.eglDisplay);
                    // eglMakeCurrent(NULL,NULL,)
                    pInfo->bClosing=true;
                    PostQuitMessage(0);
                }
            }
        break;
        default:
            return DefWindowProc(hWnd,message,wParam,lParam);
        break;
    }
    return 0;
}



WinWindow::WinWindow(int x, int y,int w, int h, std::string name, renderCb_t cb)
{
    info.x=x;
    info.y=y;
    info.w=w;
    info.h=h;
    info.wndName=name;
    info.renderCb=cb;
    if (cb==NULL)
    {
        info.renderCb=defaultRenderLoop;
    }
}

void WinWindow::setRenderCb(renderCb_t rCb)
{
    info.renderCb=rCb;
}

void WinWindow::create(HINSTANCE parent)
{

    g_pInfo=&info;
    HINSTANCE hInst = parent;
    if (hInst== NULL)
    {
        // make window child of current window
        hInst=GetModuleHandle(NULL);
    }
    info.parentWindow = hInst;

    WNDCLASS wc = {0} ;
    wc.lpfnWndProc=WndProc;
    wc.hInstance=hInst;
    wc.hbrBackground=NULL;
    wc.lpszClassName=L"oglversionchecksample";
    wc.style=CS_OWNDC;

    if (!RegisterClass(&wc))
    {
        printf("failed registering window class with n");

    } else {

        std::wstring name = std::wstring(info.wndName.begin(),info.wndName.end());


        info.renderWindow = CreateWindowW(wc.lpszClassName,
                                          name.c_str(),
                                          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                         info.x,info.y,info.w,info.h,
                                         0,0,
                                         hInst,
                                         &info);



#ifdef OGL10

        info.pfd.nVersion=1;
        info.pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
        info.pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        info.pfd.iPixelType= PFD_TYPE_RGBA;
        info.pfd.cColorBits=32;
        info.pfd.cDepthBits=24;
        info.pfd.cStencilBits=8;
        info.bClosing=false;

        SetWindowLongPtr(info.renderWindow,GWLP_USERDATA,(LONG_PTR)&info);
        UpdateWindow(info.renderWindow);

        info.dc = GetDC(info.renderWindow);
        int winPf = ChoosePixelFormat(info.dc,&info.pfd);
        SetPixelFormat(info.dc,winPf,&info.pfd);
         info.renderCtx = wglCreateContext(info.dc);
        wglMakeCurrent(info.dc,info.renderCtx);
#else
        EGLNativeDisplayType nativeDisplay = GetDC(info.renderWindow);

        // get egl display handle
        EGLDisplay eglDisplay;
        eglDisplay = eglGetDisplay(nativeDisplay);
       if(eglDisplay == EGL_NO_DISPLAY)
       {
           printf("Could not get EGL display\n");
           return;
       }
       info.display = eglDisplay;

       // Initialize the display
       EGLint major = 0;
       EGLint minor = 0;
       EGLBoolean bsuccess = eglInitialize(eglDisplay, &major, &minor);
       if (!bsuccess)
       {
           printf("Could not initialize EGL display\n");
           return ;
       }
       printf("Supported EGL version is %d - %d.",major,minor);
       if (major < 1 || minor < 4 )
       {
           // Does not support EGL 1.4
           printf("System does not support at least EGL 1.4.\n");
           return ;
       }

       // Obtain the first configuration with a depth buffer
       EGLint attrs[] = { EGL_DEPTH_SIZE, 16 , EGL_RED_SIZE , 5, EGL_GREEN_SIZE, 6 , EGL_BLUE_SIZE , 5, EGL_ALPHA_SIZE, 0 , EGL_NONE};
       EGLint numConfig =0;
       EGLConfig eglConfig = 0;
       bsuccess = eglChooseConfig(eglDisplay, attrs, &eglConfig, 1, &numConfig);
       if (!bsuccess)
       {
           printf("Could not find valid EGL config\n");
           return ;
       }

       // Get the native visual id
       int nativeVid;
       if (!eglGetConfigAttrib(eglDisplay, eglConfig, EGL_NATIVE_VISUAL_ID, &nativeVid))
       {
           printf("Could not get native visual id\n");
           return ;
       }

       EGLNativeWindowType nativeWin = info.renderWindow;
       // Create a surface for the main window
       EGLSurface eglSurface;
       eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWin, NULL);
       if (eglSurface == EGL_NO_SURFACE)
       {
           printf("Could not create EGL surface\n");
           return;
       }
       info.surface = eglSurface;

       // Create an OpenGL ES context
       EGLContext eglContext;
       EGLint ctxattrs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
       eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, ctxattrs);
       if (eglContext == EGL_NO_CONTEXT)
       {
           printf("Could not create EGL context\n");
           return ;
       }

       // Make the context and surface current
       bsuccess = eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
       if(!bsuccess)
       {
           printf("Could not activate EGL context\n");
           return ;
       }

       info.context = eglContext;
       info.nativeDisplay = nativeDisplay;


#endif

        printf("Context Created.\nOpenGL version used: %s\n",glGetString(GL_VERSION));

        SetWindowLongPtr(info.renderWindow,GWLP_USERDATA,(LONG_PTR)&info);
        UpdateWindow(info.renderWindow);

    }

}

void WinWindow::close()
{
    PostMessage(info.renderWindow,WM_CLOSE,0,(LPARAM)&info);
}
void WinWindow::destroy()
{
    WINDOWINFO inf;
    GetWindowInfo((HWND) info.renderWindow,&inf);
    DestroyWindow((HWND) info.renderWindow);
    UnregisterClass((LPCTSTR) inf.atomWindowType, GetModuleHandle(NULL));
}

void WinWindow::loop()
{

     MSG msg={0};
     while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
     {
            TranslateMessage(&msg);
            DispatchMessage( &msg );
     }

}

void WinWindow::manualLoop()
{

    MSG msg={0};
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)>0)
    {

        if (msg.message == WM_QUIT)
        {
            printf("quitting !\n");
            AppLogic::GetInstance().sendLogicMessage(LM_EXIT,nullptr);
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }



}

#endif // win 32
