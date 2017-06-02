
// OpenThosInstallerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#define UM_UPDATE_PROGRESS WM_USER + 100

DWORD WINAPI CopyThread(LPVOID lpParam);

#pragma pack ( push )     //结构体紧凑排列，以便与硬盘上读出的数据对齐
#pragma pack ( 1 )		  //按1个字节为基准对齐，即完紧凑

typedef struct
{
	UCHAR Jump[3];						//0x00  L3
	UCHAR Format[8];					//0x03	L8
	USHORT BytesPerSector;				//0x0B	L2
	UCHAR SectorsPerCluster;			//0x0D	L1
	USHORT BootSectors;					//0x0E	L2
	UCHAR  Mbz1[3];						//0x10	L3
	USHORT Reserved1;					//0x13	L2
	UCHAR MediaType;					//0x15	L1
	UCHAR Mbz2[2];						//0x16	L2
	USHORT SectorsPerTrack;				//0x18	L2
	USHORT NumberOfHeads;				//0x1A	L2
	DWORD SectorsHidden;				//0x1C	L4
	UCHAR Mbz3[4];						//0x20	L4
	USHORT Mb8000[2];					//0x24  L4
	ULONGLONG TotalSectors;				//0x28	L8
	ULONGLONG MftStartLcn;				//0x30	L8
	ULONGLONG Mft2StartLcn;				//0x38	L8
	DWORD ClustersPerFileBlock;			//0x40	L4
	DWORD ClustersPerIndexBlock;		//0x44	L4
	ULONGLONG VolumeSerialNumber;		//0x48	L8
	DWORD CheckSum;						//0x50	L4
	UCHAR Code[0x1AA];					//0x55
	USHORT BootSignature;				//0x1FE L2
} BOOT_BLOCK, *PBOOT_BLOCK;

#pragma pack ( pop )

// COpenThosInstallerDlg 对话框
class COpenThosInstallerDlg : public CDialogEx
{
// 构造
public:
	COpenThosInstallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENTHOSINSTALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_FileTree;
	void GetLogicDrives();
	HTREEITEM m_hRoot;
	void GetDriveDir(HTREEITEM hParent);
	CString GetFullPath(HTREEITEM hCurrent);
	void AddSubDir(HTREEITEM hParent);
	
	afx_msg void OnItemexpandedFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_SelectedPath;
	afx_msg void OnSelchangedFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewFolder();
	afx_msg void OnSelectFolder();
	afx_msg void OnEndlabeleditFiletree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	HTREEITEM m_SelectedItem;
	afx_msg void OnBnClickedOk();
	CString GetAppPath();
	BOOL IsFileExist(const CString & csFile);
	BOOL FileCopyAndPostMessge(const CString & strSrcFile, const CString & strDestFile, BOOL bFalseIfExists/*, const HWND & hwndToAcceptMessge*/);
	DWORD RunCmd(CString strCmd);
protected:
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam, LPARAM lParam);
	static DWORD WINAPI CopyThread(LPVOID lpParam);
public:
	CString m_SrcFile;
	CString m_DestFile;
};
