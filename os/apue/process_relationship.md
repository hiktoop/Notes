# 进程关系

## 进程组

- 同一进程组中各个进程接收来自的同一终端的信号

- 每个进程组有同一个进程组 ID，与原始组长的 pid 一致

- 只要进程组还有进程，该进程组就存在

- 一个进程只能为自己或子进程设置进程组 ID，子进程调用 exec 后就不能再更改其进程组 ID

> 大多数作业控制 shell 中，通过 fork 之后父子进程冗余设置进程组 ID 进行设置

```c
#include <unidtd.h>
pid_t getpgrp(); // 返回调用进程的进程组 ID
// 返回 pid 进程的进程组 ID，为 0 时返回调用者的进程组 ID
pid_t getpgid(pid_t pid);
// 加入进程组或创建新的进程组，都可为 0 表示 当前进程、前面的 pid
int setpgid(pid_t pid, pid_t pgid);
```

## 会话

- 会话是一个或多个进程组的集合

```c
#include <unistd.h>
pid_t setsid(); // 设置新会话，返回进程组 ID
pid_t getsid(pid_t pid); // 返回 pid 的会话首进程的进程组 ID
```

1. 调用者不是一个进程组组长，那么
   
   1. 该进程变成新会话的首进程
   
   2. 该进程成为一个新进程组的组长
   
   3. 该进程没有控制终端，有的话也会被切断

2. 否则出错

> 通常是先 fork 再终止父进程

![](../../iamge/apue/Screenshot%20from%202023-09-19%2013-25-37.png)

## tcgetpgrp, tcsetpgrp, tcgetsid

```c
#include <unistd.h>
pid_t tcgetpgrp(int fd); // 返回 fd 绑定的终端的前台进程组 ID
int tcsetpgrp(int fd, pid_t pgrpid); // 将前台进程组 ID 设置为同一会话的进程组 ID
#include <termios.h>
pid_t tcgetsid(int fd); // 返回会话首进程的进程组 ID

```

## 作业控制





![](../../iamge/apue/Screenshot%20from%202023-09-19%2013-33-07.png)


