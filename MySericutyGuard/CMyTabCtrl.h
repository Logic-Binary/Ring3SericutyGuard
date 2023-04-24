#pragma once
#include<vector>
using std::vector;

// CMyTabCtrl

class CMyTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTabCtrl)

public:
	CMyTabCtrl();
	virtual ~CMyTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()

private:
	vector<CDialogEx*> m_dlg;
public:
	//添加子窗口
	void InitTabCtrl(int count, ...);
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
};


