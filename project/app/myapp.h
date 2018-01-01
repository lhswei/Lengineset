#ifndef _MYAPP_H_
#define _MYAPP_H_

#include "myappdef.h"
#include "ltimer.h"

typedef std::chrono::duration<int64_t, ratio<1,(int)(GAME_FPS)>> LT_GAME_FPS;

class MyApp
{
public:
    MyApp() {};
    virtual ~MyApp() {};

    virtual int Init() = 0;
    virtual int UnInit() = 0;
    virtual void Run() = 0;
    virtual void Loop() = 0;
    virtual void Exit(const char *reason) = 0;
};

class MyAppInterface
{
public:
    MyAppInterface() {};
    virtual ~MyAppInterface() {};

    virtual int GetFps() = 0;
};

class LuMyAppInterface: public MyAppInterface
{
public:
    LuMyAppInterface() {};
    virtual ~LuMyAppInterface() {};

    virtual int GetFps() override final{return GAME_FPS;}
};


class LuMyApp: virtual public MyApp, virtual public LuMyAppInterface
{
public:
    LuMyApp();
    virtual ~LuMyApp();

    virtual int Init() override;
    virtual int UnInit() override;
    virtual void Run() override final;
    virtual void Loop() override final;
    virtual void Exit(const char *reason = 0) override;

    virtual void LoopFps() = 0;             // 每帧调用
    virtual void LoopSec() = 0;             // 每秒调用
    virtual void LoopMSec() = 0;            // 每毫秒调用
private:
    bool                                                        m_bRun = false;
    LUTimer<std::chrono::seconds>                               m_timerSec;
    LUTimer<LT_GAME_FPS>                                        m_timerFps;
};

#endif //_MYAPP_H_


