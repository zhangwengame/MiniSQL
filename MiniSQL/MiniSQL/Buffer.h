#ifndef _BUFFER_H 
#define _BUFFER_H

#define BLOCK_LEN			4096	// the size of one block
#define MAX_FILE_ACTIVE 	5		// limit the active files in the buffer
#define MAX_BLOCK			40		// the max number of the blocks
#include <afx.h>
#include <time.h>
#include <fstream>
#include <set>
struct fileInfo;
struct blockInfo;
struct bufferInfo{
public:
	fileInfo *fileHandle;
	blockInfo *blockHandle;
	int fileCount;
	int blockCount;
	bufferInfo() :fileHandle(NULL), blockHandle(NULL),fileCount(0){};
};
struct blockInfo  {
	int blockNum;	// the block number of the block,
	// which indicate it when it be newed
	bool dirtyBit;     // 0 -> flase£¬ 1 -> indicate dirty, write back
	blockInfo *next;  	// the pointer point to next block 
	fileInfo *file;     	// the pointer point to the file, which the block belongs to
//	int charNum;	   	// the number of chars in the block
	char *cBlock;	   	// the array space for storing the records in the block in buffer
	time_t iTime;		// it indicate the age of the block in use 
	int lock;// prevent the block from replacing
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
	int type;				// 0-> data file£¬ 1 -> index file
	CString fileName;		// the name of the file
	CString dataBase;       // dataBase it belongs to
	int recordAmount;		// the number of record in the file
//	int freeNum;			// the free block number which could be used for the file
	int recordLength;		// the length of the record in the file
	fileInfo *next;			// the pointer points to the next file
	blockInfo *firstBlock;	// point to the first block within the file
	blockInfo *lastBlock;
	std::set<int> blockSet;
	fileInfo(CString dataBase, CString fileName, int type) :
		dataBase(dataBase), fileName(fileName), type(type), next(NULL), firstBlock(NULL), lastBlock(NULL)
	{
		blockSet.clear();
	};
};	

blockInfo* existBlock(fileInfo* file, int blockNum);
fileInfo* existFile(CString DB_Name, CString fileName, bufferInfo* bufferInfo);
blockInfo*	findBlock(bufferInfo* bufferInfo);
void writeBlock(CString DB_Name, blockInfo * block);
blockInfo* readBlock(CString DB_Name, CString m_fileName, int m_blockNum, int m_fileType, bufferInfo* bufferInfo);
void closeFile(fileInfo* F,CString DB_Name, CString m_fileName, int m_fileType, bufferInfo* bufferInfo);
fileInfo* getfile(CString DB_Name,CString fileName, int m_fileType, bufferInfo* bufferInfo);
blockInfo * getblock(fileInfo* F, int blockNum, bufferInfo* bufferInfo);
void closeDatabase(CString DB_Name, bufferInfo* bufferInfo);
void quitProg(CString DB_Name, bufferInfo* bufferInfo);
#endif
