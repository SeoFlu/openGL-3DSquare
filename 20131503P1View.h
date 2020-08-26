
// 20131503P1View.h : CMy20131503P1View 클래스의 인터페이스
//

#pragma once

typedef struct square {
	GLfloat PosX;
	GLfloat PosY;
	GLfloat PosZ;
	GLfloat distance;
	GLfloat StepX;
	GLfloat StepY;
	GLfloat StepZ;
}square;

class CMy20131503P1View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMy20131503P1View();
	DECLARE_DYNCREATE(CMy20131503P1View)

// 특성입니다.
public:
	CMy20131503P1Doc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CMy20131503P1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	HGLRC m_hRC;
	CDC* m_pDC;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool InitializeOpenGL();
	bool SetPixelFormat();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void RenderScene();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();

	GLfloat m_fPosX;
	GLfloat m_fPosY;
	GLfloat m_fPosZ;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nRotateX;
	int m_nRotateY;
	int m_nRotateZ;
	GLfloat m_fStepX;
	GLfloat m_fStepY;
	GLfloat m_fStepZ;
	int m_nPolygonCount;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void SetPosSquare(square* S, GLfloat X, GLfloat Y, GLfloat Z);
	square m_squareList[20];
	void GLdrawSquare(square* S, GLfloat R, GLfloat G, GLfloat B);
	void SetDisAng(square* S, GLfloat dis, GLfloat Ang);
};

#ifndef _DEBUG  // 20131503P1View.cpp의 디버그 버전
inline CMy20131503P1Doc* CMy20131503P1View::GetDocument() const
   { return reinterpret_cast<CMy20131503P1Doc*>(m_pDocument); }
#endif

