
// CKS_ToolDlg.h: 头文件
//

#pragma once

typedef enum ERROR_TYPE {
	EMPTY = -1,
	INVALID = 0,
	VALID = 1,
} Error_Type;

// CCKSToolDlg 对话框
class CCKSToolDlg : public CDialogEx
{
// 构造
public:
	CCKSToolDlg(CWnd* pParent = nullptr);	// 标准构造函数
	enum MixDefine {
		ACCUMULATION = 0,
		XOR,
	};

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CKS_TOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CByteArray m_hexArry;
	CString m_Output;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_checktype;
	CEdit m_input;
	CEdit m_output;
	afx_msg void OnBnClickedButtonCalc();
	Error_Type GetInput();
	void SplitStringBySpace(CString str);
private:
	BYTE xorAssert();
	BYTE accAssert();
	void EditCtrlLimit(CEdit& editCtrl, int nLenLimit, CString strLimited);
	void OnEnChangeMyEdit();
public:
	afx_msg void OnEnChangeEdit1();
};
