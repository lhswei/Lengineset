# д���� ���ש
### Lengineset
### my project set

# 1. start cmake

# 2. add hiredis module

# 3. add libevent module

# 4. tcp server/client test

# 5. mysql test c �汾��api

# 6. add boost module
### ���� boost ��
> git submodule add https://github.com/boostorg/boost.git boost
> cd boost
### ������ģ��
> git submodule init
### ����һ���������Ĺ���
> git submodule update
> boost��������ģ�������ǳ��Ĵ�, windows �� git ���лᳬ����
> ����������һ��boost�ŵ������ط�ȥ����Ҫ��Щ�������ڷŵ� ��Ŀ¼�� include �� lib ��Ӧ�ļ�����
> ����������֮ǰʹ�� git config --add core.compression -1 [-1~9]
> -1 �� zlib ΪĬ��ѹ���⣬0 ��ʾ������ѹ����1..9 ��ѹ���ٶ������ջ���ļ���С�Ĳ�ͬ�̶ȵ�Ȩ�⣬����Խ��ѹ��Խ������Ȼ�õ����ļ���ԽС��
### ִ��(�õ�b2)
> ./bootstrap.sh --prefix=/usr/local

# 7. c++ ���� mysql api ��ģ��
> ����c++ �汾��Դ��
> git submodule add https://github.com/mysql/mysql-connector-cpp.git


