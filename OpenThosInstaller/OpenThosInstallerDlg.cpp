
// OpenThosInstallerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenThosInstaller.h"
#include "OpenThosInstallerDlg.h"
#include "afxdialogex.h"

#include "WinIoCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()

};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_REGISTERED_MESSAGE(WM_UPDATE_PROGRESS, &CAboutDlg::OnUpdateProgress)
END_MESSAGE_MAP()


// COpenThosInstallerDlg �Ի���



COpenThosInstallerDlg::COpenThosInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENTHOSINSTALLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SelectedPath = _T("");
	m_SrcFile = _T("");
	m_DestFile = _T("");
}

void COpenThosInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILETREE, m_FileTree);
}

BEGIN_MESSAGE_MAP(COpenThosInstallerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_FILETREE, &COpenThosInstallerDlg::OnItemexpandedFiletree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_FILETREE, &COpenThosInstallerDlg::OnSelchangedFiletree)
	ON_NOTIFY(NM_RCLICK, IDC_FILETREE, &COpenThosInstallerDlg::OnRclickFiletree)
	ON_COMMAND(ID_NEW_FOLDER, &COpenThosInstallerDlg::OnNewFolder)
	ON_COMMAND(ID_SELECT_FOLDER, &COpenThosInstallerDlg::OnSelectFolder)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_FILETREE, &COpenThosInstallerDlg::OnEndlabeleditFiletree)
	ON_BN_CLICKED(IDOK, &COpenThosInstallerDlg::OnBnClickedOk)
	ON_MESSAGE(UM_UPDATE_PROGRESS, &COpenThosInstallerDlg::OnUpdateProgress)
	//ON_REGISTERED_MESSAGE(WM_UPDATE_PROGRESS, &COpenThosInstallerDlg::OnUpdateProgress)
END_MESSAGE_MAP()


// COpenThosInstallerDlg ��Ϣ�������

BOOL COpenThosInstallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_FileTree.ModifyStyle(NULL, TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS);
	m_hRoot = m_FileTree.InsertItem(_T("�ҵĵ���"));         //������ڵ�  
	GetLogicDrives();
	GetDriveDir(m_hRoot);

	m_FileTree.Expand(m_hRoot, TVE_EXPAND);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COpenThosInstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenThosInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenThosInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COpenThosInstallerDlg::GetLogicDrives()
{
	size_t szAllDriveStrings = GetLogicalDriveStrings(0, NULL);           //�������ܳ���  
	wchar_t *pDriveStrings = new wchar_t[szAllDriveStrings + sizeof(_T(""))];  //��������  
	GetLogicalDriveStrings(szAllDriveStrings, pDriveStrings);
	size_t szDriveString = lstrlen(pDriveStrings);                        //������С  
	while (szDriveString > 0)
	{
		m_FileTree.InsertItem(pDriveStrings, m_hRoot);       //�ڸ��ڵ�hParent������̷�  
		pDriveStrings += szDriveString + 1;             //pDriveStrings��C:\ D:\ E:\��  
		szDriveString = lstrlen(pDriveStrings);
	}
}


void COpenThosInstallerDlg::GetDriveDir(HTREEITEM hParent)
{
	HTREEITEM hChild = m_FileTree.GetChildItem(hParent);   //��ȡָ��λ���е�����  
	while (hChild)
	{
		CString strText = m_FileTree.GetItemText(hChild);  //�����б�����Ŀ����  
		if (strText.Right(1) != _T("\\"))                  //���ұ�1��ʼ��ȡ��������nCount���ַ�  
			strText += _T("\\");
		strText += "*.*";
		//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ  
		CFileFind file;                                       //���屾���ļ�����  
		BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�  
		while (bContinue)
		{
			bContinue = file.FindNextFile();                  //������һ���ļ�  
			if (file.IsDirectory() && !file.IsDots())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."  
				m_FileTree.InsertItem(file.GetFileName(), hChild); //����̷�·������״�ļ���  
		}
		GetDriveDir(hChild);                                  //�ݹ����  
		hChild = m_FileTree.GetNextItem(hChild, TVGN_NEXT);        //��ȡ���οؼ�TVGN_NEXT���ֵ���  
	}
}


CString COpenThosInstallerDlg::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = _T("");
	while (hCurrent != m_hRoot)
	{
		strTemp = m_FileTree.GetItemText(hCurrent);    //�����б�����Ŀ����  
		if (strTemp.Right(1) != _T("\\")  )
			strTemp += _T("\\")  ;
		strReturn = strTemp + strReturn;
		hCurrent = m_FileTree.GetParentItem(hCurrent); //���ظ���Ŀ���  
	}
	return strReturn;
}


void COpenThosInstallerDlg::AddSubDir(HTREEITEM hParent)
{
	CString strPath = GetFullPath(hParent);     //��ȡȫ·��  
	if (strPath.Right(1) != _T("\\")  )
		strPath += _T("\\")  ;
	strPath += "*.*";
	CFileFind file;
	BOOL bContinue = file.FindFile(strPath);    //���Ұ����ַ������ļ�  
	while (bContinue)
	{
		bContinue = file.FindNextFile();        //������һ���ļ�  
		if (file.IsDirectory() && !file.IsDots())
			m_FileTree.InsertItem(file.GetFileName(), hParent);
	}
}


void COpenThosInstallerDlg::OnItemexpandedFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	TVITEM item = pNMTreeView->itemNew;                  //����\���ܹ���������ͼ��Ŀ��Ϣ  
	if (item.hItem == m_hRoot)
		return;
	HTREEITEM hChild = m_FileTree.GetChildItem(item.hItem);  //��ȡָ��λ���е�����  
	while (hChild)
	{
		AddSubDir(hChild);                               //�����Ŀ¼  
		hChild = m_FileTree.GetNextItem(hChild, TVGN_NEXT);   //��ȡ���οؼ�TVGN_NEXT���ֵ���  
	}
	*pResult = 0;
}


void COpenThosInstallerDlg::OnSelchangedFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	TVITEM item = pNMTreeView->itemNew;
	if (item.hItem == m_hRoot)
		return;
	m_SelectedPath = GetFullPath(item.hItem);
	*pResult = 0;
}


void COpenThosInstallerDlg::OnRclickFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CPoint point;

	GetCursorPos(&point);

	CPoint pointInTree = point;

	m_FileTree.ScreenToClient(&pointInTree);

	HTREEITEM item;

	UINT flag = TVHT_ONITEM;

	item = m_FileTree.HitTest(pointInTree, &flag);

	if (item != NULL)

	{

		m_FileTree.SelectItem(item);

		CMenu menu;

		menu.LoadMenu(IDR_MENU1);

		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN |

			TPM_RIGHTBUTTON, point.x, point.y, this, NULL);

	}
	*pResult = 0;
}


void COpenThosInstallerDlg::OnNewFolder()
{
	// TODO: Add your command handler code here
	//SECURITY_ATTRIBUTES securityAttributes{}
	HTREEITEM hItem;
	hItem = m_FileTree.GetSelectedItem();
	//int i;
	//for (i = 0; m_FileTree.GetParentItem(item) != NULL; i++)
	//{
	//	//item = m_FileTree.GetParentItem(item);
	//}

	HTREEITEM hSltItem = m_FileTree.InsertItem(_T("�½��ļ���"), hItem, TVI_LAST);

	//HTREEITEM hSltItem = m_FileTree.GetSelectedItem();



	if (hSltItem)
	{
		m_FileTree.Expand(hItem, TVE_EXPAND);
		CString strNewFolder = m_SelectedPath + _T("�½��ļ���");
		CreateDirectory(strNewFolder, NULL);
		m_FileTree.SelectItem(hSltItem);
		m_FileTree.EditLabel(hSltItem);

		//CEdit* pLabelEdit = m_FileTree.EditLabel(hSltItem);
		//pLabelEdit->ShowWindow(SW_SHOW);
		//ASSERT(pLabelEdit != NULL);
	}
}


void COpenThosInstallerDlg::OnSelectFolder()
{
	// TODO: Add your command handler code here

}


void COpenThosInstallerDlg::OnEndlabeleditFiletree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: Add your control notification handler code here
	TV_ITEM* ptvItem = &pTVDispInfo->item;
	// ��ȡ�޸ĺ������  
	CString LabelStr = ptvItem->pszText;
	// ���������������һЩ����  
	if (LabelStr == "")
	{
		return;
	}
	HTREEITEM hItem = m_FileTree.GetSelectedItem();
	if (hItem)
	{
		//Rename
		CString strOldDir = GetFullPath(hItem);
		//treeCtrl ��CTreeCtrl��һ������   
		m_FileTree.SetItemText(hItem, LabelStr);
		m_SelectedPath = GetFullPath(hItem);
		_trename(strOldDir, m_SelectedPath);

		m_FileTree.SelectItem(hItem);
	}
	*pResult = 0;
}


BOOL COpenThosInstallerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CEdit * pLableEdit = m_FileTree.GetEditControl();
			if (pLableEdit && pLableEdit->IsWindowVisible())
			{
				this->SetFocus(); //�༭��ʧȥ���㣬�����༭���Ӷ�����TVN_ENDLABELEDIT ����  
				return TRUE;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void COpenThosInstallerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here

	//CreateThread(NULL, 0, &CopyThread, this, 0, 0);
	CreateThread(NULL, 0, /*(LPTHREAD_START_ROUTINE)*/CopyThread, this, 0, 0);
	GetDlgItem(IDC_FILETREE)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);
	//CDialogEx::OnOK();
}

afx_msg LRESULT COpenThosInstallerDlg::OnUpdateProgress(WPARAM wParam, LPARAM lParam)
{
	ULONGLONG nTotalLen = (ULONGLONG)wParam;
	static UINT nCopied = 0;
	nCopied += (UINT)lParam;
	CProgressCtrl *pProgressBar = (CProgressCtrl *)GetDlgItem(IDC_PROGRESS_COPY);
	if (nTotalLen != 0)
	{
		pProgressBar->SetRange32(0, nTotalLen / 1024 / 1024);
	}
	if (nCopied != 0)
	{
		pProgressBar->SetPos(nCopied / 1024 /1024);
	}
	return 0;
}

CString COpenThosInstallerDlg::GetAppPath()
{
	TCHAR *lpstrAppPath = new TCHAR[_MAX_PATH];
	ZeroMemory(lpstrAppPath, _MAX_PATH);

	GetModuleFileName(NULL, lpstrAppPath, _MAX_PATH);

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];

	_tsplitpath_s(lpstrAppPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);
	CString strAppPath = CString(szDrive) + CString(szDir);

	delete lpstrAppPath;

	return strAppPath;
}

BOOL COpenThosInstallerDlg::IsFileExist(const CString& csFile)
{
	DWORD dwAttrib = GetFileAttributes(csFile);
	return INVALID_FILE_ATTRIBUTES != dwAttrib && 0 == (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
}

BOOL COpenThosInstallerDlg::FileCopyAndPostMessge(const CString& strSrcFile, const CString& strDestFile, BOOL bFalseIfExists/*, const HWND& hwndToAcceptMessge*/)
{
	if (!IsFileExist(strSrcFile) ||
		(bFalseIfExists && IsFileExist(strDestFile)))

		return FALSE;

	CFile fileSrc(strSrcFile, CFile::modeRead);
	CFile fileDest(strDestFile, CFile::modeCreate | CFile::modeWrite);
	//ע��CFile::modeNoTruncate
	char buff[1024 * 400] = { 0 };
	UINT nReadLen = fileSrc.Read(buff, sizeof(buff));
	ULONGLONG nTotalLen = fileSrc.GetLength();
	//DWORD nCopiedLen = 0;				
	while (nReadLen>0)
	{
		fileDest.Write(buff, nReadLen);
		fileDest.SeekToEnd();
		//�ƶ��ļ�ָ��,����Ḳ��
		//nCopiedLen += nReadLen;
		PostMessage(UM_UPDATE_PROGRESS, 0, nReadLen);
		nReadLen = fileSrc.Read(buff, sizeof(buff));
	}
	return TRUE;
}


DWORD COpenThosInstallerDlg::RunCmd(CString strCmd)
{
	SECURITY_ATTRIBUTES securityAttributes;
	ZeroMemory(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.lpSecurityDescriptor = NULL;
	securityAttributes.bInheritHandle = TRUE;

	//if (!CreatePipe(&hRead, &hWrite, &securityAttributes, 0))
	//{
	//	MessageBox(dlg->m_hWnd, _T("CreatePipe Failed!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
	//	return -1;
	//}


	STARTUPINFO startupInfo;
	ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&startupInfo);
	//startupInfo.hStdOutput = hWrite;
	//startupInfo.hStdError = hWrite;
	startupInfo.wShowWindow = SW_SHOW;
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	PROCESS_INFORMATION processInfomation;
	ZeroMemory(&processInfomation, sizeof(processInfomation));



	if (!CreateProcess(NULL, CT2W(strCmd), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, GetAppPath(), &startupInfo, &processInfomation))
	{
		MessageBox(_T("CreateProcess Failed!"), _T("��ʾ"), MB_OK | MB_ICONWARNING);
		return -1;
	}

	//CloseHandle(hWrite);

	//char buffer[4096] = { 0 };
	//CString strOutput;
	//DWORD bytesRead;

	//while (true)
	//{
	//	if (ReadFile(hRead, buffer, 4095, &bytesRead, NULL) == NULL)  //��ȡ�ܵ�
	//		break;

	//	strOutput += buffer;
	//	dlg->SetDlgItemText(IDC_EDIT1, strOutput);  //��ʾ�����Ϣ���༭��,��ˢ�´���
	//	dlg->UpdateWindow();
	//	Sleep(100);
	//}
	//CloseHandle(hRead);

	WaitForSingleObject(processInfomation.hProcess, INFINITE);

	CloseHandle(processInfomation.hProcess);
}


DWORD WINAPI COpenThosInstallerDlg::CopyThread(LPVOID lpParam)
{
	
	COpenThosInstallerDlg *dlg = (COpenThosInstallerDlg*)lpParam;

	CString strSrcPath = dlg->GetAppPath() + _T("OpenThos\\");
	
	ULONGLONG nTotalLen = 0;
	CFile tmpFile;
	tmpFile.Open(strSrcPath + _T("system.sfs"), CFile::modeRead);
	nTotalLen += tmpFile.GetLength();
	tmpFile.Close();
	tmpFile.Open(strSrcPath + _T("ramdisk.img"), CFile::modeRead);
	nTotalLen += tmpFile.GetLength();
	tmpFile.Close();
	tmpFile.Open(strSrcPath + _T("install.img"), CFile::modeRead);
	nTotalLen += tmpFile.GetLength();
	tmpFile.Close();
	//tmpFile.Open(strSrcPath + _T("ramdisk.img"), CFile::modeRead);
	//nTotalLen += tmpFile.GetLength();
	//tmpFile.Close();

	dlg->PostMessage(UM_UPDATE_PROGRESS, nTotalLen, 0);

	dlg->FileCopyAndPostMessge(strSrcPath + _T("system.sfs"), dlg->m_SelectedPath + _T("system.sfs"), false/*, dlg->m_hWnd*/);
	dlg->FileCopyAndPostMessge(strSrcPath + _T("ramdisk.img"), dlg->m_SelectedPath + _T("ramdisk.img"), false/*, dlg->m_hWnd*/);
	dlg->FileCopyAndPostMessge(strSrcPath + _T("install.img"), dlg->m_SelectedPath + _T("install.img"), false/*, dlg->m_hWnd*/);

	tmpFile.Open(dlg->m_SelectedPath + _T("data.img"), CFile::modeCreate | CFile::modeWrite);

	
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	

	_tsplitpath_s(dlg->m_SelectedPath, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, NULL, 0, NULL, 0);

	CString selectedRootPath = CString(szDrive) + _T("\\");
	//DWORD volSerialNum;
	//TCHAR szFileSystemName[16];
	//ZeroMemory(szFileSystemName, 16);
	//GetVolumeInformation(selectedRootPath,
	//	NULL, 0,					//VolumeName is not neccessary.
	//	&volSerialNum,
	//	NULL,					//MaximumComponentLength is not cared.
	//	NULL,					//FileSystemFlags are not cared.
	//	szFileSystemName, 16);
	BOOT_BLOCK bootBlock;
	HANDLE hVolume = CreateFile(CString(_T("\\\\.\\"))+szDrive,
							GENERIC_READ,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							0,
							NULL);
	DWORD dwError = GetLastError();
	DWORD nNumsRead;

	DWORD nNumsToRead = sizeof(BOOT_BLOCK);

	BOOL bRet = ReadFile(hVolume, &bootBlock, nNumsToRead, &nNumsRead, NULL);
	dwError = GetLastError();
	ULONGLONG nLen = strncmp((char *)bootBlock.Format, "NTFS", 4) == 0? 8ULL * 1024ULL * 1024ULL * 1024ULL : 4ULL * 1024ULL * 1024ULL * 1024ULL - 1024ULL;
	tmpFile.SetLength(nLen);
	nLen = tmpFile.GetLength();
	tmpFile.Close();

	CString strCmd = _T("cmd /C installtools\\Mke2fs.exe -F -t ext4 ") + dlg->m_SelectedPath + "data.img";
	dlg->RunCmd(strCmd);

	//HANDLE hRead, hWrite;
	tmpFile.Open(dlg->GetAppPath() + _T("installtools\\boto_linux.conf"), CFile::modeCreate | CFile::modeWrite);
	//"OpenThos" "quiet androidboot.hardware=android_x86_64 initrd=OpenThos/initrd.img BOOT_MODE=hdimgboot RAMDISK_HD_UUID=84D81B6FD81B5F2C RAMDISK_IMG=OpenThos/ramdisk.img SYSTEM_HD_UUID=84D81B6FD81B5F2C SYSTEM_IMG=openthos/system.sfs DATA_HD_UUID=84D81B6FD81B5F2C DATA_IMG=openthos/data.img"
	CString StrVolSN;
	StrVolSN.Format(_T("%llx"), bootBlock.VolumeSerialNumber);
	CString strBotoConf = _T("\"OPENTHOS\" \"initrd=/OpenThos/initrd.img quiet androidboot.hardware=android_x86_64 BOOT_MODE=hdimgboot RAMDISK_HD_UUID=")
		+ StrVolSN + _T(" RAMDISK_IMG=") + szDir + _T("ramdisk.img") + _T(" SYSTEM_HD_UUID=") + StrVolSN + _T(" SYSTEM_IMG=") 
		+ szDir + _T("system.sfs") + _T(" DATA_HD_UUID=") + StrVolSN + _T(" DATA_IMG=")	+ szDir + _T("data.img\"") ;
	strBotoConf.Replace(_T("\\"), _T("/"));
//	CString strBotoConf = _T("abcdefghijklmnopqrstuvwxyz1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	TCHAR *pCharCmd = strBotoConf.GetBuffer();
	nLen = strBotoConf.GetLength()*sizeof(TCHAR);
	tmpFile.Write(pCharCmd, nLen);
	//tmpFile.Flush();
	tmpFile.Close();



	//strCmd = /*_T("cmd /C ") + */dlg->GetAppPath() + _T("installtools\\install_files_to_esp.bat");
	//dlg->RunCmd(strCmd);
	//system(CT2A(strCmd));
	//ShellExecute(NULL, _T("runas"), _T("cmd.exe"), strCmd, NULL, SW_SHOWNORMAL);

	strCmd = /*_T("/C ") + */dlg->GetAppPath() + _T("installtools\\install_boto.bat");
	//strCmd = _T("bcdedit ");
	//dlg->RunCmd(strCmd);
	ShellExecute(NULL, _T("runas"), strCmd, NULL, dlg->GetAppPath(), SW_HIDE);


	dlg->MessageBox(_T("OpenThosϵͳ�������´�����ϵͳʱ���ã�\n�����ȷ�����󣬳����˳���"), _T("��װ���"), MB_OK);
	dlg->PostMessage(WM_QUIT, 0, 0);

	return 0;
}