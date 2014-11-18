#include "Index.h"
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
const int capacity=5;
//int root=0;
//int k_type=2;
//bufferInfo *run=new bufferInfo;
using namespace std;
string int_char(int n){
	string ss;
	char *s;
	while(n%27==0)
		n/=27;
	while(n!=0){
		ss.insert(0,1,(char)(n%27+'a'-1));
		n/=27;
	}
	return ss;
}
int char_int(const char* s){
	int num=strlen(s);
	int flag=0;
	int base=1,result=0;
	for(int i=0;i<num;i++){
		if (s[i]==' ') flag++;
		if (s[i]>='a'&&s[i]<='z'){
			result=result*27+(s[i]-'a'+1);
		}
	}
	for(int i=num-flag;i<5;i++)
		result*=27;
	return result;
}
void encodeNode(string database,string table_name,index_info& inform,const Node& p,bufferInfo *run){ //写入该块
	int block_num;
	int charNum,i,t;
	int v_width;
	char tmp[10];
	char s[100];
	if (inform.type==0)
		v_width=6;
	else if (inform.type==1)
		v_width=10;
	else if (inform.type==2)
		v_width=5; 
	else
		cout<<"Error type of key!"<<endl;
	block_num=p.node_num;
	blockInfo* b;
	b = readBlock(database,table_name,inform.index_name,block_num, 1, run);
	if (p.value.size()==0&&inform.root==p.node_num){
		strcpy(b->cBlock,"!0000000000000");
		b->dirtyBit=1;
		writeBlock(database,b);
		return;
	}
	if (p.leaf)
		charNum=1+4+p.value.size()*v_width+p.record.size()*5+9;
	else
		charNum=1+4+p.value.size()*v_width+p.record.size()*3+9;
    //cout<<"charNum"<<charNum<<endl;
	//s=new char(charNum+1);
	//memset(s,0,sizeof(s));
	//cout<<"ok"<<endl;
	//cout<<charNum<<endl;
	if (p.leaf){
		sprintf(tmp,"%c%04d",'!',p.value.size());
		strcpy(s,tmp);
		//cout<<tmp<<endl;
		//cout<<s<<endl;
		//cout<<"Record amount:"<<p.value.size()<<endl;
		/*for(i=0;i<p.value.size();i++){
			cout<<p.record[i]<<p.value[i]<<endl;
		}*/
		for(i=0;i<p.record.size();i++){
			//memset(tmp,0,sizeof(tmp));
			switch (inform.type){
				case 0:sprintf(tmp,"%05d%06d",p.record[i],(int)p.value[i]);break;
				case 1:sprintf(tmp,"%05d%10.4f",p.record[i],(float)p.value[i]);break;
				case 2:sprintf(tmp,"%05d%5s",p.record[i],int_char(p.value[i]).c_str());//cout<<"length"<<int_char(p.value[i]).length()<<endl;
					break;
				default: cout<<"Error type of key";break;
			}
			//sprintf(tmp,"%05d%c",p.record[i],p.value[i]);
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
			switch (inform.type){
				case 0:sprintf(tmp,"%03d%06d",p.record[i],(int)p.value[i]);break;
				case 1:sprintf(tmp,"%03d%10.4f",p.record[i],(float)p.value[i]);break;
				case 2:sprintf(tmp,"%03d%5s",p.record[i],int_char(p.value[i]).c_str());break;
				default: cout<<"Error type of key";break;
			}
			strcat(s,tmp);
		}
		sprintf(tmp,"%03d",p.record[i]);
		strcat(s,tmp);
	}
	//cout<<s<<endl;
	sprintf(tmp,"%03d%03d%03d",p.pre,p.suc,p.parent);
	//cout<<tmp<<endl;
	strcat(s,tmp);
	//cout<<"The result of index"<<s<<endl;
	//delete b[block_num].cBlock;//=new char(charNum);
	//memset(b[block_num].cBlock,0,sizeof(b[block_num].cBlock));
	strcpy(b->cBlock,s);
	b->dirtyBit=1;
	writeBlock(database,b);
	//cout<<"Encode ok"<<endl;
	//delete s;
}
Node parseNode(string database,string table_name,index_info& inform,int block_num,int type,bufferInfo *run){
	Node p;
	blockInfo* b;
	int num,i,result;
	string B;
	b = readBlock(database,table_name,inform.index_name,block_num, 1, run);
	B=b->cBlock;
	if(b->cBlock[0]=='#'){
        cout<<"It's a empty index"<<endl;
		p.node_num=block_num;
		p.leaf=true;
		p.pre=0;
		p.suc=0;
		p.parent=0;
		encodeNode(database,table_name,inform,p,run);
	}
	else{
        p.node_num=block_num;
		int charNum=strlen(b->cBlock);
		p.pre=(b->cBlock[charNum-9]-'0')*100+(b->cBlock[charNum-8]-'0')*10+b->cBlock[charNum-7]-'0';
		p.suc=(b->cBlock[charNum-6]-'0')*100+(b->cBlock[charNum-5]-'0')*10+b->cBlock[charNum-4]-'0';
        p.parent=(b->cBlock[charNum-3]-'0')*100+(b->cBlock[charNum-2]-'0')*10+b->cBlock[charNum-1]-'0';
		//cout<<b[t].cBlock<<endl;
		//cout<<"parseNode:"<<"block_num"<<block_num<<"parent"<<p.parent<<"pre"<<p.pre<<"suc"<<p.suc<<endl;
		if (b->cBlock[0]=='!')
		{
            p.leaf=true;
            num=(b->cBlock[1]-'0')*1000+(b->cBlock[2]-'0')*100+(b->cBlock[3]-'0')*10+b->cBlock[4]-'0';
            for(i=0;i<num;i++){
                //if (b[t].cBlock[10+i*6])
                switch (inform.type){
                	case 0:p.value.push_back(atoi(B.substr(10+i*11,6).c_str()));
                	result=(b->cBlock[5+i*11]-'0')*10000+(b->cBlock[6+i*11]-'0')*1000+(b->cBlock[7+i*11]-'0')*100+(b->cBlock[8+i*11]-'0')*10+(b->cBlock[9+i*11]-'0');
                	break;
                	case 1:p.value.push_back(atof(B.substr(10+i*15,10).c_str()));
                	result=(b->cBlock[5+i*15]-'0')*10000+(b->cBlock[6+i*15]-'0')*1000+(b->cBlock[7+i*15]-'0')*100+(b->cBlock[8+i*15]-'0')*10+(b->cBlock[9+i*15]-'0');
                		break;
                	case 2:p.value.push_back(char_int(B.substr(10+i*10,5).c_str()));
                	//cout<<B.substr(10+i*10,5).c_str()
                		result=(b->cBlock[5+i*10]-'0')*10000+(b->cBlock[6+i*10]-'0')*1000+(b->cBlock[7+i*10]-'0')*100+(b->cBlock[8+i*10]-'0')*10+(b->cBlock[9+i*10]-'0');
                		break;
                }
                
                p.record.push_back(result);
            }
		}
        else if (b->cBlock[0]=='?')
        {
            p.leaf=false;
            num=(b->cBlock[1]-'0')*1000+(b->cBlock[2]-'0')*100+(b->cBlock[3]-'0')*10+b->cBlock[4]-'0';
            for(i=0;i<num;i++){
                //if (b[t].cBlock[10+i*6])
                switch (inform.type){
                	case 0:p.value.push_back(atoi(B.substr(8+i*9,6).c_str()));
                	result=(b->cBlock[5+i*9]-'0')*100+(b->cBlock[6+i*9]-'0')*10+b->cBlock[7+i*9]-'0';
                	break;
                	case 1:p.value.push_back(atof(B.substr(8+i*13,10).c_str()));
                	result=(b->cBlock[5+i*13]-'0')*100+(b->cBlock[6+i*13]-'0')*10+b->cBlock[7+i*13]-'0';
                		break;
                	case 2:p.value.push_back(char_int(B.substr(8+i*8,5).c_str()));
                		result=(b->cBlock[5+i*8]-'0')*100+(b->cBlock[6+i*8]-'0')*10+b->cBlock[7+i*8]-'0';
                		break;
                }
                p.record.push_back(result);
            }
            switch (inform.type){
                	case 0:
                	result=(b->cBlock[5+i*9]-'0')*100+(b->cBlock[6+i*9]-'0')*10+b->cBlock[7+i*9]-'0';
                	break;
                	case 1:
                	result=(b->cBlock[5+i*13]-'0')*100+(b->cBlock[6+i*13]-'0')*10+b->cBlock[7+i*13]-'0';
                		break;
                	case 2:
                		result=(b->cBlock[5+i*8]-'0')*100+(b->cBlock[6+i*8]-'0')*10+b->cBlock[7+i*8]-'0';
                		break;
            }
            p.record.push_back(result);
        }
        else{
            cout<<"error"<<endl;
        }
	}
	return p;
}
int search_one(string database,string table_name,index_info& inform,int block_num,bufferInfo *run){
	int i,t=block_num;
	int num,result=0;
	float value;
	Node tmp=parseNode(database,table_name,inform,block_num,inform.type,run);
	num=tmp.value.size();
	switch (inform.type){
		case 0:value=*(int*)inform.value;break;
		case 1:value=*(float*)inform.value;break;
		case 2:value=char_int((char*)inform.value);break;
	}
	//cout<<strlen((char*)inform.value)<<endl;
	//cout<<"value"<<int_char((char_int((char*)inform.value))).c_str()<<endl;
	//cout<<"value"<<char_int((char*)inform.value)<<endl;
	//cout<<"node_num"<<tmp.node_num<<tmp.leaf<<endl;
	//value=*(float*)inform.value;
	//cout<<"record size"<<tmp.record.size()<<endl;
	if (tmp.leaf){
		t=tmp.node_num;
		for(i=0;i<tmp.value.size();i++){
			if (value==tmp.value[i])
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
			if (value<tmp.value[i])
			{
				return search_one(database,table_name,inform,tmp.record[i],run);
			}
		}
		//cout<<"I'm here"<<tmp.value[0]<<tmp.record[1]<<endl;
		return search_one(database,table_name,inform,tmp.record[i],run);
	}
}
float split_child(string database,string table_name,index_info& inform,Node& target,Node& new_target,bufferInfo *run){
	int i,j;
 	float mid;
	blockInfo *b;
	mid=target.value[capacity/2];
	b=readBlock(database, table_name,inform.index_name ,0, 1, run);
	int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
	inform.root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	new_target.node_num=new_block;
	sprintf(b->cBlock,"%03d%c%03d",++new_block,'#',inform.root);
	b->dirtyBit=1;
	writeBlock(database,b);
	new_target.pre=0;
	new_target.suc=0;
	new_target.parent=0;
 	//cout<<"mid"<<mid<<endl;
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
 		new_target.record.push_back(target.record.back()); 
 		for(j=capacity/2+1;j<capacity;j++){
 			target.record.pop_back();
 			target.value.pop_back();
 		}
		//new_target.record.push_back(target.record.back()); 
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
void insert_parent(string database,string table_name,index_info& inform,void *pp,int p,Node& lchild,Node& rchild,bufferInfo *run){
	int temp,j;
	Node tmp;
	Node change;
	float mid,a;
	blockInfo *b;
	a=*(float*)pp;
	//cout<<"a:"<<a<<endl;
	if (p==0){ 
		b=readBlock(database,table_name,inform.index_name,0, 1, run);
		int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
		inform.root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
		p=new_block;
		tmp.node_num=new_block;
		inform.root=p;
		sprintf(b->cBlock,"%03d%c%03d",++new_block,'#',inform.root);
		b->dirtyBit=1;
		writeBlock(database,b);
		tmp.leaf=false;
		tmp.suc=0;
		tmp.pre=0;
		tmp.parent=0;
 //p=b[root].nextNew;
 //root=p;
 //p->child_ptr[0]=lchild; 
		tmp.record.push_back(lchild.node_num);
 //p->child_ptr[1]=rchild;
		tmp.record.push_back(rchild.node_num);

 //lchild->parent=p;
		//b=readBlock("D_1", "Balance","account" ,lchild.node_num, 1, run);
		lchild.parent=p; 
 //rchild->parent=p;
		rchild.parent=p;
 //p->data[0]=a;
		tmp.value.push_back(a);
		encodeNode(database,table_name,inform,lchild,run);
 		encodeNode(database,table_name,inform,rchild,run); 	
 		encodeNode(database,table_name,inform,tmp,run);
 //p->n++;
 	}
 	else{
 		tmp=parseNode(database,table_name,inform,p,inform.type,run);
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
 		encodeNode(database,table_name,inform,lchild,run);
 		encodeNode(database,table_name,inform,rchild,run);
 		encodeNode(database,table_name,inform,tmp,run);
 		if (tmp.value.size()==capacity){
 			Node np=Node();
 			cout<<"The interior node of Index Split!"<<endl;
 			mid=split_child(database,table_name,inform,tmp,np,run);
 			insert_parent(database,table_name,inform,&mid,tmp.parent,tmp,np,run);
 			for (j=0;j<np.record.size();j++){
 				change=parseNode(database,table_name,inform,np.record[j],inform.type,run);
 				change.parent=np.node_num;
 				encodeNode(database,table_name,inform,change,run);
 			}
 //np=init();
 //temp=split_child(p,np);
 //insert_parent(temp,p->parent,p,np);
 		}
 	}
 	
}
void insert_one(string database,string table_name,index_info& inform,int block_num,int line_num,bufferInfo *run){
	int position;
	int j;
	blockInfo *b;
	float mid,k;
	b = readBlock(database, table_name,inform.index_name,block_num, 1, run);
	int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
	inform.root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	switch (inform.type){
		case 0:k=*(int*)inform.value;break;
		case 1:k=*(float*)inform.value;break;
		case 2:k=char_int((char*)inform.value);break;
	}
	//cout<<"k"<<k<<endl;
	if (new_block==inform.root){
		cout<<"Start to build index..."<<endl;
		//cout<<"root"<<inform.root<<endl;
		new_block++;
    	sprintf(b->cBlock,"%03d%c%03d",new_block,'#',inform.root);
		//cout<<"Block0:"<<b->cBlock<<endl;
		b->dirtyBit=1;
		writeBlock(database,b);
		b=readBlock(database, table_name,inform.index_name,inform.root, 1, run);
		strcpy(b->cBlock,"#");
		//cout<<"Block1:"<<b->cBlock<<endl;
		b->dirtyBit=1;
		writeBlock(database,b);
	}
	position=search_one(database,table_name,inform,inform.root,run);
	Node tmp=parseNode(database,table_name,inform,position,inform.type,run);
	b=readBlock(database, table_name,inform.index_name,position, 1, run);
	//cout<<"position"<<position<<' '<<b->cBlock<<endl;
	if (inform.offset>0){
		cout<<"index existed"<<endl;
		return;
	}
	else if (inform.offset==0)
	{
		int i=0;
        if (tmp.value.size()>0)
            while(k>tmp.value[i]&&i<tmp.value.size())
                i++;
        //cout<<"*********Insert here:"<<i<<"value i"<<tmp<<endl;
		tmp.value.push_back(0);
		tmp.record.push_back(0);
		for(j=tmp.value.size()-1;j>i;j--){
			tmp.value[j]=tmp.value[j-1];
			tmp.record[j]=tmp.record[j-1];
		}
		//cout<<"tmpvalue1"<<tmp.value[1]<<endl;
		tmp.value[i]=k;
		tmp.record[i]=line_num; 
		encodeNode(database,table_name,inform,tmp,run);
		if (tmp.value.size()>=capacity){
			Node np=Node();
			cout<<"Leaf Node Split"<<endl;
			mid=split_child(database,table_name,inform,tmp,np,run);
			//cout<<"mid"<<(int)mid<<endl;
			insert_parent(database,table_name,inform,&mid,tmp.parent,tmp,np,run);
		}
	}
    //cout<<tmp.leaf<<endl;
	//for(int i=0;i<tmp.value.size();i++)
	//	cout<<tmp.record[i]<<tmp.value[i]<<endl;
}
void delete_entry(string database,string table_name,index_info& inform,void* pp,int position,int offset,bufferInfo *run){
	int i;
	//char k;
	Node p;
	blockInfo *b;
	float k,mid;
	k=*(float*)pp;
	Node predessor,successor,parent,change;
	i=offset;
	p=parseNode(database,table_name,inform,position,inform.type,run);
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
	p.record.pop_back(); 
 	if (position==inform.root&&p.value.size()==0){
 		inform.root=p.record[0]; //free the pre-root
 		//cout<<"The new root is "<<inform.root<<endl;
 		b=readBlock(database,table_name,inform.index_name,0,1, run);
 		int new_block=(b->cBlock[0]-'0')*100+(b->cBlock[1]-'0')*10+(b->cBlock[2]-'0');
 		sprintf(b->cBlock,"%03d%c%03d",new_block,'#',inform.root);
 		b->dirtyBit=1;
 		writeBlock(database,b);
 		Node tmp=parseNode(database,table_name,inform,inform.root,inform.type,run);
 		tmp.leaf=false;  
 		//cout<<"preparent"<<tmp.parent<<endl;
 		tmp.parent=0;
 		tmp.suc=0;
 		tmp.pre=0;
 		//cout<<"newparent"<<tmp.parent<<endl;
 		encodeNode(database,table_name,inform,tmp,run);
 		return;
 	}
 	else if (position==inform.root&&p.value.size()==1){
 		encodeNode(database,table_name,inform,p,run);
 	}
 	else if (p.value.size()<capacity/2){ 
 		if (p.pre!=0)
 			predessor=parseNode(database,table_name,inform,p.pre,inform.type,run);
 		else 
 			predessor.parent=-1;
 		if (p.suc!=0)
 			successor=parseNode(database,table_name,inform,p.suc,inform.type,run);
 		else
 			successor.parent=-1;
 		if (predessor.parent==p.parent)
 			successor=p;
 		else if (successor.parent==p.parent)
 			predessor=p;
 		else{
 		
 			cout<<"The strcture of index file is wrong!"<<endl;
 			return ;
 		}
 		int t=0,temp;
 		parent=parseNode(database,table_name,inform,p.parent,inform.type,run);
 		while(parent.record[t]!=predessor.node_num&&t<parent.value.size())
 			t++;
 		k=parent.value[t]; 
 		if (predessor.value.size()+successor.value.size()<capacity){ 
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
 				for(i=0;i<successor.record.size();i++){
 					predessor.record.push_back(successor.record[i]);
 					//if (predessor.record.back()==0)
 					//	cout<<"there is an error"<<endl;
 					change=parseNode(database,table_name,inform,predessor.record.back(),inform.type,run);
 					change.parent=predessor.node_num;
 					encodeNode(database,table_name,inform,change,run);
 				}
 				for(i=0;i<successor.value.size();i++){
 					predessor.value.push_back(successor.value[i]);
 				}
			}// 
			predessor.suc=successor.suc;
			if (successor.suc!=0){
				Node next=parseNode(database,table_name,inform,successor.suc,inform.type,run);
				next.pre=predessor.node_num;
				encodeNode(database,table_name,inform,next,run); //
			}
			encodeNode(database,table_name,inform,predessor,run);
 			delete_entry(database,table_name,inform,&k,predessor.parent,t,run);
 			if (predessor.value.size()==capacity){
 				Node np=Node();
 				cout<<"The interior node Split!"<<endl;
 				mid=split_child(database,table_name,inform,predessor,np,run);
 				insert_parent(database,table_name,inform,&mid,predessor.parent,predessor,np,run);
 				//insert_parent(temp,predessor->parent,predessor,np);
 			}
			//delete_block(successor.node_num);
		}
		else{ //
			if (predessor.node_num==p.node_num) //
				if (!p.leaf){
					//cout<<"It's here"<<endl;
					//cout<<successor.value.front()<<endl;
 					predessor.value.push_back(k);
 					predessor.record.push_back(successor.record.front());
 					change=parseNode(database,table_name,inform,predessor.record.back(),inform.type,run);
 					change.parent=predessor.node_num;
 					encodeNode(database,table_name,inform,change,run);
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
			else if (successor.node_num==p.node_num){ //
 				if (!p.leaf){
 					successor.value.push_back(0);
 					successor.record.push_back(0);
 					for(i=successor.value.size()-1;i>0;i--)
 						successor.value[i]=successor.value[i-1];
 					for(i=successor.record.size()-1;i>0;i--)
 						successor.record[i]=successor.record[i-1];
 					successor.value[0]=k;
 					successor.record[0]=predessor.record.back();
 					change=parseNode(database,table_name,inform,successor.record.front(),inform.type,run);
 					change.parent=successor.node_num;
 					encodeNode(database,table_name,inform,change,run);
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
 		encodeNode(database,table_name,inform,predessor,run);
		encodeNode(database,table_name,inform,successor,run);
		//encodeNode(parent);
		}
	} //above redistribution
	else{
		encodeNode(database,table_name,inform,p,run);
	}
 // no need to change
}
void delete_one(string database,string table_name,index_info& inform,int block_num,bufferInfo *run){
	int position;
	Node p; //
	blockInfo *b;
	b = readBlock(database,table_name,inform.index_name,block_num, 1, run);
	inform.root=(b->cBlock[4]-'0')*100+(b->cBlock[5]-'0')*10+(b->cBlock[6]-'0');
	position=search_one(database,table_name,inform,inform.root,run);//
	//cout<<"position:"<<position<<endl;
	//cout<<"inform.offset:"<<inform.offset<<endl;
	delete_entry(database,table_name,inform,inform.value,position,inform.offset,run);
}
/*
int main(int argc, char const *argv[])
{	

	struct index_info index1;
	string table="Balance";
	string database="D_1";
	index1.index_name="account";
	index1.root=0;
	index1.type=2;
	char* a="jim";
	index1.value=a;
	//index1.type=0;
	index1.offset=1;
	//delete_one("database","table1",index1,0);
	//insert_one("database","table1",index1,0,index1.offset);
	insert_one(database,table,index1,0,index1.offset,run);
	char *b="kate";
	index1.value=b;
	index1.offset=2;
	insert_one(database,table,index1,0,index1.offset,run);
	char *c="john";
	index1.value=c;
	index1.offset=3;
	insert_one(database,table,index1,0,index1.offset,run);
	char *d="marry";
	index1.value=d;
	index1.offset=4;
	insert_one(database,table,index1,0,index1.offset,run);
	char*e="tom";
	index1.value=e;
	index1.offset=5;
	insert_one(database,table,index1,0,index1.offset,run);
	char *f="queen";
	index1.value=f;
	index1.offset=6;
	insert_one(database,table,index1,0,index1.offset,run);
	char *g="porry";
	index1.value=g;
	index1.offset=7;
	insert_one(database,table,index1,0,index1.offset,run);
	char *h="green";
	index1.value=h;
	index1.offset=8;
	insert_one(database,table,index1,0,index1.offset,run);
	while(1);
	//index1.offset=4;
	//index1.value='h';
	//insert_one("database","table1",index1,0,index1.offset);
	//while(1)
	//createIndex("database","table1","amount","index");
	//while(1);
	return 0;
}*/
