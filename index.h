#ifndef _INDEX_H 
#define _INDEX_H
struct BPTreeNode
{
	int node_num;
	bool leaf;
	vector<char> value;
	vector<int> record; //blocknum or recordnum 取决于leaf
}; //中间结果临时变量
typedef struct BPTreeNode Node;
Node parse(int blocknum); //将字符串解析为节点
string encode(Node p); //将节点编码存储
int search_one(string database,string table_name,struct index_info& inform,int block_num); //block_num指是目前在哪个节点搜索
void search_many(string database,string table_name,int &start,int&end,int type,struct index_info& inform,int block_num);
void insert_one(string database, string  table_name, struct index_info & inform,int block_num);
void insert_divide(string database,string table_name,struct index_info& inform,int leaf1,int leaf2,Node* leafpoint2);
string int_to_str(int value);
int find_prev_leaf_sibling(string database,string table_name, struct index_info inform, int nodenum);
int find_next_leaf_sibling(string database,string table_name,struct index_info inform,int nodenum);
int find_left_child (string database,index_info  inform);
int find_right_child(string database,index_info  inform);
int get_new_freeblocknum(string database,string table_name,struct index_info& inform);
int find_father(string database,string table_name,index_info inform, int num);
void delete_one(string database,string table_name,struct index_info & inform);
#endif