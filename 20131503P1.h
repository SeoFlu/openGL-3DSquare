
// 20131503P1.h : 20131503P1 ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CMy20131503P1App:
// �� Ŭ������ ������ ���ؼ��� 20131503P1.cpp�� �����Ͻʽÿ�.
//

class CMy20131503P1App : public CWinApp
{
public:
	CMy20131503P1App();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMy20131503P1App theApp;
