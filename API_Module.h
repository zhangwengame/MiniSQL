//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: API_Module                           ///
///       Produced by: Yuheng Zhou                     ///
///       Description: Produced to deal with the       ///
///                    functions between Interpreter,  ///
///                    Record_Manager and Index_Manager///
///       date: 2014/11/18                             ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////

#include "Record.h"
#include "Index.h"
#include <iostream>

using namespace std;

void API_Module(string SQL, bufferInfo* bufferInfo);
conditionInfo Str_To_Conds(string DB_Name,string Table_Name,string str);
