#include "stdafx.h"
#include "ImageFading.h"
#include "ParallelFade.h"
#include "BitmapLoader.h"
#include "windows.h"
ImageFading::ImageFading()
{
}


ImageFading::~ImageFading()
{
}

void ImageFading::LoadPicture()
{
	m_Heigth = 600;
	m_Width = 800;


	m_IA.m_Count = 4;
	m_IA.m_ImageArray = new BF_IMAGE[4];

	m_IA.m_ImageArray[0].m_ImageData = new float[m_Width* m_Heigth * 4];
	m_IA.m_ImageArray[1].m_ImageData = new float[m_Width* m_Heigth * 4];
	m_IA.m_ImageArray[2].m_ImageData = new float[m_Width* m_Heigth * 4];
	m_IA.m_ImageArray[3].m_ImageData = new float[m_Width* m_Heigth * 4];

	m_FadeResult.m_ImageData = new float[m_Width* m_Heigth * 4];
	
	
	
	m_FadeResultbyte = new unsigned long [m_Width* m_Heigth];

	
	for (int i = 0; i < m_Width* m_Heigth * 4; i++)
	{

		m_IA.m_ImageArray[0].m_ImageData[i] = rand() % 256;
		m_IA.m_ImageArray[1].m_ImageData[i] = rand() % 256;
		m_FadeResult.m_ImageData[i] = rand() % 256;

	}
	

	BitmapLoader bl;
	char fn[64];
	for (int i = 0; i < m_IA.m_Count; i++)
	{
		wsprintfA(fn, "a%01d.bmp", i+1);
		bl.Load((char*)fn);
		//if (bl.m_Height == m_Heigth && bl.m_Width == m_Width)
		{
			bl.Resize(m_Width, m_Heigth);
			bl.CopyRelease(m_IA.m_ImageArray[i].m_ImageData);
		}

	}
	

	for (int i = 0; i < m_Width* m_Heigth ; i++)
	{

		m_FadeResultbyte[i] = RGB(0,128,255);// m_FadeResult.m_ImageData[i];

	}



}

void ImageFading::Get(float w)
{
	if (w < 0)w = 0;
	if (w >= m_IA.m_Count)w = m_IA.m_Count;

	int f1, f2;
	f1 = w;
	f2 = w + 1;
	if (f1 >= m_IA.m_Count ) f1 = m_IA.m_Count;
	if (f2 >= m_IA.m_Count ) f2 = m_IA.m_Count;

	float weight;

	if (f2 > f1)
	{
		weight = (f2-w) / (f2 - f1);


	}
	else
	{
		weight = 1;
	}

	f1 %= m_IA.m_Count;
	f2 %= m_IA.m_Count;


	int fracnum = 4;
	int totalcount = m_Heigth * m_Width * 4;
	int fraclen = totalcount / fracnum ;


	FADEPARAM *fp;

	for (int i = 0; i < fracnum; i++)
	{
		fp = new FADEPARAM;
		fp->output = m_FadeResult.m_ImageData + i * fraclen;
		fp->len = fraclen;
		fp->fadeweight = weight;
		fp->src1 = m_IA.m_ImageArray[f1].m_ImageData + i * fraclen;
		fp->src2 = m_IA.m_ImageArray[f2].m_ImageData + i * fraclen;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ParallelFadeThread, fp, 0, 0);
	}

	//Fade(m_FadeResult.m_ImageData, , 0.5, m_IA.m_ImageArray[0].m_ImageData, m_IA.m_ImageArray[1].m_ImageData);


}

void ImageFading::BitBlt()
{
	if (m_hWnd == 0)return;

	for (int i = 0; i < m_Width* m_Heigth; i++)
	{

		m_FadeResultbyte[i] = RGB(m_FadeResult.m_ImageData[i*4], m_FadeResult.m_ImageData[i*4+1], m_FadeResult.m_ImageData[i*4+2]);// m_FadeResult.m_ImageData[i];

	}



	HDC dc = GetDC(m_hWnd);
	HDC cdc = CreateCompatibleDC(dc);
	HBITMAP hbmp = CreateCompatibleBitmap(dc, m_Width, m_Heigth);
	BITMAP bmp; 
	GetObjectW(hbmp, sizeof(bmp), &bmp);


	
	SetBitmapBits(hbmp, bmp.bmWidthBytes*bmp.bmHeight, m_FadeResultbyte);


	auto oldhbmp = SelectObject(cdc, hbmp);



	::BitBlt(dc, 0, 0, m_Width, m_Heigth, cdc, 0, 0, SRCCOPY);
	SelectObject(cdc, oldhbmp);
	
	ReleaseDC(m_hWnd,dc);
	ReleaseDC(m_hWnd, cdc);

	//CloseHandle(hbmp);

}

void ImageFading::Get()
{
	if (m_FadeStatus.IsBusy == 0)
	{
		return;
	}



	int fracnum = 4;
	int totalcount = m_Heigth * m_Width * 4;
	int fraclen = totalcount / fracnum;

	

	FADEPARAM *fp;

	for (int i = 0; i < fracnum; i++)
	{
		fp = new FADEPARAM;
		fp->output = m_FadeResult.m_ImageData + i * fraclen;
		fp->len = fraclen;
		fp->fadeweight =1- m_FadeStatus.Weight;
		fp->src1 = m_IA.m_ImageArray[m_FadeStatus.From].m_ImageData + i * fraclen;
		fp->src2 = m_IA.m_ImageArray[m_FadeStatus.To].m_ImageData + i * fraclen;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ParallelFadeThread, fp, 0, 0);
	}



}

void ImageFading::Next()
{
	if (m_FadeStatus.IsBusy == 0)
	{
		m_FadeStatus.Weight = 0;
		m_FadeStatus.IsBusy = 1;
		m_FadeStatus.To = m_FadeStatus.From + 1;
		m_FadeStatus.To %= m_IA.m_Count;
	}

}

void ImageFading::Prev()
{
	if (m_FadeStatus.IsBusy == 0)
	{
		m_FadeStatus.Weight = 0;
		m_FadeStatus.IsBusy = 1;
		m_FadeStatus.To = m_FadeStatus.From - 1;
		m_FadeStatus.To += m_IA.m_Count;
		m_FadeStatus.To %= m_IA.m_Count;
	}
}

void ImageFading::Loop()
{
	if (m_FadeStatus.IsBusy == 0)
	{
		m_FadeStatus.Loop = 1;
		Next();
	}


}

void ImageFading::Stop()
{

	m_FadeStatus.Loop = 0;
}

void ImageFading::OnTimer(float step)
{
	if (m_FadeStatus.IsBusy == 1)
	{
		m_FadeStatus.Weight += step;
		if (m_FadeStatus.Weight > 1.0f)
		{
			m_FadeStatus.From = m_FadeStatus.To;
			if (m_FadeStatus.Loop == 1)
			{
				m_FadeStatus.IsBusy = 0;
				Next();
			}
			else
			{
				m_FadeStatus.IsBusy = 0;
			}
		}
	}

}

void ImageFading::_BF_IMAGE::Release()
{
	delete (this->m_ImageData);
}
