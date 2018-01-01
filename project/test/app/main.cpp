#include "json.hpp"
#include "stdlib.h"
#include "stdio.h"
#include <fstream>
#include <iostream>
#include <string>
#include "myapp.h"

class App_Test: public LuMyApp
{
public:

    virtual ~App_Test()
    {
        
    }

    virtual int Init() override
    {
        printf("[App_Test] Init...\n");
        return 1;
    }
    virtual int UnInit() override
    {
        printf("[App_Test] UnInit...\n");
        return 1;
    }

    virtual void Exit(const char *reason = NULL) override
    {
        if(reason)
            printf(reason);
        
        printf("Exit Game!\n");
        LuMyApp::Exit(reason);
    }

    // 每帧调用
    virtual void LoopFps() override
    {

        static int tickfps = 0;
        printf("[LoopFps] tickfps: %d\n", ++tickfps);
    }
    // 每秒调用
    virtual void LoopSec() override
    {
        static int ticksec = 0;
        ++ticksec;
        printf("[LoopSec] ticksec: %d\n", ticksec);
        if (ticksec >= 10)
        {
            Exit("[LoopSec] time out.\n");
        }
    }
    // 每毫秒调用          
    virtual void LoopMSec() override
    {
        // static int tickmsec = 0;
        // printf("[LoopMSec] tickmsec: %d\n", ++tickmsec);
    }       
};

int main(int argc, char *argv[])
{
    App_Test app;
    app.Init();
    app.Run();
    app.UnInit();

    // std::cin.get();

    return 0;
}


