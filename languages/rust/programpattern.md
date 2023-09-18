## 变量的可变性

```rust
const LEN:u32 = 1024; // 常量
let mut x = 5; // 可变变量
let x = 5; let x = x + 10; // 不可变变量
```

## 变量的所有权

1. rust 的每个值都有一个所有者变量

2. 值有且只有一个所有者

3. 当所有者离开作用域，这个值将被丢弃

copy 是浅拷贝，clone 是深拷贝

## owner 语义带来的复杂度

- 传递之后原成员就没有了(比如结构体的部分成员被move)

- 不能在成员方法中把成员 move 走(self 引用)

- 使用 std::mem::replace(&dest, src)

## 引用(借用)和生命周期

```rust
compare(s1: Student, s2: Student) -> bool;
compare(s1 :&Student, s2: &Student) -> bool;
// 使用 compare(&s1, &s2) 调用
```

- 要么只能有一个可变引用，要么只能有多个不可变引用

- 引用必有生命周期

- 编译器不知道的运行时事件需要标注

## 闭包与所有权

- 函数闭包

- 线程闭包

## rust 的智能指针

```rust
std::boxed::Box // unique_ptr
std::rc::Rc     // shared_ptr
rc.clone()      // 获取引用
std::rc::Weak   // weak_ptr
std::sync::Arc  // 线程安全的智能指针
Rc::downgrade(&rc) // 转化为Weak指针
```

## 小结

- rust最重要的三个概念：**不可变、所有权、Trait**

- rust倾向于 move 所有权，如果要借用使用引用

- move 所有权会增加变成复杂度，尤其是move两个变量的时候

- 引用的问题是生命周期的问题，有时需要程序员标注生命周期

- 函数式的闭包和多线程下，所有权的麻烦更多

- 使用智能指针减少一些复杂度，但是也会带来问题

- Trait 对象完成多态和函数重载的方法
