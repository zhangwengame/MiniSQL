/*#include <python2.7/Python.h>
#include <iostream>
using namespace std;
int main()
{
 const char* pBuffer = NULL;
 int iBufferSize = 0;
 Py_Initialize();
 PyRun_SimpleString("import sys\nsys.path.append('./')");
 PyObject * pModule = NULL;
 PyObject * pFunc   = NULL,*pValue=NULL;
 pModule = PyImport_ImportModule("interpreter");
 pFunc   = PyObject_GetAttrString(pModule, "interpreter");
 pValue=PyEval_CallObject(pFunc, NULL);
 PyArg_Parse(pValue, "s", &pBuffer);
 Py_Finalize();
 cout<<pBuffer<<endl;
 return 0;
}
*/

