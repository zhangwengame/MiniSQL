#include "Record.h"
#include <string.h>
 
void Close_Database(CString DB_Name,bool closetype);
void Close_File(CString DB_Name,CString filename,int filetype,bool closetype);
void Insert_Item(CString DB_Name,CString Table_Name,CString Attr,int & record_Num);
void Print_Head(attr_info print[32],int count){
    int i;
    for (i=0;i<count;i++)
        printf("%s\t",print[i].name);
}

void Print_To_Screen(CString record,attr_info print[32],int count);

void Select_Without_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond){
    if (NULL==conds[0].left)
        Select_No_Where(DB_Name,Table_Name,print,count); 
    else
        Select_With_Where(DB_Name,Table_Name,conds,count,cond,print,Count);
}

void Select_With_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);

bool Confirm_To_Where(CString record,condition_info conds[10],int count,char cond){
     int i;
     if ('a'==cond){                // represent and
         for (i=0;i<count;i++)
         if (false==Confirm(record,conds[i]))
            return false;
         return true;
     else{                        // represent or
         for (i=0;i<count;i++)
         if (true==Confirm(record,conds[i]))
            return true;
         return false;
     }  
}

bool Confirm(CString record,condition_info condition){
     int attr,i;
     char *elem;
     
     attr=condition.left->num;    
     elem=strtok(record,esplit);
     i=0;
     while (elem!=NULL){
         detail[i]=elem;
         i++;
         elem=strtok(NULL,esplit);
     }
     switch (condition.symbol){
         case -1: if (detail[attr]>=condition.right)
                     return false;
         case 0:  if (detail[attr]!=condition.right)
                     return false;
         case 1:  if (detail[attr]<=condition.right)
                     return false;
     }
     return true;
}
        
void Select_No_Where(CString DB_Name,CString Table_Name,attr_info print[32],int count){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j;
    char *line,detail[10][20];
    
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    
    file=get_file_info(DB_Name,Table_Name,0,bufferInfo);
    head=file->firstBlock;
    for (ptr=head;ptr!=file->lastBlock;ptr=ptr->next){
        line=strtok(ptr->block,lsplit);
        while (line!=NULL){
            elem=strtok(record,esplit);
            i=0;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            for (j=0;j<Count;j++)
               printf("%s\t",detail[need[j]]);
            line=strtok(NULL,lsplit);
        } 
    }
}

void Select_With_Where(CString DB_Name,CString Table_Name,condition_info conds[10],int count,char cond,attr_info print[32],int Count){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j;
    char *line,detail[10][20];
    
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    
    file=get_file_info(DB_Name,Table_Name,0,bufferInfo);
    head=file->firstBlock;
    for (ptr=head;ptr!=file->lastBlock;ptr=ptr->next){
        line=strtok(ptr->block,lsplit);
        while (line!=NULL){
            elem=strtok(record,esplit);
            i=0;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            if (true==Confirm_To_Where(line,conds,count,cond))
            for (j=0;j<Count;j++)
               printf("%s\t",detail[need[j]]);
            line=strtok(NULL,lsplit);
        } 
    }
}

void Delete_With_Where(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_Without_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond);
void Delete_With_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,int index);
void Delete_With_Equal_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index);
void Delete_With_Greater_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,index_info nodes[32],int num,char cond,index_info Index,int type);
bool Find_Useful_Cond(condition_info conds[10],int count,int & index);
void Insert_Index_All(CString DB_Name,CString Table_Name,CString Index_Name,int length,int offset,int type);
bool Verify_Insertable(CString DB_Name,CString Table_Name,index_info nodes[32],int count,CString Attr);
void Quit(CString DB_Name);
