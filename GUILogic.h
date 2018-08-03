#ifndef GUILOGIC_H
#define GUILOGIC_H

#include "AppTypes.h"
#include <functional>


enum eGUIMessages
{
    GM_LOG,
    GM_RENDER_MODE_CHANGED,
    GM_CULL_MODE_CHANGED
};

class GUILogic : public TSingleton<GUILogic>
{
friend class TSingleton<GUILogic>;

public:

    void runOnGUIThread(std::function<void()> fct);

    void sendGUIMessage(eGUIMessages msg, void* payload);

    void startGUI();
    void closeGUI();

private:
    GUILogic();
    ~GUILogic();

    void* m_pThreadChecker;
    void* m_pThreadReceiver;
    void* m_pUI;
};

#endif // GUILOGIC_H
