#pragma once
typedef struct _FADEPARAM
{
	float * output; int len; float fadeweight; float *src1; float *src2;
}FADEPARAM;

void Fade(float * output, int len, float fadeweight, float *src1, float *src2);
DWORD ParallelFadeThread(LPVOID param);



