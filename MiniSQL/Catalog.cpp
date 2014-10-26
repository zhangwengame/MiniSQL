#include "Catalog.h"
void Create_Database(CString DB_Name){
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