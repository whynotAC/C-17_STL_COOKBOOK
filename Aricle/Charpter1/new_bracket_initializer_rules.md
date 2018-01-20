Chapter1：C++17新特性
=====================

从新的列表初始化规则中获利
-----------------------

C++11带来了列表初始化语法。它是为了集合的初始化，并且方便构造函数的调用。但是列表初始化与auto变量结合使用时，常常表达出错误的语义。 因此C++17中明确了初始化规则的语义。本小节，我们将明确使用初始化列表语法时的语义。

**1.1 如何使用**

----------------------------------------------
一步初始化变量，使用初始化列表通常有两种不同的情况。

1.使用初始化列表并且不使用auto自动类型推断

	// Three identical ways to initialize an int:
	int x1 = 1;
	int x2 {1};
	int x3 (1);
	
	std::vector<int> v1 {1, 2, 3}; // Vector with three ints
	std::vector<int> v2 = {1, 2, 3}; // same here
	std::vector<int> v3 (10, 20); // Vector with 10 ints,
										// each have value 20
										
2.使用初始化列表并且使用auto自动类型推断

	auto v {1}; // v is int
	auto w {1, 2}; // error: only single elements in directory
					  // auto initialization allowed!
	auto x = {1};	  // x is std::initializer_list<int>
	auto y = {1, 2}; // y is std::initializer_list<int>
	auto z = {1, 2, 3.0}; // error: Cannot deduce element type

**1.2 工作方式**

----------------------------------------------
不使用auto类型推断时，使用初始化列表来初始化变量时，对于结果的预期符合我们根据初始化列表规则推断出的结果。当初始化容器(例如：std::vector,std::list等)使用集合进行初始化列表时，将自动匹配上容器类型中使用std::initializer_list作为参数的初始化函数。它使用贪婪模式进行匹配，这就意味着不会匹配上非集合构造函数(非集合构造函数是常规的构造函数，这个概念是相对于使用std::initializer_list作为参数的构造函数而言)。

例如,std::vector提供了特殊的非集合初始化构造函数，它是使用相同的值来多次填充到std::vector中(`std::vector<int> v(N, value)`)。当使用`std::vector<int> v {N, value}`时，initializer_list构造函数将被选中，初始化后的vector将有两个值：N 和 value。这个特殊的陷阱已经被我们熟知了。

详细的描述使用{}来初始化对象与使用()来初始化对象的不同之处在于：{}不会使用类型隐式类型转换来调用构造函数。例如,`int x(1.2)`或者`int x = 1.2`时，x将初始化为1，首先将1.2去掉小数值，然后再转变为int类型。而`int x{1.2}`会出现编译错误，应为它无法精确的匹配构造函数类型。

>Note:
>
>一个富有争议的话题就是哪种初始化方式是最好的。对列表初始化热衷者认为采用这种方式使代码更加清晰，这种初始化方式调用不会对于参数进行隐式转变。此外，使用列表初始化将选择一个合适的初始化构造函数。使用括号来初始化，将匹配一个最接近的构造函数，甚至会造出类型转换为了满足匹配方式。

C++17中附加条件将影响使用auto类型推断的初始化列表。例如`auto x{123};`，在C++11中x将被认为是std::initializer_list<int>，其包含一个元素，很明显这不是我们想得到的。C++17中x将被认为是int类型。

C++17中同时使用auto和初始化列表的规则如下：

1. `auto var_name {one_element};`中，则`var_name`将被推断为`one_element`相同类型。
2. `auto var_name {element1, element2, ...};`，则在C++17中会被认为是错误的，无法通过编译。
3. `auto var_name = {element1, element2, ...};`,则`var_name`的类型为`std::initializizer_list<T>`。

C++17规则能够保证初始化列表没有二意性，保证都有明确的定义。

>Note:
>
>对于不同的编译器，使用`auto x {123};`来进行判断其编译的C++版本。在C++11/C++14下，x将
>推断为`std::initializer_list<int>`；而在C++17中将推断为`int`类型。写这样的代码需要
>注意这将导致移植性问题。