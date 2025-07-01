
// CircleThroughThreePointsDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircleThroughThreePoints.h"
#include "CircleThroughThreePointsDlg.h"
#include "afxdialogex.h"

#define MAX_POINT_COUNT 3
#define GRAY_CIRCLE		80


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CCircleThroughThreePointsDlg 대화 상자



CCircleThroughThreePointsDlg::CCircleThroughThreePointsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLETHROUGHTHREEPOINTS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bDragging			= FALSE;
	m_inPointRadius		= 0;
	m_inLineThickness	= 0;
	m_inDragPointIndex	= -1;
}

void CCircleThroughThreePointsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCircleThroughThreePointsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_RADIUS, &CCircleThroughThreePointsDlg::OnBnClickedButtonRadius)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_THICK, &CCircleThroughThreePointsDlg::OnBnClickedButtonThick)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CCircleThroughThreePointsDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CCircleThroughThreePointsDlg::OnBnClickedButtonRandom)
END_MESSAGE_MAP()


// CCircleThroughThreePointsDlg 메시지 처리기

BOOL CCircleThroughThreePointsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	MoveWindow(0, 0, 1600, 1300);

	GetDlgItem(IDC_EDIT_RADIUS)->MoveWindow(1400, 0, 100, 50);
	GetDlgItem(IDC_BUTTON_RADIUS)->MoveWindow(1500, 0, 70, 50);
	GetDlgItem(IDC_EDIT_THICK)->MoveWindow(1400, 50, 100, 50);
	GetDlgItem(IDC_BUTTON_THICK)->MoveWindow(1500, 50, 70, 50);
	GetDlgItem(IDC_BUTTON_RESET)->MoveWindow(1300, 100, 270, 50);
	GetDlgItem(IDC_BUTTON_RANDOM)->MoveWindow(1300, 150, 270, 50);

	GetDlgItem(IDC_STATIC_RADIUS)->MoveWindow(1310, 10, 90, 30);
	GetDlgItem(IDC_STATIC_THICKNESS)->MoveWindow(1310, 60, 90, 30);
	GetDlgItem(IDC_STATIC_POINTINFO1)->MoveWindow(1310, 250, 280, 50);
	GetDlgItem(IDC_STATIC_POINTINFO2)->MoveWindow(1310, 300, 280, 50);
	GetDlgItem(IDC_STATIC_POINTINFO3)->MoveWindow(1310, 350, 280, 50);

	GetDlgItem(IDC_STATIC_POINTINFO1)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC_POINTINFO2)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC_POINTINFO3)->SetWindowTextW(_T(""));

	SetMainCanvas(1300, 1300);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCircleThroughThreePointsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCircleThroughThreePointsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
	}
	else
	{
		CClientDC dc(this);
		m_image.Draw(dc, 0, 0);

		CDialogEx::OnPaint();
	}
}


// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleThroughThreePointsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCircleThroughThreePointsDlg::SetMainCanvas(int nWidth, int nHeight)
{
	m_image.Create(nWidth, -nHeight, 8);

	static RGBQUAD rgb[256];

	for (int i = 0; i < 256; i++)
	{
		rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
	}

	m_image.SetColorTable(0, 256, rgb);

	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);

}

void CCircleThroughThreePointsDlg::UpdateDisplay()
{
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

void CCircleThroughThreePointsDlg::DrawRandomThreePoints(unsigned char* fm)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	// 초기화
	memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);
	m_vecPoints.clear(); 

	for (int i = 0; i < MAX_POINT_COUNT; i++)
	{
		int inRandX = rand() % nWidth;
		int inRandY = rand() % nHeight;

		m_vecPoints.push_back(CPoint(inRandX, inRandY));
	}

	DrawAllPoints(fm);
	DrawCircleThroughThreePoints(fm);

	DisplayPointsInfo();
	UpdateDisplay();
}

void CCircleThroughThreePointsDlg::DrawAllPoints(unsigned char* fm)
{
	for (int i = 0; i < m_vecPoints.size(); i++)
	{
		DrawPoint(fm, m_vecPoints[i].x, m_vecPoints[i].y, m_inPointRadius, GRAY_CIRCLE);
	}

}

void CCircleThroughThreePointsDlg::DrawPoint(unsigned char* fm, int centerX, int centerY, int nRadius, int nGray)
{

	int nPitch = m_image.GetPitch();

	// 스캔 영역은 중심 기준 좌우 nRadius 범위
	for (int j = centerY - nRadius; j <= centerY + nRadius; j++)
	{
		for (int i = centerX - nRadius; i <= centerX + nRadius; i++)
		{
			double dx = i - centerX;
			double dy = j - centerY;
			double distSq = dx * dx + dy * dy;

			if (distSq < nRadius * nRadius)
			{
				if (ValidImgPos(i, j))
					fm[j * nPitch + i] = nGray;
			}
		}
	}

}


void CCircleThroughThreePointsDlg::DrawCircleLine(unsigned char* fm, int centerX, int centerY, int nRadius, int nThickness, int nGray)
{
	int nPitch = m_image.GetPitch();

	// 원 그리기 영역 두께만큼 추가

	for (int j = centerY - nRadius - (double)nThickness / 2; j <= centerY + nRadius + (double)nThickness / 2; j++)
	{
		for (int i = centerX - nRadius - (double)nThickness / 2; i <= centerX + nRadius + (double)nThickness / 2; i++)
		{
			double dx = i - centerX;
			double dy = j - centerY;

			// sqrt() 사용
			/*double dDist = sqrt(dx * dx + dy * dy);

			if (dDist >= nRadius - nThickness && dDist <= nRadius)
			{
				if (ValidImgPos(i, j))
					fm[j * nPitch + i] = nGray;
			}*/


			// 제곱으로 비교
			int outerSq = (nRadius + (double)nThickness / 2) * (nRadius + (double)nThickness / 2);
			int innerSq = (nRadius - (double)nThickness / 2) * (nRadius - (double)nThickness / 2);
			int distSq = dx * dx + dy * dy;

			if (distSq >= innerSq && distSq <= outerSq)
			{
				if (ValidImgPos(i, j))
					fm[j * nPitch + i] = nGray;
			}
		}
	}

}

BOOL CCircleThroughThreePointsDlg::ValidImgPos(int x, int y)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	CRect rect(0, 0, nWidth, nHeight);

	return rect.PtInRect(CPoint(x, y));
}

void CCircleThroughThreePointsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (point.x > 1300 || point.y > 1300)
		return;

	if (m_inLineThickness == 0 || m_inPointRadius == 0)
	{
		AfxMessageBox(_T("반지름, 두께 정보를 입력해주세요."));
		return;
	}
		
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	if (m_vecPoints.size() >= MAX_POINT_COUNT)
	{
		// 세점에 포함된 좌표를 누르고있는지 체크
		int index = GetClickedPointIndex(point);
		if (index != -1)
		{
			m_bDragging = TRUE;
			m_inDragPointIndex = index;

			m_vecPoints[index] = point;
		}
	}
	else
	{
		DrawPoint(fm, point.x, point.y, m_inPointRadius, GRAY_CIRCLE);

		m_vecPoints.push_back(point);

		if (m_vecPoints.size() >= MAX_POINT_COUNT)
			DrawCircleThroughThreePoints(fm);

		DisplayPointsInfo();
		UpdateDisplay();
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircleThroughThreePointsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

	m_bDragging = FALSE;
	m_inDragPointIndex = -1;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircleThroughThreePointsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging && m_inDragPointIndex > -1)
	{
		unsigned char* fm = (unsigned char*)m_image.GetBits();

		int nWidth = m_image.GetWidth();
		int nHeight = m_image.GetHeight();

		// 화면 초기화
		memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);

		// 세점과 지나는 원 다시 그리기 
		m_vecPoints[m_inDragPointIndex] = point;

		DrawAllPoints(fm);
		DrawCircleThroughThreePoints(fm);
		DisplayPointsInfo();
		UpdateDisplay();
	}


	CDialogEx::OnMouseMove(nFlags, point);
}

int CCircleThroughThreePointsDlg::GetClickedPointIndex(CPoint point)
{
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	for (int i = 0; i < m_vecPoints.size(); i++)
	{
		CRect rect(
			m_vecPoints[i].x - m_inPointRadius
			, m_vecPoints[i].y - m_inPointRadius
			, m_vecPoints[i].x + m_inPointRadius
			, m_vecPoints[i].y + m_inPointRadius);

		if (rect.PtInRect(point))
		{
			if (fm[point.y * nPitch + point.x] == GRAY_CIRCLE)
				return i;
		}

	}

	return -1;
}

BOOL CCircleThroughThreePointsDlg::DrawCircleThroughThreePoints(unsigned char* fm)
{
	CPoint	pCenter;
	double	dCenterRadius = 0;

	BOOL bResult = TRUE;

	bResult = SolveCircleCenter(m_vecPoints[0], m_vecPoints[1], m_vecPoints[2], pCenter, dCenterRadius);

	if (bResult)
		DrawCircleLine(fm, pCenter.x, pCenter.y, dCenterRadius, m_inLineThickness, GRAY_CIRCLE);

	return bResult;

}

BOOL CCircleThroughThreePointsDlg::SolveCircleCenter(CPoint A, CPoint B, CPoint C, CPoint& center, double& radius) {
	// (cx - x1)^2 + (cy - y1)^2 = (cx - x2)^2 + (cy - y2)^2
	// 전개 후 정리하면 아래와 같은 1차 연립방정식 형태가 됨

	double x1 = A.x, y1 = A.y;
	double x2 = B.x, y2 = B.y;
	double x3 = C.x, y3 = C.y;

	double a1 = 2 * (x2 - x1);
	double b1 = 2 * (y2 - y1);
	double c1 = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1;

	double a2 = 2 * (x3 - x1);
	double b2 = 2 * (y3 - y1);
	double c2 = x3 * x3 + y3 * y3 - x1 * x1 - y1 * y1;

	double denom = a1 * b2 - a2 * b1;

	if (abs(denom) < 1e-10)
		return false;  // 세 점이 일직선인 경우

	center.x = (c1 * b2 - c2 * b1) / denom;
	center.y = (a1 * c2 - a2 * c1) / denom;

	// 반지름 계산 (아무 점이랑 중심 거리)
	double dx = center.x - x1;
	double dy = center.y - y1;
	radius = sqrt(dx * dx + dy * dy);

	return true;
}

void CCircleThroughThreePointsDlg::OnBnClickedButtonRadius()
{
	// PointRadius 값 저장
	
	int inRadius = 0;
	CString csRadius = _T("");
	GetDlgItem(IDC_EDIT_RADIUS)->GetWindowTextW(csRadius);

	if (!csRadius.IsEmpty())
	{
		inRadius = _ttoi(csRadius);

		if (inRadius <= 0)
		{
			AfxMessageBox(_T("0 이하는 입력할 수 없습니다."));
			return;
		}

		m_inPointRadius = inRadius;
	}
}

void CCircleThroughThreePointsDlg::OnBnClickedButtonThick()
{
	// LineThickness 값 저장
	
	int inThickness = 0;
	CString csThickness = _T("");
	GetDlgItem(IDC_EDIT_THICK)->GetWindowTextW(csThickness);

	if (!csThickness.IsEmpty())
	{
		inThickness = _ttoi(csThickness);

		if (inThickness <= 0)
		{
			AfxMessageBox(_T("0 이하는 입력할 수 없습니다."));
			return;
		}

		m_inLineThickness = inThickness;
	}
}

void CCircleThroughThreePointsDlg::OnBnClickedButtonReset()
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 초기화
	memset(fm, 0xff, sizeof(unsigned char) * nWidth * nHeight);
	m_vecPoints.clear();

	GetDlgItem(IDC_STATIC_POINTINFO1)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC_POINTINFO2)->SetWindowTextW(_T(""));
	GetDlgItem(IDC_STATIC_POINTINFO3)->SetWindowTextW(_T(""));

	UpdateDisplay();
}

void CCircleThroughThreePointsDlg::DisplayPointsInfo()
{
	CString csTmp = _T("");

	for (int i = 0; i < m_vecPoints.size(); i++)
	{
		csTmp.Format(_T("Point %d (x:%d, y:%d)"), i + 1, m_vecPoints[i].x, m_vecPoints[i].y);
		GetDlgItem(IDC_STATIC_POINTINFO1 + i)->SetWindowTextW(csTmp);
	}

}
void CCircleThroughThreePointsDlg::OnBnClickedButtonRandom()
{
	std::thread _thread0(threadDrawRandomPoints, this);
	_thread0.detach();
}


void CCircleThroughThreePointsDlg::threadDrawRandomPoints(CWnd* pParent)
{
	CCircleThroughThreePointsDlg* pWnd = (CCircleThroughThreePointsDlg*)pParent;

	unsigned char* fm = (unsigned char*)pWnd->m_image.GetBits();

	for (int i = 0; i < 10; i++)
	{
		pWnd->DrawRandomThreePoints(fm);

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}