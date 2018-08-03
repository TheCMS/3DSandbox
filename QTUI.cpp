#include "QTUI.h"


QTUI::QTUI(int argc, char** argv):
    m_app(argc,argv)
{

}

//void QTUI::start(QtThreadChecker* p, QtThreadReceiver*& pReceiver)
void QTUI::start()
{
    createMainWindow();
//    pReceiver = new QtThreadReceiver();
//    p->setReceiver(pReceiver);
//    p->linkObjects();
    m_app.exec();
}
void QTUI::createMainWindow()
{

    m_MainView.setSource(QUrl(QStringLiteral("qrc:/BasicControls.qml")));
    m_MainView.show();

    m_pHandler= new BasicControlsUIHandler(m_MainView);
    m_pHandler->linkElements();
    m_MainView.rootContext()->setContextProperty("basicController",m_pHandler);

}

void QTUI::stop()
{
    m_MainView.close();
    m_app.exit();
}

BasicControlsUIHandler* QTUI::getHandler() const
{
    return m_pHandler;
}
