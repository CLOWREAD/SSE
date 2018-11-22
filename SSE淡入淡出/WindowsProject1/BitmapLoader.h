#pragma once
class BitmapLoader
{
public:
	long m_FileLen;
	long m_Width;
	long m_Height;
	unsigned char * m_ImageData;
	BitmapLoader();
	~BitmapLoader();
	void Load(char*filename);
	void CopyRelease(float * des);
	void Resize(int width, int heigth);


};

