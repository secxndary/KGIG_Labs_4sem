#include "stdafx.h"

CRectD::CRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
//------------------------------------------------------------------------------
void CRectD::SetRectD(double l, double t, double r, double b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

//------------------------------------------------------------------------------
CSizeD CRectD::SizeD()
{
	CSizeD cz;
	cz.cx = fabs(right - left);	// ������ ������������� �������
	cz.cy = fabs(top - bottom);	// ������ ������������� �������
	return cz;
}

//----------------------------------------------------------------------------

CMatrix CreateTranslate2D(double dx, double dy)
// ��������� ������� ��� �������������� ��������� ������� ��� ��� �������� 
// �� dx �� ��� X � �� dy �� ��� Y � ������������� ������� ���������
// --- ��� ---
// ��������� ������� ��� �������������� ��������� ������� ��� �������� ������
// ������� ��������� �� -dx ��� X � �� -dy �� ��� Y ��� ������������� ��������� ������� 
{
	CMatrix TM(3, 3);
	TM(0, 0) = 1; TM(0, 2) = dx;
	TM(1, 1) = 1;  TM(1, 2) = dy;
	TM(2, 2) = 1;
	return TM;
}

//------------------------------------------------------------------------------------
CMatrix CreateRotate2D(double fi)
// ��������� ������� ��� �������������� ��������� ������� ��� ��� ��������
// �� ���� fi (��� fi>0 ������ ������� �������)� ������������� ������� ���������
// --- ��� ---
// ��������� ������� ��� �������������� ��������� ������� ��� �������� ������
// ������� ��������� �� ���� -fi ��� ������������� ��������� ������� 
// fi - ���� � ��������
{
	double fg = fmod(fi, 360.0);
	double ff = (fg / 180.0)*pi; // ������� � �������
	CMatrix RM(3, 3);
	RM(0, 0) = cos(ff); RM(0, 1) = -sin(ff);
	RM(1, 0) = sin(ff);  RM(1, 1) = cos(ff);
	RM(2, 2) = 1;
	return RM;
}


//------------------------------------------------------------------------------

CMatrix SpaceToWindow(CRectD& RS, CRect& RW)
// ���������� ������� ��������� ��������� �� ������� � �������
// RS - ������� � ������� ����������� - double
// RW - ������� � ������� ����������� - int
{
	CMatrix M(3, 3);
	CSize sz = RW.Size();	 // ������ ������� � ����
	int dwx = sz.cx;	     // ������
	int dwy = sz.cy;	     // ������
	CSizeD szd = RS.SizeD(); // ������ ������� � ������� �����������

	double dsx = szd.cx;    // ������ � ������� �����������
	double dsy = szd.cy;    // ������ � ������� �����������

	double kx = (double)dwx / dsx;   // ������� �� x
	double ky = (double)dwy / dsy;   // ������� �� y

	M(0, 0) = kx;  M(0, 1) = 0;    M(0, 2) = (double)RW.left - kx * RS.left;				
	M(1, 0) = 0;   M(1, 1) = -ky;  M(1, 2) = (double)RW.bottom + ky * RS.bottom;		
	M(2, 0) = 0;   M(2, 1) = 0;    M(2, 2) = 1;
	return M;
}

//------------------------------------------------------------------------------

void SetMyMode(CDC& dc, CRectD& RS, CRect& RW) 
// ������������� ����� ����������� MM_ANISOTROPIC � ��� ���������
// dc - ������ �� ����� CDC MFC
// RS -  ������� � ������� ����������� - int
// RW -	 ������� � ������� ����������� - int  
{
	double dsx = RS.right - RS.left;
	double dsy = RS.top - RS.bottom;
	double xsL = RS.left;
	double ysL = RS.bottom;

	int dwx = RW.right - RW.left;
	int dwy = RW.bottom - RW.top;
	int xwL = RW.left;
	int ywH = RW.bottom;

	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetWindowExt((int)dsx, (int)dsy);
	dc.SetViewportExt(dwx, -dwy);
	dc.SetWindowOrg((int)xsL, (int)ysL);
	dc.SetViewportOrg(xwL, ywH);
}

/////////////////////// CLASS CSunSystem //////////////////////////////////

CSunSystem::CSunSystem()            //  ����������� �� ���������
{
	double rS = 30, rE = 20, rM = 10, rV = 15;       // ������ ������, �����, ����
	double rP = 10;

	double RoE = 10 * rS, RoM = 5 * rE, RoV = 6*rS;	 // ������ ������ ����� � ����
	double RoP = 5 * rM;

	double d = RoE + RoM + rM + RoV;		    // �������� �������� �������
	RS.SetRectD(-d, d, d, -d);					// ������� ������� � ������� �����������
	RW.SetRect(0, 0, 600, 600);					// ������� � ����
	Sun.SetRect(-rS, rS, rS, -rS);				// ������������� ������ - ��� ��������� �����
	Earth.SetRect(-rE, rE, rE, -rE);			// ������������� ����� - ��� ��������� �����
	Moon.SetRect(-rM, rM, rM, -rM);				// ������������� ���� - ��� ��������� �����
	Mars.SetRect(-rV, rV, rV, -rV);
	Phobos.SetRect(-rP, rP, rP, -rP);

	EarthOrbit.SetRect(-RoE, RoE, RoE, -RoE);	// ������������� ������ ����� - ��� ��������� �����
	MoonOrbit.SetRect(-RoM, RoM, RoM, -RoM);	// ������������� ������ ���� - ��� ��������� �����
	MarsOrbit.SetRect(-RoV, RoV, RoV, -RoV);
	PhobosOrbit.SetRect(-RoP, RoP, RoP, -RoP);

	fiE = 0;			// ������� ��������� ����� � ������� �������� ������, ����
	fiM = 0;			// ������� ��������� ���� � ������� �������� �����, ����
	fiV = 1;
	fiP = 0;

	wEarth = 5;			// ������� �������� ����� � ������� �������� ������, ����/���.
	wMoon = 50;			// ������� �������� ���� � ������� �������� �����, ����/���.
	wMars = -8;
	wPhobos = -30;

	dt = 0.1;
	MCoords.RedimMatrix(3);
	ECoords.RedimMatrix(3);
	VCoords.RedimMatrix(3);	
	PCoords.RedimMatrix(3);
}

void CSunSystem::SetNewCoords()
//��������� ����� ���������� ������ � �� ������ ����� �������� ������� dt
{
	double RoM = (MoonOrbit.right - MoonOrbit.left) / 2;// ������ ������ ����    
	double ff = (fiM / 180.0)*pi;						// ������� - ������� ��������� ���� � �� �����
	double x = RoM * cos(ff);							// x - ��������� ���������� ���� � �� �����
	double y = RoM * sin(ff);							// y - ��������� ���������� ���� � �� �����	
	MCoords(0) = x;	MCoords(1) = y;   MCoords(2) = 1;
	fiM += wMoon * dt;
	CMatrix P = CreateRotate2D(fiM);					// ������� �������� ������ ������� ������� ����
	MCoords = P * MCoords;

	double RoE = (EarthOrbit.right - EarthOrbit.left) / 2;// ������ ������ �����
	ff = (fiE / 180.0)*pi;								// ������� - ������� ��������� ����� � �� ������
	x = RoE * cos(ff);									// x - ��������� ���������� ����� � �� ������
	y = RoE * sin(ff);									// y - ��������� ���������� ����� � �� ������
	ECoords(0) = x;	ECoords(1) = y;   ECoords(2) = 1;
	P = CreateTranslate2D(x,y);
	MCoords = P * MCoords;

	fiE += wEarth * dt;
	P = CreateRotate2D(fiE);							// ������� �������� ������ ������� ������� ���� � �����
	MCoords = P * MCoords;
	ECoords = P * ECoords;



	// ==============================  PHOBOS  ==============================

	double RoP = (PhobosOrbit.right - PhobosOrbit.left) / 2;
	ff = (fiP / 180.0) * pi;
	x = RoP * cos(ff);
	y = RoP * sin(ff);
	PCoords(0) = x;
	PCoords(1) = y;
	PCoords(2) = 1;
	fiP += wPhobos * dt;
	P = CreateRotate2D(fiP);
	PCoords = P * PCoords;



	double RoV = (MarsOrbit.right - MarsOrbit.left) / 2;
	ff = (fiV / 180.0)*pi;
	x = RoV * cos(ff);
	y = RoV * sin(ff);
	VCoords(0) = x;
	VCoords(1) = y;
	VCoords(2) = 1;

	// new in Mars
	P = CreateTranslate2D(x, y);
	PCoords = P * PCoords;
	// new in Mars

	fiV += wMars * dt;
	P = CreateRotate2D(fiV);
	VCoords = P * VCoords;

	// new in Mars
	PCoords = P * PCoords;
	// new in Mars
}


void CSunSystem::Draw(CDC& dc)
{
	CBrush SBrush, EBrush, MBrush, VBrush, *pOldBrush;
	CBrush PBrush;
	CRect R;

	SBrush.CreateSolidBrush(RGB(255, 0, 0));
	EBrush.CreateSolidBrush(RGB(0, 0, 255));
	MBrush.CreateSolidBrush(RGB(0, 255, 0));
	VBrush.CreateSolidBrush(RGB(128,64,64));
	PBrush.CreateSolidBrush(RGB(255, 106, 0));


	// ������ ������
	dc.SelectStockObject(NULL_BRUSH);		// ����� ����� - �� ���� ���������
	dc.Ellipse(EarthOrbit);					// ������ �����
	dc.Ellipse(MarsOrbit);

	int d = MoonOrbit.right;				// ������ ������ ����
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.Ellipse(R);							// ������ ����

	// new: Phobos
	int p = PhobosOrbit.right;
	R.SetRect(VCoords(0) - p, VCoords(1) + p, VCoords(0) + p, VCoords(1) - p);
	dc.Ellipse(R);


// ������ ������
	pOldBrush = dc.SelectObject(&SBrush);	// ���� ������
	dc.Ellipse(Sun);  // ������

// ������ �����
	d = Earth.right;						// ������ �����
	R.SetRect(ECoords(0) - d, ECoords(1) + d, ECoords(0) + d, ECoords(1) - d);
	dc.SelectObject(&EBrush);				// ���� �����
	dc.Ellipse(R);	// �����

// ������ ����
	d = Moon.right;							// ������ ����
	R.SetRect(MCoords(0) - d, MCoords(1) + d, MCoords(0) + d, MCoords(1) - d);
	dc.SelectObject(&MBrush);				// ���� ����
	dc.Ellipse(R);	// ����

// ������ ����
	d = Mars.right;
	R.SetRect(VCoords(0) - d, VCoords(1) + d, VCoords(0) + d, VCoords(1) - d);
	dc.SelectObject(&VBrush);
	dc.Ellipse(R);

// ������ �����
	d = Phobos.right;
	R.SetRect(PCoords(0) - d, PCoords(1) + d, PCoords(0) + d, PCoords(1) - d);
	dc.SelectObject(&PBrush);
	dc.Ellipse(R);

	dc.SelectObject(pOldBrush);				//��������������� �������� �� pOldBrush 
}


void CSunSystem::GetRS(CRectD& RSX)
// RS - ���������, ���� ������������ ��������� ������� �������
{
	RSX.left = RS.left;
	RSX.top = RS.top;
	RSX.right = RS.right;
	RSX.bottom = RS.bottom;
}







