
// CircleThroughThreePointsDlg.h: 헤더 파일
//

#pragma once

#include <vector>
#include <cmath>

using namespace std;


// CCircleThroughThreePointsDlg 대화 상자
class CCircleThroughThreePointsDlg : public CDialogEx
{
// 생성입니다.
public:
	CCircleThroughThreePointsDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLETHROUGHTHREEPOINTS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	vector<CPoint> m_vecPoints;

	int m_inPointRadius;
	int m_inLineThickness;
	int m_inDragPointIndex;

	BOOL m_bDragging;
	
	void SetMainCanvas(int nWidth, int nHeight);
	void UpdateDisplay();

	void DrawRandomThreeCircle(unsigned char* fm, int nRadius, int nGray);
	void DrawPoint(unsigned char* fm, int centerX, int centerY, int nRadius = 10, int nGray = 80); // 함수 이름 바꾸자 Points
	void DrawCircleLine(unsigned char* fm, int centerX, int centerY, int nRadius, int nThickness, int nGray);
	BOOL DrawCircleThroughThreePoints(unsigned char* fm);

	void DrawAllPoints(unsigned char* fm);

	BOOL SolveCircleCenter(CPoint A, CPoint B, CPoint C, CPoint& center, double& radius);
	BOOL ValidImgPos(int x, int y);


	int GetClickedPointIndex(CPoint point);

	void DisplayPointsInfo();
protected:
	HICON m_hIcon;
	CImage m_image;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonRadius();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonThick();
	afx_msg void OnBnClickedButtonReset();
};
