#include "QtConstructs.h"


void QtThreadReceiver::runOnThread(QtMetaTypeFct fct)
{

    fct();

}

QtThreadChecker::QtThreadChecker(): m_pReceiver(nullptr){}

void QtThreadChecker::setReceiver(QtThreadReceiver *pReceiver)
{
    m_pReceiver=pReceiver;
}

void QtThreadChecker::linkObjects()
{

    qRegisterMetaType<QtMetaTypeFct>();
    // connect(this,SIGNAL(runOnOtherThread(std::function<void()>& )),m_pReceiver,SLOT(runOnThread (std::function<void ()>&)));

    connect(this, &QtThreadChecker::runOnOtherThread,m_pReceiver,&QtThreadReceiver::runOnThread);
}

 void QtThreadChecker::dummy()
{

}

 void QtThreadReceiver::dummy()
{

}
