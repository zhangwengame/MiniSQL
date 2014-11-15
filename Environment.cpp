0. 坏境设置
把python的include/libs目录分别加到vc的include/lib directories中去。另外，由于python没有提供debug lib，体地说，就是没有提供python27_d.lib了。你可以自己编译python的源代码来得到python27_d.lib的，偶还没试过，呵呵。而且网上找了一下也没下载到。所以，如果你想要在debug下运行程序的话，你要把pyconfig.h（在python25/include/目录下）的大概是在283行，把pragma comment(lib,"python25_d.lib")改成pragma comment(lib,"python25.lib")，让python都使用非debug lib.

可以使用release编译

1. 开始编程了
#include <python.h>
第一步就是包含python的头文件

2. 看一个很简单的例子
1)python文件test.py，很简单的定义了一个函数

#Filename test.py
def Hello():
    print "Hello, world!"


2)cpp文件

#include <python.h> //包含头文件，在c++中嵌入python，这是必须的
int main()
{
 Py_Initialize();
 PyRun_SimpleString("import sys\nsys.path.append('./')"); //使当前路径下的.py可以被检索到
 PyObject * pModule = NULL;
 PyObject * pFunc   = NULL;

 pModule = PyImport_ImportModule("test");
 pFunc   = PyObject_GetAttrString(pModule, "Hello");
 PyEval_CallObject(pFunc, NULL);

 Py_Finalize();

 return 0;
}

第一步还是包含头文件

第二步，使用python之前，要调用Py_Initialize();这个函数进行初始化。
帮助文档中如是说：
The basic initialization function is Py_Initialize(). This initializes the table of loaded modules, and creates the fundamental modules __builtin__, __main__, sys, and exceptions. It also initializes the module search path (sys.path).

反正，一开始你一定要调用。

第三步，声明一些Python的变量，PyObject类型的。其实声明也可放在前面，这个倒是无所谓的。

第四步，import module，也就是你的脚本名字，不需要加后缀名，否则会出错的。

第五步，从你import进来的module中得到你要的函数
 pFunc   = PyObject_GetAttrString(pModule, "Hello");
上面的例子已经够清楚的了，最后一个是你要得到的函数的名字

第六步，调用PyEval_CallObject来执行你的函数，第二个参数为我们要调用的函数的函数，本例子不含参数，所以设置为NULL。

第七步，调用Py_Finalize，这个根Py_Initialize相对应的。一个在最前面，一个在最后面。

