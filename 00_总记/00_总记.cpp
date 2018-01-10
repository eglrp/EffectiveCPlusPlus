// 00_总记.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

int main()
{
	return 0;
}

//绪论:
// 1.除非有很好的理由允许构造函数被用于隐式类型转换，否则构造函数请声明成explicit
// 2.有新对象被定义就是调用的拷贝构造函数，就算是A a = a1;拷贝赋值运算符只用于没有新对象产生的情况
// 3.面向对象:封装、继承、多态

//条款01:
// 1.内置类型通常pass-by-value更高效，用于自定义类型pass-by-reference-to-const更高效
// 2.

//条款02：尽量以const enum inline 替换#define
// 1.string 往往比 char* 合适
// 尽量用const变量替换宏定义，宏定义可能使目标码变大(代码中包含多份常量的值(比如浮点值))
// 并且编译报错的信息(报的错是定义的实际的值，不是你定义的宏)也不好找
// 2.宏是没有作用域的，而class专属常量可以有作用域，为了只有一份，加上static，static const int NumTurns=5; 
// static确保只有一份，写作成员确保作用域，可加private等控制符
// 但之后必须在实现文件里面进行定义（因为是静态变量）：
// const int ClassName::NumTurns;(现在的新编译器通常都可以in-class初值设定(整形的才可以，ints，chars，bools，并且不需要取它的地址才可以，需要取地址的话还是需要定义一下)
//const static成员和普通成员可以，non-const static成员不行)

// 3.
// 3.尽量以const常量替换#define
// 如：#define ASPECT_RATIO 1.653=>const double Aspect_Ratio=1.653
// 如果编译器不支持Inclass初值设定(用于其他成员比如数组的大小的时候就需要这种用法),可以用enum替代
// enum替代的好处是:如果你不想让别人用指针指向你的整数常量,enum可以实现这个约束并且不会出现内存分配,在这点上enum更像是define。
// 参见01_01
// 4.3.用inline-template函数可以实现对宏的替换，宏定义的函数更晦涩难懂
// 参见01_02
// 5.#include #ifdef 仍然无法被替代

//条款03:多使用const
// 1.const在*左边，被指是常量，const在*右边，指针是常量，两边，都是常量
// 所以const int* a;和int const * a;都是指向常量
// 
// 2.mutable修饰的成员，即使在const成员函数内也可以被修改
// 3.返回const 类型，可以防止一些无意义的客户端错误(自定义数据类型可能被一些无意义的赋值操作，==写成了=,比如(a+b)=c，如果这个表达式用在了if()比较里面就出大问题了)
// 4.const成员函数的作用:(1):使接口容易理解，哪些接口会修改对象哪些不会(2):使得操作const对象成为可能，因为很多地方都是pass-by-reference-to-const
// 5.两个成员函数如果只是常量性不同，可以被重载，const对象会自动调用const版本的函数
// 6.const成员函数里面可以修改static成员以及mutable成员，参见03_01
// 7.如果const和non-const版本代码重复，应该使non-const调用const版本实现。
// 其中也许需要用到两次转型操作，比如: const_cast<char &>(static_cast<const TextBlock&>(*this)[postition])
// static_cast用来加上常量性从而调用const版本，const_cast用来去掉常量性实现non-const想要的操作
// 但反向做法是错误的。
// 8.bitwise-constness(编译器也是这样实施的) 是不能更改对象(所以static成员除外)的任何一个比特，logical-constness意在可以改变其中一些(客户端侦测不出来就可以了)，用mutable修饰(非static成员变量)

//条款04:确保对象在使用前被初始化(原则:确保每一个构造函数都将每一个成员初始化)
// 1.使用成员初值列（member initialization list）进行初始化，这种方式会调用相应的构造函数版本
// 初始化发生在进入构造函数体之前，如果存在多个构造函数，都有自己的初值列，会带来重复的情况下，
// 可以选择将赋值和初始化代价一样的部分封装进一个Private函数来调用，避免重复
// 即使内置内型初始化和拷贝赋值的代价一致，建议也统一成都用初始化
// 初值列里面可以直接()，不带值，代表调用默认初始化
// const和reference必须使用初值列初始化，不能被赋值
// 初值列里初始化的顺序取决于成员在class中的声明顺序，和排列无关
// C++中成员初始化次序总是相同的:base先，derived后，成员变量按照声明顺序初始化
// 2. static对象的析构函数会在main结束时自动调用
// 3.不同编译单元(源文件)的初始化顺序是不明确的，
// 所以使用local-static对象替换non-local-static对象(global)，类似于单例模式
// 函数内的local static对象会在函数被调用期间首次遇上该对象之定义式时被初始化。
//
// 
//条款05:C++编译器默认生成的函数 
// 1.声明一个空类(class A{};),编译器自动生成:(1):默认构造函数(2)拷贝构造(3)默认析构(4)拷贝赋值
// 这些函数只有被调用，才会被编译器创建出来，这些函数都是public且inline的
// 默认构造函数只有自己未显式定义构造函数才会生成
// 默认构造函数和析构函数 主要就是调用base classes和non-static 成员变量的构造和析构函数
// 默认的copying函数就将对象的每一个non-static对象拷贝到目标对象，对于自定义类型，会调用该类型的copying函数，对于内置类型，会直接拷贝每一个bits
// 2.当成员内部有ref和const的时候，
// 如果不手动声明拷贝赋值，编译器不会自动生成拷贝赋值，因为这两种情况不允许更改
// 3.如果base class将copy assignment声明为private,那么其derived class无法自动生成（可以自己写）

//条款06：若不想使用编译器自动生成的函数，应该明确拒绝
// 1.将你不希望使用的函数声明为private并且没有实现(STL中iostream就是这样防止拷贝的)
// 2.可以写成继承自一个UnCopyable类，方便,参见06_01
// 
// 
//条款07：为多态基类声明virtual析构函数
// 1.polymorphic(带有多态性质的)base class都应该声明一个virtual析构函数，
// 否则在出现多态性的时候(通过父类指针调用delete)，将会只删除基类部分,出现部分析构
// 2.如果class带任何virtual函数，就应该声明一个virtual析构函数
// 3.如果class不作为base class使用，就不应该有virtual析构函数，virtual函数会使得对象体积变大，并且不再具有移植性(其他语言不具有vptr,虚函数表)。
// 4.声明一个纯虚(virtual void xxx()=0)，可以让这个类无法实例化(abstract)
// 5.纯虚析构函数必须有定义体(常常为空)，参见07

//条款08:别在析构函数里抛异常
// 1.如果被析构函数调用的函数可能抛出异常，则应该在析构函数里捕捉(catch)并吞掉(process)，而不是继续让其传播
// 

//条款09:不要在构造函数和析构函数里面调用virtual函数
//1.因为派生的的构造函数执行过程是，先构造父类部分，在构造派生类本身，
// 如果在派生类构造函数里面调用的话，直接就是调用父类的函数了
// 非正式的说法:在base class构造期间，virtual函数不是virtual函数
// 2.析构函数一开始执行，子类部分先被析构，就成了一个基类对象了，此时也会去调用父类的函数
// 3.如果需要在构造函数里面调用某个函数，最好是调用static成员，会先于普通成员初始化

//条款10:令operator=返回 reference to *this
//几乎所有stl里的实现都遵循这个协议。

//条款11:在operator=中处理自我赋值
// 1.如a[i]=a[j],*px=*py，都可能出现自赋值的现象
// 处理例如：if(this==&rhs) return *this;
// 但通常涉及内存分配的还需要考虑异常安全，这时候做法是先备份之前的内容，分配完成之后再delete
// 以及swap-copy技术
// 
//
//条款12:复制对象不要忘记其中任何一个成分
// 自己实现了copying函数的情况下，如果有成份没有复制，编译器是不会发出警告的!
// 1.子类的copying函数里面，不要忘记父类的成员的复制(但这些成员通常是private)，
// 所以正确的做法是子类去调用父类的copying函数来实现复制
// 2.不要让copy constructor去调用copy assignment，反过来也是一样的，
// 正确的做法是实现一个private的Init函数供两个copying函数调用
// 
//
//条款13:以对象管理资源
// 1.auto_ptr可以利用元素的析构函数进行自动的内存管理，但是不适用于动态数组的管理
// 2.auto_ptr和shared_ptr都是delete,而不是delete []
// 3.为什么没有针对动态数组的auto_ptr? 因为vector和string总是可以替代动态数组
// 4.工厂类的函数返回指针非常容易引起资源泄露，因为用户很容易忘记delete,因此需要更改，参见条款18


//条款16:new 和delete形式要一致
// 1.尽量不要对数组进行typedef,比如 typedef string AddrLines[100]; 但是delete的时候，不知道是数组，就只用了delete
// 因为其实vector和string足以替代所有的数组操作
//
//
//条款17:用独立语句将newed对象置入智能指针
// 1.对于processWidget(std::shared_ptr(new Widget()),priority()) 这个调用而言
// std::shared_ptr(new Widget())和priority()的调用不一定谁先谁后，所以如果先new Widget(),
// 然后priority(),但是报了异常的话，shared_ptr就没有管理到对象，就内存泄露了
// 2.解决办法就是，把std::shared_ptr(new Widget())单独拿出来作为一行语句。

//条款18:让接口容易被正确使用，不易被误用
// 1.许多客户端错误可以用导入新类型的方式避免,参见18_01
// 2.返回指针的工厂函数，用户可能有两个错误:(1)忘记delete(2)delete多次
// 3.所以干脆就让工厂函数返回shared_ptr
// 4.shared_ptr可以指定deleter完成自己指定的删除操作

//条款19:设计class犹如设计type
// 1.本条款讲了设计一个新class的所有注意事项(需要考虑的问题)

//条款20:尽量以pass-by-reference-to-const 替换 pass-by-value
// 1.可以避免构造函数和析构函数的无谓调用
// 2.有const的情况下可以防止对象被改变
// 3.by-reference可防止slicing(对象切割)问题，
// pass by value如果是子类对象传入基类形参的话，会被直接切割成子类对象
// C++编译器底层，reference通常以指针实现出来
// 4.pass-by-value适用于:内置类型，stl迭代器,函数对象


//条款21:必须返回对象时，千万不要返回reference
// (看到一个引用，就要认为他是一个别名，那么他本来的名字是什么?)
// 1.不要返回local对象的reference或者指针，会出大错
// 在stack上创建的对象压根出了代码块就不存在了
// 在heap上创建的对象如果被你返回了引用，根本没有办法delete
// (有时候因为一些中间情况，根本没法获得其指针来delete)
// 2.也不要用static对象的方法来避免多次调用构造函数和析构函数，
// 一个static对象的话会带来很多地方的错误
// 这里就要选择承受这点代价（让编译器来为你进行优化）
// 

//条款22:尽量把成员声明为private
// 1.声明为public或protected都是没有封装性
// 封装性主要跟当其内容改变时可能造成的代码破坏量成反比
// 改变,比如从代码中移除它(如果不封装，需要改变的地方就到处都是，封装了，需要改变的就只有一个接口函数)
// 2.可以让用户在使用的时候，.的时候不用考虑带不带括号(如果全部进行封装了，就全部都带括号)
// 3.使用函数(public成员函数，接口)可以对成员变量的处理有更精确的控制
// 4.使用接口可以对内部的实现进行封装，内部发生变化完全不让客户知道
// 5.使用函数接口还可以队所有可能的实现提供弹性，比如当变量被读写时通知其他对象，可验证class的约束条件及函数的前提和事后状态等等
// 6.public成员被删除时，所有用到它的客户都会被影响，protected的所有derived class都会受影响。很多代码需要重新编写，测试，编写文档，编译等等。

//条款23:尽量以non-member non-friend 函数替换member函数
// 1.推崇封装的原因:能够使被改变的事物只影响有限的客户
// 2.non-member non-friend不会增加能够访问class之内private成员的函数数量，因此也就没有影响封装性
// 3.建议做成一个Unity类，提供静态函数去调用这些member函数，定义于同一个namespace里面
// 4.同一个namespace，包含不同的头文件，针对不同的功能
// 
//条款24:如果需要为某个函数的所有参数进行类型转换，那么这个函数必须是non-member
// 令class支持隐式类型转换是个坏主意，但是当建立数值类型的class时有时是合理的
// 1.结论:  能够执行隐式类型转换的条件:参数被置于参数列表内时
// 例子: oneHalf.operator*(2) //此时2在列表内，发生隐式转换(假设构造函数不是explicit)
// 但是当2*oneHalf的时候，调用的是2.operator(oneHalf)(编译器也会继续寻找global空间里的operator*(2,oneHalf),不过这里没有)，
// 显然int类型并没有一个从Rational类隐式转换而来的构造函数，就出错了
// 所以最终把这种函数声明为non-member是最好的选择，
// const Rational operator*(const Rational& rhs,const Rational& lhs)//这样两个参数就都可以放在列表里面了
// 2.member函数的反面是non-member而不是friend函数，friend函数会带来太多问题，尽量避免，就像现实中，不是自己人不一定得是朋友

// 
//条款25:swap函数的高效率版本
// 1.当swap缺省版本效率不高的时候（内部含有指针或者大量数据），提供一个public成员函数
// 让其高效的交换你的类型的两个对象(内部调用std::swap只交换你想要的部分)
// 2.在你的class或template所在命名空间提供一个non-member的swap调用你写的成员函数
// 3.为你的class特性化std::swap
// 4.确保调用swap的时候包含一个using std::swap，保证调用最优版本的swap


//条款26:尽可能延后变量定义式出现的时间
// 1.当出现前面报错，直接return或者报异常被中断的操作，应该尽量延后定义，来避免构造函数调用的代价
// 2.尽量在确定知道要用一个变量的时候才定义，避免出现先调用默认构造函数，再赋值，
// 而是直接调用有参构造函数赋有意义的初值
// 3.循环的情况下，除非你知道"赋值"成本比"构造+析构"低，那么应该将定义写在循环内部 
// 

//条款27:尽量少做转型操作 (More EffectiveC++ M3.2)
// 1.static_cast类似于旧式转型，但是不能去除常量性(只是不能去除底层const，顶层和普通的const是可以去除的)
// 2.const_cast，用于移除常量性(可以移除所有的const,包括底层const)
// 3.dynamic_cast实现向下安全转型，由父类指针向下转型
// 4.reinterpret_cast实现低级转型，比如pointer to int 转 int或者int*直接转换成char *,转换后是很危险的
// 5.以上四个，除了dynamic_cast旧式操作无法实现(也可以转，只是不检测是否可以正确的转型)
// 旧式操作都可以实现
// 6.注重效率的代码不要有dynamic_cast
// 7.尽量将类型转换隐藏起来不让用户看到
// 8.新式转型各有各的职能且容易辨识，建议使用新式转型。
// 参见Test_Cpps 41

//条款28:避免返回handles指向对象内部成分
// 1.避免返回指向对象内部的reference,pointer,iterator等等
// 2.成员函数如果返回成员的引用，最好是const的，这样才不会使访问控制修饰符失效
// 

//条款30:inline函数
// 1.inline适用于小型，被调用频繁的函数身上，太大的函数目标码太大，会降低高速缓存命中率
// 2.过度热衷inline会使程序体积庞大(inline就是在编译的时候，用函数体代替函数名(C#的CLI例外，是在运行时))
// 3.编译器会拒绝太过于复杂的(带循环或者递归的)函数，以及所有的virtual函数
// 因为virtual是动态绑定，编译器在编译的时候根本不知道调用哪个函数
// 4.函数指针的inline不一定起作用
// 5.inline函数缺点是，一旦函数被改变，所有用到此函数的程序都必须重新编译
// 如果不是inline的话，直接重新连接即好
// 

//31.将文件间的编译依存关系降到最低(其实就是.NET在学的降低不同层之间的耦合，通过接口)
// 1.尽量用声明替换定义，就能够完成调用了已经
// 2.#include <iosfwd>，这种xxxfwd的头文件里面只包含声明式
// 3.使用Interface classes来制作handle class，调用的时候用智能指针便可实现内存自动管理
// 4.要使Interface classes能够实例化，可以采用在这个接口内定义一个static的工厂性质的create方法返回这个接口的指针
// ，后面再根据实现这个接口的derived class动态返回所需要对象

//32.public继承一定要是is-a的关系
// 1.public主张继承，能够实施于base class对象身上的每件事情都能实施于derived class身上
// 2.class之间的关系:is-a has-a is-implemented-in-terms-of
// 

//33.避免隐藏继承而来的名称
// 1.详情参见Test_Cpps_26_01
// 2.如果正在使用public继承却有不继承那些重载函数，就是违背了is-a的关系
// 3.using或者转交函数可以实现访问被屏蔽的父类函数

//34.non-virtual pure-virtual impure virtual
// 1.pure virtual是希望子类必须重新实现自己的版本（接口继承）
// 2.impure virtual是希望子类接口继承并且有缺省版本
// 3.non-virtual是希望子类不要去改写，就直接用父类的版本
// 4.pure virtual父类也可以写实现代码，调用的时候Derived->Base::func()

//35.

//36.不要重新定义继承来的non-virtual函数
// 1.如果重新定义继承来的non-virtual函数的话，直观表现是
// 对同一个对象会出现同一函数调用的时候因为不同的指针调用没有多态性，而出现不同的行为
// 2.更主要的是在理论层面，non-virtual函数体现的是一种不变性，凌驾于其特异性之上，
// 如果直接non-virtual函数给改写了，这样违背了public继承的is-a的关系

//37.绝不重新定义继承而来的缺省参数值(只考虑带有缺省参数的virtual函数这种情况)
// 1.virtual函数是动态绑定，而缺省参数则是静态绑定的
// 所以:Base *p = new Derived(); 这种情况下，调用
// p->VirtualFunction() , 函数是调用了子类的，但是缺省参数还是用的父类的
// 2.就算是子类和父类的缺省参数想同也不要再重新写一次，第一是代码重复，
// 第二是一旦父类改变，所有子类也得去变(才能保持一致)，耦合性太强


//38.Model has-a or is-implemented-in-terms-of through composition
// 1.public 继承代表is-a，而复合意味着has-a或者is-implemented-in-terms-of
// 2.has-a在应用域出现，is-implemented-in-terms-of发生于实现域
// 标准库的set由平衡查找树实现，每个元素有三个指针，所以如果你的程序空间比速度更重要，需要自己实现一个set
// 3.详情可看用STL的list来实现Set的做法


//派生类与基类的类型转换(来自c++primer15.2.3)
// 1.自动类型转换只对指针或者引用类型有效(多态的时候)
// 2.其他情况下，将一个派生类对象赋值给一个基类对象或者用来初始化基类对象，都是调用的基类里面定义的
// copying函数，故只处理了基类部分的成员。但是这里最后的效果看上去就像是执行了类型转换

//派生访问说明符的作用(来自c++primer15.5)
// 1.派生访问说明符的目的是控制派生类用户(包括派生类的派生类)对于基类成员的访问权限
// 并不影响派生类的成员(本身)或友元对于基类的访问，这个访问只取决于基类成员的访问说明符
// 2.派生访问说明符还控制继承自派生类的新类的访问权限，比如如果一个成员int m1基类里面是public,但是是private继承，
// 则在此派生类里面,m1是private的(但是仍然不影响派生类去访问这个成员，在类内),那么理所应当的，派生类的用户和派生类的派生类
// 就无法访问m1了

//39.private继承的使用
// 1.private继承的时候，子类对象是无法转型成父类的，不是is-a的关系了
// 2.private继承而来的父类的所有成员在子类中都是private的，无论之前是public or protected
// 3.private继承只意味着希望使用父类中的实现，接口部分被抛弃(全是private)，只是为了使用父类中“已经备妥的某些特性”
// 4.private继承意味着is-implemented-in-terms-of，但是能使用复合的时候使用复合，不要使用private继承
// 5.当需要希望继承自此类的derived class 不能改写某个函数的时候(因为子类始终可以覆写或隐藏)，
// 这时，可以通过把这个函数封装在一个private成员里面，子类就无法访问了,也就无法修改了
// 这种复合的使用还可以将文件间的编译依存性降到最低,因为没有单独的头文件存在，而是封装在了一个头文件里面
// 6.private继承还可以用在EBO(empty base optimization，空白基类最优化)中
// 因为一个空类型总是占一个字节(编译器默认分配一个char通常)，但是当这个空白类被用于组合的时候，
// 由于前面可能有int或者其他类型的数据，最终内存对齐就不止占一个字节了，造成浪费，这时可以采用
// private继承，就可以保证只占一个字节,并且也还是能够实现所需要的组合功能

//40.多重继承的使用
// 1.尽量不要使用多重继承，会导致歧义以及对virtual继承的需要
// 2.多重继承的使用通常是: public 继承自一个接口实现接口继承，
//  再private继承自某class来拿到一些有用的实现来帮助实现接口。

//41.隐式接口和编译期多态
// 1.参见41_01,给人一种像是在用脚本语言的感觉

//42.typename和class的区别
// 1.使用template参数的时候，没有任何区别
// 2.当标识嵌套从属类别的时候，必须加typename,才能判断是类别，而不是其静态成员或者是某个全局变量
// 参见42，VS可以不用

//43.如何处理模板化基类里的名称
// 1.当一个子类继承自模板化基类的时候，是不能直接"不加修饰"直接调用模板化基类里的函数的
// 因为base class templates可能被特化，而特化版本可能不提供和这个template相同的接口
// 2.解决方法是:(1)调用的时候加上this->basefunc()(2)使用using Base<T>::basefunc()
// (3)指明函数位于父类里面，Base<T>::basefunc();
// 参见43_01(VS里面貌似不用这样做了，直接动态帮你确定了已经。。)

//44.非类型模板参数往往会造成代码膨胀
// 1.模板也可以有非类型的参数，参见44_01
// 2.但是非类型的模板参数会导致代码膨胀，针对每个非类型参数都会产生一份新的二进制码
// 3.通常这种非类型的参数尽量用成员变量代替
// 4.类型模板参数也可能会造成代码膨胀，比如int和long其实是一份实现，(我相信VS是做了的)

//45.成员函数模板可以用来接受所有兼容类型
// 1.智能指针或者自己写的类型如果想实现像传统的普通指针一样支持隐式类型转换的话，可以运用成员模板来实现
// 2.泛化的copy构造函数和assignment不影响普通的(声明了泛化版本不声明普通版本，编译器还是会帮你生成)，必须同时定义

//46.

//52:placement new和placement delete
// 1.正常形式的operator new和operator delete只包含一个参数,new(size_t),delete(void *rawMemory)
// 2.如果在正常形式的基础上还有其他参数则是placement new和placement delete
// 3.placement new和placement delete必须对应，delete的时候是调用的正常形式的delete
// 4.不要让placement new掩盖了global空间的new
// 5.解决方式是建立一个基类，包含所有正常形式的new，然后在子类里面使用using声明
