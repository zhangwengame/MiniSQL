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
struct bufferInfo{
public:
	fileInfo *fileHandle;
	blockInfo *blockHandle;
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
	time_t iTime;		
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

blockInfo* existBlock(fileInfo* file, int blockNum);
fileInfo* existFile(string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo);
blockInfo*	findBlock(bufferInfo* bufferInfo);
void writeBlock(string DB_Name, blockInfo * block);
blockInfo* readBlock(string DB_Name, string fileName, string attrName, int blockNum, int fileType, bufferInfo* bufferInfo);
void closeFile(fileInfo* F, string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo);
fileInfo* getfile(string DB_Name, string fileName, string attrName,int fileType, bufferInfo* bufferInfo);
blockInfo * getblock(fileInfo* F, int blockNum, bufferInfo* bufferInfo);
void closeDatabase(string DB_Name, bufferInfo* bufferInfo);
void quitProg(string DB_Name, bufferInfo* bufferInfo);
#endif
