#include <iostream>
#include <string>

class T_New
{
public:
    T_New()
    :x(10),
    y(12)
    {
        std::cout << " C++ Constructor." << std::endl;
    }
    ~T_New()
    {
        std::cout << " C++ destructor." << std::endl;
    }

    void* operator new (size_t size, void* p, const std::string& str)
    {
        std::cout << "sizeof(T_New) = " << size << std::endl;
        std::cout << "in our own operator new." << std::endl;
        std::cout << str << std::endl;
        if(!p)
        {
            std::cout << "pointer p is null, now use default ::operator new." << std::endl;
            return ::operator new(size);
        }
        return p;
    }

    void operator delete (void* p)
    {
        std::cout << "in our own operator delete." << std::endl;
        std::cout << "We should do nothing in operator delete." << std::endl;
        // 如果取消下一行的注释，程序会在执行时crash
        // ::operator delete(p);
    }

    void f()
    {
        std::cout << "hello object: x = " << x << std::endl;
        std::cout << "hello object: y = " << y << std::endl;
    }
private:
    int x;
    char a;
    int y;
    int z;
    char b;
};

void t_array(size_t len)
{
    char buff[len];
    size_t l = sizeof(buff);
    std::cout << "t_array: l = " << l << std::endl;
}

int main(int argc, char *argv[])
{
    size_t size = sizeof(T_New);
    char buff[size];
    std::cout << "sizeof(T_New) = " << size << std::endl;
    T_New* pt = new(buff, "Yeah, I'm crazy!")T_New();
    pt->f();
    delete pt;

    // T_New tn;

    // t_array(11);

    return 0;
}