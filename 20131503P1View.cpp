
// 20131503P1View.cpp : CMy20131503P1View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "20131503P1.h"
#endif

#include "20131503P1Doc.h"
#include "20131503P1View.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy20131503P1View

IMPLEMENT_DYNCREATE(CMy20131503P1View, CView)

BEGIN_MESSAGE_MAP(CMy20131503P1View, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CMy20131503P1View 생성/소멸

CMy20131503P1View::CMy20131503P1View()
	: m_pDC(NULL)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_nRotateX(0)
	, m_nRotateY(0)
	, m_nRotateZ(0)
	, m_fStepX(0)
	, m_fStepY(0)
	, m_fStepZ(0)
	, m_nPolygonCount(0)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_fPosX = 0;
	m_fPosY = 0;
	m_fPosZ = 0;
	m_fStepX = cos(0.6) + sin(0.4) +5;
	m_fStepY = cos(0.6) + sin(0.4) + 5;
	m_fStepZ = cos(0.6) + sin(0.4) + 5;
	
	m_nPolygonCount = 0;

	SetPosSquare(m_squareList, 0, 0, 0);
	m_squareList[0].distance = 50;
	m_squareList[0].StepX = m_squareList[0].StepY = m_squareList[0].StepZ = cos(0.6) + sin(0.4) + 5;


}

CMy20131503P1View::~CMy20131503P1View()
{
}

BOOL CMy20131503P1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

// CMy20131503P1View 그리기

void CMy20131503P1View::OnDraw(CDC* pDC)
{
	CMy20131503P1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.


	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RenderScene();
	::glFinish();

	::SwapBuffers(m_pDC->GetSafeHdc());

}


// CMy20131503P1View 진단

#ifdef _DEBUG
void CMy20131503P1View::AssertValid() const
{
	CView::AssertValid();
}

void CMy20131503P1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy20131503P1Doc* CMy20131503P1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy20131503P1Doc)));
	return (CMy20131503P1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy20131503P1View 메시지 처리기


int CMy20131503P1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	InitializeOpenGL();

	return 0;
}


bool CMy20131503P1View::InitializeOpenGL()
{
	m_pDC = new CClientDC(this); // dc클라이언트 권한 얻고,

	if (m_pDC == NULL) {
		MessageBox(_T("Error Obtaining DC"));
		return FALSE;
	}
	//셋업이놈은 뭐하는놈이냐
	if (!SetPixelFormat()) {
		return FALSE;
	}
	//dc로부터 렌더링 컨테스트를 생성하고 성공했나 검사
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	if (m_hRC == 0) {
		MessageBox(_T("Error Creating RC"));
		return FALSE;
	}
	// dc안에 rc를 집어넣는데 실패하면 뜨는 오류메세지
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE) {
		MessageBox(_T("Error making RC Current"));
		return FALSE;
	}
	// 배경화면 검정색
	::glClearColor(0.0, 0.0, 0.0, 0.0);
	// 배경화면 깊이를 조절
	::glClearDepth(1.0);
	// 깊이 테스트?
	::glEnable(GL_DEPTH_TEST);

	return TRUE;
}


bool CMy20131503P1View::SetPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0) {
		return FALSE;
	}

	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE) {
		return FALSE;
	}
	
	return TRUE;
}


void CMy20131503P1View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	GLdouble aspect_ratio;

	if (0 >= cx || 0 >= cy) {
		return;
	}
	::glViewport(0, 0, cx, cy);

	aspect_ratio = (GLdouble)cx / (GLdouble)cy;

	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();

	::gluPerspective(45.0f, aspect_ratio, 0.01f, 8000.0f);

	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
}


void CMy20131503P1View::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// 배경
	{
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -250);
	
		glColor3f(1, 1, 1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_POLYGON);
		{
			glVertex3f(-110, 70, -800);
			glVertex3f(-110, 70, 100);
			glVertex3f(-110, -70, 100);
			glVertex3f(-110, -70, -800); // left
		}
		glEnd();
		glBegin(GL_POLYGON);
		{
			glVertex3f(110, 70, 100);
			glVertex3f(-110, 70, 100);
			glVertex3f(-110, 70, -800);
			glVertex3f(110, 70, -800); // top
		}
		glEnd();
		glBegin(GL_POLYGON);
		{

			glVertex3f(110, 70, 100);
			glVertex3f(110, 70, -800);
			glVertex3f(110, -70, -800);
			glVertex3f(110, -70, 100); // right
		}
		glEnd();
		glBegin(GL_POLYGON);
		{
			glVertex3f(110, -70, -800);
			glVertex3f(-110, -70, -800);
			glVertex3f(-110, -70, 100);
			glVertex3f(110, -70, 100); // bottom
		}
		glEnd();
		glPolygonMode(GL_FRONT, GL_FILL);
		glBegin(GL_POLYGON);
		{
			glVertex3f(110, 70, -800);
			glVertex3f(-110, 70, -800);
			glVertex3f(-110, -70, -800);
			glVertex3f(110, -70, -800); // forward
		}
		glEnd();

	} 

	// 인터페이스 
	switch (m_nPolygonCount)
	{
	case 0:
		GLdrawSquare(m_squareList,0,0,0.6);
		break;
	case 1:
		GLdrawSquare(m_squareList,0,0,0.6);
		GLdrawSquare(m_squareList+1, 0, 0, 0.6);
		GLdrawSquare(m_squareList+2, 0, 0, 0.6);
		GLdrawSquare(m_squareList+3, 0, 0, 0.6);
		GLdrawSquare(m_squareList+4, 0, 0, 0.6);
		GLdrawSquare(m_squareList+5, 0, 0, 0.6);
		GLdrawSquare(m_squareList+6, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 7, 0, 0, 0.6);
		break;
	case 2:
		GLdrawSquare(m_squareList, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 1, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 2, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 3, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 4, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 5, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 6, 0.6, 0, 0.6);
		GLdrawSquare(m_squareList + 7, 0.7, 0, 0.6);
		break;
	case 3:
		GLdrawSquare(m_squareList, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 1, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 2, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 3, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 4, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 5, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 6, 0.6, 0, 0.6);
		GLdrawSquare(m_squareList + 7, 0.7, 0, 0.6);
		GLdrawSquare(m_squareList+8, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 9, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 10, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 11, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 12, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 13, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 14, 0.6, 0, 0.6);
		GLdrawSquare(m_squareList + 15, 0.7, 0, 0.6);
		break;
	case 4:
		GLdrawSquare(m_squareList, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 1, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 2, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 3, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 4, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 5, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 6, 0.6, 0, 0.6);
		break;
	case 5:
		GLdrawSquare(m_squareList, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 1, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 2, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 3, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 4, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 5, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 6, 0.6, 0, 0.6);
		GLdrawSquare(m_squareList + 7, 0.7, 0, 0.6);
		GLdrawSquare(m_squareList + 8, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 9, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 10, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 11, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 12, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 13, 0, 0.8, 0.6);
		break;
	case 6:
		GLdrawSquare(m_squareList, 0.8, 0, 0.6);
		GLdrawSquare(m_squareList + 1, 0, 0.8, 0.6);
		GLdrawSquare(m_squareList + 2, 0, 1, 0.6);
		GLdrawSquare(m_squareList + 3, 0.4, 0, 0.4);
		GLdrawSquare(m_squareList + 4, 0, 0, 0.6);
		GLdrawSquare(m_squareList + 5, 0, 0.8, 0.6);
	default:
		break;
	}
	
	SetTimer(0, 33, NULL);
}


BOOL CMy20131503P1View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}


void CMy20131503P1View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (::wglMakeCurrent(0, 0) == FALSE) {
		MessageBox(_T("Could not make RC non-current"));
	}

	if (::wglDeleteContext(m_hRC) == FALSE) {
		MessageBox(_T("Could not delete RC"));
	}
	if (m_pDC) {
		delete m_pDC;
	}
	m_pDC = NULL;

	KillTimer(0);
}


void CMy20131503P1View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 초당 이동값
	int i = 0;



	if (m_nPolygonCount == 0) {
		if (m_squareList[0].PosX + m_squareList[0].distance > 100 || m_squareList[0].PosX - 30 < -100) {
			m_squareList[0].StepX = -m_squareList[0].StepX;
		}
		if (m_squareList[0].PosY + m_squareList[0].distance  > 70 || m_squareList[0].PosY - 30 < -70) {
			m_squareList[0].StepY = -m_squareList[0].StepY;
		}
		if (m_squareList[0].PosZ + m_squareList[0].distance  > 100 || m_squareList[0].PosZ - 30 < -200) {
			m_squareList[0].StepZ = -m_squareList[0].StepZ;
		}

		m_squareList[0].PosX += m_squareList[0].StepX;
		m_squareList[0].PosY += m_squareList[0].StepY;
		m_squareList[0].PosZ += m_squareList[0].StepZ;
	}
	else if (m_nPolygonCount >= 1 && m_nPolygonCount != 3 && m_nPolygonCount != 5) {
		for (i = 0; i < 8; i++) {
			if (m_squareList[i].PosX + m_squareList[i].distance > 100 || m_squareList[i].PosX - 10 < -100) {
				m_squareList[i].StepX = -m_squareList[i].StepX;
			}
			if (m_squareList[i].PosY + m_squareList[i].distance  > 70 || m_squareList[i].PosY - 10 < -70) {
				m_squareList[i].StepY = -m_squareList[i].StepY;
			}
			if (m_squareList[i].PosZ + m_squareList[i].distance  > 100 || m_squareList[i].PosZ - 10 < -200) {
				m_squareList[i].StepZ = -m_squareList[i].StepZ;
			}

			m_squareList[i].PosX += m_squareList[i].StepX;
			m_squareList[i].PosY += m_squareList[i].StepY;
			m_squareList[i].PosZ += m_squareList[i].StepZ;
		}

	}
	else if (m_nPolygonCount == 3) {
		for (i = 0; i <= 15; i++) {
			if (m_squareList[i].PosX + m_squareList[i].distance > 100 || m_squareList[i].PosX - 10 < -100) {
				m_squareList[i].StepX = -m_squareList[i].StepX;
			}
			if (m_squareList[i].PosY + m_squareList[i].distance  > 70 || m_squareList[i].PosY - 10 < -70) {
				m_squareList[i].StepY = -m_squareList[i].StepY;
			}
			if (m_squareList[i].PosZ + m_squareList[i].distance  > 100 || m_squareList[i].PosZ - 10 < -200) {
				m_squareList[i].StepZ = -m_squareList[i].StepZ;
			}

			m_squareList[i].PosX += m_squareList[i].StepX;
			m_squareList[i].PosY += m_squareList[i].StepY;
			m_squareList[i].PosZ += m_squareList[i].StepZ;
		}
	}
	else if (m_nPolygonCount == 5) {
		for (i = 0; i <= 15; i++) {
			if (m_squareList[i].PosX + m_squareList[i].distance > 100 || m_squareList[i].PosX - 10 < -100) {
				m_squareList[i].StepX = -m_squareList[i].StepX;
			}
			if (m_squareList[i].PosY + m_squareList[i].distance  > 70 || m_squareList[i].PosY - 10 < -70) {
				m_squareList[i].StepY = -m_squareList[i].StepY;
			}
			if (m_squareList[i].PosZ + m_squareList[i].distance  > 100 || m_squareList[i].PosZ - 10 < -200) {
				m_squareList[i].StepZ = -m_squareList[i].StepZ;
			}

			m_squareList[i].PosX += m_squareList[i].StepX;
			m_squareList[i].PosY += m_squareList[i].StepY;
			m_squareList[i].PosZ += m_squareList[i].StepZ;
		}
	}
	

	InvalidateRect(NULL, FALSE);

	CView::OnTimer(nIDEvent);
}


void CMy20131503P1View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nPolygonCount == 0) {
		
		SetPosSquare(m_squareList + 1, m_squareList[0].PosX + (m_squareList[0].distance / 2),m_squareList[0].PosY ,	m_squareList[0].PosZ );
		SetPosSquare(m_squareList + 2, m_squareList[0].PosX + (m_squareList[0].distance / 2),m_squareList[0].PosY,	m_squareList[0].PosZ + (m_squareList[0].distance / 2));
		SetPosSquare(m_squareList + 3, m_squareList[0].PosX, m_squareList[0].PosY + (m_squareList[0].distance / 2),	m_squareList[0].PosZ );
		SetPosSquare(m_squareList + 4, m_squareList[0].PosX + (m_squareList[0].distance / 2), m_squareList[0].PosY + (m_squareList[0].distance / 2),m_squareList[0].PosZ );
		SetPosSquare(m_squareList + 5, m_squareList[0].PosX , m_squareList[0].PosY + (m_squareList[0].distance / 2),	m_squareList[0].PosZ + (m_squareList[0].distance / 2));
		SetPosSquare(m_squareList + 6, m_squareList[0].PosX + (m_squareList[0].distance / 2),m_squareList[0].PosY + (m_squareList[0].distance / 2),	m_squareList[0].PosZ + (m_squareList[0].distance / 2));
		SetPosSquare(m_squareList + 7, m_squareList[0].PosX + (m_squareList[0].distance / 2),m_squareList[0].PosY ,	m_squareList[0].PosZ + (m_squareList[0].distance / 2));


		SetDisAng(m_squareList + 1, m_squareList[0].distance / 2, -(cos(0.5) + sin(0.5) + 5));
		SetDisAng(m_squareList + 2, m_squareList[0].distance / 2, /*cos(0.4)*/ - sin(0.4) - 5);
		SetDisAng(m_squareList + 3, m_squareList[0].distance / 2, -(cos(0.3) + sin(0.3) + 5));
		SetDisAng(m_squareList + 4, m_squareList[0].distance / 2, (cos(0.1) + sin(0.1) + 5)/2);
		SetDisAng(m_squareList + 5, m_squareList[0].distance / 2, -cos(0.9) /*+ sin(0.9)*/ + 5);
		SetDisAng(m_squareList + 6, m_squareList[0].distance / 2, /*cos(0.8)*/ + sin(0.8) + 5);
		SetDisAng(m_squareList + 7, m_squareList[0].distance / 2, cos(0.7) /*+ sin(0.7)*/ + 5);

		m_squareList[0].distance /= 2;
	}
	if (m_nPolygonCount == 2) {
		SetPosSquare(m_squareList + 8, m_squareList[7].PosX + (m_squareList[7].distance / 2), m_squareList[7].PosY, m_squareList[7].PosZ);
		SetPosSquare(m_squareList + 9, m_squareList[7].PosX + (m_squareList[7].distance / 2), m_squareList[7].PosY, m_squareList[7].PosZ + (m_squareList[7].distance / 2));
		SetPosSquare(m_squareList + 10, m_squareList[7].PosX, m_squareList[7].PosY + (m_squareList[7].distance / 2), m_squareList[7].PosZ);
		SetPosSquare(m_squareList + 11, m_squareList[7].PosX + (m_squareList[7].distance / 2), m_squareList[7].PosY + (m_squareList[7].distance / 2), m_squareList[7].PosZ);
		SetPosSquare(m_squareList + 12, m_squareList[7].PosX, m_squareList[7].PosY + (m_squareList[7].distance / 2), m_squareList[7].PosZ + (m_squareList[7].distance / 2));
		SetPosSquare(m_squareList + 13, m_squareList[7].PosX + (m_squareList[7].distance / 2), m_squareList[7].PosY + (m_squareList[7].distance / 2), m_squareList[7].PosZ + (m_squareList[7].distance / 2));
		SetPosSquare(m_squareList + 14, m_squareList[7].PosX + (m_squareList[7].distance / 2), m_squareList[7].PosY, m_squareList[7].PosZ + (m_squareList[7].distance / 2));

		SetDisAng(m_squareList + 8, m_squareList[7].distance / 2, -(cos(0.5) + sin(0.5) + 5));
		SetDisAng(m_squareList + 9, m_squareList[7].distance / 2, /*cos(0.4)*/ -sin(0.4) - 5);
		SetDisAng(m_squareList + 10, m_squareList[7].distance / 2, -(cos(0.3) + sin(0.3) + 5));
		SetDisAng(m_squareList + 11, m_squareList[7].distance / 2, (cos(0.1) + sin(0.1) + 5) / 2);
		SetDisAng(m_squareList + 12, m_squareList[7].distance / 2, -cos(0.9) /*+ sin(0.9)*/ + 5);
		SetDisAng(m_squareList + 13, m_squareList[7].distance / 2, /*cos(0.8)*/ +sin(0.8) + 5);
		SetDisAng(m_squareList + 14, m_squareList[7].distance / 2, cos(0.7) /*+ sin(0.7)*/ + 5);

		m_squareList[7].distance /= 2;
	}
	if (m_nPolygonCount == 5) {
		SetPosSquare(m_squareList + 7, m_squareList[6].PosX + (m_squareList[6].distance / 2), m_squareList[6].PosY, m_squareList[6].PosZ);
		SetPosSquare(m_squareList + 8, m_squareList[6].PosX + (m_squareList[6].distance / 2), m_squareList[6].PosY, m_squareList[6].PosZ + (m_squareList[6].distance / 2));
		SetPosSquare(m_squareList + 9, m_squareList[6].PosX, m_squareList[6].PosY + (m_squareList[6].distance / 2), m_squareList[6].PosZ);
		SetPosSquare(m_squareList + 10, m_squareList[6].PosX + (m_squareList[6].distance / 2), m_squareList[6].PosY + (m_squareList[6].distance / 2), m_squareList[6].PosZ);
		SetPosSquare(m_squareList + 11, m_squareList[6].PosX, m_squareList[6].PosY + (m_squareList[6].distance / 2), m_squareList[6].PosZ + (m_squareList[6].distance / 2));
		SetPosSquare(m_squareList + 12, m_squareList[6].PosX + (m_squareList[6].distance / 2), m_squareList[6].PosY + (m_squareList[6].distance / 2), m_squareList[6].PosZ + (m_squareList[6].distance / 2));
		SetPosSquare(m_squareList + 13, m_squareList[6].PosX + (m_squareList[6].distance / 2), m_squareList[6].PosY, m_squareList[6].PosZ + (m_squareList[6].distance / 2));

		SetDisAng(m_squareList + 7, m_squareList[6].distance / 2, -(cos(0.5) + sin(0.5) + 5));
		SetDisAng(m_squareList + 8, m_squareList[6].distance / 2, /*cos(0.4)*/ -sin(0.4) - 5);
		SetDisAng(m_squareList + 9, m_squareList[6].distance / 2, -(cos(0.3) + sin(0.3) + 5));
		SetDisAng(m_squareList + 10, m_squareList[6].distance / 2, (cos(0.1) + sin(0.1) + 5) / 2);
		SetDisAng(m_squareList + 11, m_squareList[6].distance / 2, -cos(0.9) /*+ sin(0.9)*/ + 5);
		SetDisAng(m_squareList + 12, m_squareList[6].distance / 2, /*cos(0.8)*/ +sin(0.8) + 5);
		SetDisAng(m_squareList + 13, m_squareList[6].distance / 2, cos(0.7) /*+ sin(0.7)*/ + 5);

		m_squareList[6].distance /= 2;
	}
	m_nPolygonCount += 1;

	
	CView::OnLButtonDown(nFlags, point);
}


void CMy20131503P1View::SetPosSquare(square* S, GLfloat X, GLfloat Y, GLfloat Z)
{
	S->PosX = X;
	S->PosY = Y;
	S->PosZ = Z;
}


void CMy20131503P1View::GLdrawSquare(square* S,GLfloat R, GLfloat G, GLfloat B)
{
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ + S->distance); // { Front }
	glVertex3f(S->PosX, S->PosY, S->PosZ + S->distance); // { Front }
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ + S->distance); // { Front }
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ + S->distance); // { Front }
	glEnd();
	glBegin(GL_POLYGON);
	glColor3f(R/2, G/2, B/2);
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ + S->distance); // { Right }
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ + S->distance); // { Right }
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ); // { Right }
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ); // { Right }	
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ); // { Left }
	glVertex3f(S->PosX, S->PosY, S->PosZ); // { Left }
	glVertex3f(S->PosX, S->PosY, S->PosZ + S->distance); // { Left }
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ + S->distance); // { Left }
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ); // { Top }
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ); // { Top }
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ + S->distance); // { Top }
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ + S->distance); // { Top }
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ + S->distance); // { Bottom }
	glVertex3f(S->PosX, S->PosY, S->PosZ + S->distance); // { Bottom }
	glVertex3f(S->PosX, S->PosY, S->PosZ); // { Bottom }
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ); // { Bottom }
	glEnd();
	glPolygonMode(GL_FRONT, GL_FILL);
	glBegin(GL_POLYGON);
	glColor3f(R, G, B);
	glVertex3f(S->PosX, S->PosY + S->distance, S->PosZ); // { Back }
	glVertex3f(S->PosX, S->PosY, S->PosZ); // { Back }
	glVertex3f(S->PosX + S->distance, S->PosY, S->PosZ); // { Back }
	glVertex3f(S->PosX + S->distance, S->PosY + S->distance, S->PosZ); // { Back }
	glEnd();

}


void CMy20131503P1View::SetDisAng(square* S, GLfloat dis, GLfloat Ang)
{
	S->distance = dis;
	S->StepX = S->StepY = S->StepZ = Ang;
}
