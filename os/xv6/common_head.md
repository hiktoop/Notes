# 公共头文件

buf.h:

```c
struct buf {
    int valid;
    int disk;
    uint dev;
    uint blockno;
    struct sleeplock lock;
    struct buf *prev;
    struct buf *next;
    uchar data[BSIZE];
}
```
