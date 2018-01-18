#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

class Base;
void tprint(Base* p);

struct no_st
{

};

class Base
{
public:
    Base() {};
    virtual ~Base() {tprint(this);}
    void print ()
    {
        printf("pb#");
    }
    virtual void echo()
    {
        printf("eb#");
    }
};


class Driver : public Base
{
public:
    Driver() {};
    virtual ~Driver() {tprint(this);}
    void print ()
    {
        printf("pd#");
    }
    virtual void echo()
    {
        printf("ed#");
    }
};

void tprint(Base* p)
{
    // p->print();
    // p->echo();
}


char* get_str()
{
    char *p = "hello";
    return p;
}

void new_big()
{
    char big[1024*1024*10]; // 根据系统的不同，函数栈的大小不一样。在栈上的数据大小超过了函数栈的大小，程序运行了就有可能引发崩溃。
    memset(big, 0, sizeof(big));
}

void new_array()
{
    std::string* pal = new std::string[4];  // 调用了new []
    pal[0].assign("a");
    printf("\n%s\n", pal[0].c_str());
    delete pal; // 调用delete 而非 delete [] 是未定义的行为。不可取。
}

int main(int argc, char *argv[])
{

	// char str1[] = "hello";
	// char str2[] = {'h', 'e', 'l', 'l', 'o'};
	// char str3[] = {'h', 'e', 'l', 'l', 'o', '\0'};

	// printf("%d, %d\n", sizeof(str1), strlen(str1));
	// printf("%d, %d\n", sizeof(str2), strlen(str2));//linux 32位 c++ 4.1.0 是（5， 10） linux 64 位 5.2 是 （5， 5）
	// printf("%d, %d\n", sizeof(str3), strlen(str3));

    // char *p = get_str();
    // printf("%s\n", p);

    Driver* pd  = new Driver();
    pd->echo();
    Base* pb = dynamic_cast<Base*>(pd);
    pb->echo();
    delete pd;
    printf("\n");

    new_array();

    // int i = 0;
    // for(i = 0; i < 2 ; i++)
    // {
    //     fork();
    //     printf("A\n");
    // }

    // printf("%d\n", sizeof(no_st));
    
    // new_big();
	return 0;

}
