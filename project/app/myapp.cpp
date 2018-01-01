#include "myapp.h"
#include <thread>

LuMyApp::LuMyApp()
{

}

LuMyApp::~LuMyApp()
{
    
}

int LuMyApp::Init()
{
    int nResult = 0;
    m_timerSec.initialize();
    m_timerFps.initialize();
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
    LoopMSec();

    if(m_timerFps.ticking())
        LoopFps();

    if(m_timerSec.ticking())
        LoopSec();
}

void LuMyApp::Exit(const char *reason/* = NULL*/)
{

    m_bRun = false;
}




