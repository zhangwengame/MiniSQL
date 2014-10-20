#include "Buffer.h"
blockInfo* existBlock(fileInfo* file, int blockNum){
	blockInfo* ret;
	if (file->blockSet.count(blockNum) == 0)
		return NULL;
	else
	{
		for (ret = file->firstBlock; ret->blockNum != blockNum; ret = ret->next);
		return ret;
	}
}
fileInfo* existFile(CString DB_Name, CString fileName, bufferInfo* bufferInfo){
	fileInfo *ite = bufferInfo->fileHandle;
	for (; ite != NULL; ite = ite->next)
		if (ite->fileName == fileName && ite->dataBase==DB_Name)
			return ite;
	return NULL;
}
void writeBlock(CString DB_Name, blockInfo *block){
	std::ofstream fout;
	CString path = DB_Name + "//" + block->file->fileName + ".dat";
	fout.open(path);
	fout.seekp(BLOCK_LEN*(block->blockNum), std::ios::beg);
	fout << block->cBlock;
	fout.close();
}
void closeFile(fileInfo* F,CString DB_Name, CString m_fileName, int m_fileType,bufferInfo* bufferInfo){
	fileInfo *file,*ite;
	if (F == NULL)
	{
		file = existFile(DB_Name, m_fileName, bufferInfo);
		if (file == NULL) return;
	}
	else
		file = F;
	blockInfo *block;
	for (block = file->firstBlock; block != NULL; block = block->next)
	{
		if (block->dirtyBit == 1)
			writeBlock(DB_Name, block);
		block->next = bufferInfo->blockHandle;
		bufferInfo->blockHandle = block;
	}
	if (file == bufferInfo->fileHandle)
		bufferInfo->fileHandle = file->next;
	else
	{ 
		for (ite = bufferInfo->fileHandle; ite->next != file; ite = ite->next);
		ite->next = file->next;
	}
	delete file;
	bufferInfo->fileCount--;
}
fileInfo* getfile(CString DB_Name, CString fileName, int m_fileType, bufferInfo* bufferInfo){
	fileInfo *ret;
	ret = existFile(DB_Name,fileName,bufferInfo);
	if (ret != NULL) return ret;
	/*---------------------------------------*/
	if (bufferInfo->fileCount >= MAX_FILE_ACTIVE)
		closeFile(bufferInfo->fileHandle,"","",0,bufferInfo);
	ret = new fileInfo(DB_Name,fileName,m_fileType);
	std::ifstream fin;
	CString path;
	path = DB_Name + "//" + fileName+".dat";
	fin.open(path);
	/*initialization*/
	fin.close();
	bufferInfo->fileCount++;
	ret->next = bufferInfo->fileHandle;
	bufferInfo->fileHandle = ret;
	bufferInfo->fileCount++;
	return ret;
}
blockInfo*	findBlock(bufferInfo* bufferInfo)
{
	blockInfo* block;
	if (bufferInfo->blockHandle != NULL)
	{
		block = bufferInfo->blockHandle;
		bufferInfo->blockHandle = bufferInfo->blockHandle->next;
	}
	else
	{
		if (bufferInfo->blockCount >= MAX_BLOCK)
		{
			fileInfo *fite, *minFile=bufferInfo->fileHandle;
			time_t minTime = bufferInfo->fileHandle->lastBlock->iTime;
			for (fite = bufferInfo->fileHandle; fite != NULL; fite = fite->next){
				if (fite->lastBlock->iTime < minTime)
				{
					minFile = fite;
					minTime = fite->lastBlock->iTime;
				}
			}
			blockInfo *bite = minFile->firstBlock;
			if (bite == minFile->lastBlock)
			{
				minFile->blockSet.erase(bite->blockNum);
				minFile->firstBlock = minFile->lastBlock = NULL;
				block = bite;
			}
			else
			{
				for (; bite->next != minFile->lastBlock; bite = bite->next);
				minFile->blockSet.erase(bite->next->blockNum);
				minFile->lastBlock = bite;
				block = bite->next;
				bite->next = NULL;
			}
		}
		else
		{
			block = new blockInfo();
			bufferInfo->blockCount++;
		}
	}
	return block;
}
blockInfo* getblock(fileInfo* F, int blockNum, bufferInfo* bufferInfo){
	fileInfo *file=F;
	blockInfo *block;
	block = findBlock(bufferInfo);
	block->blockNum = blockNum;
	block->dirtyBit = 0;
	block->file = file;
	block->iTime = time(NULL);
	block->lock = 0;
	block->next = file->firstBlock;
	if (file->lastBlock == NULL)
		file->lastBlock = block;
	file->firstBlock = block;
	file->blockSet.insert(blockNum);
	std::ifstream fin;
	CString path = file->dataBase + "//" + file->fileName + ".dat";
	fin.open(path);
	fin.seekg(BLOCK_LEN*blockNum, std::ios::beg);
	fin.get(block->cBlock, BLOCK_LEN, '~');
	fin.close();

	return block;
}
blockInfo* readBlock(CString DB_Name, CString m_fileName, int m_blockNum, int m_fileType, bufferInfo* bufferInfo)
{
	fileInfo *file;
	file = existFile(DB_Name, m_fileName, bufferInfo);
	if (file == NULL)
		file = getfile(DB_Name, m_fileName, m_fileType, bufferInfo);
	blockInfo *block;
	block = existBlock(file, m_blockNum);
	if (block == NULL)
		block = getblock(file, m_blockNum, bufferInfo);
	return block;
}
void closeDatabase(CString DB_Name, bufferInfo* bufferInfo){
	fileInfo *fite,*tmp=NULL;
	for (fite = bufferInfo->fileHandle; fite != NULL;fite=tmp){
		tmp = fite->next;
		closeFile(fite,"","",0, bufferInfo);
	}
}
void quitProg(CString DB_Name, bufferInfo* bufferInfo){
	closeDatabase(DB_Name, bufferInfo);
	blockInfo* bite,*tmp=NULL;
	for (bite = bufferInfo->blockHandle; bite != NULL;bite=tmp)
	{
		tmp = bite->next;
		delete bite;
	}
}
