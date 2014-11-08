#include "Catalog.h"
bool existDatabase(string DB_Name){
	int DBcount;
	string path = "Catalog//DB_Name.dat";
	FILE *fout = fopen(path.c_str(), "r");
	char name[25];
	fscanf(fout, "%d", &DBcount);
	for (int i = 1; i <= DBcount; i++)
	{
		fseek(fout, 20 * i, 0);
		fscanf(fout, "%s", name);
		if (string(name) == DB_Name)
		{			
			fclose(fout);
			return true;
		}
	}
	return false;
}
bool existTable(string DB_Name, string Table_Name){
	string dpath = DB_Name + "//" + Table_Name + ".0.dat";
	FILE *fout = fopen(dpath.c_str(), "r+");
	if (fout == NULL)
		return false;
	else
		return true;
}
int attrOrder(string DB_Name, string Table_Name, string Attr_Name){
	string path = "Catalog//" + DB_Name + "//";
	path = path + Table_Name + ".dat";
	FILE *fin = fopen(path.c_str(), "r");
	int attrCount;
	char name[25];
	fscanf(fin, "%d", &attrCount);
	for (int i = 0; i < attrCount; i++)
	{
		fseek(fin, 20+40 * i, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fin);
			if (name[j] == 0)
				break;
		}
		if (string(name) == Attr_Name)
			return i + 1;
	}
	fclose(fin);
	return 0;
}
void createDatabase(string DB_Name){
	int DBcount;
	char name[25];
	string path = "Catalog//DB_Name.dat";	
	FILE *fout = fopen(path.c_str(), "r+");
	fscanf(fout, "%d", &DBcount);
	for (int i = 1; i <= DBcount; i++)
	{
		fseek(fout, 20 * i, 0);
		fscanf(fout, "%s", name);
		if (string(name) == DB_Name)
		{
			printf("ERROR:Database name conflicts\n");
			fclose(fout);
			return;
		}
	}
	fseek(fout, 0, 0);
	fprintf(fout, "%d\n", ++DBcount);
	fseek(fout, 20*DBcount, 0);
	fprintf(fout, "%s", DB_Name.c_str());
	fclose(fout);
	string dpath = DB_Name + "//";
	CreateDirectory(dpath.c_str(), NULL);
	return;
}
void createTable(string DB_Name, string Table_Name)
{ 
	int attrCount;
	string dpath = DB_Name + "//" + Table_Name + ".0.dat";
	string path = "Catalog//" + DB_Name+"//";	
	CreateDirectory(path.c_str(), NULL);
	path = path + Table_Name + ".dat";	
	FILE *fout = fopen(path.c_str(), "w");
	fprintf(fout, "0");	
	fclose(fout);
	fout = fopen(dpath.c_str(), "w");
	fprintf(fout, " ");
	fclose(fout);
	return;
}
//Do not verify uniqueness of attrs
void addAttr(string DB_Name, string Table_Name, string Attr_Name, int Attr_Len, int Attr_Type, int Data_Type)
{
	if (Table_Name.length() > 20)
	{
		printf("ERROR:Attribute %s's name is too long !\n", Attr_Name);
		return;
	}
	if (Attr_Len>255 && Data_Type==1)
	{
		printf("ERROR:Attribute %s's length is too long !\n", Attr_Name);
		return;
	}
	if (attrOrder(DB_Name, Table_Name, Attr_Name) != 0)
	{
		printf("ERROR:Attribute name confilcts!\n");
		return;
	}
	int attrCount;
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	fscanf(fUpdate, "%d", &attrCount);
	fseek(fUpdate, 20 + attrCount * 40, 0);
	fprintf(fUpdate, "%s", Attr_Name.c_str());
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