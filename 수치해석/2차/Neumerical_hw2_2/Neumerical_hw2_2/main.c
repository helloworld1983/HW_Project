#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#pragma warning(disable:4996)

#define IMG_WIDTH	128
#define IMG_HEIGHT	128
#define NEW_IMG_WIDTH 512
#define NEW_IMG_HEIGHT 512

uint8_t** memory_alloc2D(uint32_t height, uint32_t width)
{
	uint8_t** ppMem2D = 0;
	uint32_t j;
	// array of pointer
	ppMem2D = (uint8_t**)calloc(sizeof(uint8_t*), height);
	if (ppMem2D == 0)
	{
		return 0;
	}
	*ppMem2D = (uint8_t*)calloc(sizeof(uint8_t), width * height);
	if ((*ppMem2D) == 0)
	{
		// free the memory of array of pointer
		free(ppMem2D);
		return 0;
	}
	for (j = 1; j< height; j++)
	{
		ppMem2D[j] = ppMem2D[j - 1] + width;
	}
	return ppMem2D;
}

int memory_free2D(unsigned char** ppMemAllocated)
{
	if (ppMemAllocated == 0)
	{
		return -1;
	}
	free(ppMemAllocated[0]);
	free(ppMemAllocated);
	return 0;
}

void compare_error(unsigned char** original, unsigned char** output, int size)
{
	int i, j = 0;
	double MES = 0;
	double PSNR = 0;
	double RMS = 0;


	for (i = 0; i < NEW_IMG_WIDTH; i++) {
		for (j = 0; j < NEW_IMG_HEIGHT; j++) {
			MES += pow(original[i][j] - output[i][j], 2.0);
		}
	}
	MES /= size * size;
	PSNR = 20 * log10(255 / sqrt(MES));

	RMS = sqrt(MES);

	printf("*** 3th Lagrange ***\n");
	printf("PSNR = %lf\nRMS = %lf\n", PSNR, RMS);
}
int main(void)
{
	int i, j, a, k = 0;
	int cut_x, cut_y;
	FILE* fpInputImage;
	FILE* fpOutputImage;
	FILE* fpOriginal512;
	unsigned char** ppInputImageBuffer;
	unsigned char** outputImageBuffer;
	unsigned char** tempBuffer;
	unsigned char** Original512Buffer;

	// input file open //
	fpInputImage = fopen("input_128.raw", "rb");
	// output fileopen
	fpOutputImage = fopen("result_lagrange.raw", "wb");

	// memory allocaiton //

	ppInputImageBuffer = memory_alloc2D(IMG_WIDTH, IMG_HEIGHT);				// for original image
	outputImageBuffer = memory_alloc2D(NEW_IMG_WIDTH, NEW_IMG_HEIGHT);		// for result image
	tempBuffer = memory_alloc2D(NEW_IMG_WIDTH + 5, NEW_IMG_HEIGHT + 5);		// padding한 데이터가 들어갈 temp 사이즈가 517인거는 512 +4(앞에 padding용 4칸) + 1(마지막꺼는 원본 픽셀값 복사한거만 있으면 됨)

	// input file read to memory from the file //
	fread(&ppInputImageBuffer[0][0], sizeof(char), IMG_WIDTH*IMG_HEIGHT, fpInputImage);

	// Original Image Expanding //
	for (i = 0; i < IMG_HEIGHT; i++)
	{
		for (j = 0; j < IMG_WIDTH; j++)
		{
			tempBuffer[4*(i+1)][4*(j+1)] = ppInputImageBuffer[i][j];  // 패딩 영역을 제외하고 각 위치에로 값 복사
		}
	}

	// padding //
	for (i = 4; i <= NEW_IMG_HEIGHT; i += 4)
	{
		tempBuffer[0][i] = tempBuffer[4][i];	// 위쪽
		tempBuffer[i][0] = tempBuffer[i][4];	// 왼쪽
		tempBuffer[516][i] = tempBuffer[512][i];// 아래쪽
		tempBuffer[i][516] = tempBuffer[i][512];// 오른쪽

	}

	
	//Interpolation //

	// row
	for (i = 0; i < NEW_IMG_HEIGHT+5; i += 4) {			
		for (j = 4; j < NEW_IMG_WIDTH+1; j += 4) {
			if (j < 512) {							
				for (a = 1, k = 5; a < 4; a++, k++)		
				{
					tempBuffer[i][j + a] =
						  tempBuffer[i][j-4]	* (k - 4) * (k - 8) * (k - 12) / (-4 * -8 * -12)
						+ tempBuffer[i][j]		* (k)	  * (k - 8) * (k - 12) / ( 4 * -4 * -8)
						+ tempBuffer[i][j + 4]	* (k)	  * (k - 4) * (k - 12) / (8 * 4 * -4)
						+ tempBuffer[i][j + 8]	* (k)	  * (k - 4) * (k - 8)  / (12* 8 * 4);
				}
			}

			else { // j = 512 : 찾으려는 마지막 픽셀들인 513 514 515 에 대해서 interpolation
				for (a = 1, k = 9; a < 4; a++, k++)
				{
					tempBuffer[i][j + a] =
						  tempBuffer[i][j - 8] * (k - 4) * (k - 8) * (k - 12) / (-4 * -8 * -12)
						+ tempBuffer[i][j - 4] * (k)	 * (k - 8) * (k - 12) / (4 * -4 * -8)
						+ tempBuffer[i][j]	   * (k)	 * (k - 4) * (k - 12) / (8 * 4 * -4)
						+ tempBuffer[i][j +4]  * (k)	 * (k - 4) * (k - 8)  / (12 * 8 * 4);
				}
			}
		}
	}


	//// height

	for (j = 4; j < NEW_IMG_WIDTH+5; j++) {			
		for (i = 4; i < NEW_IMG_HEIGHT+1; i += 4) {
			if (i < 512) {
				for (a = 1, k = 5; a < 4; a++, k++)	
				{
					tempBuffer[i + a][j] =
						  tempBuffer[i - 4][j]	* (k - 4) * (k - 8) * (k - 12) / (-4 * -8 * -12)
						+ tempBuffer[i][j]		* (k)	  * (k - 8) * (k - 12) /  (4 * -4 * -8)
						+ tempBuffer[i + 4][j]	* (k)	  * (k - 4) * (k - 12) / (8 * 4 * -4)
						+ tempBuffer[i + 8][j]	* (k)	  * (k - 4) * (k - 8)  / (12 * 8 * 4);
				}
			}

			else {
				for (a = 1, k = 9; a < 4; a++, k++)		
				{
					tempBuffer[i + a][j] =
						  tempBuffer[i - 8][j] * (k - 4) * (k - 8) * (k - 12) / (-4 * -8 * -12)
						+ tempBuffer[i - 4][j] * (k)	 * (k - 8) * (k - 12) / (4 * -4 * -8)
						+ tempBuffer[i][j]	   * (k)	 * (k - 4) * (k - 12) / (8 * 4 * -4)
						+ tempBuffer[i + 4][j] * (k)	 * (k - 4) * (k - 8)  / (12 * 8 * 4);
				}
			}
		}
	}


	// cutting result image from padding image
	for (cut_y = 0, i = 4; i < NEW_IMG_HEIGHT + 4; i++, cut_y++)
	{
		for (cut_x = 0, j = 4; j < NEW_IMG_WIDTH + 4; j++, cut_x++) {
			outputImageBuffer[cut_y][cut_x] = tempBuffer[i][j];
		}
	}

	fwrite(&outputImageBuffer[0][0], sizeof(char), NEW_IMG_WIDTH*NEW_IMG_HEIGHT, fpOutputImage); // 결과 이미지 확인용
	
	 //read original 512 x 512 file to compare
	fpOriginal512 = fopen("original_512.raw", "rb");
	Original512Buffer = memory_alloc2D(NEW_IMG_HEIGHT, NEW_IMG_WIDTH);
	fread(&Original512Buffer[0][0], sizeof(char), 512 * 512, fpOriginal512);

	// calculate PSNR, RMS
	compare_error(Original512Buffer, outputImageBuffer, 512);

	

	memory_free2D(ppInputImageBuffer);
	memory_free2D(outputImageBuffer);
	memory_free2D(tempBuffer);
	memory_free2D(Original512Buffer);

	fclose(fpInputImage);
	fclose(fpOutputImage);
	fclose(fpOriginal512);
	return 0;


}

