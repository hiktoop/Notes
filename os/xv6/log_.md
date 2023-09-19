# log.c

在上层，使用日志写替换掉 bio 中的 bwrite

```c
begin_op();
// ...
bp = bread();
// do something to bp->data
log_write(bp);
// ...
end_op();
```

接口：

```c
// public
void initlog(int dev, struct superblock *sb);
void begin_op();
void end_op();
void log_write(struct buf *b);
// private
struct logheader {
    int n;
    int block[LOGSIZE]; // blockno
};
struct log {
    struct spinlock lock;
    int start;
    int size;
    int outstanding; // how many fs sys calls are executing
    int committing; // in commit(), please wait
    int dev;
    struct logheader lh;
} log;
static void install_trans(int recovering);
static void read_head();
static void write_head();
static void recover_from_log();
static void write_log();
static void commit();
```

begin_op 标志着一系列系统调用的开始，end_op 是相应的结束位置，这两个函数用来管理系统调用数量，并且：

- log 正在提交或当前系统调用涉及的块超过日志空间都会造成调用 begin_op 的进程进入睡眠

- 最后一个系统调用 end_op 的时候提交所有日志到磁盘(commit)，否则 end_op 会唤醒*任意*其他调用 begin_op 的进程

log_write 作为 bwrite 的代理，将块的 blockno 记录在内存中，并为其预留一个日志中的块，接着对 buffer 进行 pin 操作以防止缓存回收它。这个块只有到提交后才能被清出缓存，在那之前缓存块是修改后的唯一记录；并且相同事务中的读能够看到修改。~~在一个事物中的多次写将被放到同一个日志块，这种优化叫 absorption~~

commit 的四个步骤：

1. 将这次事务修改的缓存写入到磁盘(write_log)

2. 将日志头写入到磁盘(write_head)

3. 将日志中的块(此时还是在缓存中)写入到相应的磁盘块，此时的参数 0 标志着不是一次恢复操作，则 unpin 这些块(install_trans(0))

4. 将磁盘的日志清空(log.lh.n = 0, write_head)

recover_from_log 被  initlog 调用，这使得重启时会读取日志文件，在有必要时进行数据恢复。
