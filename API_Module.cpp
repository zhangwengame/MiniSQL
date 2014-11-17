#include "API_Module.h"

string DB_Name="D_1",Table_Name="Balance";
attr_info print[32];
int record_Num=0; 

void API_Module(string SQL, bufferInfo* bufferInfo)
{
	string Type,Attr,Index_Name,Attr_Name,Condition,index_name[32],Cond_Info;
	string attr_list[10];
	char* asplit=".";
	int index1,index2,end,length,offset,type,count,Count,i;
	int attr_num[10],ind,index,num,primary;
	index_info nodes[32];
	conditionInfo conds[10];
	//attr_info print[32];
	char cond,AO;
	bool ok;
	string conds_str[10];
	Type.assign(SQL,0,2);
	SQL.assign(SQL,2,SQL.length()-2);

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
			cout<<Table_Name<<" "<<Attr<<endl;
			createTable(DB_Name,Table_Name);
			index=Attr.find('.');
			while (index>0){
                if (Attr[0]=='!'){
                    primary=1;
                    Attr=Attr.substr(1);
                    index--;
                }
                if (Attr[0]=='@'){
                    primary=1;
                    Attr=Attr.substr(1);
                    index--;
                }
                else primary=0;
                if (attrOrder(DB_Name,Table_Name,Attr)!=0) {
                    cout<<"error: You cannot create the same attribute again!"<<endl;
                    return;
                }
                cout<<Table_Name<<" "<<Attr.substr(0,index-1)<<" "<<primary<<endl;
                cout<<Attr[index-1]<<endl;
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
			//判断是否创建主键索引
			/*if(!Attr_Name.IsEmpty()) 
				Create_Index(Table_Name,Table_Name,Attr_Name,DB_Name,length,offset,type);*/
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
			cout<<Table_Name<<" "<<Attr_Name<<" "<<Index_Name<<endl;
			createIndex(DB_Name,Table_Name,Attr_Name,Index_Name);	
		}
	}

	//--------------------------------------------------------------------------
	//删除数据库
	else if(Type=="10")
	{
        if (!existDatabase(DB_Name)) {
            cout<<"This database doesn't exist!"<<endl;
            return;
        }
        closeDatabase(DB_Name,bufferInfo);
        if (SQL==DB_Name)
           dropDatabase(DB_Name);
	}

	//--------------------------------------------------------------------------
	//删除数据表
	/*else if(Type=="11")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Table_Name=SQL;
			closeFile(DB_Name,Table_Name,0,false);
			Drop_Table(Table_Name,DB_Name,index_name,count);
			for(index=0;index<count;index++)
				Close_File(DB_Name,index_name[index],1,false);
		}
	}

	//--------------------------------------------------------------------------
	//删除索引
	else if(Type=="12")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Index_Name=SQL;
			//删除索引的内存
			Close_File(DB_Name,Index_Name,1,false);
			Drop_Index(Index_Name,DB_Name);
		}
	}*/

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
                      cout<<index2<<endl;
                      conds_str[num++]=Cond_Info.substr(0,index2);
                      Cond_Info=Cond_Info.substr(index2+1);
                  } 
            }
			conds_str[num++]=Cond_Info;
            
            for (i=0;i<num;i++){
                conds[i]=Str_To_Conds(DB_Name,Table_Name,conds_str[i]);
				cout<<conds[i].left<<" "<<conds[i].type<<" "<<conds[i].right1<<endl;
			}
            
			if(Table_Name.find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				if(Attr=="*"){
                    Select_With_Where(DB_Name,Table_Name,conds,num,AO,print,0,1,bufferInfo);
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
					Select_With_Where(DB_Name,Table_Name,conds,num,AO,print,count,0,bufferInfo);
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
             Insert_Item(DB_Name,Table_Name,Attr,record_Num);
        }
        
    //--------------------------------------------------------------------------
    // 有where的删除操作 
    else if (Type=="40")
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
                      cout<<index2<<endl;
                      conds_str[num++]=Cond_Info.substr(0,index2);
                      Cond_Info=Cond_Info.substr(index2+1);
                  } 
            }
			conds_str[num++]=Cond_Info;
			
            for (i=0;i<num;i++){
                conds[i]=Str_To_Conds(DB_Name,Table_Name,conds_str[i]);
				cout<<conds[i].left<<" "<<conds[i].symbol<<" "<<conds[i].right0<<endl;
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

int main(){
    bufferInfo *run;
    run=new bufferInfo;
    //addAttr("D_1", "Balance", "ele1", 8, 0, 1);
    //addAttr("D_1", "Balance", "ele2", 0, 0, 0);
    //addAttr("D_1", "Balance", "ele3", 0, 0, 0);
    //API_Module("20name.salary,t1",run);
    API_Module("21name.salary,t1",run);
    //API_Module("01Balance,ele1.ele2.ele3");
    //API_Module("30Balance,11,22,33");
    //API_Module("30Balance,14,15,16");
	//API_Module("21*,Balance,ele1<ele2&ele3>34",run);
	//API_Module("40Balance,ele1>0&elem2>0");
	//API_Module("20*,Balance",run);
	//API_Module("02Balance,ele1,ind1");
	//API_Module("10D_1");
	while (1);
}


