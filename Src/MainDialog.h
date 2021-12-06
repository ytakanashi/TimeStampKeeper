//MainDialog.h

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.03 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#ifndef _MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9
#define _MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9


#include"FileListView.h"
#include"resources/resource.h"





class MainDialog:public sslib::Dialog{
public:
	MainDialog():
		Dialog(IDD_DIALOG_MAIN),
		m_listview(NULL),
		m_wnd_size_info(NULL),
		m_wnd_height(0),
		m_wnd_width(0),
		m_settings_menu(inst()),
		m_confirm_restore(false),
		m_confirm_clear(false),
		m_recursive_search(false){
		}
	~MainDialog(){
		SAFE_DELETE(m_wnd_size_info);
		SAFE_DELETE(m_listview);
	}

private:
	struct SIZE_INFO{
		HWND wnd;
		int width_diff;
		int height_diff;
		RECT parent_rect;
		RECT rect;
		POINT pt;
		SIZE_INFO(HWND parent_handle,HWND wnd_handle):
				wnd(NULL),
				width_diff(0),
				height_diff(0),
				parent_rect(),
				rect(),
				pt(){
			wnd=wnd_handle;

			::GetClientRect(parent_handle,&parent_rect);

			::GetWindowRect(wnd_handle,&rect);

			pt.x=rect.left;
			pt.y=rect.top;
			::ScreenToClient(parent_handle,&pt);

			width_diff=abs((parent_rect.right-parent_rect.left)-(rect.right-rect.left));
			height_diff=abs((parent_rect.bottom-parent_rect.top)-(rect.bottom-rect.top));
		}
	};

	FileListView* m_listview;

	//for ListView
	SIZE_INFO* m_wnd_size_info;

	int m_wnd_height;
	int m_wnd_width;

	sslib::Menu m_settings_menu;

	//復元前に確認ダイアログを表示
	bool m_confirm_restore;

	//クリア前に確認ダイアログを表示
	bool m_confirm_clear;

	//再帰的に追加
	bool m_recursive_search;

private:
	//メッセージハンドラ
	INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam);
	INT_PTR onCommand(WPARAM wparam,LPARAM lparam);
	INT_PTR onNotify(WPARAM wparam,LPARAM lparam);
	INT_PTR onOk(WPARAM wparam,LPARAM lparam);
	INT_PTR onDropFiles(HDROP drop_handle);
	INT_PTR onSize(WPARAM wparam,LPARAM lparam);
	INT_PTR onCopyData(WPARAM wparam,LPARAM lparam);
	INT_PTR onGetMinMaxInfo(WPARAM wparam,LPARAM lparam);
	INT_PTR onMessage(UINT message,WPARAM wparam,LPARAM lparam);

	void restoreTimeStamp();
};


#endif //_MAINDIALOG_H_01F1DAEA_AAF6_4a0b_9752_7279D67633F9
