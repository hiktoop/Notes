# bio.c

```c
// public 
void binit();
struct buf* bread(uint dev, uint blockno);
void bwrite(struct buf *b);
void brelse(struct buf *b);
void bpin(struct buf *b);
void bunpin(struct buf *b);
// private:
struct {
    struct spinlock lock;
    struct buf buf[NBUF];
    struct buf head;
} bcache;
static struct buf* bget(uint dev, uint blockno);
```

bio 采用 LRU 策略(替换最近最少使用的缓存块)，所以 bcache 是一个双向链表，binit 初始化 bcache 的锁和每个 buf 结点的锁，并将 buf 依次串接成链表，之后的所有访问通过 head 访问而不通过 buf 数组

buffer 的两种状态：是否有效通过 valid 参数表示，disk 表示磁盘的块是否与 buffer 相同

锁的作用：

- bcache.lock 保证了哪些块是被缓存的以及链表完整性

- sleeplock 保证了读写块内容的进程总是只有一个

而这使得获取 sleeplock 能在释放 bcache.lock 之后进行，refcnt 不为 0 保证了其不被换出。



bget 执行流程：

1. 找到缓存后获取锁并返回

2. 找不到缓存，找到 LRU 缓存从磁盘读入替换并获取锁返回

3. 沒有空闲位置，panic (一种改进是睡眠直到有空闲的缓存)

brelse 释放 sleeplock 并将 buffer 移到链表首部。


