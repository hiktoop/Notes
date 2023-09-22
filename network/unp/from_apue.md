# 套接字

本章描述套接字网络进程间通信接口，进程用该接口能与其他进程通信，无论是同一台计算机还是不同的计算机。

## 套接字描述符

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
```

- domain：地址格式

| 域         | 描述        |
| --------- | --------- |
| AF_INET   | IPv4 因特网域 |
| AF_INET6  | IPv6 因特网域 |
| AF_UNIX   | UNIX 域    |
| AF_UPSPEC | 未指定       |

- type：套接字的类型

| 类型             | 描述                       |
| -------------- | ------------------------ |
| SOCK_DGRAM     | 固定长度的、无连接的、不可考的报文传输      |
| SOCK_RAW       | IP 协议的数据报接口              |
| SOCK_SEQPACKET | 固定长度的、有序的、可靠的、面向连接而的报文传输 |
| SOCK_STREAM    | 有序的、可靠的、双向的、面向连接的字节流     |

- protocol：通常为 0，表示为给定的域和套接字类型选择默认协议。当对同一域和套接字类型支持多个协议时，可以使用 protocol 选择一个特定的协议。如在 AF_INET 通信域中， SOCK_STREAM 默认是 TCP 协议，SOCK_DGRAM 默认是 UDP 协议。

| 协议           | 描述          |
| ------------ | ----------- |
| IPPROTO_IP   | IPv4 网际协议   |
| IPPROTO_IPV6 | IPv6 网际协议   |
| IPPROTO_ICMP | 英特网控制报文协议   |
| IPPROTO_RAW  | 原始 IP 数据报协议 |
| IPPROTO_TCP  | 传输控制协议      |
| IPPROTO_UDP  | 用户数据报协议     |

- SOCK_DGRAM 接口的两个对等进程之间通信时不需要逻辑连接，只需要向对等进程所使用的套接字发送一个报文。

- SOCK_STREAM 要求在交换数据之前在本地套接字和通信对等进程的套接字之间建立一个逻辑连接。其提供的是字节流服务，所以应用程序分辨不出报文的界限(即不会知道要收到的字节数)

- SOCK_RAW 套接字提供一个数据报接口，用于直接访问下面的网络层，使用这个接口时，应用程序负责构造自己的协议头部，此时传输协议被绕过了。创建一个原始套接字需要超级用户权限。

套接字通信是双向的，可以采用 shutdown 函数来禁止一个套接字的 I/O

```c
#include <sys/socket.h>
int shutdown(int sockfd, int how);
// how == SHUT_RD 关闭读端
// how == SHUT_WR 关闭写端
```

## 寻址

进程标识 = 网络地址 + 端口号

### 字节序

要即多少遍才能记住：

- 大端：最大字节地址出现在最低有效字节

- 小端：与大端相反

最高有效字节总是在左边，小端的字节与之对应

> 网络协议使用大端字节序

处理器字节序和网络字节序之间转换的函数：

```c
#include <arpa/inet.h>
uint32_t htonl(uint32_t hostint32);
uint16_t htons(uint16_t hostint16);
uint32_t ntohl(uint32_t hostint32);
uint16_t ntohs(uint16_t hostint16);
// h 主机 n 网络 s short l long
```

### 地址格式

为使不同格式地址能够传到套接字函数，地址会被强制转换成一个通用的地址结构 sockaddr：

```c
struct sockaddr {
    sa_family_t    sa_family; // address family
    char           sa_sata[14]; // variable-length address
    // ...
}
```

套接字实现可以自由地添加额外的成员并且定义 da_data 成员的大小。

二进制地址格式与点分二进制字符之间转换的函数：

```c
#include <arpa/inet.h>
const char *inet_ntop(int domain, const void *restrict addr,
                      char *restrict str, socklen_t size);
int inet_pton(int domain, const char *restrict str,
              void *restrict addr);
```

### 地址查询

```c
#include <netdb.h>
struct hostent *gethostent();
void sethostent(int stayopen);
void endhostent();
struct hostent {
    char *h_name;
    char **h_aliases;
    int  h_addrtype;
    int  h_length;
    char **h_addr_list;
}
```


