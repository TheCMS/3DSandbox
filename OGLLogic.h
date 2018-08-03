#ifndef OGLLOGIC_H
#define OGLLOGIC_H

#include "AppManagerTypes.h"

enum eOGLMessage
{
    OGLM_RELOAD_SCENE=0,
    OGLM_VERTEX_SHADER_DATA,
    OGLM_FRAGMENT_SHADER_DATA,
    OGLM_REFRESH_PROGRAM,
    OGLM_CHANGE_RENDER_MODE,
    OGLM_CHANGE_CULL_MODE
};

class OGLLogic : public TSingleton<OGLLogic>
{
    friend class TSingleton<OGLLogic>;

public:

    static void RenderLoop();

    void sendOGLMessage(eOGLMessage msg, void* payload);

    void runOnOGLThread(std::function<void()> fct);

    void queuePreRendering(std::function<void()> fct);
    void queuePostRendering(std::function<void()> fct);


private:
    void updateOGLLogic();
    void populateSceneWithDefaults();


    std::vector<std::function<void() > > m_vPreRendering;
    std::vector<std::function<void() > > m_vPostRendering;
    bool scenePopulated;
    OGLLogic();
    ~OGLLogic();

};

#endif // OGLLOGIC_H
