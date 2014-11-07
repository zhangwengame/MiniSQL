#ifndef _CATALOG_H 
#define _CATALOG_H
//#include <afx.h>
#include <fstream>
#include <windows.h>
#include <string.h>
using namespace std;
void createDatabase(string DB_Name);
void createTable(string DB_Name, string Table_Name);
void addAttr(string DB_Name, string Table_Name, string Attr_Name, int Attr_Len, int Attr_Type, int Data_Type);

#endif
