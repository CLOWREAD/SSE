#pragma once
#include <Windows.h>
class ImageFading
{
	typedef struct _BF_IMAGE
	{
		float * m_ImageData=0;
		void Release();
	}BF_IMAGE,*LP_BF_IMAGE;
	typedef struct _BF_IMAGE_ARRAY
	{

		int m_Count=0;
		LP_BF_IMAGE  m_ImageArray;
	}BF_IMAGE_ARRAY;
	BF_IMAGE m_FadeResult;
	unsigned long *m_FadeResultbyte;


public:
	BF_IMAGE_ARRAY m_IA;
	ImageFading();
	~ImageFading();
	HWND m_hWnd;
	int m_Width,m_Heigth;
	void LoadPicture();

	void Get(float);
	void BitBlt();
	void Get();
	struct FADESTATUS
	{
		int From=0, To=0;
		float Weight=0;
		int Loop=0;
		int IsBusy = 0;
	}m_FadeStatus;

	void Next();
	void Prev();
	void Loop();
	void Stop();
	void OnTimer(float step);

};

