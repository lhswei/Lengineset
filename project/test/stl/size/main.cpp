#include <iostream>
#include <vector>
#include <map>
#include <list>

void t_sizeof_vector()
{
    std::vector<int> v(10);
    std::cout << "sizeof(std::vector<int>) = " << sizeof(std::vector<int>)
              << ": sizeof(v) = " << sizeof(v)
              << ": v.size() = " << v.size()
              << ": v.capacity() = " << v.capacity()
              << std::endl;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    std::cout << "sizeof(std::vector<int>) = " << sizeof(std::vector<int>)
              << ": sizeof(v) = " << sizeof(v)
              << ": v.size() = " << v.size()
              << ": v.capacity() = " << v.capacity()
              << std::endl
              << "sizeof(&v[0]) = " << sizeof(sizeof(&v[0]))
              << ": &v = " << &v
              << ": &v[0] = " << &v[0]
              << std::endl;

    std::vector<char> v1;
    std::cout << "sizeof(std::vector<char>) = " << sizeof(std::vector<char>)
              << ": sizeof(v1) = " << sizeof(v1)
              << ": v1.size() = " << v1.size()
              << ": v1.capacity() = " << v1.capacity()
              << std::endl;
}

void t_sizeof_map()
{
    void* p = NULL;
    std::cout << "sizeof(p) = " << sizeof(p) << std::endl;

    std::map<int, int> m;
    std::cout << "sizeof(std::map<int, int>) = " << sizeof(std::map<int, int>)
              << ": sizeof(m) = " << sizeof(m)
              << ": m.size() = " << m.size()
              << std::endl;

    m[1] = 1;
    std::cout << "sizeof(std::map<int, int>) = " << sizeof(std::map<int, int>)
              << ": sizeof(m) = " << sizeof(m)
              << ": m.size() = " << m.size()
              << std::endl;
}

int main(int argc, char* argv[])
{
    t_sizeof_vector();
    t_sizeof_map();
    return 0;
}