#include "UIHandler.h"


UIHandler*  UIHandler::s_pInstance=nullptr;

UIHandler::UIHandler()
{
}
UIHandler::~UIHandler()
{

}
UIHandler* UIHandler::getInstance()
{
    if (s_pInstance==nullptr)
    {
        s_pInstance=new UIHandler();
    }
    return s_pInstance;
}
void UIHandler::Destroy()
{
    delete s_pInstance;
    s_pInstance=nullptr;
}

void UIHandler::startGUI(int argc, char** argv)
{

}


