# 写代码 搬搬砖
### Lengineset
### my project set

# 1. start cmake
> 开始我的cmake之旅
# 2. add hiredis module

# 3. add libevent module

# 4. tcp server/client test

# 5. mysql test c 版本的api

# 6. add boost module
### 下载 boost 库
> git submodule add https://github.com/boostorg/boost.git boost

> cd boost
### 更新子模块
> git submodule init
### 这是一个很漫长的过程
> git submodule update

> boost的所有子模块下完后非常的大, windows 的 git 运行会超级慢

> 所以下完后，我会把boost放到其他地方去，需要哪些库编完后在放到 根目录的 include 和 lib 相应文件夹下

> 可以在下载之前使用 git config --add core.compression -1 [-1~9]

> -1 以 zlib 为默认压缩库，0 表示不进行压缩，1..9 是压缩速度与最终获得文件大小的不同程度的权衡，数字越大，压缩越慢，当然得到的文件会越小。

### 执行(得到b2)
> ./bootstrap.sh --prefix=/usr/local
### 编译所有
> ./b2 install --with=all
### 指定编译
> ./b2 install --with=thread

# 7. c++ 连接 mysql api 子模块
> 下载c++ 版本的源码

> git submodule add https://github.com/mysql/mysql-connector-cpp.git

> cd mysql-connector-cpp/

> 进入下载的目录, -DWITH_BOOST=<Boost location> (我放的目录是 /usr/local/lib libboost_xxx.a这类文件的目录) -DCMAKE_INSTALL_PREFIX=<Install location>

> cmake ./ -DCMAKE_BUILD_TYPE=Debug -DWITH_BOOST=/usr/local/lib -DBoost_INCLUDE_DIR:PATH=/usr/local/include -DCMAKE_INSTALL_PREFIX=/home/cmake_install/mysqlcpp -DBUILD_STATIC=yes

# c++ libcurl 
> get json

# lua5.3

LUA_BUILD_AS_DLL

> cmake ./ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/home/cmake_install/lua53

