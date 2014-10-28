#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;
const int capacity=5; //最多四个键，5个就要分裂
struct BPTreeNode
{
	int *data;
	BPTreeNode **child_ptr;
	BPTreeNode *parent;
	BPTreeNode *next,*before;
	bool leaf;
	int n;
};
struct BPTreeNode *root=NULL,*np=NULL,*x=NULL;
//*np 存放新的点
BPTreeNode* init(){
	BPTreeNode* new_p;
	int i;
	new_p=new BPTreeNode;
	new_p->data=new int[capacity];
	new_p->child_ptr=new BPTreeNode*[capacity+1];
	new_p->leaf=true;
	new_p->parent=NULL;
	new_p->next=NULL;
	new_p->before=NULL;
	new_p->n=0;
	for(i=0;i<capacity+1;i++)
		new_p->child_ptr[i]=NULL;
	for(i=0;i<capacity;i++)
		new_p->data[i]=0;
	return new_p;
}
void Print(BPTreeNode* r){
	int i;
	//cout<<r->n<<endl;
	if (!r->leaf)
		for(i=0;i<r->n+1;i++){
			Print(r->child_ptr[i]);
			//cout<<r->child_ptr[i]<<endl;
			//cout<<r->data[i]<<" ";
		}
	for(i=0;i<capacity;i++)
		cout<<r->data[i]<<" ";
	cout<<endl;
	//cout<<"before"<<r->before<<"next"<<r->next<<"r"<<r<<endl; //before next pass
}
int split_child(BPTreeNode*target,BPTreeNode*new_target){
	int mid,j;
	//BPTreeNode *np;
	//new_target=init();
	//cout<<"in"<<endl;
	mid=target->data[capacity/2];
	//cout<<mid<<endl;
	//cout<<"init"<<endl;
	if (target->leaf)
		for (j = capacity/2; j < capacity; ++j)
		{
			new_target->data[j-capacity/2]=target->data[j];
			new_target->n++;
			target->data[j]=0;
			target->n--;
		}
	if (!target->leaf){
		for(j=capacity/2+1;j<capacity;j++){
			target->child_ptr[j]->parent=new_target;
			new_target->child_ptr[j-capacity/2-1]=target->child_ptr[j];
			new_target->data[j-capacity/2-1]=target->data[j];
			new_target->n++;
			target->data[j]=0;
			target->n--;
		}
		new_target->child_ptr[capacity/2]=target->child_ptr[capacity];
		target->child_ptr[capacity]->parent=new_target;
		new_target->leaf=false;
		target->n--;//mid位置也是0
		target->data[target->n]=0;
	}
	new_target->parent=target->parent;
	target->next=new_target;
	new_target->before=target;
	return mid;
}
void insert_parent(int a,BPTreeNode* p,BPTreeNode* lchild,BPTreeNode* rchild){
	int temp,j;
	if (p==NULL){ //出root
		p=init();
		p->leaf=false;
		root=p;
		p->child_ptr[0]=lchild; //做过根节点的都是false
		p->child_ptr[1]=rchild;
		lchild->parent=p;
		rchild->parent=p;
		p->data[0]=a;
		p->n++;
	}
	else{
		int i=0;
		while(a>p->data[i]&&i<p->n)
			i++;
		for(j=capacity-1;j>i;j--)
			p->data[j]=p->data[j-1];
		p->data[i]=a;
		p->child_ptr[i+1]=rchild;
		rchild->parent=p;
		p->n++;
		if (p->n==capacity){
			np=init();
			temp=split_child(p,np);
			insert_parent(temp,p->parent,p,np);
		}
	}
}
void insert(int a,BPTreeNode* r){
	int i,temp,j;
	if (r==NULL){
		root=init();
		r=root;
		r->data[0]=a;
		r->n++;
	}
	else{
		if (a<r->data[0]&&r->leaf!=true)
			insert(a,r->child_ptr[0]);
		else if (a>r->data[r->n-1]&&r->leaf!=true)
			insert(a,r->child_ptr[r->n]);
		else{		//leaf
			i=0;
			while(a>r->data[i]&&i<r->n)
				i++;
			if (r->leaf){
				for(j=capacity-1;j>i;j--)
					r->data[j]=r->data[j-1];
				/*for(j=i+1;j<capacity;j++)
					r->data[j]=r->data[j-1];*/
				r->data[i]=a;
				r->n++;
				if (r->n==capacity){
					np=init();
					//cout<<np<<endl;
					cout<<"Split"<<endl;
					temp=split_child(r,np);
					//cout<<"r->parent"<<r->parent<<"temp"<<temp<<endl;
					insert_parent(temp,r->parent,r,np);
				}
			}
			else insert(a,r->child_ptr[i]);
		}
	}
}

void delete_entry(int a,BPTreeNode* r){
	int i,k;
	BPTreeNode *parent;
	BPTreeNode *predessor,*successor;
	//delete a
	i=0;
	if (r==root&&r->n==1){
		root=r->child_ptr[0];
		root->parent=NULL;
		root->leaf=false;
		return;
	}
	while(r->data[i]<a&&i<r->n)
		i++;
	if (r->data[i]!=a)
		cout<<"error"<<endl;
	for(int j=i;j<r->n;j++)
		r->data[j]=r->data[j+1];
	r->n--;
	if (r->n<capacity/2){
		//cout<<"In this situation"<<endl;
		if (r->before->parent==r->parent){
			predessor=r->before;
			successor=r;
		}
		else if (r->next->parent==r->parent){
			predessor=r;
			successor=r->next;
		}
		else{
			cout<<"肯定有哪里错了，不可能只有一个叶子节点"<<endl;
			return ;
		}
		int t=0,temp;
		//cout<<successor<<endl;
		//cout<<r->parent->n<<endl;
		//cout<<r->parent->child_ptr[0]<<r->parent->child_ptr[1]<<r->parent->child_ptr[2]<<r->parent->child_ptr[3]<<endl;
		while(r->parent->child_ptr[t]!=predessor&&t<r->parent->n)
			t++;
		k=r->parent->data[t];
		//cout<<"k"<<k<<endl;
		//cout<<predessor->n<<" "<<successor->n<<endl;
		if (predessor->n+successor->n<capacity){
			if (r->leaf)
				for(i=0;i<successor->n;i++){
					predessor->data[predessor->n+i]=successor->data[i];
					predessor->n++;
					//successor->n--;
				}
			if (!r->leaf){ //!r->leaf
				predessor->data[predessor->n]=k;
				predessor->n++;
				//cout<<predessor->n<<endl;
				for(i=0;i<=successor->n;i++){
					predessor->child_ptr[predessor->n+i]=successor->child_ptr[i];
				}
				for(i=0;i<successor->n;i++){
					predessor->data[predessor->n+i]=successor->data[i];
					predessor->n++;
					//successor->n--;
				}
				//cout<<successor->child_ptr[1]->data[3]<<endl;
				//cout<<predessor->child_ptr[5]->data[2]<<endl;
				cout<<predessor->n<<endl;
				/*for(i=0;i<predessor->n;i++)
					cout<<predessor->data[i]<<" ";*/
			}
			predessor->next=successor->next;
			//cout<<"predessor"<<predessor->n<<"successor"<<successor->n<<endl;
			//cout<<predessor->data[0]<<predessor->data[1]<<predessor->data[2]<<endl;
			delete_entry(k,r->parent);
			if (predessor->n==capacity){
				np=init();
				cout<<"Split"<<endl;
				temp=split_child(predessor,np);
				cout<<temp<<endl;
				insert_parent(temp,predessor->parent,predessor,np);
			}
				// destroy successor
		}
		else{
			if (!r->leaf){
				for(i=successor->n;i>0;i--)
					successor->data[i]=successor->data[i-1];
				for(i=successor->n+1;i>0;i--)
					successor->child_ptr[i]=successor->child_ptr[i-1];
				successor->data[0]=k;
				successor->child_ptr[0]=predessor->child_ptr[predessor->n];
				predessor->child_ptr[predessor->n]=NULL;
				r->parent->data[t]=predessor->data[predessor->n-1];
				predessor->data[predessor->n-1]=0;
				predessor->n--;
				successor->n++;
			}
			else{ //r->leaf
				for(i=successor->n;i>0;i--)
					successor->data[i]=successor->data[i-1];
				successor->data[0]=predessor->data[predessor->n-1];
				r->parent->data[t]=predessor->data[predessor->n-1];
				predessor->data[predessor->n-1]=0;
				predessor->n--;
				successor->n++;
			}
		}
	} //above redistribution
// no need to change
}
void __delete__(int a,BPTreeNode* r){
	BPTreeNode* p=r;
	while(!p->leaf){
		if (a<p->data[0])
			p=p->child_ptr[0];
		else if (a>p->data[p->n-1])
			p=p->child_ptr[p->n];
		else{
			int i=0;
			while (a>p->data[i]&&i<p->n)
				i++;
			p=p->child_ptr[i];
		}
	}
	// find leaf
	//cout<<"p"<<p<<endl;
	delete_entry(a,p);
}
int main(int argc, char const *argv[])
{
	int i;
	
	insert(1,root);
	insert(2,root);
	insert(3,root);
	insert(4,root);
	insert(5,root);
	insert(6,root);
	insert(20,root);
	insert(21,root);
	insert(25,root);
	insert(26,root);
	insert(27,root);
	insert(28,root);
	insert(29,root);
	insert(7,root);
	insert(8,root);
	insert(9,root);
	//insert(10,root);
	//insert(11,root);
	//insert(12,root);
	__delete__(28,root);
	__delete__(29,root); //4 situations passed
	/*BPTreeNode *r=root->child_ptr[1];
	for (int i = 0; i < r->n+1; ++i)
	{
		cout<<r->child_ptr[i]->parent<<endl;
	}
	cout<<endl;*/
	//cout<<r->child_ptr[1]->parent<<endl;
	//cout<<root->leaf<<endl;
	 //Test 1
	//cout<<root->child_ptr[0]->parent<<" root"<<root<<endl; 
	Print(root);
	return 0;
}