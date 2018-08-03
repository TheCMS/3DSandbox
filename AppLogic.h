#ifndef APPLOGIC_H
#define APPLOGIC_H


#include "AppManagerTypes.h"

enum eLogicMessages
{
    LM_EXIT=0
};

class AppLogic : public TSingleton<AppLogic>
{

friend class TSingleton<AppLogic>;
public:

        void setAppExitPtr(bool* exitRef);

        void sendLogicMessage(eLogicMessages msg, void* payload);

        void runOnAppThread(std::function<void()> fct);

        void updateAppLogic();

private:

    bool* appExit;
    AppLogic();
    ~AppLogic();
};

#endif // APPLOGIC_H
