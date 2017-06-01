// 00_总记.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int main()
{
    return 0;
}

//条款02：尽量以const enum inline 替换#define
// 1.string 往往比 char* 合适
// 
// 2.class专属常量，static const int NumTurns=5; 
// static确保只有一份，写作成员确保作用域，可加private等控制符
// 但之后必须在实现文件里面进行定义：
// const int ClassName::NumTurns;(现在的新编译器通常都可以in-class初值设定)
// 

// 3.尽量以const常量替换#define，
// 如：#define ASPECT_RATIO 1.653=>const double Aspect_Ratio=1.653
// 类内无法in-class初值设定的时候，用enum实现
//
// 4.用inline模板函数替换宏函数
// 5.#include #ifdef 仍然无法被替代

//条款03:多使用const
// 1.const在*左边，被指是常量，const在*右边，指针是常量，两边，都是常量
// 所以const int* a;和int const * a;都是指向常量
// 
// 2.mutable修饰的成员，即使在const成员函数内也可以被修改
// 3.返回const，可以防止一些无意义的客户端错误

//条款04:确保对象在使用前被初始化
// 1.使用成员初值列（member initialization list）进行初始化
// 及时内置内型初始化和拷贝赋值的代价一致，建议统一
// 初值列里面可以直接()，不带值，代表调用默认初始化
// const和reference必须使用初值列初始化，不能被赋值
// 
// 2. static对象的析构函数会在main结束时自动调用
// 
// 3.不同编译单元(源文件)的初始化顺序是不明确的，
// 所以使用local-static对象替换non-local-static对象(global)，类似于单例模式
//
// 
//条款05:C++编译器默认生成的函数 
// 1.声明一个空类(class A{};),编译器自动生成:(1):默认构造函数(2)拷贝构造(3)默认析构(4)拷贝赋值
// 这些函数只有被调用，才会被编译器创建出来
// 
// 2.说明成员内部有ref和const的时候，
// 如果不手动声明拷贝赋值，编译器不会自动生成拷贝赋值
// 

//条款06：若不想使用编译器自动生成的函数，应该明确拒绝
// 1.将你不希望使用的函数声明为private并且没有实现
// 2.可以写成继承自一个UnCopyable类，方便
// 
// 
//条款07：为多台基类声明virtual析构函数
// 1.polymorphic(带有多台性质的)base class都应该声明一个virtual析构函数
// 2.如果class带任何virtual函数，就应该声明一个virtual析构函数
// 3.如果class不作为baseclass使用，就不应该有virtual析构函数
// 4.声明一个纯虚(virtual void xxx()=0)，可以让这个类无法实例化(abstract)

//条款08:别在析构函数里抛异常
// 1.如果被析构函数调用的函数可能抛出异常，则应该在析构函数里捕捉并吞掉，而不是继续让其传播
// 

//条款09:不要在构造函数和析构函数里面调用virtual函数
//1.因为派生的的构造函数执行过程是，先构造父类部分，在构造派生类本身，
// 如果在派生类构造函数里面调用的话，直接就是调用父类的函数了
// 
// 2.析构函数一开始执行，子类部分先被析构，就成了一个基类对象了，此时也会去调用父类的函数
// 3.如果需要在构造函数里面调用某个函数，最好是调用static成员，会先于普通成员初始化

//条款10:令operator=返回 reference to *this

//条款11:在operator=中处理自我赋值
// 1.如a[i]=a[j],*px=*py，都可能出现自赋值的现象
// 处理例如：if(this==&rhs) return *this;
// 
//
//条款12:复制对象不要忘记其中任何一个成分
// 1.子类的copying函数里面，不要忘记父类的成员的复制(但这些成员通常是private)，
// 所以正确的做法是子类去调用父类的copying函数来实现复制
// 2.不要让copy constructor去调用copy assignment，反过来也是一样的，
// 正确的做法是实现一个private的Init函数共两个copying函数调用
// 
//条款16:new 和delete形式要一致
// 1.尽量不要对数组进行typedef,因为其实vector和string足以替代所有的数组操作
// 
//条款20:尽量以pass-by-reference-to-const 替换 pass-by-value
// 1.可以避免构造函数和析构函数的无谓调用
// 2.有const的情况下可以防止对象被百变
// 3.by-reference可防止slicing(对象切割)问题，
// pass by value如果是子类对象传入基类形参的话，会被直接切割成子类对象
// C++编译器底层，reference通常以指针实现出来
// 4.pass-by-value适用于:内置类型，stl迭代器,函数对象
// 

//条款21:必须返回对象时，千万不要返回reference
// (看到一个引用，就要认为他是一个别名，那么他本来的名字是什么?)
// 1.不要返回local对象的reference或者指针，会出大错
// 在stack上创建的对象压根出了代码块就不存在了
// 在heap上创建的对象如果被你返回了引用，根本没有办法delete
// (有时候因为一些中间情况，根本没法获得其指针来delete)
// 2.也不要用static对象的方法来避免多次调用构造函数和析构函数，这里就要选择承受这点代价
// 

//条款22:尽量把成员声明为private
// 1.声明为public或protected都是没有封装性
// 封装性主要跟当其内容改变时可能造成的代码破坏量成反比
// 改变,比如从代码中移除它(如果不封装，需要改变的地方就到处都是，封装了，需要改变的就只有一个接口函数)
// 2.可以让用户在使用的时候，.的时候不用考虑带不带括号(如果全部进行封装了，就全部都带括号)
// 

//条款23:尽量以non-member non-friend 函数替换member函数
// 1.推崇封装的原因:能够使被改变的事物只影响有限的客户
// 2.non-member non-friend不会增加 能够访问class之内private成员的函数数量
// 3.建议做成一个Unity类，提供静态函数去调用这些member函数，定义于同一个namespace里面
// 4.同一个namespace，包含不同的头文件，针对不同的功能
// 
//条款24:如果需要为某个函数的所有参数进行类型转换，那么这个函数必须是non-member
// 1.能够执行隐式类型转换的条件:参数被置于参数列表内时
// 例子: oneHalf.operator*(2) //此时2在列表内，发生隐式转换(假设构造函数不是explicit)
// 所以最终把这种函数声明为non-member是最好的选择，
// const Rational operator*(const Rational& rhs,const Rational& lhs)//这样两个参数就都可以放在列表里面了
// 
// 
//条款25:swap函数的高效率版本
// 1.当swap缺省版本效率不高的时候（内部含有指针或者大量数据），提供一个public成员函数
// 让其高效的交换你的类型的两个对象
// 2.在你的class或template所在命名空间提供一个non-member的swap调用你写的成员函数
// 3.为你的class特性化std::swap
// 4.确保调用swap的时候包含一个using std::swap，保证调用最优版本的swap


//条款26:尽可能延后变量定义式出现的时间
// 1.当出现前面报错，直接return或者中断的操作，应该尽量延后定义，来避免构造函数调用的代价
// 2.尽量在确定知道要用一个变量的时候才定义，避免出现先调用默认构造函数，再赋值，
// 而是直接调用有参构造函数赋有意义的初值
// 3.循环的情况下，除非你知道赋值成本比"构造+析构"低，那么应该将定义写在循环内部 
// 
//条款28:避免返回handles指向对象内部成分
// 1.避免返回指向对象内部的reference,pointer,iterator等等
// 2.成员函数如果返回成员的引用，最好是const的，这样才不会使访问控制修饰符失效