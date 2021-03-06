
// Lab02.h: основной файл заголовка для приложения Lab01
//
#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CLab01App:
// Сведения о реализации этого класса: Lab01.cpp
//

class CLab01App : public CWinApp
{
public:
	CLab01App() noexcept;

	int index;		// "флаг" для проверки загрузки изображения 
	HBITMAP hBit;	// дескриптор рисунка
	HWND hwnd;		// дескриптор окна, куда выводится изображение
	CPoint From;	// координаты курсора
	CPoint To;

// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Реализация

public:
	afx_msg void OnAppAbout();

	void LoadImageBMP(CPoint point);	// загрузка изображения
	void SaveArea();					// сохранить выделенную область окна
	DECLARE_MESSAGE_MAP()
};

extern CLab01App theApp;
