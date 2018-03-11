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

	printf("*** Six_tap ***\n");
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
	fpOutputImage = fopen("result_sixtap.raw", "wb");

	// memory allocaiton //

	ppInputImageBuffer = memory_alloc2D(IMG_WIDTH, IMG_HEIGHT);				// for original image
	outputImageBuffer = memory_alloc2D(NEW_IMG_WIDTH, NEW_IMG_HEIGHT);		// for result image
	tempBuffer = memory_alloc2D(NEW_IMG_WIDTH + 13, NEW_IMG_HEIGHT + 13);		// padding한 데이터가 들어갈 temp 사이즈가 525인거는 512 +8(앞에 padding용 8칸) + 5(마지막꺼는 원본 픽셀값 복사한거만 있으면 됨)

																			// input file read to memory from the file //
	fread(&ppInputImageBuffer[0][0], sizeof(char), IMG_WIDTH*IMG_HEIGHT, fpInputImage);

	// Original Image Expanding //
	for (i = 0, a = 8; i < IMG_HEIGHT; i++, a+=4)
	{
		for (j = 0, k = 8; j < IMG_WIDTH; j++, k+=4)
		{
			tempBuffer[a][k] = ppInputImageBuffer[i][j];  // 패딩 영역을 제외하고 각 위치에로 값 복사
		}
	}

	// padding // 
	//  [8][8]이 원본이미지의 [0][0]임
	//  확장된 이미지의 영역은 [8]~ [519](마지막에 원본 값을 갖는 확장 이미지 포인트는 516)
	for (i = 4; i <= NEW_IMG_HEIGHT; i += 4)
	{
		tempBuffer[0][i] = tempBuffer[8][i];	// 위쪽
		tempBuffer[4][i] = tempBuffer[8][i];

		tempBuffer[i][0] = tempBuffer[i][8];	// 왼쪽
		tempBuffer[i][4] = tempBuffer[i][8];

		tempBuffer[520][i] = tempBuffer[516][i];// 아래쪽
		tempBuffer[524][i] = tempBuffer[516][i];

		tempBuffer[i][520] = tempBuffer[i][516];// 오른쪽
		tempBuffer[i][524] = tempBuffer[i][516];

	}




	// Half-pel Interpolation //
	// use [1 -5 20 20 -5 1]/32 filter

	// row
	for (i = 0; i < NEW_IMG_HEIGHT + 13; i += 4) {
		for (j = 8; j < NEW_IMG_WIDTH +4; j += 4) { 
			tempBuffer[i][j + 2] =
				(		tempBuffer[i][j - 8]
				+ (-5 * tempBuffer[i][j - 4])
				+ 20 *  tempBuffer[i][j]
				+ 20 *  tempBuffer[i][j + 4]
				+ (-5 * tempBuffer[i][j + 8])
				+		tempBuffer[i][j + 12]) / 32;
		}

		// 마지막 516은 왼쪽으로 한칸 땡긴 윈도우를 사용하겠음
		tempBuffer[i][j + 2] =
			  (		tempBuffer[i][j - 12]
			+ (-5 * tempBuffer[i][j - 8])
			+ 20  * tempBuffer[i][j - 4]
			+ 20  * tempBuffer[i][j]
			+ (-5 * tempBuffer[i][j + 4])
			+		tempBuffer[i][j + 8]) / 32;
	}

	// column
	for (j = 8; j < NEW_IMG_WIDTH + 8; j += 2) {  // row [520]까지 진행
		for (i = 8; i < NEW_IMG_HEIGHT + 4; i += 4) { // colmn [512]까지 진행
			tempBuffer[i + 2][j] =
				  (		tempBuffer[i - 8][j]
				+ (-5 * tempBuffer[i - 4][j])
				+  20 * tempBuffer[i	][j]
				+  20 * tempBuffer[i + 4][j]
				+ (-5 * tempBuffer[i + 8][j])
				+		tempBuffer[i + 12][j]) / 32;
		}

		// 마지막 516은 위쪽으로 한칸 땡긴 윈도우를 사용하겠음
		tempBuffer[i + 2][j] =
			  (		tempBuffer[i - 12][j]
			+ (-5 * tempBuffer[i - 8][j])
			+  20 * tempBuffer[i - 4][j]
			+  20 * tempBuffer[i	][j]
			+ (-5 * tempBuffer[i + 4][j])
			+		tempBuffer[i + 8][j]) / 32;
	}

	// Quarter - Pel Interpolation
	// row
	for (i = 8; i < NEW_IMG_HEIGHT + 7; i += 2)
	{
		for (j = 8; j < NEW_IMG_WIDTH + 7; j += 2)
			tempBuffer[i][j + 1] =	(tempBuffer[i][j] + tempBuffer[i][j + 2]) / 2;
	}

	// column
	for (i = 8; i < NEW_IMG_HEIGHT + 7; i += 2)
	{
		for (j = 8; j < NEW_IMG_WIDTH + 7; j ++)
			tempBuffer[i+1][j]	 = (tempBuffer[i][j]   + tempBuffer[i+2][j]) / 2;  // column
	}

	// diagonal : [516][516] 까지 진행
	for (i = 8; i < NEW_IMG_HEIGHT + 5; i += 4)
	{
		for (j = 8; j < NEW_IMG_WIDTH + 5; j += 4)
		{
			tempBuffer[i + 1][j + 1] = (tempBuffer[i	][j + 2] + tempBuffer[i + 2][j	  ]) / 2; // 왼쪽 위 
			tempBuffer[i + 1][j + 3] = (tempBuffer[i + 2][j + 4] + tempBuffer[i	   ][j + 2]) / 2; // 오른쪽 위
			tempBuffer[i + 3][j + 1] = (tempBuffer[i + 2][j	   ] + tempBuffer[i + 4][j + 2]) / 2; // 왼쪽 아래
			tempBuffer[i + 3][j + 3] = (tempBuffer[i + 4][j + 2] + tempBuffer[i + 2][j + 4]) / 2; // 오른쪽 아래
		}	
	}


	// cutting result image from padding image
	for (cut_y = 0, i = 8; i < NEW_IMG_HEIGHT + 8; i++, cut_y++)
	{
		for (cut_x = 0, j = 8; j < NEW_IMG_WIDTH + 8; j++, cut_x++) {
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

