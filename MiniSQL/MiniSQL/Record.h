#ifndef _RECORD_H 
#define _RECORD_H

#define BLOCK_LEN			4096	// the size of one block
#define MAX_FILE_ACTIVE 	5		// limit the active files in the buffer
#define MAX_BLOCK			40		// the max number of the blocks
#define LinB                8
#include <afx.h>
#include <time.h>
#include <fstream>
#include <set>
#include <Catalog.h>

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

struct condition_info{
    int type,symbol;
    attr_info *left;
    int right;
};

char *lsplit=" ",*esplit=",";

void Close_Database(CString DB_Name,bool closetype);
void Close_File(CString DB_Name,CString filename,int filetype,bool closetype);
void Insert_Item(CString DB_Name,CString Table_Name,CString Attr,int & record_Num);
void Print_Head(attr_info print[32],int count);
void Print_To_Screen(CString record,attr_info print[32],int count);
void Select_Without_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond);
void Select_With_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
bool Confirm_To_Where(CString record,condition_info conds[10],int count,char cond);
bool Confirm(CString record,condition_info condition);
void Select_No_Where(CString DB_Name,CString Table_Name,attr_info print[32],int count);
void Select_With_Where(CString DB_Name,CString Table_Name,condition_info conds[10],int count,char cond,attr_info print[32],int Count);
void Delete_With_Where(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_Without_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_With_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,int index);
void Delete_With_Equal_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index);
void Delete_With_Greater_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
bool Find_Useful_Cond(condition_info conds[10],int count,int & index);
void Insert_Index_All(CString DB_Name,CString Table_Name,CString Index_Name,int length,int offset,int type);
bool Verify_Insertable(CString DB_Name,CString Table_Name,index_info nodes[32],int count,CString Attr);
void Quit(CString DB_Name);

#endif
