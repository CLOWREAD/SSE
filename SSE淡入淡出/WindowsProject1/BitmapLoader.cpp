#include "stdafx.h"
#include "BitmapLoader.h"
#include "windows.h"
#include <fstream>
BitmapLoader::BitmapLoader()
{
}


BitmapLoader::~BitmapLoader()
{
}

void BitmapLoader::Load(char * filename)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	std::ifstream ifs;
	ifs.open(filename,std::ios_base::binary);
	
	ifs.read((char*)&bfh, sizeof(bfh));

	ifs.read((char*)&bih, sizeof(bih));

	m_Height = bih.biHeight;
	m_Width = bih.biWidth;

	char * tempbuffer = new char[bih.biSizeImage];

	ifs.read(tempbuffer, bih.biSizeImage);

	ifs.close();

	m_ImageData = new unsigned char[m_Width*m_Height * 4];

	for (int i = 0; i < m_Width*m_Height; i++)
	{
		m_ImageData[i * 4 + 0] = tempbuffer[i * 3 + 0];
		m_ImageData[i * 4 + 1] = tempbuffer[i * 3 + 1];
		m_ImageData[i * 4 + 2] = tempbuffer[i * 3 + 2];
		m_ImageData[i * 4 + 3] = 255;


	}

	delete tempbuffer;

}

void BitmapLoader::CopyRelease(float * des)
{
	for (int i = 0; i < m_Width*m_Height*4; i++)
	{
		des[i]=m_ImageData[i] ;
		
	}

	delete m_ImageData;
}

void BitmapLoader::Resize(int width, int heigth)
{
	unsigned char * obuffer=m_ImageData;
	unsigned char * nbuffer = new unsigned char[width*heigth * 4];
	long indexo,indexn;
	float x, y;

	for (int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < width; j++)
		{
			y = 1.0f*i / heigth;
			x = 1.0f*j / width;
			y = 1 - y;

			indexn = i * width+j;
			indexn *= 4;

			int ox, oy;
			oy = y * m_Height;
			ox = x * m_Width;
			if (ox >= m_Width)
			{
				ox = m_Width - 1;
			}
			if (oy >= m_Height)
			{
				oy = m_Height - 1;
			}

			indexo =  oy*m_Width + ox;
			indexo *= 4;
			if (indexo >= m_Width * m_Height * 4)
			{
				continue;
			}

			nbuffer[indexn]=obuffer[indexo];
			nbuffer[indexn+1] = obuffer[indexo+1];
			nbuffer[indexn+2] = obuffer[indexo+2];
			nbuffer[indexn+3] = obuffer[indexo+3];





		}




	}
	delete obuffer;
	m_ImageData = nbuffer;
	m_Height = heigth;
	m_Width = width;


}
