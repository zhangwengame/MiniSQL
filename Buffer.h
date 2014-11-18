///////////////////////////////////////////////////////////////
///---------------------------------------------------------///
///       Module: Buffer                                    ///
///       Produced by: Wen Zhang                            ///
///       Description: Arrange buffer area for whole DBMS.  ///
///                    The whole DBMS use buffer to do file ///
///                    operations.                          ///
///       date: 2014/11/18                                  ///
///-------------------------------------------------------- ///
///////////////////////////////////////////////////////////////
#ifndef _BUFFER_H 
#define _BUFFER_H

#define BLOCK_LEN			4096	
#define MAX_FILE_ACTIVE 	5		
#define MAX_BLOCK			40		
#include <fstream>
#include <set>
#include <string.h>
#include <Windows.h>

using namespace std;

struct fileInfo;
struct blockInfo;

struct index_info
{
	string index_name;
	int type;
	int root;
	long offset; 
	void* value;
};

struct bufferInfo{
public:
	fileInfo *fileHandle;
	blockInfo *blockHandle;
	index_info index_0;
	string currentDatabase;
	int fileCount;
	int blockCount;
	bufferInfo() :fileHandle(NULL), blockHandle(NULL),fileCount(0),blockCount(0){};
};

struct blockInfo  {
	int blockNum;	
	bool dirtyBit;     
	blockInfo *next;  	
	fileInfo *file;     	
	char *cBlock;	   
	int iTime;		
	int lock;
	blockInfo()
	{
		cBlock = new char[BLOCK_LEN];
	}
	~blockInfo()
	{
		delete[] cBlock;
	}
};

struct fileInfo  {
	int type;				
	string fileName;		
	string dataBase;       
	string attrName;
	fileInfo *next;			
	blockInfo *firstBlock;	
	blockInfo *lastBlock;
	std::set<int> blockSet;
	fileInfo(string dataBase, string fileName, int type) :
		dataBase(dataBase), fileName(fileName), type(type), next(NULL), firstBlock(NULL), lastBlock(NULL)
	{
		blockSet.clear();
	};
};	

fileInfo* getFile(string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo);
blockInfo* findBlock(bufferInfo* bufferInfo);
blockInfo* getBlock(fileInfo* F, int blockNum, bufferInfo* bufferInfo);
blockInfo* existBlock(fileInfo* file, int blockNum);
fileInfo* existFile(string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo);
blockInfo* readBlock(string DB_Name, string fileName, string attrName, int blockNum, int fileType, bufferInfo* bufferInfo);
void writeBlock(string DB_Name, blockInfo * block);
void closeFile(fileInfo* F, string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo);
void closeDatabase(string DB_Name, bufferInfo* bufferInfo);
void quitProg(string DB_Name, bufferInfo* bufferInfo);
#endif
