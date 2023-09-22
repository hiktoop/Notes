# iovec

iovec 和 readv，writev 等系统调用联合使用；raedv 用于一次函数调用中读多个非连续缓冲区，writev 用于将多个非连续缓冲区写入，所以有时也将这两个函数称为 scatter read (散布读)和 gather write (聚集写)，Unix 接口中后缀加 v 的都是这类函数

iovec 结构体：

```c
struct iovec {
    void *iov_base; // Pointer to data
    size_t iov_len; // Length of data
}
```

对于 redv 和 writev, 这两个操作说是原子性的，也就是说读到 iovec 中的数据总是连续的，不会因为文件偏移被其他线程改变而混乱。

preadv 和 pwritev 和上面的两个函数差不多，只是支持了文件偏移的参数，将两个系统调用合并为一个系统调用。



无端插入

constexpr 会给对象加一个隐式的 const,而且是顶层 const !

```cpp
constexpr char *msg = "hello world!";
// <==>
char * const msg = "hello world!";
// 这导致将一个字符串常量赋值给 char* 的常指针，这会得到一个编译器警告
constexpr const char *p = "hello world!"; // 当然可以
constexpr std::string_view msg = "hello world!"; // 这样更好
```


