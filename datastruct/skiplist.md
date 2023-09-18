## 对比

rbt（红黑树）：

1. 插入、查询`O(logn)`

2. 数据有序

3. 并发复杂

skiplist（跳表）：

1. 插入、查询`O(logn)`

2. 数据有序

3. 实现简单

4. 无需加锁

## 特性

1. 分层，底层包含所有元素

2. 每层都是有序的列表

3. 底层包含高层的元素

4. 每个节点元素由key、value和指向当前节点的level的指针数组组成

论文：

https://www.cl.cam.ac.uk/teaching/2005/Algorithms/skiplists.pdf

https://15721.courses.cs.cmu.edu/spring2018/papers/08-oltpindexes1/pugh-skiplists-cacm1990.pdf
