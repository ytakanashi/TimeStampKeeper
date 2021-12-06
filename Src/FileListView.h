﻿//FileListView.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.03 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF
#define _FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF



#include"resources/resource.h"


class FileListView:public sslib::ListView{
public:
	FileListView(HWND parent_handle):
		ListView(parent_handle,IDC_LIST1,true){}
	~FileListView(){}
public:
	enum ITEM_STATE{
		STATE_INIT=0,
		STATE_ERROR=-1,
		STATE_COMPLETE=1
	};
private:
	LRESULT onNotify(WPARAM wparam,LPARAM lparam);
	LRESULT onMessage(UINT message,WPARAM wparam,LPARAM lparam);
};

#endif //_FILELISTVIEW_H_9532C8A3_060F_4e6a_A6CA_D24884AB99EF
