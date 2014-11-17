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
	cout<<"\t\t             »¶Ó­Ê¹ÓÃ MiniSQL !"<<endl;
	cout<<"\t\t               Version (1.0)  "<<endl;
	cout<<"\t\t            "<<endl;      
	cout<<"\t\t        copyright(2005) all right reserved !"<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<endl<<endl;
	while(1)
	{
		cout<<"MiniSQL-->> ";
		SQL=Interpreter();
		cout << "main" << SQL << endl;
		API_Module(SQL,run);
	}
	//createDatabase("D_1");
	/*API_Module("01Balance,ele1.ele2.ele3",run);
	API_Module("30Balance,11,22,33", run);
	API_Module("30Balance,14,15,16", run);*/
}

