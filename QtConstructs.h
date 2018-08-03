#ifndef QTCONSTRUCTS_H
#define QTCONSTRUCTS_H

#include "QObject"
#include <functional>


typedef std::function<void()> QtMetaTypeFct;
Q_DECLARE_METATYPE(QtMetaTypeFct)

class QtThreadReceiver : public QObject
{
    Q_OBJECT

private:
    virtual void dummy();

public slots:

    void runOnThread(QtMetaTypeFct fct);
};

class QtThreadChecker : public QObject
{
    Q_OBJECT
public:
    QtThreadChecker();
    void setReceiver(QtThreadReceiver* pReceiver);

    void linkObjects();


signals:
    void runOnOtherThread(QtMetaTypeFct fct);

private:
    virtual void dummy();
    QtThreadReceiver* m_pReceiver;

};


#endif // QTCONSTRUCTS_H
