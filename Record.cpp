#include "Record.h"
#include <iostream>
#include <cmath>
extern index_info  index_0;

void Insert_Item(string DB_Name,string Table_Name,string Attr,int & record_Num,bufferInfo *bufferInfo){
    char content[128];
    int start,i,bi,index,check=1,fint,index2;
    blockInfo *head;
    string one,first;
    attrInfo *ai;
    conditionInfo cond[10];
    attr_info print[32];
    float ffloat;
    //index_info  index_0;
    
    bi=record_Num/32;
    head=readBlock(DB_Name,Table_Name,"",bi,0,bufferInfo);
    //if (head->cBlock!=NULL)
       //cout<<Attr<<endl;
    start=128*record_Num-4096*bi;
    strncpy(content,Attr.c_str(),Attr.length());
    int leng=Attr.length();
    
    index=Attr.find(',');
    first=Attr.substr(index+1);
	index2 = first.find(',');
	first = first.substr(1,index2-2);
	first[0] = first[0] + 32;

	//cout << first << endl;
    fint=atoi(first.c_str());
    ffloat=atoi(first.c_str());
    i=1;
    while ((index>0)){
        one=Attr.substr(0,index);
        ai=getAttrInfo(DB_Name,Table_Name,i);
        if (ai==NULL) break;
        if (ai->pri==1){
            cond[0].left=ai->attrName;
            cond[0].symbol=0;
            cond[0].right0=atoi(one.c_str());
            cond[0].type=0;
            if (record_Num>0) Select_With_Where(DB_Name,Table_Name,cond,1,'a',print,0,1,bufferInfo,0,check);
            if (check==0){
                cout<<"error: It voilates the constrains of primary key!"<<endl;
                return;
            }
        }
        if (ai->pri==2){
            cond[0].left=ai->attrName;
            cond[0].symbol=0;
            if ((one[0]>'9')||(one[0]<'0'))
               strcpy(cond[0].right1,one.c_str());
            cond[0].type=1;
			//cout << "cond:"<<cond[0].right1 << endl;
            if (record_Num>0) Select_With_Where(DB_Name,Table_Name,cond,1,'a',print,0,1,bufferInfo,0,check);
            if (check==0){
                cout<<"error: It voilates the constrains of unique attributes!"<<endl;
                return;
            }
        }
        Attr=Attr.substr(index+1);
        index=Attr.find(',');
        i++;
    }
            
    for (i=leng;i<127;i++)
        content[i]=' ';
    content[127]=';';
    strncpy(head->cBlock+start,content,128);
    head->cBlock[start+128]='\0';
    //cout<<"block:"<<head->cBlock<<endl;
    record_Num++;
    
    if (Table_Name=="t1"){
       switch (bufferInfo->index_0.type){
	   case 0: bufferInfo->index_0.value = &fint;
              break;
	   case 1: bufferInfo->index_0.value = &ffloat;
              break;
	   case 2: bufferInfo->index_0.value =(char *)first.c_str();
              break;
       }
	   bufferInfo->index_0.offset = record_Num;
	   insert_one(DB_Name, Table_Name, bufferInfo->index_0, 0, bufferInfo->index_0.offset, bufferInfo);
    }
	cout << "Insert succeed!" << endl;
    writeBlock(DB_Name,head);
}

// 复合条件判断 
bool Confirm_To_Where(string DB_Name,string Table_Name,char *detail[10],conditionInfo conds[10],int count,char cond){
     int i;
     
     if ('a'==cond){                // represent and
         for (i=0;i<count;i++)
         switch (conds[i].type){
             case 0:
                 if (false==Confirmi(DB_Name,Table_Name,detail,conds[i]))
                    return false;
                 break;
             case 1:
                 if (false==Confirmc(DB_Name,Table_Name,detail,conds[i]))
                    return false;
                 break;
             case 2:
                 if (false==Confirmf(DB_Name,Table_Name,detail,conds[i]))
                    return false;
                 break;
             default:
                 if (conds[i].type<0)
                    conds[i].right0=atoi(detail[-conds[i].type]);
                 if (false==Confirmi(DB_Name,Table_Name,detail,conds[i]))
                    return false;
                 break;
         }
         return true;
	 }
     else{                        // represent or
         for (i=0;i<count;i++)
         switch (conds[i].type){
             case 0:
                 if (true==Confirmi(DB_Name,Table_Name,detail,conds[i]))
                    return true;
                 break;
             case 1:
                 if (true==Confirmc(DB_Name,Table_Name,detail,conds[i]))
                    return true;
                 break;
             case 2:
                 if (true==Confirmf(DB_Name,Table_Name,detail,conds[i]))
                    return true;
                 break;
             default:
                 if (conds[i].type<0)
                    conds[i].right0=atoi(detail[-conds[i].type]);
                 if (true==Confirmi(DB_Name,Table_Name,detail,conds[i]))
                    return true;
                 break;
         }
         return false;
     }  
}

// 整数的大小比较 
bool Confirmi(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition){
     int attr,ind,i;
     
     attr=attrOrder(DB_Name,Table_Name,condition.left);   
     if (attr>0)
     switch (condition.symbol){
         case -2: if (atoi(detail[attr])>=condition.right0)
                     return false;
                  break;
         case -1: if (atoi(detail[attr])>condition.right0)
                     return false;
                  break;
         case 0:  if (atoi(detail[attr])!=condition.right0)
                     return false;
                  break;
         case 1:  if (atoi(detail[attr])<condition.right0)
                     return false;
                  break;
         case 2:  if (atoi(detail[attr])<=condition.right0)
                     return false;
                  break;
         case 3:  if (atoi(detail[attr])==condition.right0)
                     return false;
                  break;
     }
     return true;
}

// 字符串的大小比较 
bool Confirmc(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition){
     int attr,ind,i;
     
     attr=attrOrder(DB_Name,Table_Name,condition.left);   
     if (attr>0)
     switch (condition.symbol){
	 case -2: if ((strcmp(detail[attr],condition.right1)>0)||(strcmp(detail[attr],condition.right1)==0))
                     return false;
                  break;
         case -1: if (strcmp(detail[attr],condition.right1)>0)
                     return false;
                  break;
         case 0:  if (strcmp(detail[attr],condition.right1)!=0)
                     return false;
                  break;
         case 1:  if (strcmp(detail[attr],condition.right1)<0)
                     return false;
                  break;
         case 2:  if ((strcmp(detail[attr],condition.right1)<0)||(strcmp(detail[attr],condition.right1)==0))
                     return false;
                  break;
         case 3:  if (strcmp(detail[attr],condition.right1)==0)
                     return false;
                  break;
     }
     return true;
}

// 实数的大小比较 
bool Confirmf(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition){
     int attr,ind,i;
     
     attr=attrOrder(DB_Name,Table_Name,condition.left);   
     if (attr>0)
     switch (condition.symbol){
         case -2: if (atoi(detail[attr])>=condition.right2)
                     return false;
                  break;
         case -1: if (atoi(detail[attr])>condition.right2)
                     return false;
                  break;
         case 0:  if (fabs(atoi(detail[attr])-condition.right2)>0.001)
                     return false;
                  break;
         case 1:  if (atoi(detail[attr])<condition.right2)
                     return false;
                  break;
         case 2:  if (atoi(detail[attr])<=condition.right2)
                     return false;
                  break;
		 case 3:  if (fabs(atoi(detail[attr]) - condition.right2)<0.001)
                     return false;
                  break;
     }
     return true;
}
        
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count,int all,bufferInfo* bufferInfo){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j,bi,li,lnum,en,record_Num;
    char *line,*detail[10],*elem,*line_c[500];
    char *lsplit=";",*esplit=",",*space=" ";
    char block[4096];
    
    for (i=0;i<count;i++)
        need[i]=print[i].num;
    
    record_Num=getRecordSum(DB_Name,Table_Name);
    for (bi=0;bi<=(int)(record_Num/32-0.01);bi++){
        head=readBlock(DB_Name,Table_Name,"",bi,0,bufferInfo);
        strcpy(block,head->cBlock);
        line=strtok(block,lsplit);
        li=0;
        while (line!=NULL){
              line_c[li]=line;
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li;
        for (li=0;li<lnum;li++){
            line_c[li]=strtok(line_c[li],space);
        }
        
        for (li=0;li<lnum;li++){
			if (line_c[li]==NULL) continue;
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            en=i-1;
            if (1==all)
                for (i=1;i<=en;i++)
                    printf("%s\t",detail[i]);
            else
                for (i=0;i<count;i++)
                    printf("%s\t",detail[need[i]]);
            printf("\n"); 
        }
    }
}

void Select_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,char cond,
                              attr_info print[32],int Count,int all,bufferInfo *bufferInfo,int use,int &check){
    blockInfo *head,*ptr;
    int need[10],i,j,bi,li,lnum,en,record_Num;
    char *line,*detail[10],*elem,*line_c[500];
    char block[4096];
    char *lsplit=";",*esplit=",",*space=" "; 
    
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    record_Num=getRecordSum(DB_Name,Table_Name);
    //cout<<record_Num<<endl;
    for (bi=0;bi<=(int)(record_Num/32-0.01);bi++){
        head=readBlock(DB_Name,Table_Name,"",bi,0,bufferInfo);
        strcpy(block,head->cBlock);
        line=strtok(block,lsplit);
        li=0;
        while (line!=NULL){
			line_c[li] = new char[130];
              strcpy(line_c[li],line);
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li;
        for (li=0;li<lnum;li++){
            line_c[li]=strtok(line_c[li],space);
        }
        
        for (li=0;li<lnum;li++){
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
				detail[i] = new char[20];
				strcpy(detail[i],elem);
                i++;
                elem=strtok(NULL,esplit);
            }
            en=i-1;
            if ((true==Confirm_To_Where(DB_Name,Table_Name,detail,conds,count,cond))&&(use==1)){ 
				if (1 == all){
					for (i = 1; i <= en; i++)
						printf("%s\t", detail[i]);
					//printf("%s\n", detail[1]);
					//printf("%s\n", conds[0].left.c_str());
				}
                else
                for (i=0;i<Count;i++)
                    printf("%s\t",detail[need[i]]);
                printf("\n");
            }
            else if ((true==Confirm_To_Where(DB_Name,Table_Name,detail,conds,count,cond))&&(use==0))
                 check=0;
        }
    } 
}

void Delete_No_Where(string DB_Name,string Table_Name,bufferInfo *bufferInfo){
    blockInfo *head;
    int i;
    //cout<<Table_Name<<endl;
    head=readBlock(DB_Name,Table_Name,"",0,0,bufferInfo);
    //cout<<head->cBlock<<endl;
    for (i=strlen(head->cBlock);i>0;i--)
        head->cBlock[i]='\0';
    head->cBlock[0]='\0';
	cout << "Delete succeed!" << endl;
    writeBlock(DB_Name,head);
}
    

void Delete_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],
                              int num,char cond,bufferInfo *bufferInfo){
    blockInfo *head;
    int i,j,bi,li,lnum;
    char *line,*detail[10],*elem,*line_c[100],content[4096],*line_b[100];
    char *lsplit=";",*esplit=",",*space=" ";
    
    head=readBlock(DB_Name,Table_Name,"",0,0,bufferInfo);
    for (bi=0;bi<1;bi++){
        strcpy(content,head->cBlock);
        line=strtok(content,lsplit);
        li=0;
        while (line!=NULL){
              line_c[li]=new char[130];
              strcpy(line_c[li],line);
              //cout<<li<<":"<<line_c[li]<<endl;
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li;
        for (li=0;li<lnum;li++){
            line_b[li]=new char[130];
            line_b[li]=strtok(line_c[li],space);
            //cout<<li<<":"<<(line_b[li]==NULL)<<endl;
        }
        
        for (li=0;li<lnum;li++){
            if (line_b[li]==NULL) {
                //cout<<"skip"<<endl;
                continue;
                }
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
			    
            if (true==Confirm_To_Where(DB_Name,Table_Name,detail,conds,count,'a')){
                cout<<li<<endl; 
                for (i=li*128;i<li*128+127;i++)
                    head->cBlock[i]=' ';
                head->cBlock[127]=';';
            }
        }
        writeBlock(DB_Name,head);
    } 
	cout << "Delete succeed!" << endl;
} 

/*int main(){
    attr_info print[32];
    attr_info *f;
    f=new attr_info;
    f->num=0;
    print[1]=*f;
    //createDatabase("D_1");
    //createTable("D_1", "Balance");
    addAttr("D_1", "Balance", "a", 8, 0, 1);
    addAttr("D_1", "Balance", "b", 0, 0, 0);
    Select_No_Where("D_1","Balance",print,1);
    printf("2333\n");
    while(1);
}*/
