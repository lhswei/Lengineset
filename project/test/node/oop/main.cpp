#include <iostream>
#include <string>

class Bird 
{
public:
    Bird() {};
    virtual ~Bird(){};
    virtual void Walk() const
    {
        std::cout << "This Is A " << GetName().c_str() << std::endl;
        std::cout << "Bird Walking!" << std::endl;
    }
    virtual void Fly() const
    {
        std::cout << "Bird  Flying!" << std::endl;
    }
private:
    std::string GetName() const {return "Bird";} 
};

class PeeBird: public Bird
{
public: 
    PeeBird() {};
    ~PeeBird() {};
    virtual void Walk() const override
    {
        std::cout << "This Is A " << GetName().c_str() << std::endl;
        std::cout << "PeeBird Walking!" << std::endl;
    }
    virtual void Fly() const override
    {
        std::cout << "PeeBird  Flying!" << std::endl;
    }
private:
    std::string GetName() const {return "PeeBird";}
};

class Penguin: public Bird
{
public:
    Penguin() {};
    ~Penguin() {};
    virtual void Walk() const override
    {
        std::cout << "This Is A " << GetName().c_str() << std::endl;
        std::cout << "Penguin Walking!" << std::endl;
    }
private:

    // 企鹅不会飞，所以将Fly放到private,不让外部调用。
    // 然而由于基类的Fly是public的，通过基类指针调用的话还是可以调用到这个函数的。
    // 另外如果把const 去掉，保留override，编译报错。因为去掉const之后就不是 基类的virtual void Fly() const了。
    // 如果把const override 都去掉，编译可以通过，但不是基类的virtual void Fly() const，通过基类调用Fly编译报错。
    virtual void Fly() const override final
    {
        std::cout << "Penguin Can't Fly!" << std::endl;
    }

    std::string GetName() const {return "Penguin";} 
};

int main(int argc, char *argv[])
{
    std::cout << "hellow world!" << std::endl;

    Penguin p;
    // p.Walk();
    // p.Fly();         // Penguin 的 Fly 是 private ，编译报错

    auto b = dynamic_cast<const Bird*>(&p);
    if (b)
    {
        b->Walk();
        // Bird 类的Fly是public，virtual的，通过Bird指针调用，Fly是public，并调用的是Penguin 的Fly（即使Penguin的Fly是private的）。
        b->Fly();        
    }

    // PeeBird pe;
    // pe.Walk();
    // pe.Fly();
    // auto b1 = dynamic_cast<const Bird*>(&pe);
    // if (b1)
    // {
    //     b1->Walk();
    //     b1->Fly();
    // }

    // const PeeBird& pe = dynamic_cast<const PeeBird&>(p);    // 可以编译通过，但运行崩溃，所以dynamic_cast 最好转换成指针
                                                               // 然后判断指针是否不为null，然后再使用。
    // pe.Walk();    

    // const PeeBird* ppt = dynamic_cast<const PeeBird*>(&p);      // Penguin 不是 PeeBird* 所以运行期转换返回null;
    // if (ppt)
    //     ppt->Walk();
    // else
    //     std::cout << "Penguin is not a PeeBird." << std::endl;

    return 0;
}
