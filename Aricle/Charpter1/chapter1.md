Chapter1：C++17新特性
=====================

本章节，我们将介绍一下内容：

1. 通过绑定结构化数据结构解析函数返回值


1.通过绑定结构化数据来解析函数返回值
----------------------------
C++17带来了一种全新的特性，**结构化绑定**--它结合了语法糖和自动类型推断。结构化绑定能够将pairs、tuples或者structs中的值转移到自定义变量中。结构化绑定在其他语言中，叫做解包。

**1.1 如何使用**

----------------------------------------
应用结构化绑定的方式来从绑定的结构体中获取多个变量值通常只需要一步。首先让我们看一下C++17之前是如何解决的。然后我们通过几个例子，看它如何在C++17中工作。

1.我们要访问std::pair中的值，它是由下面所示的函数divide_remainder返回：

	std::pair<int, int> divide_remainder(int dividend, int divisor);

考虑用下面的方式访问返回结果中的值：

		 const auto result(divide_remainder(16, 3));
		 std::cout << "16 / 3 is"
		 			 << result.first << " with a remainder of "
		 			 << result.second << '\n';

取代上面代码片段，我们可以获取pair中的值，并且命名成有意义的变量名，使其更加方便读。

		 auto [fraction, remainder] = divide_remainder(16, 3);
		 std::cout << "16 / 3 is "
		 			 << fraction << " with aremainder of "
		 			 << remainder << '\n';

2.结构化绑定也适用于std::tuple。通过以下例子，看它如何工作。
	
	std::tuple<std::string, std::chrono::system_lock::time_point, unsigned>
	stock_info(const std::string name);

使用结构化绑定如下所示：

	const auto [name, valid_time, price] = stock_info("INTC");
	
3.结构化绑定也适用于自定义结构体，假设结构体如下:

	struct employee {
		unsigned id;
		std::string name;
		std::string role;
		unsigned salary;
	};
	
现在，我们通过结构化绑定来访问这些成员。假定所有有一个vector数据来存放employee结构体变量，我们使用循环来进行处理。

	int main() {
		std::vector<empolyee> employees {
			/* Initialized from somewhere */ };
		
		for (const auto &[id, name, role, salary] : employees) {
			std::cout << "Name: " << name
						<< "Role: " << role
						<< "Salary: " << salary << '\n';
		}
	}
	
**1.2 工作方式**

--------------------
结构化绑定经常用于以下模型中：

`auto [var1, var2, ...] = <pair, tuple, struct, or array expression>`

1. 结构化绑定中变量(var...)个数必须与表达式(pair、tuple...)中变量个数一一对应，不允许缺少或者过多
2. 模型(pair, tuple, struct, or array expression)必须是以下结构中的一种: An std::pair、An std::tuple、A struct或者An array of fixed size(定长数组)。
3. var...的类型可以是auto, const auto, const auto&或者是auto &&

> Note:
> 
> 1.对于结构体struct来讲，所有成员必须是非静态成员，并且是基础类型数据。
> 2.这种方式不仅让使用者更加方便，并且通过引用尽量减少了值拷贝，以提高性能。

如果我们写太多或者太少变量在方括号之间，编译器将报错。

`std::tuple<int, float, long> tup {1, 2.0, 3};
 auto [a, b] = tup; // Does not work`
 
 上述代码段中明显少了一个变量名，编译器将直接告诉我们错误原因如下“
 
 `error: type 'std::tuple<int, float, long>' decomposes into 3 element
  auto [a, b] = top`
  
**1.3 更多例子**

---------------------------------
STL中很多基础数据结构在不改变其中数据的情况下，我们可以使用结构化绑定来进行遍历。例如，当我们需要遍历打印std::map中所有的值：

	std::map<std::string, size_t> animal_population {
		{"humans", 7000000000},
		{"chickens", 17863376000},
		{"camels", 24246291},
		{"sheep", 1086881528},
		/*....*/
	};

	for (const auto &[species, count] : animal_population) {
		std::cout << "There are " << cout << " " << species
					<< " on this planet.\n";
	}
	
在这个特殊的例子中，我们每遍历std::map容器一步就会获取到一个std::pair\<const key_type, value_type>一个结构体。可以用结构化绑定准确的获取其中的值，方便在遍历中直接使用。
  
在C++17之前，我们可以使用std::tie来实现相同的效果。

	int remainder;
	std::tie(std::ignore, remainder) = divide_remainder(16, 5);
	std::cout << "16 % 5 is " << remainder << '\n';

这个例子中演示了如何将解压返回结果pair将其存在两个变量中。在解压过程中，std::tie需要提前定义好要使用的变量，明显没有结构化绑定强大。另一面，例子也展示了std::tie的长处，不需要关心的变量，在std::tie中可以使用std::ignore来声明一个伪变量。

>Note：
>
>当使用结构化绑定来解包时，我们没有std::tie中的伪变量，因此必须将所有变量都绑定到命名变量上，
>编译器会自动帮我们优化，忽略那些没有使用的变量。

回到最初的例子，divide_remainder函数经常会声明称如下形式，使用输出参数：

	bool divide_remainder(int dividend, int divisor,
							  int &fraction, int &remainder);
							  
访问返回值，如下所示：

	int fraction, remainder;
	const bool success {divide_remainder(16, 3, fraction, remainder)};
	if (success) {
		std::cout << "16 / 3 is " << fraction << " with a remainder "
					<< remainder << '\n';
	}

很多人都喜欢采用这种方式来传递返回值，认为这种方式比返回复杂结构(pairs,tuples,structs)更加高效，因为这种方式避免了中间副本变量拷贝。但是这种认知是错误的，因为现在的编译器会帮我们优化到中间副本变量拷贝。

>Note:
>
>除了C语言中缺少对应的语言特性，通过返回值传递一个复杂的结构体通常认为是耗时的工作，复杂结构体
>不仅要在返回函数中进行初始化，还需要调用方进行值拷贝。但事实是现在编译器已经支持return value
>optimization(RVO)编译选项，用于省略中间副本变量。




  
		

