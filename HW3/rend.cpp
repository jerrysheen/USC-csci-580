/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (float) 3.14159265358979323846

/* HW1 HW2 CONST: */
#define MAX_COLOR 4095
#define LEFT 0
#define RIGHT 1

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/
	int rows = sizeof mat / sizeof mat[0];
	int cols = sizeof mat[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180.0f;
	mat[0][0] = 1.0;
	mat[1][1] = cos(radians);
	mat[1][2] = -sin(radians);
	mat[2][1] = sin(radians);
	mat[2][2] = cos(radians);
	mat[3][3] = 1.0;
	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{

	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	int rows = sizeof mat / sizeof mat[0];
	int cols = sizeof mat[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180.0f;
	mat[1][1] = 1.0;
	mat[0][0] = cos(radians);
	mat[2][0] = -sin(radians);
	mat[0][2] = sin(radians);
	mat[2][2] = cos(radians);
	mat[3][3] = 1.0;
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	int rows = sizeof mat / sizeof mat[0];
	int cols = sizeof mat[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180.0f;
	mat[2][2] = 1.0;
	mat[0][0] = cos(radians);
	mat[0][1] = -sin(radians);
	mat[1][0] = sin(radians);
	mat[1][1] = cos(radians);
	mat[3][3] = 1.0;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	int rows = sizeof mat / sizeof mat[0];
	int cols = sizeof mat[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1.0;
	mat[1][1] = 1.0;
	mat[2][2] = 1.0;
	mat[3][3] = 1.0;
	for (int i = 0; i < 3; i++) {
		mat[i][3] = translate[i];
	}
	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	int rows = sizeof mat / sizeof mat[0];
	int cols = sizeof mat[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			mat[i][j] = 0;
		}
	}
	mat[3][3] = 1.0;
	for (int i = 0; i < 3; i++) {
		mat[i][i] = scale[i];
	}
	return GZ_SUCCESS;
}


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

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	// screen to perspective space...
	
	// init Xsp...
	int rows = sizeof Xsp / sizeof Xsp[0];
	int cols = sizeof Xsp[0] / sizeof(float);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Xsp[i][j] = 0;
		}
	}
	Xsp[0][0] = (float)xres / 2.0;
	Xsp[0][3] = (float)xres / 2.0;
	Xsp[1][1] = (float)-yres / 2.0;
	Xsp[1][3] = (float)yres / 2.0;
	Xsp[2][2] = (float)INT_MAX;
	Xsp[3][3] = 1.0;

	// set camera position to default position.
	m_camera.FOV = DEFAULT_FOV;
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	for (int i = 0; i < 3; i++) {
		m_camera.lookat[i] = 0;
		m_camera.worldup[i] = 0;
	}
	m_camera.worldup[1] = 1.0;
	// set stack to default place;
	matlevel = -1;
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
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
	}

	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
	/* HW 3.7
	- setup for start of each frame - init frame buffer color,alpha,z
	- compute Xiw and projection xform Xpi from camera definition
	- init Ximage - put Xsp at base of stack, push on Xpi and Xiw
	- now stack contains Xsw and app can push model Xforms when needed
	*/

	// init Xpi :
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = 0;
		}
	}
	m_camera.Xpi[0][0] = 1;
	m_camera.Xpi[1][1] = 1;
	m_camera.Xpi[3][3] = 1;
	float  oneOfd = tan((m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[2][2] = oneOfd;
	m_camera.Xpi[3][2] = oneOfd;

	// init Xiw.
	GzCoord cl;
	GzCoord Z_image;
	GzCoord up_prime;
	GzCoord Y_image;
	GzCoord X_image;

	// calculate norm of the cl;
	float cl_norm = 0;
	for (int i = 0; i < 3; i++) {
		cl[i] = m_camera.lookat[i] - m_camera.position[i];
		cl_norm += pow(cl[i], 2);
	}
	cl_norm = sqrt(cl_norm);

	// calculate Z cordi for image space.
	for (int i = 0; i < 3; i++) {
		Z_image[i] = cl[i] / cl_norm;
	}

	// calculate up_prime using up - (up * Z) * Z;
	// here up equal to camera.world up.
	float projection = 0;
	for (int i = 0; i < 3; i++) {
		projection += m_camera.worldup[i] * Z_image[i];
	}

	for (int i = 0; i < 3; i++) {
		up_prime[i] = m_camera.worldup[i] - projection * Z_image[i];
	}

	float up_prime_norm = 0;
	for (int i = 0; i < 3; i++) {
		up_prime_norm += pow(up_prime[i], 2);
	}
	up_prime_norm = sqrt(up_prime_norm);

	// calculate Y_image;
	for (int i = 0; i < 3; i++) {
		Y_image[i] = up_prime[i] / up_prime_norm;
	}

	// calculate X_image;
	X_image[0] = Y_image[1] * Z_image[2] - Z_image[1] * Y_image[2];
	X_image[1] = -(Y_image[0] * Z_image[2] - Z_image[0] * Y_image[2]);
	X_image[2] = Y_image[0] * Z_image[1] - Z_image[0] * Y_image[1];

	// init Xiw;
	for (int i = 0; i < 3; i++) {
		m_camera.Xiw[X][i] = X_image[i];
		m_camera.Xiw[Y][i] = Y_image[i];
		m_camera.Xiw[Z][i] = Z_image[i];
		m_camera.Xiw[3][i] = 0;
	}
	m_camera.Xiw[3][3] = 1;
	// translation Tx = -X * c;
	// Tx Ty Tz.
	m_camera.Xiw[0][3] = 0;
	m_camera.Xiw[1][3] = 0;
	m_camera.Xiw[2][3] = 0;
	for (int i = 0; i < 3; i++) {
		m_camera.Xiw[0][3] -= X_image[i] * m_camera.position[i];
		m_camera.Xiw[1][3] -= Y_image[i] * m_camera.position[i];
		m_camera.Xiw[2][3] -= Z_image[i] * m_camera.position[i];
	}

	// push matrix to the stack;
	GzPushMatrix(Xsp);
	GzPushMatrix(m_camera.Xpi);
	GzPushMatrix(m_camera.Xiw);
	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/

	// if we use second camera position. we need change default camera
	m_camera.position[X] = camera.position[X];
	m_camera.position[Y] = camera.position[Y];
	m_camera.position[Z] = camera.position[Z];

	m_camera.lookat[X] = camera.lookat[X];
	m_camera.lookat[Y] = camera.lookat[Y];
	m_camera.lookat[Z] = camera.lookat[Z];

	m_camera.worldup[X] = camera.worldup[X];
	m_camera.worldup[Y] = camera.worldup[Y];
	m_camera.worldup[Z] = camera.worldup[Z];

	m_camera.FOV = camera.FOV;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	if (matlevel >= MATLEVELS) {
		return GZ_FAILURE;
	}

	if (matlevel == -1) {
		// no previous matrix in the stack. directly add;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[0][i][j] = matrix[i][j];
			}
		}
	}
	else {
		// it has previews matrix. we need to multiply them then apply to the top 
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Ximage[matlevel + 1][i][j] = 0;
				for (int k = 0; k < 4; k++) {
					Ximage[matlevel + 1][i][j] += Ximage[matlevel][i][k] * matrix[k][j];
				}
			}
		}
	}
	matlevel++;
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/
	if (matlevel <= -1) {
		return GZ_FAILURE;
	}
	matlevel--;
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
		GzCoord* triangle_matrix = (GzCoord*)valueList[0];
		GzCoord pointer[3];

		// verti
		// pointer contains the after change cordinate data.
		// triangle_matrix contains 3 sets of codinate
		// [x1, y1, z1, 1];
		// [x2, y2, z2, 1];
		// [x3, y3, z3, 1];
		float homo_triangle_matrix[3][4];
		float temp[3][4];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				homo_triangle_matrix[i][j] = triangle_matrix[i][j];
			}
			homo_triangle_matrix[i][3] = 1.0;
		}
		// [4 * 4] * [4 * 1] do the transformation 
		// but we have 3 cordinate so we add it up 
		// for triangle cord set [1] [2] [3]

		for (int k = 0; k < 3; k++) {
			//temp[1][i] += Ximage[matlevel][i][j] * homo_triangle_matrix[1][j];
			for (int i = 0; i < 4; i++) {
				temp[k][i] = 0;
				for (int j = 0; j < 4; j++) {
					temp[k][i] += Ximage[matlevel][i][j] * homo_triangle_matrix[k][j];
				}
				
			}
		}
		// [x y z w] -> [x/w y/w z/w 1] normalized
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				pointer[i][j] = temp[i][j] / temp[i][3];
			}
		}


		GzCoord vertices[3];

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
		int edgeLR0, edgeLR1, edgeLR2;
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
					normX = vector01Y * (vector02Z)-(vector02Y)* vector01Z;
					normY = -((vector01X * (vector02Z)-(vector02X)* vector01Z));
					normZ = vector01X * (vector02Y)-(vector02X)* vector01Y;
					// normXlx + normYy + normZz + k = 0;
					float k = -(normX * x0 + normY * y0 + normZ * z0);
					// pz = (-k - normYj - normXi)/normZz
					float z = (-k - normY * j - normX * i) / normZ;

					if (z < pixelbuffer[ARRAY(i, j)].z && z > 0) {
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
	}
	return GZ_SUCCESS;
}
