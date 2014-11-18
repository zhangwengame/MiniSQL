#include "API_Module.h"

string DB_Name,Table_Name="t1";
attr_info print[32];
int record_Num=0; 

void API_Module(string SQL, bufferInfo* bufferInfo)
{
	string Type,Attr,Index_Name,Attr_Name,Condition,index_name[32],Cond_Info;
	string attr_list[10];
	char* asplit=".";
	int index1,index2,end,length,offset,type,count,Count,i;
	int attr_num[10],ind,index,num,primary,check;
	index_info nodes[32];
	conditionInfo conds[10];
	char cond,AO;
	bool ok;
	string conds_str[10];
	Type.assign(SQL,0,2);
	SQL.assign(SQL,2,SQL.length()-2);
    
    DB_Name=bufferInfo->currentDatabase;
	//--------------------------------------------------------------------------
	//创建数据库
	if(Type=="00")
		createDatabase(SQL);
	
	//--------------------------------------------------------------------------
	//创建数据表
	else if(Type=="01")
	{
        if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            }  
		else
		{
            index=SQL.find(',');
			Table_Name=SQL.substr(0,index);
			if (existTable(DB_Name,Table_Name)) {
                cout<<"error: This table has already existed!"<<endl;
                return;
            }
			Attr=SQL.substr(index+1,SQL.length()-index-1);
			//cout<<Table_Name<<" "<<Attr<<endl;
			createTable(DB_Name,Table_Name);
			index=Attr.find('.');
			while (index>0){
                if (Attr[0]=='!'){
                    primary=1;
                    Attr=Attr.substr(1);
                    index--;
                }
                else if (Attr[0]=='@'){
                    primary=2;
                    Attr=Attr.substr(1);
                    index--;
                }
                else primary=0;
                if (attrOrder(DB_Name,Table_Name,Attr)!=0) {
                    cout<<"error: You cannot create the same attribute again!"<<endl;
                    return;
                }
                //cout<<Table_Name<<" "<<Attr.substr(0,index-1)<<" "<<primary<<endl;
                //cout<<Attr[index-1]<<endl;
                switch (Attr[index-1]){
                    case 'i':
                        addAttr(DB_Name,Table_Name,Attr.substr(0,index-1),8,primary,0);
                        break;
                    case 'c':
                        addAttr(DB_Name,Table_Name,Attr.substr(0,index-1),8,primary,1);
                        break;
                    case 'f':
                        addAttr(DB_Name,Table_Name,Attr.substr(0,index-1),8,primary,2);
                        break;
                } 
                Attr=Attr.substr(index+1);
                index=Attr.find('.');
            }
		}		
	}

	//--------------------------------------------------------------------------
	//创建索引
	else if(Type=="02")
	{
		if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            } 
		else
		{
			index=SQL.find(',');
			Index_Name=SQL.substr(0,index);
			index++;
			SQL=SQL.substr(index);
			index=SQL.find(',');
			Table_Name=SQL.substr(0,index);
			Attr_Name=SQL.substr(index+1);
			//cout<<Table_Name<<" "<<Attr_Name<<" "<<Index_Name<<endl;
			createIndex(DB_Name,Table_Name,Attr_Name,Index_Name);
            //bufferInfo->index_0.type=(getAttrInfo(DB_Name,Table_Name,Attr_Name))->type;
			bufferInfo->index_0.type = 2;
			bufferInfo->index_0.index_name = Index_Name;
			bufferInfo->index_0.root = 0;
		}
	}
    //--------------------------------------------------------------------------
    //认定数据库
    else if(Type=="03")
	{
         if (DB_Name.length()>0)
             closeDatabase(DB_Name,bufferInfo);
         bufferInfo->currentDatabase=SQL;
    }
    
	//--------------------------------------------------------------------------
	//删除数据库
	else if(Type=="10")
	{
        DB_Name=SQL;
        if (!existDatabase(DB_Name)) {
            cout<<"This database doesn't exist!"<<endl;
            return;
        }
        closeDatabase(DB_Name,bufferInfo);
        dropDatabase(DB_Name);
	}

	//--------------------------------------------------------------------------
	//删除数据表
	else if(Type=="11")
	{
		if (!existDatabase(DB_Name)) {
            cout<<"This database doesn't exist!"<<endl;
            return;
        }
		else
		{
			Table_Name=SQL;
			closeFile(NULL,DB_Name,Table_Name,"",0,bufferInfo);
			dropTable(DB_Name,Table_Name);
		}
	}

	//--------------------------------------------------------------------------
	//删除索引
	else if(Type=="12")
	{
		if (!existDatabase(DB_Name)) {
            cout<<"This database doesn't exist!"<<endl;
            return;
        }
		else
		{
			Index_Name=SQL;
			closeFile(NULL,DB_Name,Table_Name,Index_Name,1,bufferInfo);
			dropIndex(DB_Name,Table_Name,Index_Name);
		}
	}

	//--------------------------------------------------------------------------
	//选择语句(无where子句)
	else if(Type=="20")
	{
        if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            } 
		else
		{
		    index1=SQL.find(',');
			Attr=SQL.substr(0,index1);
			index1++;
			Table_Name=SQL.substr(index1,SQL.length()-index1);
			if(Table_Name.find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				if(Attr=="*"){
					Select_No_Where(DB_Name,Table_Name,print,0,1,bufferInfo);
                }
				else {
                    count=0;
                    ind=Attr.find('.');
                    while (ind!=-1){
                       attr_list[count++]=Attr.substr(0,ind);
                       Attr=Attr.substr(ind+1,Attr.length()-ind-1);
                       ind=Attr.find('.');
                    }
                    attr_list[count++]=Attr;
                    for (i=0;i<count;i++)
                        print[i].num=attrOrder(DB_Name,Table_Name,attr_list[i]);
					Select_No_Where(DB_Name,Table_Name,print,count,0,bufferInfo);
                }
			}
		}		
	}

	//--------------------------------------------------------------------------
	//选择语句(有where子句)
	else if(Type=="21")
	{
		if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            } 
		else
		{
			index1=SQL.find(',');
			Attr=SQL.substr(0,index1);
			index1++;
			SQL=SQL.substr(index1,SQL.length()-index1);
			index2=SQL.find(',',0);
			Table_Name=SQL.substr(0,index2);
			index2++;
			Cond_Info=SQL.substr(index2,SQL.length()-index2);
			
			num=0;
			while ((Cond_Info.find('&')!=-1)||(Cond_Info.find('|')!=-1)){
                  index1=Cond_Info.find('&');
                  index2=Cond_Info.find('|');
                  if (index1>0){
                      AO='a';
                      conds_str[num++]=Cond_Info.substr(0,index1);
					  Cond_Info=Cond_Info.substr(index1+1);
                  }
                  else if (index2>0){
                      AO='o';
                      //cout<<index2<<endl;
                      conds_str[num++]=Cond_Info.substr(0,index2);
                      Cond_Info=Cond_Info.substr(index2+1);
                  } 
            }
			conds_str[num++]=Cond_Info;
            
            for (i=0;i<num;i++){
                conds[i]=Str_To_Conds(DB_Name,Table_Name,conds_str[i]);
				//cout<<conds[i].left<<" "<<conds[i].type<<" "<<conds[i].right1<<endl;
			}
            
			if(Table_Name.find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				if(Attr=="*"){
                    //cout<<"begin"<<endl;
                    Select_With_Where(DB_Name,Table_Name,conds,num,AO,print,0,1,bufferInfo,1,check);
                }
				else {
                    count=0;
                    ind=Attr.find('.');
                    while (ind!=-1){
                       attr_list[count++]=Attr.substr(0,ind);
                       Attr=Attr.substr(ind+1,Attr.length()-ind-1);
                       ind=Attr.find('.');
                    }
                    attr_list[count++]=Attr;
                    for (i=0;i<count;i++)
                        print[i].num=attrOrder(DB_Name,Table_Name,attr_list[i]);
					Select_With_Where(DB_Name,Table_Name,conds,num,AO,print,count,0,bufferInfo,1,check);
				}
			}		
		}
    }
    
    //--------------------------------------------------------------------------
    // 插入元组 
    else if (Type=="30")
        if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            } 
		else{
             index=SQL.find(',');
             Table_Name=SQL.substr(0,index);
             if (!existTable(DB_Name,Table_Name)) {
                 cout<<"error: This table doesn't exist!"<<endl;
                 return;
             } 
             Attr=SQL.substr(index+1);
             record_Num=getRecordSum(DB_Name,Table_Name);
             //cout<<record_Num<<endl;
             Insert_Item(DB_Name,Table_Name,Attr,record_Num,bufferInfo);
             setRecordSum(DB_Name,Table_Name,record_Num);
        }
        
    //--------------------------------------------------------------------------    
    // 无where的删除操作
    else if (Type=="40"){
         Table_Name=SQL;
         Delete_No_Where(DB_Name,Table_Name,bufferInfo);
    }
    
    //--------------------------------------------------------------------------
    // 有where的删除操作 
    else if (Type=="41")
        if (!existDatabase(DB_Name)) {
                 cout<<"error: This database doesn't exist!"<<endl;
            } 
		else{
            index=SQL.find(',');
            Table_Name=SQL.substr(0,index);
            if (!existTable(DB_Name,Table_Name)) {
                 cout<<"error: This table doesn't exist!"<<endl;
                 return;
            } 
            Cond_Info=SQL.substr(index+1);
            num=0;
			while ((Cond_Info.find('&')!=-1)||(Cond_Info.find('|')!=-1)){
                  index1=Cond_Info.find('&');
                  index2=Cond_Info.find('|');
                  if (index1>0){
                      AO='a';
                      conds_str[num++]=Cond_Info.substr(0,index1);
					  Cond_Info=Cond_Info.substr(index1+1);
                  }
                  else if (index2>0){
                      AO='o';
                      //cout<<index2<<endl;
                      conds_str[num++]=Cond_Info.substr(0,index2);
                      Cond_Info=Cond_Info.substr(index2+1);
                  } 
            }
			conds_str[num++]=Cond_Info;
			
            for (i=0;i<num;i++){
                conds[i]=Str_To_Conds(DB_Name,Table_Name,conds_str[i]);
				//cout<<conds[i].left<<" "<<conds[i].symbol<<" "<<conds[i].right0<<endl;
			}
			if(Table_Name.find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			    Delete_With_Where(DB_Name,Table_Name,conds,num,nodes,0,AO,bufferInfo);		
		}
    else cout<<"error: invalid type of insruction!"<<endl;
}

conditionInfo Str_To_Conds(string DB_Name,string Table_Name,string str){
    conditionInfo conds;
    attrInfo *ai;
    int index=-1;
    char c;
    
    conds.symbol=0;
    index=str.find('<');
    c=str[index+1];
    if (index>0){
        conds.left=str.substr(0,index);
        ai=getAttrInfo(DB_Name,Table_Name,conds.left);
        conds.type=ai->type;
        
        if (attrOrder(DB_Name,Table_Name,str.substr(index+1))>0)
            conds.type=-attrOrder(DB_Name,Table_Name,str.substr(index+1));
        if (attrOrder(DB_Name,Table_Name,str.substr(index+2))>0)
            conds.type=-attrOrder(DB_Name,Table_Name,str.substr(index+2));
        
        switch (conds.type){
            case 0:  
                 conds.right0=atoi((str.substr(index+2)).c_str());
                 break;
            case 1:
                 strcpy(conds.right1,(str.substr(index+2)).c_str());
                 break;
            case 2:
                 conds.right2=atoi((str.substr(index+2)).c_str());
                 break;
        }
        if (c=='=')
            conds.symbol=-1;
        else if (c=='>')
            conds.symbol=3;
        else {
            switch (conds.type){
            case 0:  
                 conds.right0=atoi((str.substr(index+1)).c_str());
                 break;
            case 1:
                 strcpy(conds.right1,(str.substr(index+1)).c_str());
                 break;
            case 2:
                 conds.right2=atoi((str.substr(index+1)).c_str());
                 break;
            }
            conds.symbol=-2;
        }
    }
    index=str.find('>');
    c=str[index+1];
    if ((index>0)&&(conds.symbol!=3)){
        conds.left=str.substr(0,index);
        ai=getAttrInfo(DB_Name,Table_Name,conds.left);
        conds.type=ai->type;
        
        if (attrOrder(DB_Name,Table_Name,str.substr(index+1))>0)
            conds.type=-attrOrder(DB_Name,Table_Name,str.substr(index+1));
        if (attrOrder(DB_Name,Table_Name,str.substr(index+2))>0)
            conds.type=-attrOrder(DB_Name,Table_Name,str.substr(index+2));
        
        switch (conds.type){
            case 0:  
                 conds.right0=atoi((str.substr(index+2)).c_str());
                 break;
            case 1:
                 strcpy(conds.right1,(str.substr(index+2)).c_str());
                 break;
            case 2:
                 conds.right2=atoi((str.substr(index+2)).c_str());
                 break;
        }
        if (c=='=')
            conds.symbol=1;
        else {
            switch (conds.type){
            case 0:  
                 conds.right0=atoi((str.substr(index+1)).c_str());
                 break;
            case 1:
                 strcpy(conds.right1,(str.substr(index+1)).c_str());
                 break;
            case 2:
                 conds.right2=atoi((str.substr(index+1)).c_str());
                 break;
            }
            conds.symbol=2;
        }
    }
    if (conds.symbol==0){
        index=str.find('=');
        conds.left=str.substr(0,index);
        ai=getAttrInfo(DB_Name,Table_Name,conds.left);
        conds.type=ai->type;
        switch (conds.type){
            case 0:  
                 conds.right0=atoi((str.substr(index+1)).c_str());
                 break;
            case 1:
                 strcpy(conds.right1,(str.substr(index+1)).c_str());
                 break;
            case 2:
                 conds.right2=atoi((str.substr(index+1)).c_str());
                 break;
            }
    }
    return conds;
}

/*
int main(){
    bufferInfo *run;
    run=new bufferInfo;
    int i;
    //addAttr("D_1", "Balance", "ele1", 8, 0, 1);
    //addAttr("D_1", "Balance", "ele2", 0, 0, 0);
    //addAttr("D_1", "Balance", "ele3", 0, 0, 0);
    //API_Module("21*,t1,age>25&age<30",run);
    //API_Module("00D_1",run);
    //for (i=1;i<=10;i++)
        //API_Module("30Balance,2,'naab',1",run);
    //createIndex("D_1","Balance","ele1","ele1");
    //API_Module("03D_1",run);
    //API_Module("01t1,!ele1i.@ele2c.ele3i.ele4f.",run);
    //API_Module("30t1,1,'Jim',20,2000.00",run);
    //API_Module("30t1,2,'Kate',24,1800.00",run);
    //API_Module("30t1,3,'John',34,4000.00",run);
    //API_Module("30t1,4,'John',34,4000.00",run);
    //API_Module("30Balance,15,'Jim',17",run);
	//API_Module("21*,Balance,ele1>1",run);
	//API_Module("21*,t1,ele1>1",run);
	//API_Module("40t1",run);
	//API_Module("41t1,ele1<3",run);
	//API_Module("20*,Balance",run);
	//API_Module("02Balance,ele1,ind1");
	//API_Module("10D_1");
	while (1);
}
*/

