#include "Record.h"
#include <iostream>
 
//void Close_Database(string DB_Name,bool closetype);
//void Close_File(string DB_Name,string filename,int filetype,bool closetype);
void Insert_Item(string DB_Name,string Table_Name,string Attr,int & record_Num){
    bufferInfo *run;
    char content[128];
    int start,i;
    blockInfo *head;
    
	run= new bufferInfo;
    head=readBlock(DB_Name,Table_Name,"",0,0,run);
    if (head->cBlock!=NULL)
       cout<<Attr<<endl;
    start=128*record_Num;
    strncpy(content,Attr.c_str(),Attr.length());
    for (i=Attr.length();i<127;i++)
        content[i]=' ';
    content[127]=';';
    strncpy(head->cBlock+start,content,128);
    cout<<strlen(head->cBlock)<<endl;
    record_Num++;
    writeBlock(DB_Name,head);
}
     
void Print_Head(attr_info print[32],int count){
    int i;
    //for (i=0;i<count;i++)
        //printf("%s\t",print[i].name);
}

void Print_To_Screen(string record,attr_info print[32],int count);
void Select_With_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);

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
         }
         return false;
     }  
}

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

bool Confirmc(string DB_Name,string Table_Name,char *detail[10],conditionInfo condition){
     int attr,ind,i;
     
     attr=attrOrder(DB_Name,Table_Name,condition.left);   
     if (attr>0)
     switch (condition.symbol){
         case -2: if ((strcmp(detail[attr],condition.right1)>0)&&(strcmp(detail[attr],condition.right1)==0))
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
         case 2:  if ((strcmp(detail[attr],condition.right1)<0)&&(strcmp(detail[attr],condition.right1)==0))
                     return false;
                  break;
         case 3:  if (strcmp(detail[attr],condition.right1)==0)
                     return false;
                  break;
     }
     return true;
}

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
         case 0:  if (atoi(detail[attr])!=condition.right2)
                     return false;
                  break;
         case 1:  if (atoi(detail[attr])<condition.right2)
                     return false;
                  break;
         case 2:  if (atoi(detail[attr])<=condition.right2)
                     return false;
                  break;
         case 3:  if (atoi(detail[attr])==condition.right0)
                     return false;
                  break;
     }
     return true;
}
        
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count,int all,bufferInfo* bufferInfo){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j,bi,li,lnum;
    char *line,*detail[10],*elem,*line_c[100];
    char *lsplit=";",*esplit=",",*space=" ";
    
    for (i=0;i<count;i++)
        need[i]=print[i].num;
    
    head=readBlock(DB_Name,Table_Name,"",0,0,bufferInfo); 
    for (bi=0;bi<1;bi++){
        line=strtok(head->cBlock,lsplit);
        li=0;
        while (line!=NULL){
              line_c[li]=line;
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li-1;
        for (li=0;li<lnum;li++){
            line_c[li]=strtok(line_c[li],space);
        }
        
        for (li=0;li<lnum;li++){
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            
            if (1==all)
                for (i=1;detail[i+2]!=NULL;i++)
                    printf("%s\t",detail[i]);
            else
                for (i=0;i<count;i++)
                    printf("%s\t",detail[need[i]]);
            printf("\n"); 
        }
    }
}

void Select_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,char cond,
                              attr_info print[32],int Count,int all,bufferInfo *bufferInfo){
    blockInfo *head,*ptr;
    int need[10],i,j,bi,li,lnum;
    char *line,*detail[10],*elem,*line_c[100];
    char *lsplit=";",*esplit=",",*space=" "; 
    
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    head=readBlock(DB_Name,Table_Name,"",0,0,bufferInfo);
    for (bi=0;bi<1;bi++){
        line=strtok(head->cBlock,lsplit);
        li=0;
        while (line!=NULL){
              line_c[li]=line;
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li-1;
        for (li=0;li<lnum;li++){
            line_c[li]=strtok(line_c[li],space);
        }
        
        for (li=0;li<lnum;li++){
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            if (true==Confirm_To_Where(DB_Name,Table_Name,detail,conds,count,cond)){ 
                if (1==all)
                for (i=1;detail[i+3]!=NULL;i++)
                    printf("%s\t",detail[i]);
                else
                for (i=0;i<Count;i++)
                    printf("%s\t",detail[need[i]]);
            }
        }
    } 
}

void Delete_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],
                              int num,char cond,bufferInfo *bufferInfo){
    blockInfo *head;
    int i,j,bi,li,lnum;
    char *line,*detail[10],*elem,*line_c[100],content[4096];
    char *lsplit=";",*esplit=",",*space=" ";
    
    head=readBlock(DB_Name,Table_Name,"",0,0,bufferInfo);
    for (bi=0;bi<1;bi++){
        strcpy(content,head->cBlock);
        line=strtok(content,lsplit);
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
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
			    
            if (true==Confirm_To_Where(DB_Name,Table_Name,detail,conds,count,'a')){
                cout<<li<<endl; 
                for (i=li*128;i<li*128+128;i++)
                    head->cBlock[i]=' ';
            }
        }
        writeBlock(DB_Name,head);
    } 
} 
void Delete_Without_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_With_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,int index);
void Delete_With_Equal_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,index_info Index);
void Delete_With_Greater_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
bool Find_Useful_Cond(conditionInfo conds[10],int count,int & index);
void Insert_Index_All(string DB_Name,string Table_Name,string Index_Name,int length,int offset,int type);
bool Verify_Insertable(string DB_Name,string Table_Name,index_info nodes[32],int count,string Attr);
void Quit(string DB_Name);

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
