#ifndef BASICCONTROLSUIHANDLER_H
#define BASICCONTROLSUIHANDLER_H

#include <QObject>
#include <QQuickView>
#include <QQuickWindow>
#include <QQuickItem>
#include <QTextEdit>

class BasicControlsUIHandler : public QObject
{
    Q_OBJECT
public:
    BasicControlsUIHandler(QQuickView& rootView);

    virtual void linkElements();

    Q_INVOKABLE void clearConsoleText();
    Q_INVOKABLE void updateVertexData();
    Q_INVOKABLE void updateShaderData();
    Q_INVOKABLE void updateDrawMode();
    Q_INVOKABLE void updateCullMode();

    virtual void appendConsoleText(const char* aText);
    virtual void updateRenderModeText(const char* aText);
    virtual void updateCullModeText(const char* aText);

private:
    QQuickView& m_rootView;


    QObject* m_pConsole;
    QObject* m_pInData;
    QObject* m_pVertSource;
    QObject* m_pFragSource;
    QObject* m_pRenderModeLabel;
    QObject* m_pCullModeLabel;

    QObject* m_pPosData;
    QObject* m_pColorData;

    int curDrawMode;


};

#endif
