#ifndef _INDEX_H 
#define _INDEX_H
#include "Buffer.h"
#include <vector>
using namespace std;
struct Node
{
	int node_num;
	bool leaf;
	vector<float> value;
	vector<int> record; //blocknum or recordnum
	int pre;
	int suc;
	int parent;
};

string int_char(int n);
int char_int(const char* s);
void encodeNode(string database,string table_name,index_info& inform,const Node& p,bufferInfo *run);
Node parseNode(string database,string table_name,index_info& inform,int block_num,int type,bufferInfo *run);
int search_one(string database,string table_name,index_info& inform,int block_num,bufferInfo *run);
//void search_many(string database,string table_name,struct index_info& inform);
float split_child(string database,string table_name,index_info& inform,Node& target,Node& new_target,bufferInfo *run);
void insert_parent(string database,string table_name,index_info& inform,void *pp,int p,Node& lchild,Node& rchild,bufferInfo *run);
void insert_one(string database,string table_name,index_info& inform,int block_num,int line_num,bufferInfo *run);
void delete_entry(string database,string table_name,index_info& inform,void* pp,int position,int offset,bufferInfo *run);
void delete_one(string database,string table_name,index_info& inform,int block_num,bufferInfo *run);
#endif
