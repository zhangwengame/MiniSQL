#include "MiniSQL.h"
#include "Interpreter.h"
#include "API_Module.h"
#include "Buffer.h"

void main()
{
	string SQL;
	bufferInfo *run=new bufferInfo;
	cout<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<"\t\t             ��ӭʹ�� MiniSQL !"<<endl;
	cout<<"\t\t               Version (1.0)  "<<endl;
	cout<<"\t\t            "<<endl;      
	cout<<"\t\t        copyright(2005) all right reserved !"<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<endl<<endl;
	while(1)
	{
		cout<<"MiniSQL-->> ";
		SQL=Interpreter();
		API_Module(SQL);
	}
}

