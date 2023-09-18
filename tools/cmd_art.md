# 命令行知识
## 基础
### cat/more/less/head/tail 的区别
- cat 命令可以一次显示整个文件，文件较大时不方便
- more 命令让屏幕在显示满一屏幕时暂停，按空格往前翻页，按 b 往后翻页
- less 命令分页显示文件，和 more的区别在于支持上下键卷动屏幕、查找；不需要一开始就读整个文件，打开大文件的时候比 more、vim 更快
- head 查看文件的前 n 行，-n 后加行数
- tail 查看文件的后 n 行，加上 -f 查看在线日志很方便，可以打印最新增加的日志

### &、&& 和 ; 的区别
```shell
# 同时执行三个命令
cmd1 & cmd2 & cmd3

# 不管前面的命令执行成功与否，继续执行后面的任务
cmd1; cmd2; cmd3

# 要有前面的任务执行完成才能执行后面的任务
cmd1 && cmd2
```
### ln
```shell
ln [-s] src dsc
```
硬链接：
1. 不允许给目录创建链接
2. 只有同一文件系统才能创建硬链接，而且要超级用户权限
硬链接就是文件的另一个指针(指向 inode)，只有所有指针都被删除文件才会被删除

软链接：
1. 软链接没有以上的两点限制
2. 软链接基于源文件的路径，类似于 windows 的快捷方式

### df
查看文件系统的硬盘使用情况

### fdisk
操纵硬盘分区表

### mkfs
build a linux file system

### lsblk
list block devices

### inode 是什么
- ls -i
- df -i

### network

#### ip

#### dig
DNS lookup utiles

#### regular expression
- grep/egrep
- -i -o -v -A -B -C

### apt/yum/pip

## 日常使用
- C-r: search the cmd line history, right buttom for put in line

- C-w: delete the last word you input
- C-u: delete the content before cursor
- C-k: delete the content after cursor

- alt-b,alt-f: move cursor in words

- C-a: put cursor at begin of the line
- C-e: put cursor at end of the line

- man readline: see Bash quick key board

- !$: last args
- !n: n for history cmd
- !!: last cmd

- use xargs

- man 7 signal

## 文件及数据处理

## 系统调试

## 单行脚本
```shell
sort a b | uniq > c         # c is a | b
sort a b | uniq -d > c      # c is a & b
sort a b b | uniq -u > c    # c is a - b

grep . *
head -100 *

# print all number's sum in columns 3 of myfile
# is so fast that more three times than python
awk '{ x += $3 } END { print x }' myfile

# more lookful than ls -l
find . -type f -ls

```

## 冷门但有用:
- bc: 计算器
- factor: 分解因数
- expr: 计算正则表达式
- yes: 打印多次字符串
- nl: 添加行号
- time: 执行命令并计算执行时间
- timeout: 指定执行命令的时长，超时后结束进程
- tac: 反向输出文件
[the art of command
line](https://github.com/jlevy/the-art-of-command-line/blob/master/README-zh.md)

