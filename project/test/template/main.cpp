#include <stdlib.h>
#include "templatefun.h"
#include <vector>

class Test
{
public:
    Test(std::string name) {m_name = name; printf("test %s constructor!\n", name.c_str());}
    ~Test() {printf("test %s desconstructor!\n", m_name.c_str());}
private:
    std::string m_name;
};

int main(int argc, char *argv[])
{
    int x = 7;
    int a = tf_add(x, x);
    float y = 7.1;
    float b = tf_add(y, y);
    double z = 8.2;
    double c = tf_add(z, z);

    tf_max(x, x);
    tf_max<double>(2.1, 2.3);

    tf_add(3.2, 2.3);
    int u = tf_add<int>(3.5, 2.6);
    int v = tf_add<double>(3.5, 2.6);

    std::string strInt = tf_number2str(a);
    std::string strFloat = tf_number2str(b);
    std::string strD = tf_number2str(c);
    std::string strU = tf_number2str(u);
    std::string strV = tf_number2str(v);

    printf("a = %s, b = %s, c = %s, u = %s, v = %s\n", strInt.c_str(), strFloat.c_str(), strD.c_str(), strU.c_str(), strV.c_str());

    tf_h(2, 5);

    std::vector<Test> vT1;
    printf("======================1\n");    
    Test t1("t1"), t2("t2"), t3("t3");
    printf("======================2\n");
    vT1.push_back(t1);
    printf("======================3\n");
    vT1.push_back(t2);
    printf("======================4\n");
    vT1.push_back(t3);
    printf("======================5\n");

    vT1.clear();
    printf("======================6\n");

    return 0;
}


