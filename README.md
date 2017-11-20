# Lengineset
# my project set

1. start cmake

2. add hiredis module

3. add libevent module

4. tcp server/client test

# 5. mysql test 
这个版本用的是c 版本的连接


6. add boost module
# 下载 boost 库
git submodule add https://github.com/boostorg/boost.git boost
cd boost
# 更新子模块
git submodule init
# 这是一个很漫长的过程
git submodule update
# 执行(得到b2)
./bootstrap.sh --prefix=/usr/local




