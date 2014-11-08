#include "Record.h"
 
void Close_Database(string DB_Name,bool closetype);
void Close_File(string DB_Name,string filename,int filetype,bool closetype);
void Insert_Item(string DB_Name,string Table_Name,string Attr,int & record_Num);
void Print_Head(attr_info print[32],int count){
    int i;
    //for (i=0;i<count;i++)
        //printf("%s\t",print[i].name);
}

void Print_To_Screen(string record,attr_info print[32],int count);

void Select_Without_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,attr_info print[32],int Count,char cond){
    if (NULL==conds[0].left)
        Select_No_Where(DB_Name,Table_Name,print,count); 
    else
        Select_With_Where(DB_Name,Table_Name,conds,count,cond,print,Count);
}

void Select_With_Useful_Cond(string DB_Name,string Table_Name,conditionInfo conds[10],int count,attr_info print[32],int Count,char cond,int index);
void Select_With_Equal_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index);
void Select_With_Greater_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);
void Select_With_Smaller_Cond(string DB_Name,string Table_Name,conditionInfo conds[32],int count,attr_info print[32],int Count,char cond,index_info Index,int type);

bool Confirm_To_Where(string record,conditionInfo conds[10],int count,char cond){
     int i;
     if ('a'==cond){                // represent and
         for (i=0;i<count;i++)
         if (false==Confirm(record,conds[i]))
            return false;
         return true;
	 }
     else{                        // represent or
         for (i=0;i<count;i++)
         if (true==Confirm(record,conds[i]))
            return true;
         return false;
     }  
}

bool Confirm(string record,conditionInfo condition){
     int attr,i;
     char *detail[10],*elem,*record_c;
     
     strcpy(record_c,record.c_str());
     attr=condition.left->num;    
     elem=strtok(record_c,",");
     i=0;
     while (elem!=NULL){
         detail[i]=elem;
         i++;
         elem=strtok(NULL,",");
     }
     switch (condition.symbol){
         case -1: if (detail[attr][0]>=condition.right)
                     return false;
         case 0:  if (detail[attr][0]!=condition.right)
                     return false;
         case 1:  if (detail[attr][0]<=condition.right)
                     return false;
     }
     return true;
}
        
void Select_No_Where(string DB_Name,string Table_Name,attr_info print[32],int count){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j,bi,li,lnum;
    char *line,*detail[10],*elem,*line_c[100];
    char *lsplit=" ",*esplit=","; 
    
    for (i=0;i<count;i++)
        need[i]=print[i].num;
    
    bufferInfo *run;
	run= new bufferInfo;
    head=readBlock(DB_Name,Table_Name,0,0,run);
    for (bi=0;bi<1;bi++){
        line=strtok(head->cBlock,lsplit);
        li=0;
        while (line!=NULL){
              //printf("%s\n",line);
              line_c[li]=line;
              li++;
              line=strtok(NULL,lsplit);
        }
        lnum=li;
        
        for (li=0;li<lnum;li++){
            elem=strtok(line_c[li],esplit);
            i=1;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,esplit);
            }
            for (j=0;j<count;j++)
               printf("%s\t",detail[need[j]]);
        }
    } 
}

void Select_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,char cond,attr_info print[32],int Count){
    blockInfo *head,*ptr;
    fileInfo *file;
    int need[10],i,j;
    char *line,*detail[10],*elem,*line_c;
    
    for (i=0;i<Count;i++)
        need[i]=print[i].num;
    
    bufferInfo *run;
	run= new bufferInfo;
    file=getfile(DB_Name,Table_Name,0,run);
    head=file->firstBlock;
    for (ptr=head;ptr!=file->lastBlock;ptr=ptr->next){
        line=strtok(ptr->cBlock," ");
        while (line!=NULL){
            strcpy(line_c,line);
            elem=strtok(line_c,",");
            i=0;
            while (elem!=NULL){
                detail[i]=elem;
                i++;
                elem=strtok(NULL,",");
            }
            if (true==Confirm_To_Where(line,conds,count,cond))
            for (j=0;j<Count;j++)
               printf("%s\t",detail[need[j]]);
            line=strtok(NULL," ");
        } 
    }
}

void Delete_With_Where(string DB_Name,string Table_Name,conditionInfo conds[10],int count,index_info nodes[32],int num,char cond);
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
