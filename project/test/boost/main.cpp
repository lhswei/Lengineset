#include <boost/thread.hpp>   
#include <iostream>

class Test
{
    public: 
    Test(int a = 0 ) : m_a(a) { }
    ~Test( )
    { 
       std::cout << "Calling destructor" << std::endl; 
    }
    public: int m_a;
};

void task1() {    
    // do stuff   
    std::cout << "This is task1!" << std::endl;   
}   
     
void task2() {    
    // do stuff   
    std::cout << "This is task2!" << std::endl;   
}   
     
int main (int argc, char ** argv) {   
    using namespace boost;    
    thread thread_1 = thread(task1);   
    thread thread_2 = thread(task2);   
     
    // do other stuff   
    thread_2.join();   
    thread_1.join();   

    std::auto_ptr<Test> p(new Test(5));
    std::cout << "p->m_a = " << p->m_a << std::endl;

    return 0;   
}  