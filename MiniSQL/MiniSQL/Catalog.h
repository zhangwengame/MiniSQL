#ifndef _CATALOG_H 
#define _CATALOG_H
#include <afx.h>
#include <fstream>
struct attr_info{
	char name[20],index_name[20];
	char data_type,attr_type;
	int  data_len,num;
};
void Create_Database(CString DB_Name);
#endif
