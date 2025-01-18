## IOC C++ Implement

### 单例模式

IOC容器负责管理对象的生命周期，依赖关系以及提供对象的实例。通常被设计称唯一的（如果应用模块化拆分，可能每个模块有一个IOC容器，但在单个模块下，IOC容器仍然是全局唯一的）

如何实现一个全局唯一的容器？ **单例模式**

```c++
class ApplicationContext
{
private:
    ApplicationContext() {} 
public:
    static ApplicationContext &instance()
    {                                       
        static ApplicationContext instance;
        return instance;
    }
}
```

1. 构造函数私有化：确保外界无法创建ApplicationContext的实例
2. 局部静态变量：C++11标准保证了静态局部变量的初始化是线程安全的
   1. 静态私有变量：存在线程安全问题，也即当多个线程同时访问getInstance方法时可能会创建多个实例，此时需要添加互斥锁来保证线程安全
3. 静态方法获取实例：实现了单例对象的懒加载


### 依赖注入

实例对象的获取，参考Spring ioc，有以下几种方式：
1. 根据类型获取实例（此处实现后者）
   1. 如果容器中存在多个同类型实例，需要配置有限使用哪个实例，否则会报错
2. 根据名称获取实例

依赖注入的优势：
1. 解耦：不需要直接在类中实例化依赖的对象
   1. 如果其依赖对象名称发生变动，

#### RTTI
> Run-Time Type Information，运行时类型识别技术

提供了以下接口：
1. `typeid`：返回`type_info`对象，包含有关类型的信息，其`name()`函数返回类型名称（编译器决定）
2. `dynamic_cast`：运行时类型转换，常用于基类指针到派生类指针转换，可确保转换安全
   1. `static_cast`：编译时类型转换，常用于处理基本类型之间的转换

两个函数使用的前提是：开启RTTI，而开启RTTI的条件是，基类中存在虚函数。

```c++
template <typename T>
std::shared_ptr<T> get()
{
    auto it = object_map.find(typeid(T).name());
    if (it != object_map.end())
    {
        // 将shared_ptr -> shared_ptr<T>
        return std::static_pointer_cast<T>(it->second);
    }
    // 找不到该类型的实例，便创建并添加到map中（依赖注入）
    auto obj = std::make_shared<T>();
    object_map[typeid(T).name()] = obj;
    return obj;
}
```