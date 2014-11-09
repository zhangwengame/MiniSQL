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
bool existDatabase(string DB_Name);
bool existTable(string DB_Name, string Table_Name);
int attrOrder(string DB_Name, string Table_Name, string Attr_Name);
void createIndex(string DB_Name, string Table_Name, string Attr_Name, string Index_Name);
void dropIndex(string DB_Name, string Table_Name, string Attr_Name, string Index_Name);
void dropTable(string DB_Name, string Table_Name);
#endif
