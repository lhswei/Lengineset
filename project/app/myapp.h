#ifndef _MYAPP_H_
#define _MYAPP_H_

#include "myappdef.h"

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

    virtual void SetFPSInterval(unsigned uMilliseconds) = 0;
    virtual unsigned GetFPSInterval() = 0;

};

class LuMyAppInterface: public MyAppInterface
{
public:
    LuMyAppInterface() {};
    virtual ~LuMyAppInterface() {};

    virtual void SetFPSInterval(unsigned uMilliseconds) override {m_nFpsInterval = uMilliseconds;}
    virtual unsigned GetFPSInterval() override {return m_nFpsInterval;}

private:
    unsigned m_nFpsInterval = s_fpsinterval;

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
    bool    m_bRun = false;

};

#endif //_MYAPP_H_