#include "json.hpp"
#include "stdlib.h"
#include "stdio.h"
#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    std::ifstream sjconfig("test.json");
    nlohmann::json j;
    sjconfig >> j;

    std::string str = j["string"].get<std::string>;
    const char *ch = str.c_str();
    printf("test.json:\n%s\n", ch);
    int n = j["number"].get<int>();
    printf("number:%d\n", n);
    printf("%s\n", j.dump(4).c_str());

    return 0;
}


