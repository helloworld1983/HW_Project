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

	printf("*** Bilinear ***\n");
	printf("PSNR = %lf\nRMS = %lf\n", PSNR, RMS);
}

int main(void)
{
	int i, j, f = 0;
	int new_x, new_y = 0;
	FILE* fpInputImage = 0;
	FILE* fpOutputImage = 0;
	FILE* fpOriginal512 = 0;
	unsigned char** ppInputImageBuffer = 0;
	unsigned char** outputImageBuffer = 0;
	unsigned char** Original512Buffer = 0;

	// parameters for interpolation //
	int up_height, up_width;
	int down_height, down_width;

	int interpolation_height;
	int interpolation_width;

	double distance[4];			// distance between each points
	double find_min;
	double p1, p2, p3, p4;
	double linear1, linear2;		// gray scale value of adjacent two point
	double final_linear;			// gray scale value of interpolated point
	double final_x;
	double final_y;

	double reset_height;
	double reset_width;

	double PSNR, RMS, MES= 0;


	// input file open //
	fpInputImage = fopen("input_128.raw", "rb");
	// output fileopen
	fpOutputImage = fopen("result.raw", "wb");

	// memory allocaiton //

	ppInputImageBuffer = memory_alloc2D(IMG_WIDTH, IMG_HEIGHT);				// for original image
	outputImageBuffer = memory_alloc2D(NEW_IMG_WIDTH, NEW_IMG_HEIGHT);		// for result image
	
	// input file read to memory from the file //
	fread(&ppInputImageBuffer[0][0], sizeof(char), IMG_WIDTH*IMG_HEIGHT, fpInputImage);



	// Original Image Expanding //
	for (i = 0; i < IMG_HEIGHT; i++)
	{
		new_y = ceil(i * 4);
		if (new_y >= NEW_IMG_HEIGHT)
			new_y = NEW_IMG_HEIGHT - 1;// 가장자리를 넘어가면 마무리
		for (j = 0; j < IMG_WIDTH; j++)
		{
			new_x =ceil(j * 4);
			if (new_x >= NEW_IMG_WIDTH) 
				new_x = NEW_IMG_WIDTH - 1;
				
			outputImageBuffer[new_y][new_x] = ppInputImageBuffer[i][j];
		}
	}

	//Interpolation //
	for (i = 0; i < NEW_IMG_HEIGHT; i++) {
		for (j = 0; j < NEW_IMG_WIDTH; j++) {
			if (outputImageBuffer[i][j] == 0)		// when need interpolation 
			{
				reset_height = i / 4;				////// original상의 주변점들 찾기 위해서 
				reset_width = j / 4;
				// original image 상의 주변점 찾기 
				up_height	= ceil(reset_height);
				up_width	= ceil(reset_width);
				down_height = floor(reset_height);
				down_width	= floor(reset_width);


				//////////////////////////////////////////////////////////////////////////////////////
				//																					//
				//		*(0) (down_width, down_height)				*(1) (up_width, down_height)    //
				//																					//
				//																					//
				//										+ (찾으려는 좌표)							//
				//																					//
				//																					//
				//																					//
				//																					//
				//																					//
				//		        *(2) (down_width, up_height)			*(3) (up_width, up_height)  //
				//																					//
				//////////////////////////////////////////////////////////////////////////////////////
				
				// 각 지점까지의 거리를 찾음 
				distance[0] = pow(down_height - reset_height, 2.0) + pow(down_width - reset_width, 2.0);
				distance[1] = pow(up_height - reset_height, 2.0) + pow(down_width - reset_width, 2.0);
				distance[2] = pow(down_height - reset_height, 2.0) + pow(up_width - reset_width, 2.0);			
				distance[3] = pow(up_height - reset_height, 2.0) + pow(up_width - reset_width, 2.0);

				
				find_min = distance[f];

				for (f = 1; f < 4; f++)
					if (distance[f] < find_min) find_min = distance[f];

				if (f == 0) {
					interpolation_height = down_height;
					interpolation_width = down_width;
				}
				else if (f == 1) {
					interpolation_height = up_height;
					interpolation_width = down_width;
				}
				else if (f == 2) {
					interpolation_height = down_height;
					interpolation_width = up_width;
				}
				else {
					interpolation_height = up_height;
					interpolation_width = up_width;
				}

				// 결과 이미지 상의 interpolation 값을 찾기 위해 원본 이미지의 주변 4점을 찾음
				p1 = ppInputImageBuffer[down_height][down_width];
				p2 = ppInputImageBuffer[down_height][up_width];
				p3 = ppInputImageBuffer[up_height][down_width];
				p4 = ppInputImageBuffer[up_height][up_width];

				if (reset_width>interpolation_width)
					final_x = (reset_width - interpolation_width);
				else
					final_x = (interpolation_width - reset_width);

				if (reset_height>interpolation_height)
					final_y = (reset_height - interpolation_height);
				else
					final_y = (interpolation_height - reset_height);
				
				linear1 = (1 - final_x)*p1 + final_x * p2;
				linear2 = (1 - final_x)*p3 + final_x * p4;

				final_linear = (1.0 - final_y)*linear1 + final_y*linear2;
				
				outputImageBuffer[i][j] = final_linear;
			}
		}
	}


	// write the file
	fwrite(&outputImageBuffer[0][0], sizeof(char), NEW_IMG_WIDTH*NEW_IMG_HEIGHT, fpOutputImage);
	
	//read original 512 x 512 file to compare
	fpOriginal512 = fopen("original_512.raw", "rb");
	Original512Buffer = memory_alloc2D(NEW_IMG_HEIGHT, NEW_IMG_WIDTH);
 	fread(&Original512Buffer[0][0], sizeof(char), 512*512, fpOriginal512);

	// calculate PSNR, RMS
	compare_error(Original512Buffer, outputImageBuffer, 512);

	memory_free2D(ppInputImageBuffer);
	memory_free2D(outputImageBuffer);
	memory_free2D(Original512Buffer);

	fclose(fpInputImage);
	fclose(fpOutputImage);
	fclose(fpOriginal512);
	return 0;


}

