
// opencvProjectDlg.h: 头文件
//

#pragma once
#include <iostream>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/types_c.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

// CopencvProjectDlg 对话框
class CopencvProjectDlg : public CDialogEx
{
// 构造
public:
	CopencvProjectDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVPROJECT_DIALOG };
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
	Mat src;
	CString weiPath;
	CString imgPath;
	CEdit infoCtrl;
	CString str;
	dnn::Net net;
public:
	CString loadImg();
	CString loadWei();
	void drawMat(cv::Mat _img, UINT _nID);
	void infoTextOut(CString _str);
	void infoTextOutNullR(CString _str);
	int find_max_prob_char(Mat pred, float t = 0.5);
	int yolo_predict(dnn::Net model, Mat img, int t = 0.5);
	int judge_License_Color(Mat inputMat);
	int mainFunction(Mat img);

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	
	afx_msg void OnEnChangeEdit3();
};
