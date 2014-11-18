#include "Buffer.h"

fileInfo* getFile(string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo){
	fileInfo *ret;
	ret = existFile(DB_Name, fileName, attrName, fileType, bufferInfo);
	if (ret != NULL) return ret;
	if (bufferInfo->fileCount >= MAX_FILE_ACTIVE)
		closeFile(bufferInfo->fileHandle, "", "", attrName, 0, bufferInfo);
	ret = new fileInfo(DB_Name, fileName, fileType);
	std::ifstream fin;
	string path;
	path = "Data//" + DB_Name + "//" + fileName;
	if (fileType == 0)
		path = path + ".0.dat";
	else
	{
		ret->attrName = attrName;
		path = path + "_" + attrName + ".1.dat";
	}
	fin.open(path.c_str());
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
			fileInfo *fite, *minFile;
			blockInfo *bite, *minBlock = NULL;
			int minTime = -1;
			for (fite = bufferInfo->fileHandle; fite != NULL; fite = fite->next)
			{
				for (bite = fite->firstBlock; bite != NULL; bite = bite->next)
					if ((bite->iTime < minTime || minTime == -1) && bite->lock == 0)
					{
					minBlock = bite;
					minTime = bite->iTime;
					}
			}
			if (minBlock == NULL) return NULL;
			minFile = minBlock->file;
			if (minBlock == minFile->firstBlock)
			{
				minFile->blockSet.erase(minBlock->blockNum);
				minFile->firstBlock = minBlock->next;
				if (minBlock->next == NULL)
					minFile->lastBlock = NULL;
				block = minBlock;
			}
			else
			{
				for (bite = minFile->firstBlock; bite->next != minBlock; bite = bite->next);
				minFile->blockSet.erase(minBlock->blockNum);
				if (minBlock->next == NULL)
					minFile->lastBlock = bite;
				bite->next = minBlock->next;
				block = minBlock;
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

blockInfo* getBlock(fileInfo* F, int blockNum, bufferInfo* bufferInfo){
	fileInfo *file = F;
	blockInfo *block;
	SYSTEMTIME  time;
	GetSystemTime(&time);
	block = findBlock(bufferInfo);
	if (block == NULL) return NULL;
	block->blockNum = blockNum;
	block->dirtyBit = 0;
	block->file = file;
	block->iTime = time.wMilliseconds;
	block->lock = 0;
	block->next = file->firstBlock;
	block->lock = 0;
	if (file->lastBlock == NULL)
		file->lastBlock = block;
	file->firstBlock = block;
	file->blockSet.insert(blockNum);
	std::ifstream fin;
	string path = "Data//" + file->dataBase + "//" + file->fileName + "//" + file->fileName;
	if (file->type == 0)
		path += ".0.dat";
	else
		path += "_" + file->attrName + ".1.dat";
	fin.open(path.c_str());
	fin.seekg(BLOCK_LEN*blockNum, std::ios::beg);
	fin.get(block->cBlock, BLOCK_LEN, '~');
	fin.close();
	return block;
}

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

fileInfo* existFile(string DB_Name, string fileName, string attrName, int fileType, bufferInfo* bufferInfo){
	fileInfo *ite = bufferInfo->fileHandle;
	for (; ite != NULL; ite = ite->next)
		if (ite->fileName == fileName && ite->dataBase == DB_Name && ite->type == fileType &&ite->attrName == attrName)
			return ite;
	return NULL;
}

blockInfo* readBlock(string DB_Name, string fileName, string attrName, int blockNum, int fileType, bufferInfo* bufferInfo)
{
	fileInfo *file;
	SYSTEMTIME  time;
	GetSystemTime(&time);
	file = existFile(DB_Name, fileName, attrName, fileType, bufferInfo);
	if (file == NULL)
		file = getFile(DB_Name, fileName, attrName, fileType, bufferInfo);
	blockInfo *block;
	block = existBlock(file, blockNum);
	if (block == NULL)
		block = getBlock(file, blockNum, bufferInfo);
	if (block != NULL)
		block->iTime = time.wMilliseconds;
	return block;
}

void writeBlock(string DB_Name, blockInfo *block){
	string path = "Data//" + DB_Name + "//" + block->file->fileName + "//" + block->file->fileName;
	if (block->file->type == 0)
		path = path + ".0.dat";
	else
		path = path + "_"+block->file->attrName+".1.dat";
	FILE *fout = fopen(path.c_str(), "r+");
	if (fout == NULL) return;
	fseek(fout,BLOCK_LEN*(block->blockNum), 0);
	for (int i = 0; i < BLOCK_LEN;i++)
	fprintf(fout, "%c",block->cBlock[i] );
	fclose(fout);
}

void closeFile(fileInfo* F, string DB_Name, string fileName,string attrName,int fileType, bufferInfo* bufferInfo){
	fileInfo *file,*ite;
	if (F == NULL)
	{
		file = existFile(DB_Name, fileName, attrName,fileType, bufferInfo);
		if (file == NULL) return;
	}
	else
		file = F;
	blockInfo *block,*tmp;
	for (block = file->firstBlock; block != NULL; block = tmp)
	{
		if (block->dirtyBit == 1)
			writeBlock(DB_Name, block);
		tmp = block->next;
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

void closeDatabase(string DB_Name, bufferInfo* bufferInfo){
	fileInfo *file,*tmp=NULL;
	for (file = bufferInfo->fileHandle; file != NULL;file=tmp){
		tmp = file->next;
		closeFile(file,"","","",0, bufferInfo);
	}
}

void quitProg(string DB_Name, bufferInfo* bufferInfo){
	closeDatabase(DB_Name, bufferInfo);
	blockInfo* bite,*tmp=NULL;
	for (bite = bufferInfo->blockHandle; bite != NULL;bite=tmp)
	{
		tmp = bite->next;
		delete bite;
	}
}
