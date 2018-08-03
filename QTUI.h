#ifndef QTUI_H
#define QTUI_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "BasicControlsUIHandler.h"

#include "QtConstructs.h"

class QTUI
{
public:
    QTUI(int argc, char** argv);

    // void start(QtThreadChecker* p,QtThreadReceiver*& pReceiver);
    void start();
    void stop();

    BasicControlsUIHandler* getHandler() const;

private:
    void createMainWindow();
    QGuiApplication m_app;
    QQuickView m_MainView;
    BasicControlsUIHandler* m_pHandler;

};

#endif // QTUI_H
