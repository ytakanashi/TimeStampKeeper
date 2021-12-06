//FileListView.cpp
//ファイルリストビュー

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.03 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"FileListView.h"


using namespace sslib;


LRESULT FileListView::onNotify(WPARAM wparam,LPARAM lparam){
	static HWND edit_handle=NULL;

	switch(((LPNMHDR)lparam)->code){
		case LVN_ITEMCHANGED:
			if((((LPNMLISTVIEW)lparam)->uNewState&LVIS_STATEIMAGEMASK)!=
			   (((LPNMLISTVIEW)lparam)->uOldState&LVIS_STATEIMAGEMASK)){
				if((((LPNMLISTVIEW)lparam)->uNewState&LVIS_STATEIMAGEMASK)==INDEXTOSTATEIMAGEMASK(1)){
					setItemData(STATE_INIT,((LPNMLISTVIEW)lparam)->iItem);
				}
			}
			break;

		case NM_CUSTOMDRAW:{
			LPNMLVCUSTOMDRAW lv_custom_draw=(LPNMLVCUSTOMDRAW)lparam;

			if(lv_custom_draw->nmcd.dwDrawStage==CDDS_PREPAINT){
				::SetWindowLong(parent_handle(),DWL_MSGRESULT,CDRF_NOTIFYITEMDRAW);
			}
			if(lv_custom_draw->nmcd.dwDrawStage==CDDS_ITEMPREPAINT){
				if(getCheckState(lv_custom_draw->nmcd.dwItemSpec)){
					switch(lv_custom_draw->nmcd.lItemlParam){
						case STATE_COMPLETE:
							lv_custom_draw->clrTextBk=RGB(203,255,203);
							break;
						case STATE_ERROR:
							lv_custom_draw->clrTextBk=RGB(255,203,203);
							break;
						default:
							break;
					}
					::SetWindowLong(parent_handle(),DWL_MSGRESULT,CDRF_NEWFONT);
				}
			}
			return CDRF_DODEFAULT;
		}

		case LVN_KEYDOWN:
			if(((LPNMLVKEYDOWN)lparam)->wVKey==VK_F2){
				SendMessage(GetParent(handle()),WM_COMMAND,MAKEWPARAM(IDM_MENU_EDIT,0),0);
			}else if(((LPNMLVKEYDOWN)lparam)->wVKey==VK_SPACE){
				for(int item=-1;;){
					if((item=getNextItem(item))==-1)break;
					//フォーカスがあるアイテムは自動で反転してくれる
					if(!getItemState(item,LVIS_FOCUSED)){
						setCheckState(item,!getCheckState(item));
					}
				}
			}
			break;

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),WM_NOTIFY,wparam,lparam);
}

LRESULT FileListView::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_DROPFILES:
			//親に送る
			::PostMessage(parent_handle(),WM_DROPFILES,wparam,lparam);
			break;

		case WM_KEYDOWN:
			switch(wparam){
				case VK_DELETE:{
					//選択しているアイテムを削除
					for(int item=-1;(item=getNextItem())!=-1;){
						deleteItem(item);
					}
					break;
				}

				case 'A':
					if(::GetAsyncKeyState(VK_CONTROL)<0&&
					   ::GetAsyncKeyState(VK_SHIFT)>=0&&
					   ::GetAsyncKeyState(VK_MENU)>=0){
						//Ctrl+A押下ならば全選択
						selectAll();
					}
				default:
					break;
			}
			break;

		default:
			break;
	}
	return ::CallWindowProc(default_proc(),handle(),message,wparam,lparam);
}
