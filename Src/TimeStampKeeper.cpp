//TimeStampKeeper.cpp
//TimeStampKeeperメイン

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.03 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"TimeStampKeeper.h"
#include"MainDialog.h"


using namespace sslib;



MAIN_INSTANCE(TimeStampKeeper);



bool TimeStampKeeper::init(){
	return true;
}

void TimeStampKeeper::cleanup(){
}

bool TimeStampKeeper::run(CommandArgument& cmd_arg,int cmd_show){
	if(cmd_arg.filepaths().size()){
		HWND wnd=::FindWindow(_T("#32770"),
							  (tstring(_T("TimeStamp Keeper Ver."))+SOFTWARE_VERSION).c_str());
		if(wnd){
			for(std::vector<tstring>::size_type i=0,size=cmd_arg.filepaths().size();i<size;++i){
				COPYDATASTRUCT copy_data={};
				std::vector<TCHAR> buffer(cmd_arg.filepaths()[i].length()+1);
				lstrcpy(&buffer[0],cmd_arg.filepaths()[i].c_str());

				copy_data.dwData=1;
				copy_data.cbData=(cmd_arg.filepaths()[i].length()+1)*sizeof(TCHAR);
				copy_data.lpData=(LPTSTR)&buffer[0];
				::SendMessage(wnd,WM_COPYDATA,NULL,(LPARAM)&copy_data);
			}

			if(!(::GetAsyncKeyState(VK_CONTROL)&0x8000)){
				::SetForegroundWindow(wnd);
			}
			return true;
		}
	}

	MainDialog dlg;

	dlg.setParam(&cmd_arg);
	dlg.doModal();

	return true;
}
