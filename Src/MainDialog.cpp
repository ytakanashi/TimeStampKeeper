//MainDialog.cpp
//メインダイアログ

//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`
//          TimeStamp Keeper Ver.1.02 by x@rgs
//              under NYSL Version 0.9982
//
//`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`~^`


#include"StdAfx.h"
#include"MainDialog.h"
#include<shlwapi.h>

using namespace sslib;


namespace{
	class DeferPos{
		public:
		DeferPos(int number=1):m_hdwp(NULL){m_hdwp=::BeginDeferWindowPos(number);}
		virtual ~DeferPos(){::EndDeferWindowPos(m_hdwp);}
		private:
			HDWP m_hdwp;
		public:
			bool move(HWND wnd,HWND insert_after,int x,int y,int cx,int cy,UINT flags){
				return NULL!=(m_hdwp=::DeferWindowPos(m_hdwp,wnd,insert_after,x,y,cx,cy,flags));
			}
	};

	struct isndigit{bool operator()(char c)const{return !isdigit(c);}};

	void longlong2SYSTEMTIME(SYSTEMTIME* result_st,const long long date_time){
		result_st->wYear=static_cast<WORD>(date_time/10000000000000);
		result_st->wMonth=static_cast<WORD>((date_time/100000000000)%100);
		result_st->wDay=static_cast<WORD>((date_time/1000000000)%100);
		result_st->wHour=static_cast<WORD>((date_time/10000000)%100);
		result_st->wMinute=static_cast<WORD>((date_time/100000)%100);
		result_st->wSecond=static_cast<WORD>((date_time/1000)%100);
		result_st->wMilliseconds=static_cast<WORD>(date_time%1000);
	}

	void getTimeStampStr(TCHAR* result,const TCHAR* file_path){
		FILETIME ft,fttmp;
		SYSTEMTIME st;
		File file(file_path,
				  OPEN_EXISTING,
				  GENERIC_READ,
				  FILE_SHARE_READ|FILE_SHARE_WRITE);

		file.getFileTime(&ft);
		::FileTimeToLocalFileTime(&ft,&fttmp);
		::FileTimeToSystemTime(&fttmp,&st);

		lstrcpy(result,format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
							  st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds).c_str());
		return;
	}

	bool setTimeStamp(const TCHAR* file_path,FILETIME& ft){
		DWORD attr=::GetFileAttributes(file_path);

		::SetFileAttributes(file_path,
							attr&~FILE_ATTRIBUTE_READONLY);

		File file(file_path,
				  OPEN_EXISTING,
				  GENERIC_WRITE,
				  FILE_SHARE_READ);

		bool result=file.setFileTime(&ft);

		::SetFileAttributes(file_path,attr);

		return result;
	}

	void addTimeStampItem(const FileListView& listview,const TCHAR* file_path,const int index){
		LV_ITEM lvitem={};
		std::vector<TCHAR> timestamp(128);

		getTimeStampStr(&timestamp[0],file_path);

		lvitem.mask=LVIF_TEXT;
		lvitem.pszText=&timestamp[0];
		lvitem.iItem=index;
		lvitem.iSubItem=1;
		listview.setItem(&lvitem);
	}

	void insertFileItem(const FileListView& listview,const TCHAR* filepath,bool recursive){
		INNER_FUNC(insert,
			void operator()(const FileListView& listview,const TCHAR* filepath){
				int item=listview.insertItem(filepath);

				if(item!=-1){
					addTimeStampItem(listview,filepath,item);
					listview.setCheckState(item);
					listview.setItemData(FileListView::STATE_INIT,item);
				}
			}
		);
		if(recursive&&path::isDirectory(filepath)){
			//ディレクトリを再帰的に追加
			std::list<tstring> file_list;

			insert(listview,filepath);
			path::recursiveSearch(&file_list,filepath,_T("*"),true);
			for(std::list<tstring>::iterator ite=file_list.begin(),end=file_list.end();ite!=end;++ite){
				insert(listview,ite->c_str());
			}
		}else{
			//ファイルをリストに追加
			insert(listview,filepath);
		}
	}

	class EditDialog:public Dialog{
	public:
		EditDialog(tstring& name,tstring& timestamp):
		Dialog(IDD_DIALOG_EDIT),
		m_name(name),
		m_timestamp(timestamp),
		m_wMilliseconds(0){}
	private:
		tstring& m_name;
		tstring& m_timestamp;
		WORD m_wMilliseconds;
	private:
		INT_PTR onInitDialog(WPARAM wparam,LPARAM lparam){
			if(!m_name.empty()){
				::SetWindowText(getDlgItem(IDC_EDIT_FILE),m_name.c_str());

				tstring timestamp(m_timestamp);

				timestamp.erase(std::remove_if(timestamp.begin(),timestamp.end(),isndigit()),timestamp.end());

				SYSTEMTIME st;

				longlong2SYSTEMTIME(&st,_tcstoll(timestamp.c_str(),NULL,10));

				m_wMilliseconds=st.wMilliseconds;

				sendItemMessage(IDC_DATETIMEPICKER_FILE,
								DTM_SETFORMAT,
								0,
								(LPARAM)_T("yyyy/MM/dd HH:mm:ss.XXX"));

				sendItemMessage(IDC_DATETIMEPICKER_FILE,
								DTM_SETSYSTEMTIME,
								(WPARAM)GDT_VALID,
								(LPARAM)&st);
			}
			return true;
		}

		//DTPでミリ秒を扱うためのあれやこれや
		INT_PTR onNotify(WPARAM wparam,LPARAM lparam){
			switch(((LPNMHDR)lparam)->code){
				case DTN_FORMATQUERY:{
					HDC hDC=GetDC(handle());
					HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
					HFONT hOldFont=(HFONT)SelectObject(hDC,hFont);

					if(lstrcmp(_T("XXX"),((LPNMDATETIMEFORMATQUERY)lparam)->pszFormat)==0){
						GetTextExtentPoint32(hDC,
											 _T("999"),
											 3,
											 &((LPNMDATETIMEFORMATQUERY)lparam)->szMax);
					}
					SelectObject(hDC,hOldFont);
					ReleaseDC(handle(),hDC);
					break;
				}

				case DTN_FORMAT:{
					wsprintf(const_cast<LPTSTR>(((LPNMDATETIMEFORMAT)lparam)->pszDisplay),
							 _T("%03d"),
							 m_wMilliseconds);
					break;
				}

				case DTN_WMKEYDOWN:{
					if(((LPNMDATETIMEWMKEYDOWN)lparam)->nVirtKey==VK_DOWN){
						if(m_wMilliseconds==0){
							m_wMilliseconds=999;
						}else{
							m_wMilliseconds--;
						}
					}else if(((LPNMDATETIMEWMKEYDOWN)lparam)->nVirtKey==VK_UP){
						if(m_wMilliseconds>=999){
							m_wMilliseconds=0;
						}else{
							m_wMilliseconds++;
						}
					}else if(((LPNMDATETIMEWMKEYDOWN)lparam)->nVirtKey>='0'&&
							 ((LPNMDATETIMEWMKEYDOWN)lparam)->nVirtKey<='9'){
						TCHAR milliseconds[4]={};
						wsprintf(milliseconds,_T("%d"),m_wMilliseconds);

						//入力された数字
						TCHAR number[2]={(TCHAR)((LPNMDATETIMEWMKEYDOWN)lparam)->nVirtKey};

						if(lstrlen(milliseconds)==3){
							//3桁なら文字列をクリアし新たに数字を追加
							m_wMilliseconds=::StrToInt(number);
						}else{
							//3桁未満なら現在の文字列に数字を追加
							lstrcat(milliseconds,number);
							m_wMilliseconds=::StrToInt(milliseconds);
						}
					}
					InvalidateRect(getDlgItem(IDC_DATETIMEPICKER_FILE),NULL,TRUE);
					break;
				}

				default:
					break;
			}
			return false;
		}

		INT_PTR onOk(WPARAM wparam,LPARAM lparam){
			std::vector<TCHAR> name(MAX_PATHW);
			::GetWindowText(getDlgItem(IDC_EDIT_FILE),&name[0],name.size());

			if(!name.empty()){
				m_name.assign(&name[0]);

				SYSTEMTIME st;

				sendItemMessage(IDC_DATETIMEPICKER_FILE,DTM_GETSYSTEMTIME,(WPARAM)NULL,(LPARAM)&st);
				//注意:ミリ秒はm_wMilliseconds使用
				m_timestamp.assign(format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
										  st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,m_wMilliseconds).c_str());
			}
			return true;
		}
	};
}

INT_PTR MainDialog::onInitDialog(WPARAM wparam,LPARAM lparam){
	//アイコンの設定(タイトルバー)
	setIcon(IDI_ICON1);

	//タイトルの設定(タイトルバー)
	::SetWindowText(handle(),(tstring(_T("TimeStamp Keeper Ver."))+SOFTWARE_VERSION).c_str());

	m_listview=new FileListView(handle());
	if(m_listview==NULL)return false;

	RECT rc={0};

	::GetWindowRect(handle(),&rc);

	//onGetMinMaxInfo()用
	m_wnd_width=rc.right-rc.left;
	m_wnd_height=rc.bottom-rc.top;

	::GetWindowRect(m_listview->handle(),&rc);

	m_listview->insertColumn(0,
							 _T("ファイル名"),
							 LVCFMT_LEFT,
							 rc.right-rc.left);
	m_listview->insertColumn(1,
							 _T("更新日時"),
							 LVCFMT_LEFT,
							 140);

	m_listview->setExtendedListViewStyle(m_listview->getExtendedListViewStyle()|
										 //LVS_EX_FULLROWSELECT(行全体選択)
										 LVS_EX_FULLROWSELECT|
										 //LVS_EX_GRIDLINES(罫線表示)
										 LVS_EX_GRIDLINES|
										 //LVS_EX_CHECKBOXES(チェックボックス表示)
										 LVS_EX_CHECKBOXES);

	//サイズ変更時に場所を変更
	m_wnd_size_info=new SIZE_INFO(handle(),getDlgItem(IDC_LIST1));

	{
		//引数解析
		std::vector<tstring>& options=static_cast<CommandArgument*>(param())->options();
		std::vector<tstring>& filepaths=static_cast<CommandArgument*>(param())->filepaths();

		for(std::vector<tstring>::size_type i=0,size=options.size();i<size;++i){
			if(str::isEqualStringIgnoreCase(options[i],_T("r"))){
				//再帰的に追加
				m_recursive_search=true;
			}
		}

		for(std::vector<tstring>::size_type i=0,size=filepaths.size();i<size;++i){
			insertFileItem(*m_listview,filepaths[i].c_str(),m_recursive_search);
		}
	}

	//設定メニュー読み込み
	m_settings_menu.load(handle(),3);

	::DragAcceptFiles(handle(),true);

	if(::GetAsyncKeyState(VK_CONTROL)&0x8000){
		showDialog(SW_MINIMIZE);
	}

	::SetFocus(m_listview->handle());

	return true;
}

INT_PTR MainDialog::onCommand(WPARAM wparam,LPARAM lparam){
	switch(LOWORD(wparam)){
		case IDM_MENU_RESTORE:
			sendMessage(WM_COMMAND,IDOK,0);
			break;

		case IDM_MENU_CLEAR:
			m_listview->clear();
			break;

		case IDM_MENU_EDIT:{
			int index=m_listview->getNextItem();
			if(index==-1)break;
			tstring name(m_listview->getItemText(index,0));
			tstring timestamp(m_listview->getItemText(index,1));
			tstring name_old(name);
			tstring timestamp_old(timestamp);
			EditDialog edit_dialog(name,timestamp);

			edit_dialog.doModal(handle());
			m_listview->setItemText(name.c_str(),index,0);
			m_listview->setItemText(timestamp.c_str(),index,1);
			if(name!=name_old||timestamp!=timestamp_old){
				m_listview->setItemData(FileListView::STATE_INIT,index);
			}
			break;
		}

		case IDM_MENU_RECURSIVE:
			m_recursive_search=!m_recursive_search;
			break;

		default:
			break;
	}
	return false;
}

INT_PTR MainDialog::onNotify(WPARAM wparam,LPARAM lparam){
	static HWND edit_handle=NULL;

	switch(((LPNMHDR)lparam)->idFrom){
		case IDC_LIST1:
			//子へ投げる
			SendMessage(m_listview->handle(),WM_NOTIFY,wparam,lparam);
			return ::GetWindowLong(handle(),DWL_MSGRESULT);
		default:
			break;
	}
	return false;
}

INT_PTR MainDialog::onOk(WPARAM wparam,LPARAM lparam){
	restoreTimeStamp();
	return false;
}

INT_PTR MainDialog::onDropFiles(HDROP drop_handle){
	DropFiles drop_files(drop_handle);

	for(size_t i=0,drop_files_count=drop_files.getCount(),count=m_listview->getItemCount();i<drop_files_count;i++){
		insertFileItem(*m_listview,drop_files.getFile(i).c_str(),m_recursive_search);
	}
	return true;
}

INT_PTR MainDialog::onSize(WPARAM wparam,LPARAM lparam){
	if(!m_wnd_size_info)return false;

	DeferPos defer_pos;

	defer_pos.move(m_wnd_size_info->wnd,
				   NULL,
				   0,0,
				   LOWORD(lparam)-m_wnd_size_info->width_diff,
				   HIWORD(lparam)-m_wnd_size_info->height_diff,
				   SWP_NOMOVE|SWP_NOZORDER);
	return false;
}

INT_PTR MainDialog::onCopyData(WPARAM wparam,LPARAM lparam){
	PCOPYDATASTRUCT copy_data=reinterpret_cast<PCOPYDATASTRUCT>(lparam);

	if(copy_data->dwData==1){
		insertFileItem(*m_listview,static_cast<LPTSTR>(copy_data->lpData),m_recursive_search);
		return true;
	}
	return false;
}

INT_PTR MainDialog::onGetMinMaxInfo(WPARAM wparam,LPARAM lparam){
	LPMINMAXINFO info=reinterpret_cast<LPMINMAXINFO>(lparam);

	info->ptMinTrackSize.x=m_wnd_width;
	info->ptMinTrackSize.y=m_wnd_height;
	return true;
}

INT_PTR MainDialog::onMessage(UINT message,WPARAM wparam,LPARAM lparam){
	switch(message){
		case WM_INITMENUPOPUP:{
			if(m_settings_menu.sub_handle()==reinterpret_cast<HMENU>(wparam)){
				m_settings_menu.check(IDM_MENU_RECURSIVE,m_recursive_search);
			}
			break;
		}
		case WM_COPYDATA:
			return onCopyData(wparam,lparam);
		case WM_GETMINMAXINFO:
			return onGetMinMaxInfo(wparam,lparam);
		case WM_SETFOCUS:
			::SetFocus(m_listview->handle());
			return true;
		default:
			break;
	}
	return false;
}

void MainDialog::restoreTimeStamp(){
	int cnt=m_listview->getItemCount();
	if(!cnt)return;

	for(int item=0;item<cnt;++item){
		if(m_listview->getCheckState(item)){
			tstring timestamp_str=m_listview->getItemText(item,1);

			if(!path::fileExists(m_listview->getItemText(item).c_str())){
				m_listview->setItemData(FileListView::STATE_ERROR,item);
#if 0
				::MessageBox(handle(),format(_T("ファイルが見つかりません。\n%s"),
											 m_listview->getItemText(item).c_str()).c_str(),
							 (tstring(_T("TimeStamp Keeper Ver."))+SOFTWARE_VERSION).c_str(),MB_ICONSTOP);
#endif
				continue;
			}

			timestamp_str.erase(std::remove_if(timestamp_str.begin(),timestamp_str.end(),isndigit()),timestamp_str.end());

			SYSTEMTIME st;
			FILETIME ft,fttmp;

			longlong2SYSTEMTIME(&st,_tcstoll(timestamp_str.c_str(),NULL,10));
			::SystemTimeToFileTime(&st,&fttmp);
			::LocalFileTimeToFileTime(&fttmp,&ft);

			if(setTimeStamp(m_listview->getItemText(item).c_str(),ft)){
				m_listview->setItemData(FileListView::STATE_COMPLETE,item);
			}else{
				m_listview->setItemData(FileListView::STATE_ERROR,item);
#if 0
				::MessageBox(handle(),format(_T("タイムスタンプの復元に失敗しました。\n%s [%s]"),
											 m_listview->getItemText(item).c_str(),
											 timestamp_str.c_str()
											 ).c_str(),
							 (tstring(_T("TimeStamp Keeper Ver."))+SOFTWARE_VERSION).c_str(),MB_ICONSTOP);
#endif
			}
		}
	}
}
