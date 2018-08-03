#ifndef BAISCSCENEHANDLER_H
#define BAISCSCENEHANDLER_H

#include <AppTypes.h>
#include <vector>

#define GL_GLEXT_PROTOTYPES

#include "GLES3/gl3.h"



struct _4fHolder{
    union
    {
        struct {GLfloat x, y, z,w;};
        struct {GLfloat r,g, b, a;};
        GLfloat v[4];
    };
};


// Hardcoded scene handler
// it has embeded what should be Material Manager , Renderer and Object Manager
class BasicSceneHandler : public TSingleton<BasicSceneHandler>
{
    friend class TSingleton<BasicSceneHandler>;
    BasicSceneHandler();
public:



    virtual void renderScene();

    virtual void prepareShaders();
    virtual void prepareBuffers();

    virtual void loadSceneFromFile(const char* filePath);
    virtual void loadSceneFromData(const char* data);

    virtual void setVertexShaderInfo(const char* shaderData);
    virtual void setFragmentShaderInfo(const char* shaderData);

    virtual void changeRenderMode();
    virtual void changeCullMode();

private:

    void logSceneMessage(std::string aMessage);

    GLuint compileShader(GLenum type,const char* shaderData);

    std::vector<_4fHolder> pos;
    std::vector<_4fHolder> col;
    std::vector<_4fHolder> norm;

    GLbyte* vertData;
    GLbyte* linesData;
    GLbyte* normData;
    GLuint* idxBuffer;

    GLuint renderProg;
    GLint posIdx;
    GLint colIdx;    
    GLint timeIdx;
    GLint normIdx;
    GLint lightIdx;

    GLuint fragProg;
    GLuint vertProg;


    std::string vertStr;
    std::string fragStr;

    int m_nCurRenderMode;
    int m_nCullMode;

    bool hasNormals;
    unsigned int passedMillis;



};

#endif // BAISCSCENEHANDLER_H
