#ifndef _INDEX_H 
#define _INDEX_H
//#include <afx.h>
#include <vector>
using namespace std;
struct Node
{
	int node_num;
	bool leaf;
	vector<char> value;
	vector<int> record; //blocknum or recordnum
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
	char value;
};
void encodeNode(const Node& p);
Node parseNode(int block_num);
int search_one(string database,string table_name,struct index_info& inform,int block_num);
char split_child(Node& target,Node& new_target);
void insert_parent(char a,int p,Node& lchild,Node& rchild);
void insert_one(string database,string table_name,struct index_info& inform,int block_num,int line_num);
void delete_entry(char k,int position,int offset);
void delete_one(string database,string table_name,struct index_info& inform,int block_num);
#endif
