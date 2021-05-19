#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

/***********************************************/
/* HW1 methods: copy here the methods from HW1 */
#include    "stdafx.h"  
#include <iostream>

#define MAX_COLOR 4095
#define LEFT 0
#define RIGHT 1

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

	if (xres > MAXXRES || yres > MAXYRES || xres <= 0 || yres <= 0) {
		AfxMessageBox(_T("The resolution is wrong, please reinput resolution within 1024 * 1024"));
		return GZ_FAILURE;
	}

	// set background color BLACK, clean the frame
	// modify both framebuffer and pixelbuffer here... 
	int res = xres * yres;
	GzPixel origin_value = { 0, 1250, 3000, 1, INT_MAX };
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
	GzPixel temp = pixelbuffer[ARRAY(i, j)];
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
		fprintf(outfile, "%c%c%c", red, green, blue);
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


/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/
	if (numAttributes == 1) {
		GzToken token = nameList[0]; 
		float* color = (float*)valueList[0];
		for (int i = 0; i < 3; i++) {
			flatcolor[i] = color[i];
		}
		return GZ_SUCCESS;
	}
	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	if (numParts == 1) {

		// Scan line algorithm
		GzCoord* pointer = (GzCoord*)valueList[0];
		GzCoord vertices[3], sortedVertices[3];

		// sort verts by Y ( pointer[x][1] ): 
		// if y is small then it means y is at the top of the graph.
		// vertices0 will be the top vertex. then vertex1 be the second. vertex2...
		int v0 = 0, v1 = 1, v2 = 0;
		if (pointer[0][Y] <= pointer[1][Y]) {
			if (pointer[0][Y] <= pointer[2][Y]) {
				v0 = 0;
				// case 0, 1, 2  / 0, 2, 1
				v1 = pointer[1][Y] <= pointer[2][Y] ? 1 : 2;
				v2 = v1 == 1 ? 2 : 1;
			}
			else {
				// case 2, 0 , 1
				v0 = 2;
				v1 = 0;
				v2 = 1;
			}
		}
		else {
			//pointer[0][Y] > pointer[1][Y]
			if (pointer[1][Y] <= pointer[2][Y]) {
				v0 = 1;
				// case 1 ,0 ,2 / 1, 2, 0
				v1 = pointer[0][Y] <= pointer[2][Y] ? 0 : 2;
				v2 = v1 == 0 ? 2 : 0;
			}
			else {
				// case 2, 1, 0
				v0 = 2;
				v1 = 1;
				v2 = 0;
			}
		}
		// assign value by the y location:
		for (int i = 0; i < 3; i++) {
			vertices[0][i] = pointer[v0][i];
			vertices[1][i] = pointer[v1][i];
			vertices[2][i] = pointer[v2][i];
		}



		// Find L/R edge.
		// on the right side of the tri, right edge,
		// left side , left edge.
		// down edge slope >= 0 right edge.
		// down edge slope <0 left edge.
		float y0 = vertices[0][Y];
		float y1 = vertices[1][Y];
		float y2 = vertices[2][Y];
		float x0 = vertices[0][X];
		float x1 = vertices[1][X];
		float x2 = vertices[2][X];
		float z0 = vertices[0][Z];
		float z1 = vertices[1][Z];
		float z2 = vertices[2][Z];
		float edgeLR0, edgeLR1, edgeLR2;
		// using mid point.
		// y0 != y2
		// xp != x1
		float xp = (y1 - y2) / (y0 - y2)*(x0 - x2) + x2;
		if (xp < x1) {
			edgeLR0 = RIGHT;
			edgeLR2 = LEFT;
			edgeLR1 = y1 <= y2 ? RIGHT : LEFT;
		}
		else {
			edgeLR0 = LEFT;
			edgeLR2 = RIGHT;
			edgeLR1 = y1 >= y2 ? RIGHT : LEFT;
		}


		/**
			to compute the plane, we first find the box bound of the triangle,
			then inside we test every point:
				first we test whether it is inside or out side the triangle(using x,y equation),
				then we test whether it is closer compare to the other point on the same framebuffer...
		**/
		// Get Bounding Box:
		float minX = max(0, floor(min(min(x0, x1), x2)));
		float maxX = min(256, ceil(max(max(x0, x1), x2)));
		float minY = max(0, floor(min(min(y0, y1), y2)));
		float maxY = min(256, ceil(max(max(y0, y1), y2)));

		// if a dot is inside the tri, say (xp, yp).
		// then the vet(i -> p) * (i - > j) will be all positive/negative;
		// if the edge is top horizon and left vertical... we render,  otherwise not...
		float vector01X = x1 - x0;
		float vector01Y = y1 - y0;
		float vector01Z = z1 - z0;
		float vector12X = x2 - x1;
		float vector12Y = y2 - y1;
		float vector12Z = z2 - z1;
		float vector20X = x0 - x2;
		float vector20Y = y0 - y2;
		float vector20Z = z0 - z2;

		for (int i = minX; i < maxX; i++) {
			for (int j = minY; j < maxY; j++) {
				float vector0pX = i - x0;
				float vector0pY = j - y0;
				float vector1pX = i - x1;
				float vector1pY = j - y1;
				float vector2pX = i - x2;
				float vector2pY = j - y2;
				float crossProduct0 = vector01X * vector0pY - vector0pX * vector01Y;
				float crossProduct1 = vector12X * vector1pY - vector1pX * vector12Y;
				float crossProduct2 = vector20X * vector2pY - vector2pX * vector20Y;
				if ((crossProduct0 > 0 && crossProduct1 > 0 && crossProduct2 > 0) ||
					(crossProduct0 < 0 && crossProduct1 < 0 && crossProduct2 < 0) ||
					(crossProduct0 == 0 || crossProduct1 == 0 || crossProduct2 == 0)) {
					// this i, j is inside or on the edge....
					// next we need to compute whether it is close to the viewpoint or far from the viewpoint
					// we need to get the plane generate by 3 vertex, we first select two vector inside the plane
					// then calculate normal (a, b, c):
					// compute normal (01X, 01Y, 01Z) X (-20X, -20Y, -20Z);
					// so the plane will be ax + by + cz + k = 0,
					// the case 3 is the case when dot is on the edge.
					if ((crossProduct0 == 0 || crossProduct1 == 0 || crossProduct2 == 0)) {
						// if this edge is a right edge, then we skip it, otherwise we deal it as a inner pixel
						if (crossProduct0 == 0) {
							if (edgeLR0 == RIGHT) {
								continue;
							}
						}
						else if (crossProduct1 == 0) {
							if (edgeLR1 == RIGHT) {
								continue;
							}
						}
						else if (crossProduct2 == 0) {
							if (edgeLR2 == RIGHT) {
								continue;
							}
						}
					}
					float vector02X = -vector20X;
					float vector02Y = -vector20Y;
					float vector02Z = -vector20Z;
						float normX, normY, normZ;
						normX = vector01Y * (vector02Z) - (vector02Y) * vector01Z;
						normY = -((vector01X * (vector02Z) - (vector02X) * vector01Z));
						normZ = vector01X * (vector02Y) - (vector02X) * vector01Y;
					// normXlx + normYy + normZz + k = 0;
						float k = -(normX * x0 + normY * y0 + normZ * z0);
					// pz = (-k - normYj - normXi)/normZz
						float z = (-k - normY * j - normX * i) / normZ;
					
						if (z < pixelbuffer[ARRAY(i, j)].z) {
							// we shoould render this pixel
							GzIntensity redIntensity = ctoi(flatcolor[0]);
							GzIntensity greenIntensity = ctoi(flatcolor[1]);
							GzIntensity blueIntensity = ctoi(flatcolor[2]);
							// Call GzPut to push the pixel to pixelbuffer.
							GzPut(i, j, redIntensity, greenIntensity, blueIntensity, 0, z);

						}
					
				}
			}
		}
		return GZ_SUCCESS;
	}
}


