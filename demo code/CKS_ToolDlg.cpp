/*
 *********************************************************************************************************
 *
 *	 工具名称 : CKS_Tool
 *	 文件名称 : CKS_ToolDlg.cpp
 *	 版    本 : V1.0.0.1
 *	 修改记录 :
 *		 版本号       日期       作者    说明
 *		 v1.0.0.1    2021-01-21 veis    首发
 *
 *	 Copyright (C), 2021-2022, 博客园 https://www.cnblogs.com/veis/
 *
 *********************************************************************************************************
 */

// CKS_ToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CKS_Tool.h"
#include "CKS_ToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCKSToolDlg 对话框



CCKSToolDlg::CCKSToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CKS_TOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCKSToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XORSUM, m_checktype);
	DDX_Control(pDX, IDC_EDIT1, m_input);
	DDX_Control(pDX, IDC_EDIT2, m_output);
}

BEGIN_MESSAGE_MAP(CCKSToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CCKSToolDlg::OnBnClickedButtonCalc)
	ON_EN_CHANGE(IDC_EDIT1, &CCKSToolDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CCKSToolDlg 消息处理程序

BOOL CCKSToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_checktype.SetCheck(XOR);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCKSToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCKSToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCKSToolDlg::OnBnClickedButtonCalc()
{
	// TODO: 在此添加控件通知处理程序代码

	Error_Type ret = GetInput();
	switch(ret)
	{
		case EMPTY:
		{
			MessageBox(_T("请先输入再进行转换！"), _T("提示"), MB_ICONWARNING);
			return;
		}
		case INVALID:
		{
			MessageBox(_T("请输入错误！请按提示的格式输入：AA 0D 12 34"), _T("提示"), MB_ICONWARNING);
			return;
		}
	}

	BYTE res = 0;
	int nSel = m_checktype.GetCheck();
	switch (nSel)
	{
		case ACCUMULATION: // 累加和
		{
			res = accAssert();
			break;
		}
		case XOR: // 异或和
		{
			res = xorAssert();
			break;
		}
	}
	
	m_Output.Format(_T("%02x"), res);
	m_Output.MakeUpper();
	m_output.SetWindowText(m_Output);
	m_hexArry.RemoveAll();
}


Error_Type CCKSToolDlg::GetInput()
{
	// TODO: 在此处添加实现代码.
	CString strInput;
	m_input.GetWindowText(strInput);

	if (0 == strInput.GetLength())
		return EMPTY;
	
	strInput.TrimLeft(_T(" "));	
	strInput.TrimRight(_T(" "));

	for (int i = 2; i < strInput.GetLength(); i += 3)
	{
		if (strInput.GetAt(i) != _T(' ')) {
			return INVALID;
		}
	}
	SplitStringBySpace(strInput);

	return VALID;
}


void CCKSToolDlg::SplitStringBySpace(CString str)
{
	// TODO: 在此处添加实现代码.
	static TCHAR hexCh[] = _T("0123456789ABCDEF");
	BYTE tmp = 0;
	int len = str.GetLength();
	int i = 0, index = 0, j = 0;
	while (i < len)
	{
		TCHAR ch = str.GetAt(i);
		if (_T(' ') != ch) {
			for (j = 0; j < 16; ++j)
			{
				if (ch == hexCh[j])
					break;
			}
			tmp = j | tmp << index * 4;
			index++;
			if (index != 0 && index % 2 == 0) {
				m_hexArry.Add(tmp);
				tmp = 0;
				index = 0;
			}
		}
		++i;
	}
}

BYTE CCKSToolDlg::xorAssert()
{
	BYTE res = 0;
	for (int i = 0; i < m_hexArry.GetCount(); ++i)
	{
		res ^= m_hexArry.GetAt(i);
	}
	return res;
}

BYTE CCKSToolDlg::accAssert()
{
	BYTE res = 0;
	for (int i = 0; i < m_hexArry.GetCount(); ++i)
	{
		res |= m_hexArry.GetAt(i);
	}
	res &= 0xff;
	return res;
}

void CCKSToolDlg::EditCtrlLimit(CEdit& editCtrl, int nLenLimit, CString strLimited)
{
	static BOOL sIsIn = FALSE;

	if (!sIsIn)
	{
		sIsIn = TRUE;
		CString strNum;
		editCtrl.GetWindowText(strNum); // 获取编辑框的文本

		int  iPos = -1;

		for (int i = 0; i < strNum.GetLength(); i++)
		{
			if (strLimited.Find(strNum[i]) == -1 && strLimited != _T("")) // 逐个字符在所限制的字符集中查找
			{
				strNum = strNum.Left(i) + strNum.Mid(i + 1); // 从非法字符的输入位置处截断（不包括非法字符）
				iPos = i; // 记下截断的位置
				i--;
			}
		}

		if (strNum.GetLength() > nLenLimit && nLenLimit != 0)
		{
			strNum.Delete(nLenLimit, strNum.GetLength() - nLenLimit); //  若总长度大于限制的输入长度，则将超长部分删掉
			iPos = editCtrl.GetSel() % 256;
		}

		if (iPos != -1) // 判断位置变量状态
		{
			editCtrl.SetWindowText(strNum); // 重新设置编辑框的文本
			editCtrl.SetSel(iPos, iPos); // 定位光标
		}
		sIsIn = FALSE;
	}
}

void CCKSToolDlg::OnEnChangeMyEdit()
{
	EditCtrlLimit(m_input, 128, _T("0123456789ABCDEFabcdef ")); // 限制长度和输入类型
}

void CCKSToolDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	OnEnChangeMyEdit();
}
