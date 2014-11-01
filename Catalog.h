#ifndef _CATALOG_H 
#define _CATALOG_H
#include <afx.h>
#include <fstream>
#include <windows.h>
void createDatabase(CString DB_Name);
void createTable(CString DB_Name, CString Table_Name);
void addAttr(CString DB_Name, CString Table_Name, CString Attr_Name, int Attr_Len, int Attr_Type, int Data_Type);

#endif
