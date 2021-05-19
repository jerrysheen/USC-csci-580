#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"

#define MAX_COLOR 4095

GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	if (xRes > MAXXRES || yRes > MAXYRES || xRes <= 0 || yRes <= 0) {
		AfxMessageBox(_T("The resolution is wrong, for system not crush it's alreadt trun to positive resolution"));
	}
	xres = (unsigned)xRes;
	yres = (unsigned)yRes;
	int resolution = xres * yres;
	// frame buffer is used to show color on screen with other Draw api.
	framebuffer = new char[3 * resolution];
	// pixelbuffer contains the info for every block rgb...
	pixelbuffer = new GzPixel[resolution];
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete[]	framebuffer;
	delete[]	pixelbuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	// if outof bundle then print false;

	if (xres > MAXXRES || yres > MAXYRES || xres <= 0 || yres <= 0) {
		AfxMessageBox(_T("The resolution is wrong, please reinput resolution within 1024 * 1024"));
		return GZ_FAILURE;
	}

	// set background color BLACK, clean the frame
	// modify both framebuffer and pixelbuffer here... 
	int res = xres * yres;
	GzPixel origin_value = { 0, 0, 0, 1, 0 };
	for (int i = 0; i < res; i++) {
		pixelbuffer[i] = origin_value;
		for (int j = 0; j < 3; j++) {
			framebuffer[3 * i + j];
		}
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */

	// clamp(0, resolution) the rect to prevent error
	if (i < 0 || i >= xres) {
		i = i < 0 ? 0 : xres - 1;
	}
	if (j < 0 || j >= yres) {
		j = j < 0 ? 0 : yres - 1;
	}
	// clamp rgb to 0~ 4095 to prevent overflow...
	// GzIntensity should within ~ 4095
	if (r < 0 || r > MAX_COLOR) {
		r = r < 0 ? 0 : MAX_COLOR;
	}
	if (g < 0 || g > MAX_COLOR) {
		g = g < 0 ? 0 : MAX_COLOR;
	}
	if (b < 0 || b > MAX_COLOR) {
		b = b < 0 ? 0 : MAX_COLOR;
	}

	GzPixel temp = { r, g, b, a, z };
	pixelbuffer[ARRAY(i, j)] = temp;
	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	// *r is a short pointer. r is address, *r is value
	GzPixel temp =  pixelbuffer[ARRAY(i, j)];
	*r = temp.red;
	*g = temp.blue;
	*b = temp.blue;
	*a = temp.alpha;
	*z = temp.z;
	return GZ_SUCCESS;

}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */

	// shift the rgb value and then turn to char...
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	for (int i = 0; i < xres * yres; i++) {
		GzPixel curr = pixelbuffer[i];
		char red = (char)(curr.red >> 4);
		char green = (char)(curr.green >> 4);
		char blue = (char)(curr.blue >> 4);
		fprintf(outfile, "%c%c%c", red, green, blue );
	}
	return GZ_SUCCESS;

}

int GzRender::GzFlushDisplay2FrameBuffer()
{
	/* HW1.7 write pixels to framebuffer:
		- put the pixels into the frame buffer
		- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red
		- NOT red, green, and blue !!!
	*/
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			
			/*
			framebuff[0] -> B [2]
			framebuff[1] -> G [1]
			framebuff[2] -> R [0]
			*/
			int index = ARRAY(i, j);
			GzPixel curr = pixelbuffer[index];
			// already clamp when Gzput, don't need to clamp here 
			// modify the rgb value to bgr
			// since we do GzFlushDisplay2File first, here we can directly modify framebuffer.
			// the output file won't get influenced.
			framebuffer[3 * index] = (char)(pixelbuffer[index].blue >> 4);
			framebuffer[3 * index + 1] = (char)(pixelbuffer[index].green >> 4);
			framebuffer[3 * index + 2] = (char)(pixelbuffer[index].red >> 4);
		}
	}



	return GZ_SUCCESS;
}