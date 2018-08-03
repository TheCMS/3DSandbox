#include "BasicSceneHandler.h"
#include "GUILogic.h"

#include "Utils.h"

// #define LINE_STRIP

BasicSceneHandler::BasicSceneHandler():
    renderProg(0),
    posIdx(-1),
    colIdx(-1),
    timeIdx(-1),
    normIdx(-1),
    lightIdx(-1),
    vertProg(0),
    fragProg(0),
    vertData(nullptr),
    linesData(nullptr),
    m_nCurRenderMode(0),
    m_nCullMode(1)
{

}

long long oldT=0;

int frameCnter=0;

void BasicSceneHandler::renderScene()
{


    long long delta=0;
    if (oldT > 0)
    {
        delta = getCurrentTimeMillis() - oldT;
    }
    oldT = getCurrentTimeMillis();

    passedMillis += delta;


    // compute deltaT;
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (m_nCullMode == 0)
    {
    glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        if (m_nCullMode== 1)
        {
            glCullFace(GL_BACK);
        } else if (m_nCullMode==2)
        {
            glCullFace(GL_FRONT);
        }
    }



    if ( vertData!=nullptr && renderProg!=0)
    {
        glUseProgram(renderProg);

        glEnableVertexAttribArray(posIdx);
        glEnableVertexAttribArray(colIdx);

        if (hasNormals)
            glEnableVertexAttribArray(normIdx);
        else
            glDisableVertexAttribArray(normIdx);


        float val = passedMillis*0.001f;
        glUniform1f(timeIdx,val);
        if (passedMillis>3*1000* 1000 * 1000)
            passedMillis=0;


        if (m_nCurRenderMode == 0 || m_nCurRenderMode==2 )
        {


            // interlaced
            glVertexAttribPointer(posIdx,4,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),vertData);
            glVertexAttribPointer(colIdx,4,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),vertData+4*sizeof(float));

            // non interlaced...to lazy to remap the buffer
            glVertexAttribPointer(normIdx,4,GL_FLOAT,GL_FALSE,4*sizeof(GL_FLOAT),normData);

            unsigned int cnt = pos.size();
            if (cnt>col.size())
                cnt = col.size();

            if (m_nCurRenderMode==0)
                glDrawArrays(GL_TRIANGLE_STRIP,0,cnt);
            else if (m_nCurRenderMode==2)
                glDrawArrays(GL_POINTS,0,cnt);
//            else if (m_nCurRenderMode==1)
//                glDrawArrays(GL_LINE_STRIP,0,cnt);
        } else {

            glDisableVertexAttribArray(normIdx);
            // interlaced
            glVertexAttribPointer(posIdx,4,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),linesData);
            glVertexAttribPointer(colIdx,4,GL_FLOAT,GL_FALSE,8*sizeof(GL_FLOAT),linesData+4*sizeof(float));
            // non interlaced...to lazy to screw the buffer up again
            // glVertexAttribPointer(colIdx,4,GL_FLOAT,GL_FALSE,4*sizeof(GL_FLOAT),vertData+4*sizeof(float));

            unsigned int cnt = pos.size();
            if (cnt > col.size())
                cnt = col.size();

#ifdef LINE_STRIP
            int nLines = (cnt / 3) * 4 + (cnt %3);
            if (cnt%3) nLines++;
            glDrawArrays(GL_LINE_STRIP,0,nLines);
#else
            int nLines = (cnt - 2)*6;
            if (cnt == 2 ) nLines = 2;
            glDrawArrays(GL_LINES,0,nLines);

#endif



        }

    } else {

        glClearColor(1.0,0.0,1.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glFlush();
}

void BasicSceneHandler::loadSceneFromFile(const char* filePath)
{
    (void) filePath;
}
void BasicSceneHandler::loadSceneFromData(const char* data)
{

    std::vector<std::string> lines = getLinesFromChar(data);
    pos.clear();
    col.clear();
    norm.clear();

    bool parsingPos=false;
    bool parsingCol= false;
    bool parsingNorm=false;
    hasNormals =false;

    for (unsigned int i=0;i<lines.size();i++)
    {

        std::string line = trimm(lines[i]);
        if (line[0]=='#')
        {
            if (line.find("Positions")!=std::string::npos)
            {
                parsingPos=true;
                parsingCol=false;
                parsingNorm=false;
            } else if (line.find("Colors")!=std::string::npos)
            {
                parsingCol=true;
                parsingPos=false;
                parsingNorm=false;
            } else if (line.find("Normals")!=std::string::npos)
            {
                parsingNorm=true;
                parsingCol=false;
                parsingPos=false;
            }
        } else {
        if (parsingCol)
        {
            if (line.size()>0)
            {
                _4fHolder holder;

                sscanf_s(line.c_str(),"%f %f %f %f",&holder.r,&holder.g,&holder.b,&holder.a);
                col.push_back(holder);
                printf(" pushed color %f %f %f %f\n",holder.v[0],holder.v[1],holder.v[2], holder.v[3]);

            }

        } else if (parsingPos)
        {
            if (line.size()>0)
            {
                _4fHolder holder;

                holder.w=1.0f;
                sscanf_s(line.c_str(),"%f %f %f ",&holder.x,&holder.y,&holder.z);
                pos.push_back(holder);
                printf(" pushed position %f %f %f %f\n",holder.v[0],holder.v[1],holder.v[2], holder.v[3]);

            }
        } else if (parsingNorm)
        {
            hasNormals=true;
            if (line.size()>0)
            {
                _4fHolder holder;
                holder.w=0;
                sscanf_s(line.c_str(),"%f %f %f ",&holder.x,&holder.y,&holder.z);
                printf("pushing line info for normals: %s\n",line.c_str());
                norm.push_back(holder);
            }
        }
        }
    }

    printf("parsing done ! %d vertices processed !\n",pos.size());
    printf("normals available: %s. Number of normals %d!\n", (hasNormals ? "True" : "False" ), norm.size());

}

void BasicSceneHandler::prepareShaders()
{
    GLint linked;
    if (renderProg!=0)
    {
        glDeleteProgram(renderProg);
        renderProg=0;
    }

    if (vertProg!=0)
    {
        glDeleteShader(vertProg);
        vertProg=0;
    }

    if (fragProg!=0)
    {
        glDeleteShader(fragProg);
        fragProg=0;
    }


    vertProg = compileShader(GL_VERTEX_SHADER,vertStr.c_str());
    fragProg = compileShader(GL_FRAGMENT_SHADER,fragStr.c_str());

    if (vertProg==0 || fragProg==0)
    {
        char buffer[1028];
        sprintf(buffer,"Failed compiling shaders! vert is %d and frag is %d.",vertProg,fragProg);
        logSceneMessage(buffer);
        return;
    }

    renderProg = glCreateProgram();

    if(renderProg == 0)
    {
        logSceneMessage("Failed creating program!");
       return ;
    }

    glAttachShader(renderProg, vertProg);
    glAttachShader(renderProg, fragProg);
    // Link the program
    glLinkProgram(renderProg);

    // Check the link status
    glGetProgramiv(renderProg, GL_LINK_STATUS , &linked);

    if(!linked)
    {
       GLint infoLen = 0;

       glGetProgramiv(renderProg, GL_INFO_LOG_LENGTH,&infoLen);

       if(infoLen > 1)
       {
          char* infoLog =(char*) malloc(sizeof(char) * infoLen);

          glGetProgramInfoLog(renderProg, infoLen, NULL,infoLog);
          logSceneMessage("Error linking program:\n");
          logSceneMessage(infoLog);

          free(infoLog) ;
       }

       glDeleteProgram(renderProg);

       return;
    }

    posIdx = glGetAttribLocation(renderProg,"inPos");
    colIdx = glGetAttribLocation(renderProg,"inColor");
    timeIdx = glGetUniformLocation(renderProg,"uTime");
    lightIdx = glGetUniformLocation(renderProg,"uLight");
    normIdx = glGetAttribLocation(renderProg,"inNormal");

    printf("time location : %d \n",timeIdx);

}

void BasicSceneHandler::prepareBuffers()
{
    if (vertData!=nullptr)
    free(vertData);

    if (linesData!=nullptr)
    free(linesData);


    unsigned int size = pos.size();
    if (size > col.size())
        size = col.size();

    if (pos.size() != col.size())
    {
        logSceneMessage("Inconsistent size between color and position attributes.\n");
        if (pos.size() > col.size())
        {
            logSceneMessage("Insufficient color information. Filling buffer with last color!\n");

            if (col.size()==0)
            {
                col.push_back(_4fHolder{1.0f,0.0f,1.0f,1.0f});
            }
            for (int i=col.size();i<pos.size();i++)
            {
                col.push_back(col[i-1]);
            }
        } else {
            logSceneMessage("Insufficient position information. Truncating color attributes!\n");
        }
    }

    if (pos.size() != norm.size())
    {
        logSceneMessage("Inconsistent size between normals and position attributes.\n");
        if (pos.size() > norm.size())
        {
            logSceneMessage("Insufficient normal information. Filling buffer with last normal!\n");

            if (norm.size()==0)
            {
                norm.push_back(_4fHolder{0.0f,0.0f,1.0f,0.0f});
            }
            for (int i=norm.size();i<pos.size();i++)
            {
                norm.push_back(norm[i-1]);
            }
        } else {
            logSceneMessage("Insufficient position information. Truncating color attributes!\n");
        }
    }



//     start interlacing data
    vertData = (GLbyte*) malloc(sizeof(float)*4*2*size);
    normData = (GLbyte*) malloc(sizeof(float)*4*size);



    for (int i=0;i<size;i++)
    {
        memcpy(vertData+i*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
        memcpy(vertData+i*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));
        memcpy(normData+i*4*sizeof(float),&norm[i].v,4*sizeof(float));
    }




//#define NON_FAN_APPROACH

#ifdef NON_FAN_APPROACH
    // number of vertices in the lines buffer
    // for non fan approach we define 2 points for each line
    int nLines = size*2;
    linesData = (GLbyte*) malloc(sizeof(float)*4*2*nLines);
    int offset=0; // difference between pos idx and lines data idx
    for (int i=0;i<size;i+=3)
    {
        if (i+2 < size) // add line from last vertex to the first in the iteration
        {
            memcpy(linesData+(i+offset)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+offset)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

            memcpy(linesData+(i+offset+1)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
            memcpy(linesData+(i+offset+1)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

            memcpy(linesData+(i+offset+2)*4*2*sizeof(float),&pos[i+2].v,4*sizeof(float));
            memcpy(linesData+(i+offset+2)*4*2*sizeof(float)+4*sizeof(float),&col[i+2].v,4*sizeof(float));

            memcpy(linesData+(i+offset+3)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+offset+3)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

            offset++;
        } else { // copy from the last queued vertex to first
            int idx=offset;
            if (i > 0)
            {
                memcpy(linesData+i*4*2*sizeof(float),&pos[i-1].v,4*sizeof(float));
                memcpy(linesData+i*4*2*sizeof(float)+4*sizeof(float),&col[i-1].v,4*sizeof(float));
                idx++;
            }
            memcpy(linesData+(i+idx)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+idx)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));
            idx++;
            if (i+1 < size)
            {
                memcpy(linesData+(i+idx)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
                memcpy(linesData+(i+idx)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

            }

        }
    }
#else



#ifndef LINE_STRIP

    // we use line strip

    // nTriangles: 1st 3 vertices define a triangle
    // each subsequent vertex will define a triangle
    int nTriangles = size - 2;
    if (nTriangles<0) nTriangles=0;
    int nLines = nTriangles * 6;

    // corner case with only 2 vertices
    if (size == 2)
    {
        nLines=2;
    }

    linesData = (GLbyte*) malloc (sizeof(float)*4*2*nLines);

    if (nLines==2)
    {
        logSceneMessage("Single Line case detected\n");
        memcpy(linesData + 0 * 4*2*sizeof(float),&pos[0].v,4*sizeof(float));
        memcpy(linesData + 0 * 4*2*sizeof(float)+4*sizeof(float),&col[0].v,4*sizeof(float));

        memcpy(linesData + 1 * 4*2*sizeof(float),&pos[1].v,4*sizeof(float));
        memcpy(linesData + 1 * 4*2*sizeof(float)+4*sizeof(float),&col[1].v,4*sizeof(float));

        return;
    }

    int offset = 0; // diff between pos idx and lines data idx

    char buff[2048];
    sprintf(buff, "Number of lines identified : %d\n",nLines);
    logSceneMessage(std::string(buff));
    for (int i=0;i<size - 2;i++)
    {
        // process triangle lines

        // line i -> i+1
        memcpy(linesData + (offset)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
        memcpy(linesData + (offset)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

        memcpy(linesData + (offset+1)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
        memcpy(linesData + (offset+1)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

        // line i+1 -> i+2
        memcpy(linesData + (offset+2)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
        memcpy(linesData + (offset+2)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

        memcpy(linesData + (offset+3)*4*2*sizeof(float),&pos[i+2].v,4*sizeof(float));
        memcpy(linesData + (offset+3)*4*2*sizeof(float)+4*sizeof(float),&col[i+2].v,4*sizeof(float));

        // line i+2 -> i
        memcpy(linesData + (offset+4)*4*2*sizeof(float),&pos[i+2].v,4*sizeof(float));
        memcpy(linesData + (offset+4)*4*2*sizeof(float)+4*sizeof(float),&col[i+2].v,4*sizeof(float));

        memcpy(linesData + (offset+5)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
        memcpy(linesData + (offset+5)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

        offset+=6;

    }

#else // line strip

    // number of vertices in the lines buffer
    // for tirangles we have 4 vertices to draw 3 lines, and for dangling vertices we link them to the last vertex in the buffer.
    int nLines = (size / 3) * 4 + (size %3);
    if (size%3) nLines++;

    printf(" number of vertices for lines are %d\n",nLines);
    linesData = (GLbyte*) malloc(sizeof(float)*4*2*nLines);
    int offset=0; // difference between pos idx and lines data idx
    for (int i=0;i<size;i+=3)
    {
        if (i+2 < size) // add line from last vertex to the first in the iteration
        {
            memcpy(linesData+(i+offset)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+offset)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

            memcpy(linesData+(i+offset+1)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
            memcpy(linesData+(i+offset+1)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

            memcpy(linesData+(i+offset+2)*4*2*sizeof(float),&pos[i+2].v,4*sizeof(float));
            memcpy(linesData+(i+offset+2)*4*2*sizeof(float)+4*sizeof(float),&col[i+2].v,4*sizeof(float));

            memcpy(linesData+(i+offset+3)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+offset+3)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));

            offset+=4;
        } else { // copy from the last queued vertex to first
            int idx=offset;
            if (i > 0)
            {
                memcpy(linesData+(i+idx)*4*2*sizeof(float),&pos[i-1].v,4*sizeof(float));
                memcpy(linesData+(i+idx)*4*2*sizeof(float)+4*sizeof(float),&col[i-1].v,4*sizeof(float));
                idx++;
            }
            memcpy(linesData+(i+idx)*4*2*sizeof(float),&pos[i].v,4*sizeof(float));
            memcpy(linesData+(i+idx)*4*2*sizeof(float)+4*sizeof(float),&col[i].v,4*sizeof(float));
            idx++;
            if (i+1 < size)
            {
                memcpy(linesData+(i+idx)*4*2*sizeof(float),&pos[i+1].v,4*sizeof(float));
                memcpy(linesData+(i+idx)*4*2*sizeof(float)+4*sizeof(float),&col[i+1].v,4*sizeof(float));

            }

        }
    }
#endif
#endif

}


void BasicSceneHandler::setVertexShaderInfo(const char* shaderData)
{
    vertStr = std::string(shaderData);
}

void BasicSceneHandler::setFragmentShaderInfo(const char* shaderData)
{
    fragStr = std::string(shaderData);
}
void BasicSceneHandler::changeRenderMode()
{
    m_nCurRenderMode++;
    if (m_nCurRenderMode>2)
    {
        m_nCurRenderMode=0;
    }

    const char* modeText = "Triangles";
    if (m_nCurRenderMode == 1) modeText = "Lines";
    if (m_nCurRenderMode == 2) modeText = "Points";
    char* payload = (char*) malloc (strlen(modeText)+1);
    strcpy(payload,modeText);
    GUILogic::GetInstance().sendGUIMessage(GM_RENDER_MODE_CHANGED,payload);
}
void BasicSceneHandler::changeCullMode()
{
    m_nCullMode++;
    if (m_nCullMode>2)
    {
        m_nCullMode=0;
    }
    const char* modeText = "Disabled";
    if (m_nCullMode == 1) modeText = "Back";
    if (m_nCullMode == 2) modeText = "Front";
    char* payload = (char*) malloc(strlen(modeText)+1);
    strcpy(payload,modeText);
    GUILogic::GetInstance().sendGUIMessage(GM_CULL_MODE_CHANGED,payload);
}

GLuint BasicSceneHandler::compileShader(GLenum type,const char* shaderData)
{
    GLuint shader;
    GLint compiled;

     // Create the shader object
    shader = glCreateShader(type);

    if(shader == 0)
        return 0;

  // Load the shader source
    glShaderSource(shader, 1, &shaderData, NULL);

  // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
        GLint infoLen = 0;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

        if(infoLen > 1)
       {
           char* infoLog =(char*) malloc(sizeof(char) * infoLen);

           glGetShaderInfoLog(shader, infoLen, NULL, infoLog);

           logSceneMessage("Error Compiling shader:\n");
           logSceneMessage(infoLog);

           free(infoLog);

       }

       glDeleteShader(shader);
       return 0;
    }

    return shader;

}

void BasicSceneHandler::logSceneMessage(std::string aMessage)
{
    printf("[SceneHandler] %s\n",aMessage.c_str());

    char* str = (char*) malloc(aMessage.size()+1);
    strcpy(str,aMessage.c_str());
    GUILogic::GetInstance().sendGUIMessage(GM_LOG,str);
}
