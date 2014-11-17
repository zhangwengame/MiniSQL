#ifndef _INDEX_H 
#define _INDEX_H
//#include <afx.h>
#include "Buffer.h"
//#include "Buffer.cpp"
//#include "Catalog.h"
//#include "Catalog.cpp"
#include <vector>
using namespace std;
struct Node
{
	int node_num;
	bool leaf;
	vector<float> value;
	vector<int> record; //blocknum or recordnum
	int type;
	int pre;
	int suc;
	int parent;
};
struct index_info
{
	string index_name;
	int length;
	char type;
	long offset; //在表中的行数
	void* value;
};
string int_char(int n);
int char_int(const char* s);
void encodeNode(string database,string table_name,string index_name,const Node& p,bufferInfo *run);
Node parseNode(string database,string table_name,string index_name,int block_num,int type,bufferInfo *run);
int search_one(string database,string table_name,struct index_info& inform,int block_num,bufferInfo *run);
//void search_many(string database,string table_name,struct index_info& inform);
float split_child(string database,string table_name,string index_name,Node& target,Node& new_target,bufferInfo *run);
void insert_parent(string database,string table_name,string index_name,void *pp,int p,Node& lchild,Node& rchild,bufferInfo *run);
void insert_one(string database,string table_name,struct index_info& inform,int block_num,int line_num,bufferInfo *run);
void delete_entry(string database,string table_name,string index_name,void* pp,int position,int offset,bufferInfo *run);
void delete_one(string database,string table_name,struct index_info& inform,int block_num,bufferInfo *run);
#endif
