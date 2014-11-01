#include "Catalog.h"
void createDatabase(CString DB_Name){
	int DBcount;
	char name[25];
	CString path = "Catalog//DB_Name.dat";	
	FILE *fout = fopen(path, "r+");
	fscanf(fout, "%d", &DBcount);
	for (int i = 1; i <= DBcount; i++)
	{
		fseek(fout, 20 * i, 0);
		fscanf(fout, "%s", name);
		if (CString(name) == DB_Name)
		{
			printf("ERROR:Database name conflicts\n");
			fclose(fout);
			return;
		}
	}
	fseek(fout, 0, 0);
	fprintf(fout, "%d\n", ++DBcount);
	fseek(fout, 20*DBcount, 0);
	fprintf(fout, "%s", DB_Name);
	fclose(fout);
	return;
}
void createTable(CString DB_Name, CString Table_Name)
{ 
	int attrCount;
	CString path = "Catalog//" + DB_Name+"//";
	CreateDirectory(path, NULL);
	path = path + Table_Name + ".dat";
	FILE *fout = fopen(path, "w");
	fprintf(fout, "0");	
	fclose(fout);
	return;
}
//Do not verify uniqueness of attrs
void addAttr(CString DB_Name, CString Table_Name, CString Attr_Name, int Attr_Len, int Attr_Type, int Data_Type)
{
	if (Table_Name.GetLength() > 20)
	{
		printf("ERROR:Attribute %s's name is too long !\n", Attr_Name);
		return;
	}
	if (Attr_Len>255 && Data_Type==1)
	{
		printf("ERROR:Attribute %s's length is too long !\n", Attr_Name);
		return;
	}
	int attrCount;
	CString path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fUpdate = fopen(path, "r+");
	fscanf(fUpdate, "%d", &attrCount);
	fseek(fUpdate, 20 + attrCount * 40, 0);
	fprintf(fUpdate, "%s", Attr_Name);
	fseek(fUpdate, 20 + attrCount * 40 + 20, 0);
	fprintf(fUpdate, "%d", Attr_Len);
	fseek(fUpdate, 20 + attrCount * 40 + 23, 0);
	fprintf(fUpdate, "%d", Attr_Type);
	fseek(fUpdate, 20 + attrCount * 40 + 24, 0);
	fprintf(fUpdate, "%d", Data_Type);
	fseek(fUpdate, 20 + attrCount * 40 + 25, 0);
	fprintf(fUpdate, "%d",0);
	fseek(fUpdate,0, 0);
	fprintf(fUpdate, "%d", attrCount+1);
	fclose(fUpdate);
	return;
}