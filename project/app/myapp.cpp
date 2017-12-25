#include "myapp.h"
#include "platform.h"

LuMyApp::LuMyApp()
{

}

LuMyApp::~LuMyApp()
{
    
}

int LuMyApp::Init()
{
    int nResult = 0;

    nResult = 1;
    return nResult;
}

int LuMyApp::UnInit()
{
    int nResult = 0;

    nResult = 1;
    return nResult;
}

void LuMyApp::Run()
{
    m_bRun = true;
    while(m_bRun)
    {
        Loop();
        LU_PLATFORM::sleep(1);
    }
}

void LuMyApp::Loop()
{
    static unsigned tickmsec = s_fpsinterval;
    static unsigned tickfps = GAME_FPS;
    LoopMSec();
    if((++tickmsec) >= GetFPSInterval())
    {
        tickmsec = 0;
        LoopFps();
        if((++tickfps) >= GAME_FPS)
        {
            tickfps = 0;
            LoopSec();
        }
    }
}

void LuMyApp::Exit(const char *reason/* = NULL*/)
{

    m_bRun = false;
}




