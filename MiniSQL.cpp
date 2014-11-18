#include "MiniSQL.h"

void main()
{
	string SQL;
	bufferInfo *run=new bufferInfo;
	cout<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<"\t\t             »¶Ó­Ê¹ÓÃ MiniSQL !"<<endl;
	cout<<"\t\t               Version (1.0)  "<<endl;
	cout<<"\t\t            "<<endl;      
	cout<<"\t\t        copyright(2014) all right reserved !"<<endl;
	cout<<"\t\t***********************************************"<<endl;
	cout<<endl<<endl;
	while(1)
	{
		cout<<"MiniSQL--";
		SQL=Interpreter();
		if (SQL == "99")
		{
			quitProg(run->currentDatabase, run);
			break;
		}
		API_Module(SQL,run);
	}
}

