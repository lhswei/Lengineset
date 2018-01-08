#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    p->print();
    p->echo();
}


char* get_str()
{
    char *p = "hello";
    return p;
}

void new_big()
{
    char big[1024*1024*10];
    memset(big, 0, sizeof(big));
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

    // Base* pt  = new Driver();
    // delete pt;
    // printf("\n");

    // int i = 0;
    // for(i = 0; i < 2 ; i++)
    // {
    //     fork();
    //     printf("A\n");
    // }

    // printf("%d\n", sizeof(no_st));
    
    new_big();
	return 0;

}
