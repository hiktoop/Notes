# 智能指针

~~幽默：你能指出原始指针的六点不足吗？~~

## unique_ptr

- 只可移动的独有指针

- 工厂模式返回unique智能指针，调用者可以直接用shared指针接收。

- unique好在内存和普通指针一样少，但是有删除器并且删除器是函数时内存变多，而用不带捕获(无状态)的lambda表达式作为删除器就不会占用内存。

- 指针指向实现(pimpl idiom)减弱编译依赖一般用独有指针实现

## shared_ptr

- 移动比拷贝快，因为不需要改变计数（原子操作）

- 不同于独有指针，共享指针不把删除器作为类型的一部分

- 每个指针管理的对象都有控制块，用于存放引用计数、删除器等

- make_shared，从unique_ptr及原始指针转化都会生成控制块

- 建议不要使用原始指针初始化，而是用make或new初始化指针（避免多次构造而引起多次析构）

## weak_ptr

效率与 shared_ptr 相当，不参与资源所有权，不能解引用，与 shared_ptr 配合使用

```cpp
auto sp = std::make_shared<T>();
std::weak_ptr<T> wp(sp);
if(wp.expired()) /* ... */ ; // 是否过期
std::shared_ptr<T> sp2 = wp.lock(); // 过期时为空
std::shared_ptr<T> sp3(wp);         // 过期时抛出异常
```

前面说到用 unique_ptr 实现工厂模式，当需要缓存的时候可以用 weak_ptr 实现：

```cpp
std::shared_ptr<const T> fastLoadT(int id) {
    static std::unordered_map<int, std::weak_ptr<const T> cache;
    auto obj = cache[id].lock();
    if(!obj) {
        obj = loadT(id); // 之前的工厂模式
        cache[id] = obj; // 缓存
    }
    return obj;
}
```

观察者模式也是 weak_ptr 的一个应用场景，subjects 持有指向 observers 的 weak_ptr，当观察者取消订阅的时候，subjects 可以知道。

一般的使用指针的数据结构一般都是分层的，子节点只被父节点持有，当父节点销毁的时候子节点已经被销毁，所以可以使用 unique_ptr 指向子节点，用原始指针指向父节点。但是如果不是这种严格分层结构，就要考虑使用 weak_ptr 。
