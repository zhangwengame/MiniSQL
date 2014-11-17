#include "Catalog.h"

void createDatabase(string DB_Name){
	if (existDatabase(DB_Name))
	{
		printf("ERROR: Database name conflicts\n");
		return;
	}
	int DBcount;
	char name[25];
	string path = "Catalog//DB_Name.dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	fscanf(fUpdate, "%d", &DBcount);
	fseek(fUpdate, 0, 0);
	fprintf(fUpdate, "%d\n", ++DBcount);
	fseek(fUpdate, 20 * DBcount, 0);
	fprintf(fUpdate, "%s", DB_Name.c_str());
	if (DB_Name.length()<20)
		fprintf(fUpdate, "%c", 0);
	fclose(fUpdate);
	string dpath = "Catalog//" + DB_Name + "//";
	CreateDirectory(dpath.c_str(), NULL);
	dpath = dpath + "TA_Name.dat";
	fUpdate = fopen(dpath.c_str(), "w");
	fprintf(fUpdate, "0");
	fclose(fUpdate);
	dpath = "Data//";
	CreateDirectory(dpath.c_str(), NULL);
	dpath = dpath + DB_Name + "//";
	CreateDirectory(dpath.c_str(), NULL);
	return;
}

void createTable(string DB_Name, string Table_Name)
{
	if (existTable(DB_Name, Table_Name))
	{
		printf("ERROR: There is already such a table!\n");
		return;
	}
	string dpath = "Data//" + DB_Name + "//" + Table_Name + "//";
	string path = "Catalog//" + DB_Name + "//";
	CreateDirectory(path.c_str(), NULL);
	path = path + Table_Name + ".dat";
	FILE *fOut = fopen(path.c_str(), "w"), *fUpdate;
	fprintf(fOut, "0");
	fprintf(fOut, "%c", 0);
	fseek(fOut, 10, 0);
	fprintf(fOut, "0");
	fprintf(fOut, "%c", 0);
	fclose(fOut);
	int TAcount;
	path = "Catalog//" + DB_Name + "//TA_Name.dat";
	fUpdate = fopen(path.c_str(), "r+");
	fscanf(fUpdate, "%d", &TAcount);
	fseek(fUpdate, 0, 0);
	fprintf(fUpdate, "%d\n", ++TAcount);
	fseek(fUpdate, 20 * TAcount, 0);
	fprintf(fUpdate, "%s", Table_Name.c_str());
	if (Table_Name.length()<20)
		fprintf(fUpdate, "%c", 0);
	fclose(fUpdate);
	CreateDirectory(dpath.c_str(), NULL);
	dpath = dpath + Table_Name + ".0.dat";
	fOut = fopen(dpath.c_str(), "w");
	fprintf(fOut, " ");
	fclose(fOut);
	return;
}

void createIndex(string DB_Name, string Table_Name, string Attr_Name, string Index_Name){
	if (Index_Name.length() > 14)
	{
		printf("ERROR: Index %s's name is too long!", Index_Name.c_str());
		return;
	}
	int attrNo = attrOrder(DB_Name, Table_Name, Attr_Name);
	if (attrNo == 0)
	{
		printf("ERROR: There is no such attribute!\n");
		return;
	}
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	fseek(fUpdate, 20 + (attrNo - 1) * 40 + 25, 0);
	char t = fgetc(fUpdate);
	if (t == '1')
	{
		fclose(fUpdate);
		printf("ERROR: There is already an index!\n");
		return;
	}
	fseek(fUpdate, 20 + (attrNo - 1) * 40 + 25, 0);
	fprintf(fUpdate, "1");
	fprintf(fUpdate, "%s", Index_Name.c_str());
	if (Index_Name.length()<14)
		fprintf(fUpdate, "%c", 0);
	fclose(fUpdate);
	string dpath = "Data//" + DB_Name + "//" + Table_Name + "//" + Table_Name + "_" + Attr_Name + ".1.dat";
	FILE *fOut = fopen(dpath.c_str(), "w");
	fprintf(fOut, "001#001");
	fclose(fOut);
}

void addAttr(string DB_Name, string Table_Name, string Attr_Name, int Attr_Len, int Attr_Type, int Data_Type)
{
	if (Attr_Name.length() > 14)
	{
		printf("ERROR: Attribute %s's name is too long !\n", Attr_Name);
		return;
	}
	if (Attr_Len>255 && Data_Type == 1)
	{
		printf("ERROR: Attribute %s's length is too long !\n", Attr_Name);
		return;
	}
	if (attrOrder(DB_Name, Table_Name, Attr_Name) != 0)
	{
		printf("ERROR: Attribute name confilcts!\n");
		return;
	}
	int Attrcount;
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	fscanf(fUpdate, "%d", &Attrcount);
	fseek(fUpdate, 20 + Attrcount * 40, 0);
	fprintf(fUpdate, "%s", Attr_Name.c_str());
	fseek(fUpdate, 20 + Attrcount * 40 + 20, 0);
	fprintf(fUpdate, "%d", Attr_Len);
	fseek(fUpdate, 20 + Attrcount * 40 + 23, 0);
	fprintf(fUpdate, "%d", Attr_Type);
	fseek(fUpdate, 20 + Attrcount * 40 + 24, 0);
	fprintf(fUpdate, "%d", Data_Type);
	fseek(fUpdate, 20 + Attrcount * 40 + 25, 0);
	fprintf(fUpdate, "%d", 0);
	fseek(fUpdate, 0, 0);
	fprintf(fUpdate, "%d", Attrcount + 1);
	fclose(fUpdate);
	return;
}

int attrOrder(string DB_Name, string Table_Name, string Attr_Name){
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fIn = fopen(path.c_str(), "r");
	int Attrcount;
	char name[25];
	fscanf(fIn, "%d", &Attrcount);
	for (int i = 0; i < Attrcount; i++)
	{
		fseek(fIn, 20 + 40 * i, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fIn);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (string(name) == Attr_Name)
		{
			fclose(fIn);
			return i + 1;
		}
	}
	fclose(fIn);
	return 0;
}

bool existDatabase(string DB_Name){
	int DBcount;
	string path = "Catalog//DB_Name.dat";
	FILE *fIn = fopen(path.c_str(), "r");
	char name[25];
	fscanf(fIn, "%d", &DBcount);
	for (int i = 1; i <= DBcount; i++)
	{
		fseek(fIn, 20 * i, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fIn);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (string(name) == DB_Name)
		{			
			fclose(fIn);
			return true;
		}
	}
	return false;
}

bool existTable(string DB_Name, string Table_Name){
	string path = "Catalog//"+DB_Name+"//"+Table_Name+".dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	if (fUpdate == NULL)
		return false;
	else
	{
		fclose(fUpdate);
		return true;
	}		
}

void dropIndex(string DB_Name, string Table_Name,  string Index_Name)
{
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat",Attr_Name;
	FILE *fUpdate = fopen(path.c_str(), "r+");
	int attrNo=-1, attrCount;
	char c, name[25];
	fscanf(fUpdate, "%d", &attrCount);
	for (int i = 0; i < attrCount; i++)
	{
		fseek(fUpdate, 20 + i * 40 + 25,0);
		c = fgetc(fUpdate);
		if (c != '1') continue;
		for (int j = 0; j < 14; j++)
		{
			name[j] = fgetc(fUpdate);
			if (name[j] == 0)
				break;
		}
		name[14] = 0;
		if (string(name) == Index_Name)
		{
			attrNo = i;
			fseek(fUpdate, 20 + i * 40, 0);
			for (int j = 0; j < 20; j++)
			{
				name[j] = fgetc(fUpdate);
				if (name[j] == 0)
					break;
			}
			name[20] = 0;
			Attr_Name = string(name);
			break;
		}
	}
	if (attrNo == -1)
	{
		printf("ERROR: There is no such index!\n");
		fclose(fUpdate);
		return;
	}
	fseek(fUpdate, 20 + attrNo * 40 + 25, 0);
	fprintf(fUpdate, "0");
	for (int i = 0; i < 14; i++)
		fprintf(fUpdate, "%c", 0);
	fclose(fUpdate);
	string dpath = "Data//"+DB_Name + "//" + Table_Name + "//" + Table_Name + "_" + Attr_Name + ".1.dat";
	DeleteFile(dpath.c_str());
}

void dropTable(string DB_Name, string Table_Name){
	if (!existTable(DB_Name, Table_Name))
	{
		printf("ERROR: There is no such table\n");
		return;
	}
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE* fUpdate = fopen(path.c_str(), "r+");
	int Attrcount;
	char c,name[25];
	string indexPath,recordPath,tablePath,tableNamePath;
	fscanf(fUpdate, "%d", &Attrcount);
	for (int i = 0; i < Attrcount; i++)
	{
		fseek(fUpdate, 20 + i * 40+25, 0);
		c = fgetc(fUpdate);
		fseek(fUpdate, 20 + i * 40, 0);
		if (c == '1')
		{
			for (int j = 0; j < 20; j++)
			{
				name[j] = fgetc(fUpdate);
				if (name[j] == 0)
					break;
			}
			name[20] = 0;
			indexPath = "Data//" + DB_Name + "//" + Table_Name + "//" + Table_Name + "_" + string(name) + ".1.dat";
			DeleteFile(indexPath.c_str());
		}
	}
	recordPath = "Data//" + DB_Name + "//" + Table_Name + "//" + Table_Name + ".0.dat";
	DeleteFile(recordPath.c_str());
	tablePath = "Data//" + DB_Name + "//" + Table_Name + "//";
	RemoveDirectory(tablePath.c_str());
	fclose(fUpdate);
	tableNamePath = "Catalog//" + DB_Name + "//TA_Name.dat";
	fUpdate = fopen(tableNamePath.c_str(),"r+");
	int TAcount;
	fscanf(fUpdate, "%d", &TAcount);
	for (int i = 0; i < TAcount; i++)
	{
		fseek(fUpdate, 20 + i * 20, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fUpdate);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (string(name) == Table_Name)
		{
			fseek(fUpdate, 20 + i * 20, 0);
			for (int j = 0; j < 20; j++)
				fprintf(fUpdate, "%c", 0);
			break;
		}
	}
	fclose(fUpdate);
	DeleteFile(path.c_str());
}

void dropDatabase(string DB_Name){
	string tableNamePath,databaseCatalog,databaseData,databaseNamePath;
	tableNamePath = "Catalog//" + DB_Name + "//TA_Name.dat";
	FILE* fUpdate = fopen(tableNamePath.c_str(), "r");
	char name[25];
	int TAcount;
	fscanf(fUpdate, "%d", &TAcount);
	for (int i = 0; i < TAcount; i++)
	{
		fseek(fUpdate, 20 + i * 20, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fUpdate);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (strlen(name) != 0)
			dropTable(DB_Name, string(name));
	}
	fclose(fUpdate);
	DeleteFile(tableNamePath.c_str());
	databaseCatalog = "Catalog//" + DB_Name + "//";
	RemoveDirectory(databaseCatalog.c_str());
	databaseData = "Data//" + DB_Name + "//";
	RemoveDirectory(databaseData.c_str());
	databaseNamePath = "Catalog//DB_Name.dat";
	int DBcount;
	fUpdate = fopen(databaseNamePath.c_str(),"r+");
	fscanf(fUpdate, "%d", &DBcount);
	for (int i = 0; i < DBcount; i++)
	{
		fseek(fUpdate, 20 + i * 20, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fUpdate);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (string(name) == DB_Name)
		{
			fseek(fUpdate, 20 + i * 20, 0);
			for (int j = 0; j < 20; j++)
				fprintf(fUpdate, "%c", 0);
			break;
		}
	}
	fclose(fUpdate);
}

int getRecordSum(string DB_Name, string Table_Name)
{
	if (!existTable(DB_Name, Table_Name))
	{
		printf("ERROR: There is no such Table %s:%s\n", DB_Name.c_str(), Table_Name.c_str());
		return -1;
	}
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fIn = fopen(path.c_str(), "r");
	fseek(fIn, 10, 0);
	int ret = 0;
	char c;
	for (int j = 0; j < 10; j++)
	{
		c = fgetc(fIn);
		if ('0' <= c && c <= '9')
			ret = (c - '0') + ret * 10;
		else
			break;
	}
	fclose(fIn);
	return ret;
}

void setRecordSum(string DB_Name, string Table_Name, int val){
	if (!existTable(DB_Name, Table_Name))
	{
		printf("ERROR: There is no such Table %s:%s\n", DB_Name.c_str(), Table_Name.c_str());
		return;
	}
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fUpdate = fopen(path.c_str(), "r+");
	fseek(fUpdate, 10, 0);
	fprintf(fUpdate, "%d", val);
	fclose(fUpdate);
	return;
}

attrInfo *getAttrInfo(string DB_Name, string Table_Name, string Attr_Name){
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fIn = fopen(path.c_str(), "r");
	attrInfo *ret=new attrInfo;
	int Attrcount;
	char name[25],c;
	fscanf(fIn, "%d", &Attrcount);
	for (int i = 0; i < Attrcount; i++)
	{
		fseek(fIn, 20 + 40 * i, 0);
		for (int j = 0; j < 20; j++)
		{
			name[j] = fgetc(fIn);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		if (string(name) == Attr_Name)
		{
			fseek(fIn, 20 + 40 * i+23, 0);
			ret->attrName = Attr_Name;
			c = fgetc(fIn);
			ret->pri=c-'0';
			c = fgetc(fIn);
			ret->type = c - '0';
			c = fgetc(fIn);
			ret->index = c - '0';
			if (ret->index == 1)
			{
				for (int j = 0; j < 14; j++)
				{
					name[j] = fgetc(fIn);
					if (name[j] == 0)
						break;
				}
				name[20] = 0;
				ret->indexName = string(name);
			}
			else
				ret->indexName = "";
			fclose(fIn);
			return ret;
		}
	}
	fclose(fIn);
	printf("ERROR: There is no such attr : %s\n", Attr_Name.c_str());
	return NULL;
}

attrInfo *getAttrInfo(string DB_Name, string Table_Name, int Attr_No){
	string path = "Catalog//" + DB_Name + "//" + Table_Name + ".dat";
	FILE *fIn = fopen(path.c_str(), "r");
	attrInfo *ret =NULL;
	int Attrcount;
	char name[25], c;
	fscanf(fIn, "%d", &Attrcount);
	if (Attr_No > Attrcount)
	{
		fclose(fIn);
		return ret;
	}
	ret = new attrInfo;
	fseek(fIn, 20 + 40 * (Attr_No-1), 0);
	for (int j = 0; j < 20; j++)
	{
		name[j] = fgetc(fIn);
		if (name[j] == 0)
			break;
	}
	name[20] = 0;
	ret->attrName = string(name);
	fseek(fIn, 20 + 40 * (Attr_No - 1) + 23, 0);
	c = fgetc(fIn);
	ret->pri = c - '0';
	c = fgetc(fIn);
	ret->type = c - '0';
	c = fgetc(fIn);
	ret->index = c - '0';
	if (ret->index == 1)
	{
		for (int j = 0; j < 14; j++)
		{
			name[j] = fgetc(fIn);
			if (name[j] == 0)
				break;
		}
		name[20] = 0;
		ret->indexName = string(name);
	}
	else
		ret->indexName = "";
	fclose(fIn);
	return ret;
}
