#include "BasicControlsUIHandler.h"
#include "QQmlProperty"
#include "OGLLogic.h"
#include <stdio.h>

BasicControlsUIHandler::BasicControlsUIHandler(QQuickView& rootView):
    m_rootView(rootView),
    m_pConsole(nullptr),
    m_pInData(nullptr),
    m_pFragSource(nullptr),
    m_pPosData(nullptr),
    m_pColorData(nullptr),
    m_pRenderModeLabel(nullptr),
    curDrawMode(0)
{

}

void BasicControlsUIHandler::linkElements()
{
    // fetch all elements
    m_pConsole = m_rootView.rootObject()->findChild<QObject*>("consoleOutput");
    m_pVertSource = m_rootView.rootObject()->findChild<QObject*>("vertexTextInput");
    m_pFragSource = m_rootView.rootObject()->findChild<QObject*>("fragmentTextInput");

    m_pPosData= m_rootView.rootObject()->findChild<QObject*>("positionInText");
    m_pColorData= m_rootView.rootObject()->findChild<QObject*>("colorInText");
    m_pRenderModeLabel = m_rootView.rootObject()->findChild<QObject*>("renderModeText");
    m_pCullModeLabel = m_rootView.rootObject()->findChild<QObject*>("cullModeText");

    if (m_pVertSource==NULL)
    {
        printf("Failed to link to vertex source element !\n");
    }
    if (m_pFragSource==NULL)
    {
        printf("Failed to link to fragment source element !\n");
    }
    if (m_pPosData==NULL)
    {
        printf("Failed to link to vertex position data!\n");
    }
    if (m_pColorData==NULL)
    {
        printf("Failed to link to vertex color data!\n");
    }
    if (m_pConsole==NULL)
    {
        printf("Failed to link to the console !\n");
    }
    if (m_pRenderModeLabel==NULL)
    {
        printf("Failed to link render mode label !\n");
    }
}


void BasicControlsUIHandler::appendConsoleText(const char *aText)
{

    QString rez = m_pConsole->property("text").toString();

    rez = QString(aText).append(rez);
    m_pConsole->setProperty("text",rez);
}
void BasicControlsUIHandler::updateRenderModeText(const char *aText)
{
    QString str= QString("Draw:").append(aText);

    m_pRenderModeLabel->setProperty("text",str);

}
void BasicControlsUIHandler::updateCullModeText(const char* aText)
{
    QString str = QString("Cull:").append(aText);
    m_pCullModeLabel->setProperty("text",str);
}
void BasicControlsUIHandler::updateDrawMode()
{
    OGLLogic::GetInstance().sendOGLMessage(OGLM_CHANGE_RENDER_MODE,NULL);
}
void BasicControlsUIHandler::updateCullMode()
{
    OGLLogic::GetInstance().sendOGLMessage(OGLM_CHANGE_CULL_MODE,NULL);
}
void BasicControlsUIHandler::clearConsoleText()
{
    m_pConsole->setProperty("text","");
}

void BasicControlsUIHandler::updateVertexData()
{
    QString str = QQmlProperty::read(m_pPosData,"text").toString();
    QString str2 = QQmlProperty::read(m_pColorData,"text").toString();

    str = str.append("\n").append(str2);
    QByteArray ba = str.toLatin1();
    const char* chr = ba.data();

    char* pload = (char*) malloc(strlen(chr)+1);
    strcpy(pload,chr);

    printf("updating vertex data");

    OGLLogic::GetInstance().sendOGLMessage(OGLM_RELOAD_SCENE,pload);


}

void BasicControlsUIHandler::updateShaderData()
{

    QString str = QQmlProperty::read(m_pVertSource,"text").toString();
    QByteArray ba = str.toLatin1();
    const char* chr = ba.data();

    char* vert = (char*) malloc(strlen(chr)+1);

    strcpy(vert,chr);

    QString str2 = QQmlProperty::read(m_pFragSource,"text").toString();
    ba= str2.toLatin1();
    chr = ba.data();

    char* frag = (char*) malloc(strlen(chr)+1);

    strcpy(frag,chr);

    printf(" Updating shader Data \n");


    OGLLogic::GetInstance().sendOGLMessage(OGLM_VERTEX_SHADER_DATA,vert);
    OGLLogic::GetInstance().sendOGLMessage(OGLM_FRAGMENT_SHADER_DATA,frag);
    OGLLogic::GetInstance().sendOGLMessage(OGLM_REFRESH_PROGRAM,NULL);
}

