#include <cstdio>
#include <iostream>
#include <string>
#include "Buffer.h"
#include "Catalog.h"
//using namespace std;
 int main(){
 /*	bufferInfo *run;
	run= new bufferInfo;
	std::ofstream fout;
	std::ifstream fin;
	fout.open("out.txt");
	fout.seekp(5, std::ios::beg);
	fout << "test" << std::endl;
	std::cout << fout.tellp();*/
	/* FILE *fout = fopen("out.txt", "w+");
	 fseek(fout, 5, 0);
	 fwrite("test", 4, 1, fout);
	 //fprintf(fout, "%s\n", "test");
	 fclose(fout);*/
	/*fin.open("out.txt");
	fin.seekg(0, std::ios::end);
	//fout << "test" << std::endl;
	std::cout << fin.tellg();*/
/*	std::ifstream fin;
	CString path, DB_Name("Bank"),fileName("Balance");
	std::string tmp;
	int k;
	path = DB_Name + "//" + fileName + ".dat";
//	std::cout << path;
	fin.open(path);
	fin.seekg(4, std::ios::beg);
	fin >> tmp;
	std::cout << tmp;
	while (1);*/
//	Create_Database("D_2");
	 /*
	 40字符/表 0~19 属性名 20~22 长度 23 属性类型(0普通 1主键) 24数据类型 (0 int ,1 char,2 float) 25是否索引 26~40索引名
	 */
	/* createDatabase("D_1");
	 printf("%d\n", existTable("D_1", "Balance"));
	 createTable("D_1", "Balance");
	 addAttr("D_1", "Balance", "account", 8, 0, 1);
	 addAttr("D_1", "Balance", "accounsa", 0, 0, 0);
	 printf("%d\n", existTable("D_1", "Balance"));*/
	/* printf("%d\n", existDatabase("D_1"));
	 printf("%d\n", existDatabase("D_2"));*/
	// createDatabase("D_2");
	// printf("%d\n", existDatabase("D_2"));
	/* createTable("D_2", "Balance");
	 addAttr("D_2", "Balance", "account", 8, 0, 1);
	 addAttr("D_2", "Balance", "accounsa", 0, 0, 0);*/
	 bufferInfo *run;
	 run = new bufferInfo;
	/* fileInfo *t = getfile("D_1", "Balance", 0, run);
	 blockInfo *b = getblock(t, 0, run);*/
	/* blockInfo *b;
	 b=readBlock("D_1", "Balance", 0, 0, run);
	 printf("%s\n", b->cBlock);*/
	  while (1);
 }
