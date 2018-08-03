#ifndef UIHANDLER_H
#define UIHANDLER_H


class UIHandler
{
private:
        UIHandler();
        ~UIHandler();
public:


    static UIHandler* getInstance();
    static void Destroy();

           void startGUI(int argc, char** argv);

private:

    static UIHandler*  s_pInstance;
    void*  pImpl; // type errasure

};

#endif // UIHANDLER_H
