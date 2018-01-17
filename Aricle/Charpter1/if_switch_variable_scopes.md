Chapter1：C++17新特性
=====================


增加if和switch语句作用域变量
-------------------------
能够有效的控制变量的作用域是一个很好的特性。有些时候，某些变量只是在某些特定条件下进行才需要进一步使用。

以此为目的，C++17种增加了if和switch语句中的初始化。

**1.1 如何使用**

在这个小章节中，我们将使用初始化语法包含在语句的上下文中以简洁代码。

1.下面的if语句中我们使用std::map中的find方法来查找所需的字符

	if (auto itr (character_map.find(c)); itr != character_map::end()) {
		// *itr is valid. Do something with it.
	} else {
		// itr is the end-iterator. Don't dereference.
	}
	// itr is not available here at all
	
2.下述的switch语句演示了游戏中，通过输入设备来获取输入字符，并在switch语句中来控制计算机程序进行。

	switch (char c(getchar()); c) {
		case 'a': move_left(); break;
		case 's': move_back(); break;
		case 'w': move_fwd(); break;
		case 'd': move_right(); break;
		case 'q': quit_game(); break;
		case '0'...'9': select_tool('0' - c); break;
		default:
			std::cout << "invalid input: " << c << '\n';
	}

**1.2 工作方式**

在if和switch语句中进行初始化，就像语法糖一样。下面两个例子是相同的。

C++17之前：

	{
		auto var (init_value);
		if (condition) {
			// branch A. var is accessible
		} else {
			// branch B. var is accessible
		}
		// var is still accessible
	}

C++17:

	if (auto var (init_value); condition) {
		// branch A. var is accessible
	} else {
		// branch B. var is accessible
	}
	// var is not accessible any longer
	
同样初始化变量也可以运用在switch语句中

C++17之前：

	{
		auto var (init_value);
		switch (var) {
		case 1: ...
		case 2: ...
		...
		}
		// var is still accessible
	}

C++17:

	switch (auto var (init_value); var) {
	case 1: ...
	case 2: ...
	...
	}
	// var is not accessible any longer
	
这种新特性能够有效的控制变量的作用域。在C++17之前，必须在代码的外围添加上大括号来限制其生命周期，正如上面的代码片段所示。这种控制变量声明周期使代码更加整洁并且容易重构。

**1.3 更多例子**

使用这种新特性来控制临界区的进入，是一个很好的例子。如下所示：

	if (std::lock_guard<std::mutex> lg {my_mutex}; some_condition) {
		// Do something
	}
	
在if中初始化std::lock_guard，它的初始化函数需要一个std::mutex作为参数。它在初始化过程中，将对mutex加锁，当代码运行到if范围以外时，std::lock_guard变量将析构，在析构函数中对std::mutex进行解锁操作。通过这种方式，避免忘记解锁操作。在C++17之前，通常用打括号来扩住要执行的代码来限制std::lock_guard变量的作用域，来实现相同的功能。

另一个很有用的例子是针对std::week_ptr指针，其情形如下：

	if (auto shared_pointer (week_pointer.lock()); shared_pointer != nullptr) {
		// Yes, the shared object does still exist
	} else {
		// shared_pointer var is accessiable, but a null pointer.
	}
	// shared_pointer is not accessible any longer

通过上述代码的方式，来有效的控制shared_pointer的作用域，防止其生命周期扩散到整个代码区域中。

if作用域变量对于使用API也是很有用的，例如下面调用Windows的API(GetExitCodeProcess)。

	if (DWORD exit_code; GetExitCodeProcess(process_handle, &exit_code) {
		std::cout << "Exit code of process was: " << exit_code << '\n';
	}
	// No useless exit_code variable outside the if-conditional
	
备注：

	GetExitCodeProcess是Windows内核API，用于获取给定的处理进程的返回值。如上图代码所示，
	当离开if语句的作用域，则变量exit_code将无法使用。
	
通过上面的几个例子，我们可以感受到if语句块作用域变量带来的好处，尤其是在使用API接口的时候。

Note:
	
	使用if或者switch语句块变量来代替原来的大括号限制变量作用域，将使你的代码更加简洁明了，
	容易读懂。并且使用这种方法，有利于以后代码重构。
		