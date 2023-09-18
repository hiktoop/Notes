

# 并发 API

```cpp
// 线程
int doAsyncWork();
std::thread t(doAsyncWork);
// 任务
auto fut = std::async(doAsyncWork); // fut 表示 future
```

## 为什么基于任务(future)更好？

1. 能通过 future 的 get 函数获得任务的返回值，并提供抛出异常的访问；而 thread 中的异常会使程序直接终止。

2. 本质上抽象层次更高，使得开发者从线程管理的细节中解放出来；使用线程需要考虑线程数量和资源超额等问题。

3. 从 2 延伸而来，你确定你能写的比系统级线程池和工作窃取算法更好？

第二个好处使得我们偏向于使用任务，换个角度讲，如果我们不用，语言开发者开发它的意义是什么呢？

当然基础线程也有它的用武之处：

1. 普遍地，能够访问最为基础的线程 API，如设置线程优先级

2. 对机器了解，且要的特定的优化

3. 实现 C++ 线程中尚未实现的技术

---

```cpp
// 下面这两种方式是等价的
auto fut1 = std::async(f); // <==>
auto fut2 = std::async(std::launch::async | std::launch::deferred, f);
// async 策略要求异步执行(不同线程)
// deferred 策略在调用 get 或 wait 时同步执行
```

## 对异步任务指定 std::launch::async

考虑这样一种情形：

```cpp
void f = f() { std::this_thread::sleep_for(1s); } // 睡眠一秒
auto fut = std::async(f);
while(fut.wait_for(100ms) != std::future_statue::ready) ; // 等任务完成
```

平时这段代码可以很好的运行，但是到负载过高的时候任务采用 deferred 策略使得任务陷入循环。所以在使用默认启动时请考虑：

- 任务不需要 get 或 wait 启动

- 无关 thread_local 变量

- 保证调用 get 或 wait，或者任务不执行也能接受

- 调用 wait_for 考虑延迟(甚至所有)状态

> 使用 launch::async 觉得麻烦，使用封装好的函数 reallyAsync 吧！
> 
> ```cpp
> template<typename F, typename... Ts>
> inline auto reallyAsync(F &&f, Ts &&... params) {
> 
> }
> auto fut = reallyAsync(f);
> ```

---

## 使 thread 最终总是 unjoinable

unjoinable thread 包括：

- 默认构造（不传入函数）的 thread

- 被 move 走的 thread

- 已经被 join 的 thread

- 已经被 detach 的 thread

析构时 join 会导致难以调试的表现异常问题，析构时 detach 会导致难以调试的未定义行为，所以标准委员会规定销毁 joinable thread 将导致程序终止；而我们又不能总是确保最终 thread 是 unjoinable 的，所以可以考虑使用 RAII 技巧封装线程：

```cpp
class ThreadRAII {
public:
    snum class DtorAction { join, detach };
    ThreadRAII(std::thread&& t, DtorAction a)
    : action(a), t(std::move(t)) {}

    ~ThreadRAII() {
        if(t.joinable()) {
            if(action == DtorAction::join)
                t.join();
            else t.detach();
        }
    }
    ThreadRAII(Thread&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;
    std::thread& get() { return t; }
private:
    DtorAction action;
    std::thread t;
};
```

---

### 关注 future 的析构行为*

future 的析构函数与 thread 是如此不同，它有时执行 join 有时执行 detach，有时又都不执行，但是从不会终止程序。

调用者（可能被 shared_future 复制多次）和被调用者（执行结束后销毁）都不适合用来存储函数的结果，故堆空间会开辟一块共享状态（shared state）

- 正常析构函数会销毁 future 本身的数据状态，递减共享状态的引用计数；延迟任务不做考虑

- 引用了共享状态的最后一个 future 的析构函数会阻塞，直到任务完成(隐式join)后销毁共享状态（非 shared_thread 总是如此）

## 考虑使用 void 的 future*

~~吐槽：我并不觉得meyer的写作方式很好，可能他确实写出了许多有用的东西，但是他总是举出一段代码，然后紧接着说 “但这是错的”，虽然能给读者以警示，然而次数多了难免有不合适的地方~~

- 使用条件变量和一个 bool 变量传递信息（防止虚假唤醒）

- 让检测任务设置 promise ，反应任务用 wait 等待（只能设置一次）

## atomic 和 volatile

volatile 告诉编译器不要对内存进行优化（改变语句的顺序），而且要及时写入内存，而 volatile 不能保证这一点；但是看下面的例子：

```cpp
volatile int x;
auto y = x;
y = x; // 不会被优化
std::atomic<int> a;
auto b = a;
b = a; // 会被优化并且不能通过编译
```

atomic 不支持拷贝和移动，可以使用

```cpp
std::atomic<int> y(x.load());
y.store(x.load());
```

但这仍然会被编译器优化为

```cpp
register = x.load();
std::atomic<int> y(register);
y.store(register);
```

所以：

- std::atomic 用于并发编程，对访问特殊内存没用

- volatile 用于访问读取和写入不应被优化的内存上，对并发编程没用
