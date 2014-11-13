#include "Buffer.h"
#include "Buffer.cpp"
#include "Catalog.h"
#include "Catalog.cpp"
#include "Index.h"
#include <vector>
#include <iostream>
using namespace std;
const int capacity=5;
int root=0;
bufferInfo *run=new bufferInfo;
using namespace std;
void encodeNode(const Node& p){ //写入该块
	int block_num;
	int charNum,i,t;
	char tmp[10];
	char s[100];
	block_num=p.node_num;
	blockInfo* b;
	b = readBlock("D_1", "Balance","account" ,block_num, 1, run);
	if (p.value.size()==0){
		strcpy(b->cBlock,"!0000000000000");
		b->dirtyBit=1;
		writeBlock("D_1",b);
		return;
	}
	if (p.leaf)
		charNum=1+4+p.value.size()+p.record.size()*5+9;
	else
		charNum=1+4+p.value.size()+p.record.size()*3+9;
    cout<<"charNum"<<charNum<<endl;
	//s=new char(charNum+1);
	//memset(s,0,sizeof(s));
	//cout<<"ok"<<endl;
	//cout<<charNum<<endl;
	if (p.leaf){
		sprintf(tmp,"%c%04d",'!',p.value.size());
		strcpy(s,tmp);
		//cout<<tmp<<endl;
		//cout<<s<<endl;
		cout<<"Record amount:"<<p.value.size()<<endl;
		for(i=0;i<p.value.size();i++){
			cout<<p.record[i]<<p.value[i]<<endl;
		}
		for(i=0;i<p.record.size();i++){
			//memset(tmp,0,sizeof(tmp));
			sprintf(tmp,"%05d%c",p.record[i],p.value[i]);
			//cout<<p.value[i]<<endl;
			strcat(s,tmp);
		}
		//cout<<s<<endl;
	}
	else  //p is ?
	{
		sprintf(tmp,"%c%04d",'?',p.value.size());
		strcpy(s,tmp);
		for(i=0;i<p.value.size();i++){
			sprintf(tmp,"%03d%c",p.record[i],p.value[i]);
			strcat(s,tmp);
		}
		sprintf(tmp,"%03d",p.record[i]);
		strcat(s,tmp);
	}
	//cout<<s<<endl;
	sprintf(tmp,"%03d%03d%03d",p.pre,p.suc,p.parent);
	//cout<<tmp<<endl;
	strcat(s,tmp);
	cout<<"The result of index"<<s<<endl;
	//delete b[block_num].cBlock;//=new char(charNum);
	//memset(b[block_num].cBlock,0,sizeof(b[block_num].cBlock));
	strcpy(b->cBlock,s);
	b->dirtyBit=1;
	writeBlock("D_1",b);
	cout<<"Encode ok"<<endl;
	//delete s;
}
Node parseNode(int block_num){
	Node p;
	blockInfo* b;
	int num,i,result;
	b = readBlock("D_1", "Balance","account" ,block_num, 1, run);
	if(b->cBlock[0]=='#'){
        cout<<"It's a empty index"<<endl;
		p.node_num=block_num;
		p.leaf=true;
		p.pre=0;
		p.suc=0;
		p.parent=0;
		encodeNode(p);
	}
	else{
        p.node_num=block_num;
		int charNum=strlen(b->cBlock); //读取这块的内容
		p.pre=(b->cBlock[charNum-9]-'0')*100+(b->cBlock[charNum-8]-'0')*10+b->cBlock[charNum-7]-'0';
		p.suc=(b->cBlock[charNum-6]-'0')*100+(b->cBlock[charNum-5]-'0')*10+b->cBlock[charNum-4]-'0';
        p.parent=(b->cBlock[charNum-3]-'0')*100+(b->cBlock[charNum-2]-'0')*10+b->cBlock[charNum-1]-'0';
		//cout<<b[t].cBlock<<endl;
		cout<<"parseNode:"<<"block_num"<<block_num<<"parent"<<p.parent<<"pre"<<p.pre<<"suc"<<p.suc<<endl;
		if (b->cBlock[0]=='!')
		{
            p.leaf=true;
            num=(b->cBlock[1]-'0')*1000+(b->cBlock[2]-'0')*100+(b->cBlock[3]-'0')*10+b->cBlock[4]-'0';
            for(i=0;i<num;i++){
                //if (b[t].cBlock[10+i*6])
                p.value.push_back(b->cBlock[10+i*6]);
                result=(b->cBlock[5+i*6]-'0')*10000+(b->cBlock[6+i*6]-'0')*1000+(b->cBlock[7+i*6]-'0')*100+(b->cBlock[8+i*6]-'0')*10+(b->cBlock[9+i*6]-'0');
                p.record.push_back(result);
            }
		}
        else if (b->cBlock[0]=='?')
        {
            p.leaf=false;
            num=(b->cBlock[1]-'0')*1000+(b->cBlock[2]-'0')*100+(b->cBlock[3]-'0')*10+b->cBlock[4]-'0';
            for(i=0;i<num;i++){
                //if (b[t].cBlock[10+i*6])
                p.value.push_back(b->cBlock[8+i*4]);
                result=(b->cBlock[5+i*4]-'0')*100+(b->cBlock[6+i*4]-'0')*10+b->cBlock[7+i*4]-'0';
                p.record.push_back(result);
            }
            result=(b->cBlock[5+i*4]-'0')*100+(b->cBlock[6+i*4]-'0')*10+b->cBlock[7+i*4]-'0';
            p.record.push_back(result);
        }
        else{
            cout<<"error"<<endl;
        }
	}
	return p;
}
int search_one(string database,string table_name,struct index_info& inform,int block_num){
	int i,t=block_num;
	int num,result=0;
	Node tmp=parseNode(block_num);
	num=tmp.value.size();
	//cout<<"record size"<<tmp.record.size()<<endl;
	if (tmp.leaf){
		t=tmp.node_num;
		for(i=0;i<tmp.value.size();i++){
			if (inform.value==tmp.value[i])
			{
				inform.offset=i;
				return t;
			}
            
		}
		cout<<"No index now"<<endl;
		inform.offset=0;
		return t;
	}
	else //?
	{
		//num=(b[t].cBlock[1]-'0')*1000+(b[t].cBlock[2]-'0')*100+(b[t].cBlock[3]-'0')*10+b[t].cBlock[4]-'0';
        //cout<<"Record Size"<<tmp.record[1]<<endl;
		for(i=0;i<num;i++){
			if (inform.value<tmp.value[i])
			{
				return search_one(database,table_name,inform,tmp.record[i]);
			}
		}
		return search_one(database,table_name,inform,tmp.record[i]);
	}
}
char split_child(Node& target,Node& new_target){
	int i,j;
 	char mid;
	blockInfo *b;
	mid=target.value[capacity/2];
	b=readBlock("D_1", "Balance","account" ,0, 1, run);
	int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
	int root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	new_target.node_num=new_block;
	sprintf(b->cBlock,"%03d%c%03d",++new_block,'#',root);
	b->dirtyBit=1;
	writeBlock("D_1",b);
	new_target.pre=0;
	new_target.suc=0;
	new_target.parent=0;
 	cout<<"mid"<<mid<<endl;
 	if (target.leaf){
 		for (j = capacity/2; j < capacity; ++j){
 			new_target.value.push_back(target.value[j]);
 			new_target.record.push_back(target.record[j]);
 		}
 		for (j = capacity/2; j < capacity; ++j){
 			target.value.pop_back();
 			target.record.pop_back();
 		}
 		new_target.leaf=true;
 	}
 	if (!target.leaf){
 		for(j=capacity/2+1;j<capacity;j++){
 			//b[target.record[j]].parent=new_target.node_num;
			new_target.record.push_back(target.record[j]);
			new_target.value.push_back(target.value[j]);
 		}
 		new_target.record.push_back(target.record.back()); //最后一个没被考虑进去
 		for(j=capacity/2+1;j<capacity;j++){
 			target.record.pop_back();
 			target.value.pop_back();
 		}
		//new_target.record.push_back(target.record.back()); //最后一个没被考虑进去
		//b[target.record[capacity]].parent=new_target.node_num;
		new_target.leaf=false;
 		target.record.pop_back();
 		target.value.pop_back();
 	}
 	//new_target.parent=target.parent;
 	target.suc=new_target.node_num;
 	new_target.pre=target.node_num;
 	return mid;
}
void insert_parent(char a,int p,Node& lchild,Node& rchild){
	int temp,j;
	Node tmp;
	char mid;
	blockInfo *b;
	if (p==0){ //向上合并的是root
		b=readBlock("D_1", "Balance","account" ,0, 1, run);
		int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
		root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
		p=new_block;
		tmp.node_num=new_block;
		root=p;
		sprintf(b->cBlock,"%03d%c%03d",++new_block,'#',root);
		b->dirtyBit=1;
		writeBlock("D_1",b);
		tmp.leaf=false;
		tmp.suc=0;
		tmp.pre=0;
		tmp.parent=0;
 //p=b[root].nextNew;
 //root=p;
 //p->child_ptr[0]=lchild; //做过根节点的都是false
		tmp.record.push_back(lchild.node_num);
 //p->child_ptr[1]=rchild;
		tmp.record.push_back(rchild.node_num);

 //lchild->parent=p;
		//b=readBlock("D_1", "Balance","account" ,lchild.node_num, 1, run);
		lchild.parent=p; //两个修改的块，因为之前申请的新块改动已经提交
 //rchild->parent=p;
		rchild.parent=p;
 //p->data[0]=a;
		tmp.value.push_back(a);
 //p->n++;
 	}
 	else{
 		tmp=parseNode(p);
 		int i=0;
 //while(a>p->data[i]&&i<p->n)
 		while(a>tmp.value[i]&&i<tmp.value.size())
 			i++;
 		tmp.value.push_back(0);
 		for(j=tmp.value.size()-1;j>i;j--)
			tmp.value[j]=tmp.value[j-1];
		tmp.record.push_back(0);
		for(j=tmp.record.size()-1;j>i+1;j--)
			tmp.record[j]=tmp.record[j-1];
 //p->data[i]=a;
 		tmp.value[i]=a;
 //p->child_ptr[i+1]=rchild;
 		tmp.record[i+1]=rchild.node_num;
 //rchild->parent=p;
 		rchild.parent=p;
 //p->n++;
 //if (p->n==capacity){
 		if (tmp.value.size()==capacity){
 			Node np=Node();
 			cout<<"Parent Split"<<endl;
 			mid=split_child(tmp,np);
 			insert_parent(mid,tmp.parent,tmp,np);
 //np=init();
 //temp=split_child(p,np);
 //insert_parent(temp,p->parent,p,np);
 		}
 	}
 	encodeNode(lchild);
 	encodeNode(rchild);
 	encodeNode(tmp);
}
void insert_one(string database,string table_name,struct index_info& inform,int block_num,int line_num){
	int position;
	int j;
	char mid;
	blockInfo *b;
	b = readBlock("D_1", "Balance","account" ,block_num, 1, run);
	int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
	root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	if (new_block==root){
		cout<<"Start to build index..."<<endl;
		cout<<"root"<<root<<endl;
		new_block++;
    	sprintf(b->cBlock,"%03d%c%03d",new_block,'#',root);
		cout<<"Block0:"<<b->cBlock<<endl;
		b->dirtyBit=1;
		writeBlock("D_1",b);
		b=readBlock("D_1", "Balance","account" ,root, 1, run);
		strcpy(b->cBlock,"#");
		cout<<"Block1:"<<b->cBlock<<endl;
		b->dirtyBit=1;
		writeBlock("D_1",b);
	}
	position=search_one(database,table_name,inform,root);
	Node tmp=parseNode(position);
	b=readBlock("D_1", "Balance","account" ,position, 1, run);
	cout<<position<<' '<<b->cBlock<<endl;
	if (inform.offset>0){
		cout<<"index existed"<<endl;
	}
	else if (inform.offset==0)
	{
		int i=0;
        if (tmp.value.size()>0)
            while(inform.value>tmp.value[i]&&i<tmp.value.size())
                i++;
		tmp.value.push_back(0);
		tmp.record.push_back(0);
		for(j=tmp.value.size()-1;j>i;j--){
			tmp.value[j]=tmp.value[j-1];
			tmp.record[j]=tmp.record[j-1];
		}
		//cout<<"tmpvalue1"<<tmp.value[1]<<endl;
		tmp.value[i]=inform.value;
		tmp.record[i]=line_num; //这个地方没理解行号应该怎么编？ 应该是在record中拿的
		if (tmp.value.size()>=capacity){
			Node np=Node();
			cout<<"Split"<<endl;
			mid=split_child(tmp,np);
			insert_parent(mid,tmp.parent,tmp,np);
		}
	}
	encodeNode(tmp);
    //cout<<tmp.leaf<<endl;
	for(int i=0;i<tmp.value.size();i++)
		cout<<tmp.record[i]<<tmp.value[i]<<endl;
}
void delete_block(int block_num){
	blockInfo *b;
	b=readBlock("D_1", "Balance","account" ,0, 1, run);
	//int new
	b=readBlock("D_1", "Balance","account" ,block_num, 1, run);
}
void delete_entry(char k,int position,int offset){
	int i;
	//char k;
	Node p; //当前正在操作的节点
	blockInfo *b;
	char mid;
	/*b = readBlock("D_1", "Balance","account" ,block_num, 1, run);
	root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	position=search_one(database,table_name,inform,root);//找到删除节点的叶子块*/
	Node predessor,successor,parent;
	i=offset;
	p=parseNode(position);
 	for(int j=i;j<p.value.size();j++){
			//r->data[j]=r->data[j+1];
			p.value[j]=p.value[j+1];
	}
	if (p.leaf)
		for(int j=i;j<p.record.size();j++){
			p.record[j]=p.record[j+1];
		}
	else 
		for(int j=i+1;j<p.record.size();j++){
			p.record[j]=p.record[j+1];
		}
	p.value.pop_back();
	p.record.pop_back(); //去掉k的位置 考虑了是从叶子还是内部节点删除的不同
 	if (position==root&&p.value.size()==0){
		// 处理当前节点变成一个废块，在需要新块时优先考虑这些
 		root=p.record[0]; //free the pre-root
 		cout<<"The new root is "<<root<<endl;
 		Node tmp=parseNode(root);
 		tmp.leaf=true;  //只有一层既是根也是
 		cout<<"preparent"<<tmp.parent<<endl;
 		tmp.parent=0;
 		tmp.suc=0;
 		tmp.pre=0;
 		cout<<"newparent"<<tmp.parent<<endl;
 		encodeNode(tmp);
 		return;
 	}
 	else if (p.value.size()<capacity/2){ //需要调整
 		if (p.pre!=0)
 			predessor=parseNode(p.pre);
 		else 
 			predessor.parent=-1;
 		if (p.suc!=0)
 			successor=parseNode(p.suc);
 		else
 			successor.parent=-1;
 		if (predessor.parent==p.parent)
 			successor=p;
 		else if (successor.parent==p.parent)
 			predessor=p;
 		else{
 			cout<<"肯定有哪里错了，不可能只有一个叶子节点"<<endl;
 			return ;
 		}
 		int t=0,temp;
 		parent=parseNode(p.parent);
 		while(parent.record[t]!=predessor.node_num&&t<parent.value.size())
 			t++;
 		k=parent.value[t]; //parent中处于分界位置的点
 		if (predessor.value.size()+successor.value.size()<capacity){ //这里需要把后面那块删掉加入废块链表
 			if (p.leaf)
 				for(i=0;i<successor.value.size();i++){
 					predessor.value.push_back(successor.value[i]);
 					predessor.record.push_back(successor.record[i]);
 				}
 			else if (!p.leaf){
 			//predessor->data[predessor->n]=k;
 				predessor.value.push_back(k);
	 		//predessor->n++;
 			//cout<<predessor->n<<endl;
 				for(i=0;i<=successor.record.size();i++){
 					predessor.record.push_back(successor.record[i]);
 				}
 				for(i=0;i<successor.value.size();i++){
 					predessor.value.push_back(successor.value[i]);
 				}
			}// destroy successor 加入废块链表
			predessor.suc=successor.suc;
			encodeNode(predessor);
 			delete_entry(k,predessor.parent,t);
 			if (predessor.value.size()==capacity){
 				Node np=Node();
 				cout<<"Split"<<endl;
 				mid=split_child(predessor,np);
 				insert_parent(mid,predessor.parent,predessor,np);
 				//insert_parent(temp,predessor->parent,predessor,np);
 			}
			//delete_block(successor.node_num);
		}
		else{ //合并超出容量 ps是否只考虑前面那个多的情况？
			if (predessor.node_num==p.node_num) //前面那个缺
				if (!p.leaf){
					//cout<<"It's here"<<endl;
					//cout<<successor.value.front()<<endl;
 					predessor.value.push_back(k);
 					predessor.record.push_back(successor.record.front());
 					parent.value[t]=predessor.value.front();
 					for(i=0;i<successor.value.size();i++)
 						successor.value[i]=successor.value[i+1];
 					for(i=0;i<successor.record.size();i++)
 						successor.record[i]=successor.record[i+1];
 					//successor.value[0]=k;
 					//successor.record[0]=predessor.record.back();
 					successor.record.pop_back();
 					successor.value.pop_back();
 				}
 				else{ //r->leaf
 					predessor.value.push_back(successor.value.front());
 					predessor.record.push_back(successor.record.front());
 					for(i=0;i<successor.value.size();i++){
 						successor.value[i]=successor.value[i+1];
 						successor.record[i]=successor.record[i+1];
 					}
 					//successor.value[0]=predessor.value.back();
 					//successor.record[0]=predessor.record.back();
 					successor.value.pop_back();
 					successor.record.pop_back();
 					parent.value[t]=successor.value.front();
 				}
			else if (successor.node_num==p.node_num){ //后面那个缺 
 				if (!p.leaf){
 					successor.value.push_back(0);
 					successor.record.push_back(0);
 					for(i=successor.value.size()-1;i>0;i--)
 						successor.value[i]=successor.value[i-1];
 					for(i=successor.record.size()-1;i>0;i--)
 						successor.record[i]=successor.record[i-1];
 					successor.value[0]=k;
 					successor.record[0]=predessor.record.back();
 					predessor.record.pop_back();
 					parent.value[t]=predessor.value.back();
 					predessor.value.pop_back();
 				}
 				else{ //r->leaf
 					successor.value.push_back(0);
 					successor.record.push_back(0);
 					for(i=successor.value.size();i>0;i--){
 						successor.value[i]=successor.value[i-1];
 						successor.record[i]=successor.record[i-1];
 					}
 					successor.value[0]=predessor.value.back();
 					successor.record[0]=predessor.record.back();
 					parent.value[t]=predessor.value.back();
 					predessor.value.pop_back();
 					predessor.record.pop_back();
 				}
 			}
 		encodeNode(predessor);
		encodeNode(successor);
		//encodeNode(parent);
		}
	} //above redistribution
	else{
		encodeNode(p);
	}
 // no need to change
}
void delete_one(string database,string table_name,struct index_info& inform,int block_num){
	int position;
	Node p; //当前正在操作的节点
	blockInfo *b;
	b = readBlock("D_1", "Balance","account" ,block_num, 1, run);
	root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	position=search_one(database,table_name,inform,root);//找到删除节点的叶子块
	cout<<"position:"<<position<<endl;
	cout<<"inform.offset:"<<inform.offset<<endl;
	delete_entry(inform.value,position,inform.offset);
}
int main(int argc, char const *argv[])
{	
	/*createDatabase("D_1");
	createTable("D_1", "Balance");
    createTable("D_1", "Balance1");
	createTable("D_1", "Balance2");
	addAttr("D_1", "Balance", "account", 8, 0, 1);
	addAttr("D_1", "Balance", "accounsa", 0, 0, 0); 
 	createIndex("D_1", "Balance", "account", "index1");*/
	struct index_info index1;
	index1.value='q';
	index1.offset=54;
	//delete_one("database","table1",index1,0);
	//index1.offset=4;
	//index1.value='h';
	insert_one("database","table1",index1,0,index1.offset);
	/*index1.value='k';
	index1.offset=100;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='l';
	index1.offset=102;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='m';
	index1.offset=103;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='n';
	index1.offset=104;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='o';
	index1.offset=105;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='p';
	index1.offset=106;
	insert_one("database","table1",index1,0,index1.offset);*/
	/*insert_one("database","table1",index1,0,index1.offset);
	index1.value='a';
	index1.offset=1;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='b';
	index1.offset=2;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='d';
	index1.offset=3;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='h';
	index1.offset=5;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='e';
	index1.offset=6;
	insert_one("database","table1",index1,0,index1.offset);
	index1.value='i';
	index1.offset=7;
	insert_one("database","table1",index1,0,index1.offset);*/
	//while(1)
	//createIndex("database","table1","amount","index");
	while(1);
	return 0;
}
