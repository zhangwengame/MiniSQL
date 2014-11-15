#include <python2.7/Python.h>
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
 //cout<<"ok"<<endl;
 pModule = PyImport_ImportModule("Inter");
 //cout<<"ok"<<endl;
 pFunc   = PyObject_GetAttrString(pModule, "interpreter");
 //cout<<"ok"<<endl;
 pValue=PyEval_CallObject(pFunc, NULL);
 PyArg_Parse(pValue, "s", &pBuffer);
 //cout<<"ok"<<endl;
 Py_Finalize();
 cout<<pBuffer<<endl;
 while (1);
 return 0;
}


