
// ChildView.h: интерфейс класса CChildView
//


#pragma once


// Окно CChildView

class CChildView : public CWnd
{
// Создание
public:
	CChildView();

// Атрибуты
public:
	CRect WinRect;
	CMatrix PView;
	CMatrix PLight;
	int Index;

// Операции
public:

// Переопределение
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Реализация
public:
	virtual ~CChildView();

	// Созданные функции схемы сообщений
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	// действия при выборе пункта меню
	afx_msg void OnSphere_Mirror();
	afx_msg void OnSphere_Diffuse(); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
