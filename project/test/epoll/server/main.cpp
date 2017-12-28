#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <string>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#include "json.hpp"
#include "platform.h"

#define _TCPTEST_IPADDRESS              "127.0.0.1"
#define _TCPTEST_DEFAULT_SERVER_PORT    8869
#define _TCPTEST_MAX_CONN_WAIT          5
#define _TCPTEST_CHAR_IP_LEN            33
#define _TCPTEST_MAX_BUFFER_SIZE        1024
#define _TCPTEST_FDSIZE                 1000
#define _TCPTEST_EPOLLEVENTS            100

struct JS_CONN_CONFIG
{
public:
    JS_CONN_CONFIG() {};
    ~JS_CONN_CONFIG() {};

    void load(std::string fconfig) 
    {
        // json = 
        // {
        //     "ip" : "10.20.77.106",
        //     "port" : 8869,
        //     "maxwait" : 5,
        //     "fdssize" : 1000,
        //     "eventepolls" : 100
        // } 
        std::ifstream sjconfig(fconfig.c_str());
        nlohmann::json j;
        sjconfig >> j;
        
        m_ip = j["ip"].get<std::string>();
        m_port = j["port"].get<int>();
        m_max_wait = j["maxwait"].get<int>();
        m_fds_size = j["fdssize"].get<int>();
        m_epoll_events = j["eventepolls"].get<int>();
    }
public:
    int m_port = _TCPTEST_DEFAULT_SERVER_PORT;
    int m_max_wait = _TCPTEST_MAX_CONN_WAIT;
    int m_fds_size = _TCPTEST_FDSIZE;
    int m_epoll_events = _TCPTEST_EPOLLEVENTS;
    std::string m_ip = _TCPTEST_IPADDRESS;
};

/*函数声明*/
// 创建套接字并进行绑定
int socket_bind(const char* ip, int port);

// io 多路复用
void do_epoll(int listenfd);

// 事件处理函数
void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);

// 处理接收到的连接
void handle_accept(int epollfd, int listenfd);

// 读处理
void do_read(int epollfd, int fd, char *buff);

// 写处理
void do_write(int epollfd, int fd, char *buf);

// 添加事件
void add_event(int epollfd, int fd, int state);

// 修改事件
void modify_event(int epollfd, int fd, int state);

// 删除事件
void delete_event(int epollfd, int fd, int state);

JS_CONN_CONFIG g_config;

int main(int argc, char *argv[])
{
    int listenfd;
    g_config.load("../config.json");
    listenfd = socket_bind(g_config.m_ip.c_str(), g_config.m_port);
    listen(listenfd, g_config.m_max_wait);
    do_epoll(listenfd);
    close(listenfd);
    return 0;
}

int socket_bind(const char *ip, int port)
{
    int listenfd;

    sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1)
    {
        perror("socket erro!\n");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    int ret = bind(listenfd, (sockaddr*)&servaddr, sizeof(servaddr));
    if (ret == -1)
    {
        perror("bind error\n");
        exit(1);
    }

    return listenfd;
}


void do_epoll(int listenfd)
{
    int epollfd;
    struct epoll_event events[_TCPTEST_EPOLLEVENTS];
    int ret;
    char buff[_TCPTEST_MAX_BUFFER_SIZE];
    memset(buff, 0, _TCPTEST_MAX_BUFFER_SIZE);

    // 创建一个描述符
    epollfd = epoll_create(g_config.m_fds_size);
    // 添加监听描述符
    add_event(epollfd, listenfd, EPOLLIN);

    while (1)
    {
        // 获取已经准备好的描述符事件
        ret = epoll_wait(epollfd, events, _TCPTEST_EPOLLEVENTS, -1);
        handle_events(epollfd, events, ret, listenfd, buff);
    }
    close(epollfd);
}

void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buff)
{
    int i = 0;
    int fd;

    // 进行选好的遍历
    for (i = 0; i < num; ++i)
    {
        fd = events[i].data.fd;
        // 根据描述符的类型和事件类型进行处理
        if ((fd == listenfd) && (events[i].events & EPOLLIN))
            handle_accept(epollfd, listenfd);
        else if(events[i].events & EPOLLIN)
            do_read(epollfd, fd, buff);
        else if(events[i].events & EPOLLOUT)
            do_write(epollfd, fd, buff);
    }
}

void handle_accept(int epollfd, int listenfd)
{
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    clifd = accept(listenfd, (sockaddr*)&cliaddr, &cliaddrlen);
    if(clifd == -1)
        perror("accept erro!");
    else
    {
        printf("accept a new client: %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
        // 添加一个客户描述符事件
        add_event(epollfd, clifd, EPOLLIN);
    }
}

void do_read(int epollfd, int fd, char *buff)
{
    int nread;
    nread = read(fd, buff, _TCPTEST_MAX_BUFFER_SIZE);
    if (nread == -1)
    {
        perror("read error:");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else if (nread == 0)
    {
        printf("client close!\n");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    }
    else 
    {
        printf("read message is: %s", buff);

        // 修改描述符对应的事件, 由读改为写
        modify_event(epollfd, fd, EPOLLOUT);
    }
}

void do_write(int epollfd, int fd, char *buff)
{
    int nwrite;
    nwrite = write(fd, buff, strlen(buff));

    if (nwrite == -1)
    {
        perror("write erro:");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    }
    else
        modify_event(epollfd, fd, EPOLLIN);

    memset(buff, 0, _TCPTEST_MAX_BUFFER_SIZE);
}

void add_event(int epollfd, int fd, int nstate)
{
    struct epoll_event ev;
    ev.events = nstate;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void delete_event(int epollfd, int fd, int nstate)
{
    struct epoll_event ev;
    ev.events = nstate;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);
}

void modify_event(int epollfd, int fd, int nstate)
{
    struct epoll_event ev;
    ev.events = nstate;
    ev.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}



















