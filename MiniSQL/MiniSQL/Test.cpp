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
	Create_Database("D_2");
	//while (1);
 }
