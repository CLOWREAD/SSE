#include "stdafx.h"
#include "ParallelFade.h"
#include <xmmintrin.h>



void Fade(float * output, int len, float fadeweight ,float *src1, float *src2 )
{
	float fw1[4], fw2[4];
	fw1[0] = fw1[1] = fw1[2] = fw1[3] = fadeweight;
	fw2[0] = fw2[1] = fw2[2] = fw2[3] = 1-fadeweight;

	auto mmxfw1 = _mm_load_ps(fw1);
	auto mmxfw2 = _mm_load_ps(fw2);

	for (int i = 0; i < len; i+=4)
	{


		auto mmx1 = _mm_load_ps(src1+i);
		auto mmx2 = _mm_load_ps(src2+i);



		auto mmxres1 = _mm_mul_ps(mmx1,mmxfw1);
		auto mmxres2 = _mm_mul_ps(mmx2, mmxfw2);
		auto mmxres = _mm_add_ps(mmxres1, mmxres2);

		_mm_store_ps(output + i, mmxres);

	}




}
DWORD ParallelFadeThread(LPVOID param)
{
	FADEPARAM *fp = (FADEPARAM*)param;
	Fade(fp->output, fp->len, fp->fadeweight, fp->src1, fp->src2);

	delete fp;

	return 0;
}