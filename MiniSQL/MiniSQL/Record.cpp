#include <Record.h> 
void Close_Database(CString DB_Name,bool closetype);
void Close_File(CString DB_Name,CString filename,int filetype,bool closetype);
void Insert_Item(CString DB_Name,CString Table_Name,CString Attr,int & record_Num);
void Print_Head(attr_info print[32],int count){
     int i
     for (i=0;i<count;i++)
         printf("%s\t",print[i].name);
}

void Print_To_Screen(CString record,attr_info print[32],int count);
void Select_Without_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond){
    blockInfo *head,*ptr;
    fileInfo *file;
    int attr[10],need[10],i,j,k;
    const char *lsplit=" ",*esplit=",";
    char *line,*elem;
    char detail[10][20];
    bool flag;
    
    for (i=0;i<count;i++)
        attr[i]=conds[i].left->num;
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    
    file=get_file_info(DB_Name,Table_Name,0,bufferInfo);
    head=file->firstBlock;
    for (ptr=head;ptr!=file->lastBlock;ptr=ptr->next){
        elem=strtok(ptr->cBlock,esplit);
        i=0;
        while (elem!=NULL){
            detail[i]=elem;
            i++;
            elem=strtok(NULL,split);
        }
        flag=true;
        for (j=0;j<count;j++)
            if (!cond_judge(detail[attr[j]],conds[j].symbol,conds[j].right))
               flag=false;
        if (flag){
           for (j=0;j<Count;j++)
               print("%s\t",detail[need[j]]);
        }        
}
void Select_With_Useful_Cond(CString DB_Name,CString Table_Name,condition_info conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(CString DB_Name,CString Table_Name,condition_info conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
bool Confirm_To_Where(CString record,condition_info conds[10],int count,char cond);
bool Confirm(CString record,condition_info condition);
void Select_No_Where(CString DB_Name,CString Table_Name,attr_info print[32],int count);
void Select_With_Where(CString DB_Name,CString Table_Name,condition_info conds[10],int count,char cond,attr_info print[32],int Count);
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
