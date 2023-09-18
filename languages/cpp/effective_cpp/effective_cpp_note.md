# effective cpp note

## 导言

- 编写易于理解、可维护、易移植、可扩展、效率高的代码

- 选择合适的特性

- 认识到编译器未定义的行为

- **通过案例学习背后的原理**

## 习惯c++

### cpp是语言联邦

- 过程化(procedural)

- 面向对象(object-oriented)

- 函数化(functional)

- 泛型(generic)

- 元编程(metaprogramming)

> 如此丰富，如何避免混乱？

#### 分类

1. C

2. Object-Oriented cpp

3. Template cpp

4. STL

### 用consts/enums/inlines取代#definie

#### 原理

`#define`不是语言的一部分，预编译的结果：

- 直接替换，不加入符号表 --> 报错常量引人疑惑、多处拷贝占用空间

#### 类内常量

只需声明：`static const int pi = 3.14;`c++要求为使用的任何东西提供定义，但是static整型族的类内常量是例外。

此时不能使用它的地址，除非你给出定义：`const int item::pi;`注意是不需要初始值的。

> 比较老的编译器声明时初始化是不被允许的，这时定义时初始化即可。

#### the enum hack

当你需要在类内声明一个常量数组时，这样：

```cpp
class item{
private: 
    enum {num = 5};
    int arr[num];
}
```

`enum`更像是`#define`而不是`const`，没有地址，不可被引用或指向。

#### 对function-like macros用inline-functions取代

感受：这种宏确实很难理解，但是为何很多地方还要用呢？

### 尽可能用const

谋求编译器做出限制，**人是经常出错的**、**限制就是确定，反之可能是未知**

> `int const *a == const int *a ！= int * const a`，**从右往左读**

#### STL iterator以pointer为原型

`iterator`类似于`T *pointer`，故`const iterator`类似于声明指针为`const`，而其指向的东西可变。

```cpp
vector<int> vec;
const std::vector<int>::iterator it = vec.begin();
*it = 10 // OK
it++; // error! it is const
std::vector<int>::const_iterator it = vec.begin();
*it = 10; // error! *it is const
it++; // OK
```

#### const 成员函数

```c
const char &operator[](size_t pos) const {}
```

> 两个成员函数如果只是常量性不同，可以被重载

##### bitwise constness(编译器强制实施)

`const`成员函数不可以更改对象内任何`non-static`成员变量，值得注意的是**修改成员內指针的值**是满足此条件的。例如这可能导致成员内`char *`变量的改变。

##### logical constness

> 把可能在常量函数中修改的变量定义为`mutable`

客户端检测不出时函数可以修改它所处理的对象的某些bits

##### 相同的实现时，non-const函数调用const函数，而不是相反！

> 怎么能对外保证`const`而实际用`non-const`呢？及其失信！

### 确认对象使用前已经初始化

- 内置类型手工初始化

- 自定义类型确保每个构造函数对每个成员都初始化，**注意区分初始化与赋值**

对象的成员变量的初始化动作发生在进入构造函数本体之前，一般是`default`构造函数。

> 使用`menber initialization list`替换赋值动作：

```c
item::item(const string &name, const list &phones) {
    thename = name; thePhones = phones; num = 0; // 先default初始化后才赋值！
}
item::item(const string &name, const list &phones) :
    theName(name), thePhones(phones), num(0) {} // 是初始化，效率很高！
```

> 尽量以声明的顺序书写初值列表

感受：这一条款的后半部分有些复杂，需要重读

## 构造/析构/赋值运算

> 兵者，国之大事，死生之地，存亡之道，不可不察也！
> 
>                                                                             ----孙子

### c++默认编写并调用的函数

### 不想编译器生成就明确拒绝

### 为多态基类声明vaitual析构函数

### 别让异常逃离析构函数

### 绝不在构造和析构过程中调用virtual

### 令 operator= 返回 reference to *this

### 在 operator= 中处理“自我赋值”

### 复制对象时勿忘其每一个成分

## 资源管理

### 以对象管理资源

### 在资源管理类中小心 coping 行为

### 在资源管理类中提供对原始资源的访问

### 成对使用new和delete时要采取相同形式

### 以独立语句将newed对象置入智能指针

## 设计与声明

### 让接口容易被正确使用，不容易误用

### 设计class犹如设计type

### 宁以pass-by-reference-to-const替换pass-by-value

### 返回对象时不返回其reference

### 将成员变量声明为private

### 以non-member、non-friend替换member函数

### 若所有参数皆需类型转换，采用non-member函数

### 考虑写出一个不抛异常的swap函数

## 实现

### 延后变量定义式的出现

### 少做类型转换

### 避免返回handles指向对象内部成分

### 为“异常安全”努力

### 透彻了解inlining

### 将文件间的编译依赖关系降到最低

## 继承与面向对象

### 确保public继承是is-a关系

### 避免遮盖继承而来的名称

### 区分接口继承和实现继承

### 考虑virtual函数以外的选择

### 不重新定义继承而来的non-virtual函数

### 不重新定义继承而来的缺省参数值

### 通过复合塑模出has-a

### 明智使用private

### 明智使用多重继承

## 模板与泛型编程

### 了解隐式接口和编译期多态

### 了解typename的双重意义

### 学习处理模板化基类内的名称

### 将与参数无关的代码抽离templates

### 运用成员函数模板接受所有兼容类型

### 需要类型转换时为模板定义非成员函数

### 使用traits class表现类型信息

### 认识template元编程

## 定制new和delete

### 了解new-handler的行为

### 了解new和delete的合理替换时机

### 编写new和delete时固守常规

### 写了placement new也要写placement delete

# 
