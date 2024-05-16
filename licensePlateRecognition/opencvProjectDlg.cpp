
// opencvProjectDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "opencvProject.h"
#include "opencvProjectDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

String classnames[70] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"A", "B", "C", "D", "E", "F", "G", "H", "J", "K", "L", "M", "N", "P", "Q",
	"R", "S", "T", "U", "V", "W", "X", "Y", "Z", "川", "鄂", "赣",
	"甘", "贵", "桂", "zh_hei", "沪", "zh_ji", "zh_jin",
	"zh_jing", "zh_jl", "zh_liao", "zh_lu", "zh_meng", "zh_闽", "zh_ning",
	"zh_qing", "zh_qiong", "zh_shan", "zh_su", "zh_sx", "zh_wan", "zh_xiang"
	, "zh_xin", "zh_yu", "zh_yu1", "zh_yue", "zh_yun", "zh_zhe" };

struct License
{
	Mat mat;  //ROI图片
	Rect rect; //ROI所在矩形
	int color;
};

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
END_MESSAGE_MAP()


// CopencvProjectDlg 对话框



CopencvProjectDlg::CopencvProjectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVPROJECT_DIALOG, pParent)
	, weiPath(_T(""))
	, imgPath(_T(""))
	, str(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CopencvProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, weiPath);
	DDX_Text(pDX, IDC_EDIT2, imgPath);
	DDX_Control(pDX, IDC_EDIT3, infoCtrl);
	DDX_Text(pDX, IDC_EDIT3, str);
}

BEGIN_MESSAGE_MAP(CopencvProjectDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CopencvProjectDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CopencvProjectDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CopencvProjectDlg::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT3, &CopencvProjectDlg::OnEnChangeEdit3)
END_MESSAGE_MAP()


// CopencvProjectDlg 消息处理程序

BOOL CopencvProjectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("车牌识别系统"));
	infoTextOut(_T("请先加载权重文件和图像"));
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CopencvProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CopencvProjectDlg::OnPaint()
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
HCURSOR CopencvProjectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CopencvProjectDlg::loadImg()
{
	CString img = L"";
	CFileDialog dlg(true, _T("*.bmp;*png;*.jpg"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("image files (*.bmp ;*.jpg)|*.bmp; *.jpg |ALL Files (*.*) |*.*||"), NULL);

	//打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("open Image ");

	if (dlg.DoModal() == IDOK) {
		img = dlg.GetPathName();
	}
	else {
		return 0;
	}
	return img;
}

CString CopencvProjectDlg::loadWei()
{
	CString wei = L"";
	CFileDialog dlg(true, _T("*.onnx"), NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T("weight files (*.onnx)|*.onnx|ALL Files (*.*) |*.*||"), NULL);

	//打开文件对话框的标题名
	dlg.m_ofn.lpstrTitle = _T("open Wright Image ");

	if (dlg.DoModal() == IDOK) {
		wei = dlg.GetPathName();
	}
	else {
		return 0;
	}
	return wei;
}

void CopencvProjectDlg::drawMat(cv::Mat _img, UINT _nID)
{
	Mat imgTmp;
	CRect rect;
	GetDlgItem(_nID)->GetClientRect(&rect);  // 获取控件大小
	cv::resize(_img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩放Mat并备份
	// 转一下格式 ,这段可以放外面,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // 计算一个像素多少个字节
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap数据头 -> MFC
	CDC* pDC = GetDlgItem(_nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

int CopencvProjectDlg::find_max_prob_char(Mat pred, float t)
{
	infoTextOutNullR(_T("\r\n=======寻找可信度最高的字符======="));
	// 输出最可能的结果
	int max_prob_arg = -1;
	double max_prob = -1;
	// int t = 0.5; // 阈值，低于这个阈值，则没有匹配到任何字符

	for (int i = 0; i < pred.rows; i++) {
		for (int j = 0; j < pred.cols; j++) {

			if (pred.at<float>(i, j) > max_prob) {
				max_prob_arg = j;
				max_prob = pred.at<float>(i, j);
			}
		}
	}

	if (max_prob > t) {
		infoTextOutNullR(_T("\r\n最有可能的字符为: "));
		infoTextOutNullR((CString)(classnames[max_prob_arg].c_str()));
		infoTextOutNullR(_T("\r\n可信度： "));
		CString maxProb;
		maxProb.Format(_T("%0.4f"), max_prob);
		infoTextOutNullR(maxProb);
		infoTextOutNullR(_T("\r\n"));
		//std::cout << "最有可能的字符为：" << classnames[max_prob_arg] << endl;
		//std::cout << "可信度为：  " << max_prob << endl;
		return max_prob_arg;
	}
	else {
		infoTextOut(_T("未发现可行解"));
		//std::cout << "未发现可行解" << endl;
		return -1;
	}
}

int CopencvProjectDlg::yolo_predict(dnn::Net model, Mat img, int t)
{
	GaussianBlur(img, img, Size(3, 3), 0, 0);
	Ptr<CLAHE> clahe = createCLAHE(2.0, Size(5, 5));
	// 限制对比度的自适应直方图均衡化
	//clahe->apply(img, img);
	// 填充边界
	img = img.clone();
	if (img.cols > img.rows)
		copyMakeBorder(img, img, (img.cols - img.rows) / 2, (img.cols - img.rows) / 2, 0, 0, BORDER_CONSTANT, 0);
	else
		copyMakeBorder(img, img, 0, 0, (img.rows - img.cols) / 2, (img.rows - img.cols) / 2, BORDER_CONSTANT, 0);

	dilate(img, img, getStructuringElement(MORPH_RECT, Size(3, 3)));
	erode(img, img, getStructuringElement(MORPH_RECT, Size(3, 3)));

	cvtColor(img, img, cv::COLOR_GRAY2BGR);
	//cv::imshow("ayyy", img);
	//cv::waitKey(1000);
	Mat inputBlob = dnn::blobFromImage(img, 1.0 / 256, Size(64, 64), Scalar(), true, false);
	model.setInput(inputBlob);//set the network input, "data" is the name of the input layer     
	Mat pred = model.forward();
	// cout << pred.size<< "   " << pred.rows << "    " << pred.cols << endl;

	// 输出最可能的结果
	return find_max_prob_char(pred);
}

int CopencvProjectDlg::judge_License_Color(Mat inputMat)
{
	Mat hsv, mask;
	cvtColor(inputMat, hsv, COLOR_BGR2HSV);
	// 判断是否为蓝色
	inRange(hsv, Scalar(100, 80, 46), Scalar(124, 255, 255), mask);

	float num_color_blocks = 0;
	for (int i = 0; i < mask.rows; i++)
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) == 255)
				num_color_blocks++;
		}
	// imshow("mask", mask);
	// waitKey(0);
	if (num_color_blocks / mask.rows / mask.cols >= 0.4) return 1;

	//判断是否为橙色
	inRange(hsv, Scalar(11, 43, 46), Scalar(25, 255, 255), mask);

	num_color_blocks = 0;
	for (int i = 0; i < mask.rows; i++)
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) == 255)
				num_color_blocks++;
		}
	// imshow("mask", mask);
	// waitKey(0);
	if (num_color_blocks / mask.rows / mask.cols >= 0.4) return 2;

	return -1;
}

int CopencvProjectDlg::mainFunction(Mat img)
{

	//---------------------------------------图像预处理----------------------------------------------
	Mat img_blur, hsv, mask;
	// 滤波
	medianBlur(img, img_blur, 3);
	GaussianBlur(img_blur, img_blur, Size(9, 9), 0, 0);
	cvtColor(img_blur, hsv, COLOR_BGR2HSV);

	// 查找车牌轮廓
	vector<License> License_ROIs; // 存储车牌轮廓
	//使用 RETR_EXTERNAL 找到最外轮廓
	vector<vector<Point>>contours, contours_blue, contours_orange, contours_yellow;
	vector<Vec4i> hierarchy, hierarchy_blue, hierarchy_orange, hierarchy_yellow;


	//-------------------------------利用色块进行掩膜----------------------------------------------
	//-------------------------------蓝色---------------------------------------------------------
	inRange(hsv, Scalar(100, 80, 46), Scalar(124, 255, 255), mask);
	erode(mask, mask, getStructuringElement(MORPH_RECT, Size(5, 5)));
	dilate(mask, mask, getStructuringElement(MORPH_RECT, Size(3, 3)));
	cv::findContours(mask, contours_blue, hierarchy_blue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//imshow("mask", mask);
	//waitKey(0);
	//-------------------------------黄色---------------------------------------------------------
	inRange(hsv, Scalar(11, 43, 46), Scalar(25, 255, 255), mask);
	erode(mask, mask, getStructuringElement(MORPH_RECT, Size(5, 5)));
	dilate(mask, mask, getStructuringElement(MORPH_RECT, Size(3, 3)));
	cv::findContours(mask, contours_orange, hierarchy_orange, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//imshow("mask", mask);
	//waitKey(0);
	//-------------------------------新能源---------------------------------------------------------
	inRange(hsv, Scalar(30, 10, 46), Scalar(90, 255, 255), mask);
	erode(mask, mask, getStructuringElement(MORPH_RECT, Size(5, 5)));
	dilate(mask, mask, getStructuringElement(MORPH_RECT, Size(5, 5)));
	cv::findContours(mask, contours_yellow, hierarchy_yellow, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
	//imshow("mask", mask);
	//waitKey(0);


	for (int i = 0; i < contours_orange.size(); i++) {
		contours.push_back(contours_orange[i]);
		hierarchy.push_back(hierarchy_orange[i]);
	}

	for (int i = 0; i < contours_blue.size(); i++) {
		contours.push_back(contours_blue[i]);
		hierarchy.push_back(hierarchy_blue[i]);
	}

	for (int i = 0; i < contours_yellow.size(); i++) {
		contours.push_back(contours_yellow[i]);
		hierarchy.push_back(hierarchy_yellow[i]);
	}


	//-----------------------------------提取车牌轮廓----------------------------------------------------
	vector<vector<Point>>conPoly(contours.size());

	Mat imageContours = Mat::zeros(img.size(), CV_8UC1);
	Mat Contours = Mat::zeros(img.size(), CV_8UC1);
	for (int i = 0; i < contours.size(); i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
		double area = contourArea(contours[i]);
		double peri = arcLength(contours[i], true);
		if (area > 1000) {
			//使用多边形近似，进一步确定车牌区域轮廓
			approxPolyDP(contours[i], conPoly[i], 40, true);

			if (conPoly[i].size())
			{
				//计算矩形区域宽高比
				Rect box = boundingRect(contours[i]);
				double ratio = double(box.width) / double(box.height);
				/*
				RotatedRect box = minAreaRect(contours[i]);
				double ratio = double(box.size.width) / double(box.size.height) > 1 ?
					double(box.size.width) / double(box.size.height) :
					double(box.size.height) / double(box.size.width);*/
				if (ratio > 2.5 && ratio < 4.5 && area / box.area() > 0.6)
				{
					//截取ROI区域
					Rect rect = box;

					if (rect.y - 3 > 0) {
						rect.y -= 3;
					}
					License License_ROI = { img(rect),rect };
					cout << "矩形面积:    " << box.area() << endl;
					cout << "轮廓面积:    " << area << endl;
					License_ROIs.push_back(License_ROI);


					for (int j = 0; j < contours[i].size(); j++)
					{
						//绘制出contours向量内所有的像素点
						Point P = Point(contours[i][j].x, contours[i][j].y);
						Contours.at<uchar>(P) = 255;
					}
					//绘制轮廓
					drawContours(imageContours, contours, i, Scalar(255), 1, 8, hierarchy);


					//cv::imshow("Contours Image", imageContours); //轮廓
					//cv::imshow("Point of Contours", Contours);   //向量contours内保存的所有轮廓点集
					//cv::waitKey(2000);
				}
			}
		}
	}


	Mat result_mat = img.clone();// 存储最后的结果图像

	if (License_ROIs.empty()) {
		infoTextOut(_T("没有找到车牌"));
		//cout << "没有找到车牌" << endl;
	}
	for (int i = 0; i < License_ROIs.size(); i++) {
		vector<String> result_str_list; // 存储字符

		//----------------------------判断车牌颜色------------------------------------------------
		int license_color = -1;
		license_color = judge_License_Color(img(License_ROIs[i].rect));
		if (license_color == 1)
			infoTextOut(_T("为蓝色车牌"));
			//cout << "为蓝色车牌" << endl;
		else
			infoTextOut(_T("非蓝色车牌"));
			//cout << "非蓝色车牌" << endl;

		//---------------------------对车牌预处理------------------------------------------------
		Mat gray_ROI, gray_ROI_resized;
		cvtColor(img(License_ROIs[i].rect), gray_ROI, COLOR_BGR2GRAY);

		if (license_color != 1) bitwise_not(gray_ROI, gray_ROI);

		int width = gray_ROI.cols;
		int height = gray_ROI.rows;
		// 按比例进行缩放
		resize(gray_ROI, gray_ROI_resized, Size(0, 0), (float)64 / height, (float)64 / height);
		width = gray_ROI_resized.cols;
		height = gray_ROI_resized.rows;
		Mat thresh;
		threshold(gray_ROI_resized, thresh, 127, 255, THRESH_BINARY | THRESH_OTSU);
		//输出到控件
		drawMat(thresh, IDC_SHOW_IMG);
		//imshow("车牌图像", thresh);

		//---------------------------根据像素分布分割图像-----------------------------------------

		//---------------------------按照竖直像素进行分割-----------------------------------------
		int perPixelValue;//每个像素的值
		int* projectValArry_rows = new int[height];//创建一个用于储存每列白色像素个数的数组
		memset(projectValArry_rows, 0, height * 4);//必须初始化数组

		//遍历每一行,查找每一行255的值
		for (int row = 0; row < height; ++row)
		{
			for (int col = 0; col < width; ++col)
			{
				perPixelValue = thresh.at<uchar>(row, col);
				if (perPixelValue == 255)//如果是黑底白字
				{
					projectValArry_rows[row]++;
				}
			}
		}

		/*新建一个Mat用于储存投影直方图并将背景置为白色*/
		Mat horizontalProjectionMat(height, width, CV_8UC1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				perPixelValue = 255;  //背景设置为白色。   
				horizontalProjectionMat.at<uchar>(i, j) = perPixelValue;
			}
		}
		// 绘制直方图
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < projectValArry_rows[i]; j++)
			{
				perPixelValue = 0;  //直方图设置为黑色  
				horizontalProjectionMat.at<uchar>(i, width - 1 - j) = perPixelValue;
			}
		}
		// 分割直方图
		vector<Mat> roiList_rows;//用于储存分割出来的每个字符
		int startIndex = 0;//记录进入字符区的索引
		int endIndex = 0;//记录进入空白区域的索引
		bool inBlock = false;//是否遍历到了字符区内

		int char_length = 0;
		Mat roiImg;
		for (int i = 0; i < height - 2; ++i)
		{
			if (!inBlock && projectValArry_rows[i] > 20)//进入字符区了
			{
				inBlock = true;
				startIndex = i;
				//cout << "startIndex is " << startIndex << endl;
			}
			else if ((projectValArry_rows[i] < 20 && inBlock) || i == height - 2)//进入空白区了
			{
				endIndex = i;
				inBlock = false;
				//cout << endIndex << endl;
				if (endIndex < height - 2)
					roiImg = thresh(Range(startIndex, endIndex + 1), Range(0, gray_ROI_resized.cols));
				else roiImg = thresh(Range(startIndex, height - 1), Range(0, gray_ROI_resized.cols));
				if (-startIndex + endIndex > 10) {
					roiList_rows.push_back(roiImg);
					break;
				}
			}
		}

		Mat result;
		cout << roiList_rows.size() << "   " << "水平分解总数" << endl;
		if (roiList_rows.size() == 0) {
			result = thresh;
			infoTextOut(_T("水平分解失败"));
			//cout << "水平分解失败" << endl;
		}
		else {
			int max_length = 0;
			int arg_max = 0;
			for (int i = 0; i < roiList_rows.size(); i++) {
				if (roiList_rows[i].rows > max_length) {
					arg_max = i;
					max_length = roiList_rows[i].rows;
				}
			}
			result = roiList_rows[0];
		}

		//-------------------------------------------按水平分布直方图进行分割----------------------------------------------------------
		width = result.cols;
		height = result.rows;

		Mat close;
		// 闭运算去除噪点
		morphologyEx(result, close, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3)));

		int* projectValArry = new int[width];//创建一个用于储存每列白色像素个数的数组
		memset(projectValArry, 0, width * 4);//必须初始化数组

		//遍历每一列的图像灰度值，查找每一行255的值
		for (int col = 0; col < width; ++col)
		{
			for (int row = 0; row < height; ++row)
			{
				perPixelValue = close.at<uchar>(row, col);
				if (perPixelValue == 255)//如果是黑底白字
				{
					projectValArry[col]++;
				}
			}
		}

		/*新建一个Mat用于储存投影直方图并将背景置为白色*/
		Mat verticalProjectionMat(height, width, CV_8UC1);

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				perPixelValue = 255;  //背景设置为白色。   
				verticalProjectionMat.at<uchar>(i, j) = perPixelValue;
			}
		}

		/*将直方图的曲线设为黑色*/
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < projectValArry[i]; j++)
			{
				perPixelValue = 0;  //直方图设置为黑色  
				verticalProjectionMat.at<uchar>(height - 1 - j, i) = perPixelValue;
			}
		}

		// 展示直方图
		//cv::imshow("【水平投影】", verticalProjectionMat);
		//cv::imshow("【垂直投影】", horizontalProjectionMat);

		//waitKey(2000);

		// 分割直方图
		vector<Mat> roiList;//用于储存分割出来的每个字符
		startIndex = 0;//记录进入字符区的索引
		endIndex = 0;//记录进入空白区域的索引
		inBlock = false;//是否遍历到了字符区内


		char_length = 0;
		for (int i = 0; i < width; ++i)
		{
			if (!inBlock && projectValArry[i] > 5)//进入字符区了
			{
				inBlock = true;
				startIndex = i;
				//cout << "startIndex is " << startIndex << endl;
			}
			else if (projectValArry[i] < 5 && inBlock)//进入空白区了
			{
				endIndex = i;
				inBlock = false;
				Mat roiImg = result(Range(0, result.rows), Range(startIndex, endIndex + 1));
				if (-startIndex + endIndex > 1)
					roiList.push_back(roiImg);
			}
		}
		if (-startIndex + width - 1 > 1) {
			roiImg = result(Range(0, result.rows), Range(startIndex, width - 1));
			roiList.push_back(roiImg);
		}


		// ---------------------------------利用YOLO预测字符--------------------------------------------
		string RESULT_ = "";
		for (int i = 0; i < roiList.size(); i++) {
			if (i == 0) {
				drawMat(roiList[i], IDC_END_01);
			}
			if (i == 1) {
				drawMat(roiList[i], IDC_END_02);
			}
			if (i == 2) {
				drawMat(roiList[i], IDC_END_03);
			}
			if (i == 3) {
				drawMat(roiList[i], IDC_END_04);
			}
			if (i == 4) {
				drawMat(roiList[i], IDC_END_05);
			}
			if (i == 5) {
				drawMat(roiList[i], IDC_END_06);
			}
			if (i == 6) {
				drawMat(roiList[i], IDC_END_07);
			}
			if (i == 7) {
				drawMat(roiList[i], IDC_END_08);
			}
			//哥们使用简单的方式，编译器报异常。还得靠if 大法 ~_~
			//drawMat(roiList[i],IDC_END_01);
			//drawMat(roiList[i],atoi(id[i].c_str()));
			//imshow(to_string(i), roiList[i]);
			int r_i = yolo_predict(net, roiList[i], 0.1);	// 使用yolo算法进行分类预测


			int num_chars = 7;
			if (license_color == -1)  num_chars = 8;
			//------------------整理字符------------------------
			if (r_i != -1 && result_str_list.size() < num_chars) {
				// 如果是可行解

				if (result_str_list.size() == 0) {
					// 判断是否为首字符
					if (r_i >= 34)// 判断是否为中文字符
					{
						result_str_list.push_back(classnames[r_i]);
					}
				}
				else if (result_str_list.size() == 1)
					// 判断是否为次字符, 必然为英文字母
					if (r_i < 34 && r_i >= 10)// 判断是否为中文字符
						result_str_list.push_back(classnames[r_i]);
					else if (r_i == 6)
						result_str_list.push_back("G");
					else
						result_str_list.push_back(classnames[r_i]);
				else {
					if (r_i >= 34)// 判断是否为中文字符
					{
						if (r_i == 62)
							result_str_list.push_back("6");
						else
							result_str_list.push_back("1");
					}
					else {
						result_str_list.push_back(classnames[r_i]);
					}
				}
			}

		}

		infoTextOut(_T("结果为"));
		for (int i = 0; i < result_str_list.size(); i++) {
			//cout << result_str_list[i] << "--";
			if (i > 0)RESULT_ += result_str_list[i];
		}
		infoTextOut((CString)RESULT_.c_str());
		//------------------------------------绘制预测结果------------------------------
		if (result_str_list.size() > 3) {
			rectangle(result_mat, License_ROIs[i].rect, Scalar(255, 0, 0), 3);
			putText(result_mat, RESULT_, Point(License_ROIs[i].rect.x, License_ROIs[i].rect.y + License_ROIs[i].rect.height), 0, 2, Scalar(255, 255, 0), 4);
		}
		//imshow("result_img", result_mat);
		drawMat(result_mat, IDC_SHOW_SRCIMG);
	}
	waitKey(000);


	return 1;
}

void CopencvProjectDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	weiPath = loadWei();
	UpdateData(false);
	net = cv::dnn::readNetFromONNX(W2A(weiPath));
	infoTextOut(_T("加载权重成功，从ONNX读取模型"));
}

void CopencvProjectDlg::infoTextOut(CString _str)
{
	
	int LineCnt = infoCtrl.GetLineCount();
	if (LineCnt > 100)
	{
		infoCtrl.SetWindowText(_T(""));
	}

	CString tmpStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	tmpStr.Format(_T(">[%02d:%02d:%02d]"), sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	str += tmpStr;	// 添加log显示时间
	str += _str;	// 需要显示的信息
	str += "\r\n";	// 换行
	UpdateData(FALSE);
	infoCtrl.LineScroll(infoCtrl.GetLineCount());
}

void CopencvProjectDlg::infoTextOutNullR(CString _str)
{
	int LineCnt = infoCtrl.GetLineCount();
	if (LineCnt > 100)
	{
		infoCtrl.SetWindowText(_T(""));
	}

	CString tmpStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	tmpStr.Format(_T(""));
	str += tmpStr;	// 添加log显示时间
	str += _str;	// 需要显示的信息
	//str += "\r\n";	// 换行
	UpdateData(FALSE);
	infoCtrl.LineScroll(infoCtrl.GetLineCount());
}

void CopencvProjectDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	USES_CONVERSION;
	imgPath = loadImg();
	UpdateData(false);
	src = cv::imread(W2A(imgPath));
	drawMat(src, IDC_SHOW_SRCIMG);
	infoTextOut(_T("图像加载成功"));
}

void CopencvProjectDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (imgPath == "" || weiPath == "") {
		infoTextOut(_T("请先加载权重文件和图像文件"));
	}
	else {
		infoTextOut(_T("开始检测，正在检测中..."));
		mainFunction(src);
		infoTextOut(_T("检测完成"));
	}
	
}


void CopencvProjectDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
