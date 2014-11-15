#include <cstdio>
#include <iostream>
#include <string>
#include "Buffer.h"
#include "Catalog.h"
//using namespace std;
 int main(){
 	 /*
	 40字符/表 0~19 属性名 20~22 长度 23 属性类型(0普通 1主键) 24数据类型 (0 int ,1 char,2 float) 25是否索引 26~39索引名
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
	// createTable("D_1", "Balance4");
	/* printf("get %d\n", getRecordSum("D_1", "Balance4"));
	 setRecordSum("D_1", "Balance4",123);
	 printf("get %d\n", getRecordSum("D_1", "Balance4"));*/
	 //createDatabase("D_1");
	/* createTable("D_2", "Balance");
	 addAttr("D_2", "Balance", "account", 8, 0, 1);
	 addAttr("D_2", "Balance", "accounsa", 0, 0, 0);*/
	 /*bufferInfo *run;
	 run = new bufferInfo;
	 blockInfo *b;
	 for (int i = 0; i < 50; i++)
	 {
		 b = readBlock("D_1", "Balance", "account", i, 1, run);
		 if (i == 0 || i == 3 || i == 5)
			 b->lock = 1;  
		 printf("%d: %d\n", i,run->blockCount);
	 }
	 fileInfo *fite;
	 for (fite = run->fileHandle; fite != NULL; fite = fite->next)
		 for (b = fite->firstBlock; b != NULL; b = b->next)
			 printf("%s %d\n", b->file->fileName.c_str(), b->blockNum);*/
	 /*attrInfo *ret;
	 ret = getAttrInfo("D_1", "Balance", "accsounsa");
	 if (ret!=NULL) printf("%s %s %d %d %d\n", ret->attrName.c_str(), ret->indexName.c_str(), ret->pri, ret->type, ret->index);*/
	 /*printf("%s\n", b->cBlock);
	 strcpy(b->cBlock, "hahaha");
	 b->dirtyBit = 1;
	 writeBlock("D_1", b);*/
	// createIndex("D_1", "Balance", "account", "index1"); 
	// dropIndex("D_1", "Balance", "index2");
	// dropIndex("D_1", "Balance", "index1");
	 dropDatabase("D_1");
	/* createDatabase("D_1");*/
	/* createTable("D_1", "Balance");
	 addAttr("D_1", "Balance", "account", 8, 0, 1);
	 addAttr("D_1", "Balance", "accounsa", 0, 0, 0);*/
	 /*addAttr("D_1", "Balance", "aaaaabbbbbcccccddddd", 8, 0, 1);
	 printf("%d\n", attrOrder("D_1", "Balance", "account"));
	 printf("%d\n", attrOrder("D_1", "Balance", "accounsa"));
	 printf("%d\n", attrOrder("D_1", "Balance", "aaaaabbbbbcccccddddd"));*/
	 /*createDatabase("D_1");*/
	
	 /*createTable("D_1", "Balance");
     createTable("D_1", "Balance1");
	 createTable("D_1", "Balance2");
	 addAttr("D_1", "Balance", "account", 8, 0, 1);
	 addAttr("D_1", "Balance", "accounsa", 0, 0, 0); 
     createIndex("D_1", "Balance", "account", "index1");*/
	 /*string dpath = "D_1//";
	 CreateDirectory(dpath.c_str(), NULL);*/
	/* createTable("D_1", "Balance");
	 addAttr("D_1", "Balance", "account", 8, 0, 1);
	 addAttr("D_1", "Balance", "accounsa", 0, 0, 0);*/
	 //createIndex("D_1", "Balance", "account", "index1");
	 //DeleteFile("Catalog//D_1//Balance.dat");
	// dropTable("D_1", "Balance");
	// dropDatabase("D_1");
	 //dropIndex("D_1", "Balance", "account", "index1");
	// createIndex("D_1", "Balance", "account", "index2");
	// createIndex("D_1", "Balance", "accounts", "index1");
	/* createIndex("D_1", "Balance", "account", "index1");
	 createIndex("D_1", "Balance", "account", "index1");
	 dropIndex("D_1", "Balance", "account", "index1");
	 createIndex("D_1", "Balance", "account", "index2");*/
	 //createIndex("D_1", "Balance", "account", "index2");
	  while (1);
 }
