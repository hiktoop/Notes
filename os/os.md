# os

## 进程

### 进程介绍

- 进程是操作系统最基础的抽象（运行的程序）

- 调度算法决定如何管理进程

---

#### 进程的状态：运行、阻塞、就绪

---

#### 进程的组成部分

1. 内存（地址空间）

2. 寄存器

3. 打开的文件列表

---

#### 进程的实现（操作系统设计与实现2.16）：

1. 进程表（维持进程的状态、程序计数器、栈指针、内存分配情况、打开文件统计、统计和调度信息、定时器和其他信号，以及进程从运行态到就绪态切换时所要保存的信息

进程控制块

minix3 的进程表域：

| 内核                    | 进程管理        | 文件管理               |
| --------------------- | ----------- |:------------------ |
| 寄存器                   | 正文段指针       | umask掩码            |
| 程序计数器                 | 数据段指针       | 根目录                |
| 程序状态字                 | bss 段指针     | 工作目录               |
| 栈指针                   | 退出状态        | 文件描述符              |
| 进程状态                  | 信号状态        | 真实id               |
| 当前调度优先权               | 进程标识号       | 有效uid              |
| scheduling ticks left | 父进程         | 真实gid              |
| 配额大小                  | 进程组         | 有效gid              |
| 使用的cpu时间              | 子进程的cpu时间   | 控制tty              |
| 消息队列指针                | 真实uid       | 用于 read/write 的保存区 |
| 挂起的信号位                | 有效uid       | 系统调用参数             |
| 进程名字                  | 真实gid       | 各种标志位              |
|                       | 有效gid       |                    |
|                       | 代码段共享所需文件信息 |                    |
|                       | 信号位图        |                    |
|                       | 各种标志位       |                    |
|                       | 进程名字        |                    |

xv6 的进程状态

```c
struct proc {
    struct spinlock lock;          // spin lock
    enum prostate state;           // process state
    void *chan;                    // is sleeping on chan?
    int killed;                    // have been killed?
    int xstate;                    // exit status to be returned to parent's wait
    int pid;                       // process id
    struct proc *parent;           // parent process
    uint64 kstack;                 // virtual address of kernel stack
    uint64 sz;                     // size of process memory(bytes)
    pagetable_t pagetable;         // user page table
    struct trapframe *trapframe;   // data page for tarmpoline.S
    struct context context;        // swich here to run process
    struct file *ofile[NOFILE];    // open file
    struct inode *cwd;             // current dircetory
    char name[16];                 // process name
}
// more detail in xv6's proc.h
```

---

#### 进程 api：fork、wait、exec、kill

好处？：运行子进程之前改变进程环境

#### 线程

内存布局：

| 进程共享      | 线程独有  |
| --------- | ----- |
| 地址空间      | 程序计数器 |
| 全局变量      | 寄存器   |
| 打开文件      | 堆栈    |
| 子进程       | 状态    |
| 定时器       |       |
| 信号和信号处理程序 |       |
| 统计信息      |       |

- 线程是交给内核还是用户管理？

- 线程的设计任重道远

---

#### 进程环境

##### 启动例程

```c
int main(int argc, char *argv[] /*, char *envp[] */ );
// posix 取消了 environ 的第三个参数，改而使用: getenv() 和 putenv()
```

使用 exec 之后，在调用 main 函数之前，先调用一个一个特殊的启动例程，可执行文件将启动例程指定为程序的起始地址，启动例程从内核获取命令行参数和环境变量值，然后调用 main 函数

##### 进程终止

正常中止终止：

1. main 返回

2. 调用 exit

3. 调用 _exit 或 _Exit

4. 最后一个线程从启动例程返回

5. 最后一个线程调用 pthread_exit

异常中止：

1. 调用 abort

2. 收到信号

3. 最后一个线程对取消请求作出响应

在 1 中当 main 函数返回时，上面的启动例程也会调用 exit 函数退出

```c
#include <stdlib.h>
void exit(int status);     // 执行清理程序再进入内核
// 清理程序对所有的打开流调用 fclose 函数（缓冲区冲洗）
void _Exit(int status);    // 直接进入内核
#include <unistd.h>
void _exit(int status);    // 直接进入内核

#include <stdlib.h>
int atexit(void (*func) (void)); // 登记终止处理程序
// 执行顺序与登记顺序相反
```

##### 环境表

```c
extern char **environ;
// 通常使用 getenv 和 putenv 访问与修改
```

##### c 程序空间布局

- 高地址：命令行参数和环境变量

- 栈

- |

- 堆

- 未初始化数据段（由 exec 初始化为 0）

- 初始化数据段 （exec 从程序文件中读取）

- 正文段（可共享只读、从文件读取）

- 低地址

```shell
size /bin/sh
# size 命令报告正文段、数据段和 bss 段的长度及总长度
```

##### 共享库

共享库使得可执行文件中不再需要包含公用的库函数，而只需在所有进程都可引用的存储区中保存这种库例程的一个副本

- 减少可执行文件长度、库函数版本替代无需重新链接程序

- 第一次执行时增加时间开销

##### 存储空间分配

```c
#include <stdlib.h>
void *malloc(size_t size);
void *calloc(size_t size);
void *realloc(void *ptr, size_t newsize); // like go's append
void free(void *ptr);
sbrk(2); // syscall
```

##### 环境变量

```c
#include <stdlib.h>
char *getenv(const char *name);
int putenv(char *str); // 不分配空间，直接使用 str 指向的地址
int setenv(const char *name, const char *value, int rewrite);  // 分配空间
int unsetenv(const char *name);
```

##### setjmp 与 longjmp

非局部 goto

```c
#include <setjmp.h>
int setjmp(jmp_buf env); // let env be a global variable
void longjmp(jmp_buf env, int val);
// longjmp 后全局、静态和 volatile 变量保持最近状态
```

##### getrlimit 和 setrlimit

```c
#include <sys/resource.h>
int getrlimit(int resource, struct rlimit *rlptr);
int setrlimit(int resource, const struct rlimit *rlptr);
```

#### 进程控制

##### 进程标识

- pid 为 0 的是调度进程（交换进程swapper）

- pid 为 1 的是 init 进程，读取系统有关的初始化文件

```c
#include <unistd.h>
pid_t getpid(void);
pid_t getppid(void); // parent
uid_t getuid(void);
uid_t geteuid(void); // effect
gid_t getgid(void);
gid_t getegid(void);
```

##### 父进程与子进程的区别 apue8.3

##### vfork

- 期望立即调用 exec 或 exeit

- 在 exec 之前在父进程空间中运行

- 保证子线程先运行，在子线程调用 exec 或 exit 后才回到父线程

##### wait

```c
#include <sys/wait.h>
pid_t wait(int *statloc);
pid_t waipid(pid_t pid, int *statloc, int options);
```

#### 进程关系

#### 信号

### 进程间通信

### 进程调度

低级机制与高级策略分离：

1. 机制是 how， 如何执行上下文切换？

2. 策略是 which，具体执行哪个进程？

## io

## 存储管理

## 文件系统

```c
#include <strio.h>

int main() {
    int x = 10;
    int y = 3;
    printf("%d\n", x + y);
    return 0;
}
```
