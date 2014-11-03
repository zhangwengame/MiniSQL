#include <string>
#include "API_Module.h"
#include <iostream>
//#include "Record.h"
//#include "Catalog.h"
//#include"Index.h"
using namespace std;

string DB_Name="AB";
conditionInfo Str_To_Conds(string str);

void API_Module(string SQL)
{
	string Type,Attr,Index_Name,Table_Name,Attr_Name,Condition,index_name[32],Cond_Info;
	int index1,index2,end,length,offset,type,count,num,record_Num,Count,i;
	//index_info nodes[32];
	conditionInfo conds[10];
	//attr_info print[32];
	char cond,AO;
	bool ok;
	string conds_str[10];
	Type.assign(SQL,0,2);
	cout<<Type<<endl;
	SQL.assign(SQL,2,SQL.length()-2);
	cout<<SQL<<endl;

	/////////////////////////////////////////////////////////////////////////////////////////
	//�������ݿ�
	/*if(Type=="00")
		Create_Database(SQL);
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//�������ݱ�
	else if(Type=="01")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			index=SQL.Find(',');
			Table_Name=SQL.Left(index);
			Attr=SQL.Right(SQL.GetLength()-index-1);
			Attr_Name.Empty();
			//�������ݱ�
			Create_Table(Table_Name,Attr,DB_Name,Attr_Name);
			//�ж��Ƿ񴴽���������
			if(!Attr_Name.IsEmpty())
				Create_Index(Table_Name,Table_Name,Attr_Name,DB_Name,length,offset,type);
		}		
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//��������
	else if(Type=="02")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			index=SQL.Find(' ');
			//��ȡ������
			Index_Name=SQL.Left(index);
			index++;
			end=SQL.Find(' ',index);
			//��ȡ����
			Table_Name=SQL.Mid(index,end-index);
			//��ȡ������
			Attr=SQL.Right(SQL.GetLength()-end-1);
			Create_Index(Index_Name,Table_Name,Attr,DB_Name,length,offset,type);
			//�������������ڵ�
			if(length!=-1)
				Insert_Index_All(DB_Name,Table_Name,Index_Name,length,offset,type);			
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݿ�
	else if(Type=="10")
	{
		if(SQL==DB_Name)
		{
			//�ͷ����ݿ���ڴ�
			Close_Database(DB_Name,false);
			DB_Name.Empty();
		}
		Drop_Database(SQL);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݱ�
	else if(Type=="11")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Table_Name=SQL;
			//�ͷű���ڴ�
			Close_File(DB_Name,Table_Name,0,false);
			//ɾ�����ļ�
			Drop_Table(Table_Name,DB_Name,index_name,count);
			//�ͷű��������������ڴ�
			for(index=0;index<count;index++)
				Close_File(DB_Name,index_name[index],1,false);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ������
	else if(Type=="12")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			Index_Name=SQL;
			//ɾ���������ڴ�
			Close_File(DB_Name,Index_Name,1,false);
			Drop_Index(Index_Name,DB_Name);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ�)
	else if(Type=="20")
	{
		if(DB_Name.IsEmpty())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			//��ȡѡ��������
			index=SQL.Find(',');
			Attr=SQL.Left(index);
			index++;
			//��ȡ����
			Table_Name=SQL.Right(SQL.GetLength()-index);
			Table_Name=Table_Name.Left(Table_Name.GetLength()-1);
			if(Table_Name.Find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				//��ȡ��ʾ��¼��ʽ
				if(Attr=="*")
					Get_Attr_Info_All(DB_Name,Table_Name,print,count);
				else
					Get_Attr_Info(DB_Name,Table_Name,print,count,Attr);	
				if(count!=0)
					Select_No_Where(DB_Name,Table_Name,print,count);
			}
		}		
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ�)
	else if(Type=="21")*/
	if (1)
	{
		if(0==DB_Name.length())
			cout<<"error: you have not specified the database to be used!"<<endl;
		else
		{
			index1=SQL.find(',');
			Attr=SQL.substr(0,index1);
				cout<<Attr<<endl;
			index1++;
			SQL=SQL.substr(index1,SQL.length()-index1);
			index2=SQL.find(',',0);
			Table_Name=SQL.substr(0,index2);
			index2++;
			Cond_Info=SQL.substr(index2,SQL.length()-index2);
				cout<<Table_Name<<endl;
			
			num=0;
			while ((Cond_Info.find('&')!=-1)||(Cond_Info.find('|')!=-1))
                  if (Cond_Info.find('&')>0){
                      AO='a';
                      conds_str[num++]=Cond_Info.substr(0,Cond_Info.find('&'));
					  Cond_Info=Cond_Info.substr(Cond_Info.find('&')+1,Cond_Info.length()-Cond_Info.find('&')-1);
                  }
                  else if (Cond_Info.find('|')>0){
                      AO='o';
                      conds_str[num++]=Cond_Info.substr(0,Cond_Info.find('|'));
                  } 
			conds_str[num++]=Cond_Info;

            for (i=0;i<num;i++){
                conds[i]=Str_To_Conds(conds_str[i]);
				cout<<conds[i].left<<" "<<conds[i].symbol<<" "<<conds[i].right<<endl;
			}
            
			//Table_Name=Table_Name.Left(Table_Name.GetLength()-1);
			/*if(Table_Name.find(' ')!=-1)
				cout<<"error: can not select from more than one table!"<<endl;
			else
			{
				if(Attr=="*")
					Get_Attr_Info_All(DB_Name,Table_Name,print,count);
				else
					Get_Attr_Info(DB_Name,Table_Name,print,count,Attr);	
				if(count!=0)
					Select_No_Where(DB_Name,Table_Name,print,count);
					Select_With_Where(DB_Name,Table_Name,conds,num-1,AO,print,count)
			}*/
		}		
	}
	
}

conditionInfo Str_To_Conds(string str){
    conditionInfo conds;
    int index=-1;
    index=str.find('<');
    if (index>0){
        (conds.left).assign(str.substr(0,index));
        conds.symbol=-1;
        conds.right=(int)str[index+1]-48;
    }
    index=str.find('=');
    if (index>0){
        (conds.left).assign(str.substr(0,index));
        conds.symbol=0;
        conds.right=(int)str[index+1]-48;
    }
    index=str.find('>');
    if (index>0){
        (conds.left).assign(str.substr(0,index));
        conds.symbol=1;
        conds.right=(int)str[index+1]-48;
    }
    return conds;
}

int main(){
	API_Module("09ele1,table,a<0&b>0");
    while (1);
} 

