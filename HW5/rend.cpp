/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#include	<algorithm>

#define PI (float) 3.14159265358979323846

/* HW1 HW2 CONST: */
#define MAX_COLOR 4095
#define LEFT 0
#define RIGHT 1

#define RGBA_DIMEMSION	4	/* RGBA -> 4D color */
#define RGB_DIMEMSION	3	/* RGB -> 3D color */
#define COLOR_LIMIT		4095	/* Clamping the color value into 0~4095. */
#define Z_MAX INT_MAX
static short Normalmatlevel;


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
	mat[0][0] = 1.0;
	mat[1][1] = cos(radians);
	mat[1][2] = -sin(radians);
	mat[2][1] = sin(radians);
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

	//............................
	matlevel = -1;


	numlights = 0;
	matlevel = -1;
	Normalmatlevel = -1;
}


GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;
}

int GzRender::GzDefault()
{
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

	if (matlevel >= MATLEVELS || Normalmatlevel >= MATLEVELS) {
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

	GzMatrix matrix_I;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			matrix_I[j][i] = 0;
		}
	}
	matrix_I[0][0] = 1.0f;
	matrix_I[1][1] = 1.0f;
	matrix_I[2][2] = 1.0f;
	matrix_I[3][3] = 1.0f;

	// first 2 matrix won't cause change
	// the vertex won't change in Xsp and Xpi, push identity
	if (Normalmatlevel < 2) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Xnorm[Normalmatlevel + 1][i][j] = matrix_I[i][j];
			}
		}
	}
	// Xiw space calculate, we should discard translation part, and make it unitary
	else if (Normalmatlevel == 2) {
		GzMatrix Xiw;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				Xiw[i][j] = m_camera.Xiw[i][j];
			}
		}
		Xiw[0][3] = 0;
		Xiw[1][3] = 0;
		Xiw[2][3] = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float sum = 0;
				for (int k = 0; k < 4; k++) {
					sum += Xnorm[Normalmatlevel][i][k] * Xiw[k][j];
				}
				Xnorm[Normalmatlevel + 1][i][j] = sum;
			}
		}
	}
	else {
		// because there are only three types of matrix.
		//[Tx, Ty, Tz]
		//[Rx, Ry, Rz]
		//[Sx, Sy, Sz]
		// [Sx, Sy, Sz]
		// in these three. since we only allowed uniform scales, so once we meet scale, the 
		// normal won't change,
		// [Tx, Ty, Tz]
		// since we only calculate upper 3 x 3 matrix, and [Tx, Ty, Tz] change won't 
		// change the normal direction, so we just do multiply.
		// we just need rotation matrix
		// if its not a rotation matirx, then ignore it,
		// [ a 0 0 x  ]
		//   0 b 0 y
		//   0 0 c z
		if (matrix[0][1] == 0 && matrix[0][2] == 0 &&
			matrix[1][0] == 0 && matrix[1][2] == 0 &&
			matrix[2][0] == 0 && matrix[2][1] == 0) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[Normalmatlevel + 1][i][j] = 0;
					for (int k = 0; k < 4; k++) {
						Xnorm[Normalmatlevel + 1][i][j] += Xnorm[Normalmatlevel][i][k] * matrix_I[k][j];
					}
				}
			}


		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Xnorm[Normalmatlevel + 1][i][j] = 0;
					for (int k = 0; k < 4; k++) {
						Xnorm[Normalmatlevel + 1][i][j] += Xnorm[Normalmatlevel][i][k] * matrix[k][j];
					}
				}
			}


		}


	}
	Normalmatlevel++;
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
	if (pixelbuffer[ARRAY(i, j)].z > z) {
		pixelbuffer[ARRAY(i, j)] = temp;
	}
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

/*
- GzPutAttribute() must accept the following tokens/values:

- GZ_RGB_COLOR					GzColor		default flat-shade color
- GZ_INTERPOLATE				int			shader interpolation mode
- GZ_DIRECTIONAL_LIGHT			GzLight
- GZ_AMBIENT_LIGHT            	GzLight		(ignore direction)
- GZ_AMBIENT_COEFFICIENT		GzColor		Ka reflectance
- GZ_DIFFUSE_COEFFICIENT		GzColor		Kd reflectance
- GZ_SPECULAR_COEFFICIENT       GzColor		Ks coef's
- GZ_DISTRIBUTION_COEFFICIENT   float		spec power
*/
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/

	for (int i = 0; i < numAttributes; i++) {
		GzToken token = nameList[i];
		switch (token) {

		case GZ_DIRECTIONAL_LIGHT: {
			GzLight* directional_light = (GzLight*)valueList[i];
			for (int j = 0; j < 3; j++) {
				lights[numlights].direction[j] = directional_light->direction[j];
				lights[numlights].color[j] = directional_light->color[j];
			}
			numlights++;
		}
								   break;
		case GZ_AMBIENT_LIGHT: {
			GzLight* ambient_light = (GzLight*)valueList[i];
			for (int j = 0; j < 3; j++) {
				ambientlight.direction[j] = ambient_light->direction[j];
				ambientlight.color[j] = ambient_light->color[j];

			}

		}
							   break;
		case GZ_DIFFUSE_COEFFICIENT: {
			GzColor* diffuse_coefficient = (GzColor*)valueList[i];
			for (int j = 0; j < 3; j++) {
				Kd[j] = (*diffuse_coefficient)[j];
			}
		}

									 break;

		case GZ_AMBIENT_COEFFICIENT: {
			GzColor* ambient_coefficient = (GzColor*)valueList[i];
			for (int j = 0; j < 3; j++) {
				Ka[j] = (*ambient_coefficient)[j];
			}
		}

									 break;

		case GZ_SPECULAR_COEFFICIENT: {
			GzColor* specular_coefficient = (GzColor*)valueList[i];
			for (int j = 0; j < 3; j++) {
				Ks[j] = (*specular_coefficient)[j];
			}
		}

									  break;
		case GZ_INTERPOLATE: {
			int* mode = (int*)valueList[i];
			interp_mode = *mode;
		}
							 break;
		case GZ_DISTRIBUTION_COEFFICIENT: {
			float* distribution_coefficient = (float*)valueList[i];;
			spec = *distribution_coefficient;
		}

										  break;
		case GZ_RGB_COLOR: {
			GzColor * color = (GzColor*)valueList[i];
			for (int j = 0; j < 3; j++) {
				flatcolor[j] = *color[j];
			}
		}
						   break;
		case GZ_TEXTURE_MAP: {
			if ((GzTexture)valueList[i] != NULL) {
				tex_fun = (GzTexture)valueList[i];
			}
		}
							 break;
		}
	}

	return GZ_SUCCESS;
}


int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	//if (numParts == 1) {

	GzCoord* triangle_matrix = (GzCoord*)valueList[0];
	GzCoord* normal_matrix = (GzCoord*)valueList[1];
	GzTextureIndex* uv_pointer = (GzTextureIndex*)valueList[2];
	GzCoord vertices[3], normals[3];
	GzTextureIndex uv_list[3];

	// verti
	// pointer contains the after change cordinate data.
	// triangle_matrix contains 3 sets of codinate
	// [x1, y1, z1, 1];
	// [x2, y2, z2, 1];
	// [x3, y3, z3, 1];
	float vertices4D[3][4], normals4D[3][4];
	float temp_vertex[3][4], temp_normal[3][4];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			vertices4D[i][j] = triangle_matrix[i][j];
			normals4D[i][j] = normal_matrix[i][j];
		}
		vertices4D[i][3] = 1.0f;
		normals4D[i][3] = 1.0f;
	}

	// [4 * 4] * [4 * 1] do the transformation 
		// but we have 3 cordinate so we add it up 
		// for triangle cord set [1] [2] [3]

	for (int k = 0; k < 3; k++) {
		//temp[1][i] += Ximage[matlevel][i][j] * homo_triangle_matrix[1][j];
		for (int i = 0; i < 4; i++) {
			temp_vertex[k][i] = 0, temp_normal[k][i] = 0;
			for (int j = 0; j < 4; j++) {
				temp_vertex[k][i] += Ximage[matlevel][i][j] * vertices4D[k][j];
				temp_normal[k][i] += Xnorm[Normalmatlevel][i][j] * normals4D[k][j];
			}
			/*temp_vertex[k][i] = sumV;
			temp_normal[k][i] = sumN;*/
		}
	}

	// [x y z w] -> [x/w y/w z/w 1] normalized
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			vertices[i][j] = temp_vertex[i][j] / temp_vertex[i][3];
			normals[i][j] = temp_normal[i][j] / temp_normal[i][3];
		}
	}

	// assigan value to uv_list..
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			uv_list[i][j] = uv_pointer[i][j];
		}
	}
	
	// Begin Rasterization:
	if (vertices[0][1] > vertices[1][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(vertices[0][i], vertices[1][i]);
			std::swap(normals[0][i], normals[1][i]);
			if (i < 2) {
				std::swap(uv_list[0][i], uv_list[1][i]);
			}
		}
	}
	if (vertices[0][1] > vertices[2][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(vertices[0][i], vertices[2][i]);
			std::swap(normals[0][i], normals[2][i]);
			if (i < 2) {
				std::swap(uv_list[0][i], uv_list[2][i]);
			}
		}
	}
	if (vertices[1][1] > vertices[2][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(vertices[1][i], vertices[2][i]);
			std::swap(normals[1][i], normals[2][i]);
			if (i < 2) {
				std::swap(uv_list[1][i], uv_list[2][i]);
			}
		}
	}

	// sort verts by Y ( pointer[x][1] ): 
	// if y is small then it means y is at the top of the graph.
	// vertices0 will be the top vertex. then vertex1 be the second. vertex2...
	int v0 = 0, v1 = 1, v2 = 0;
	if (vertices[0][Y] <= vertices[1][Y]) {
		if (vertices[0][Y] <= vertices[2][Y]) {
			v0 = 0;
			// case 0, 1, 2  / 0, 2, 1
			v1 = vertices[1][Y] <= vertices[2][Y] ? 1 : 2;
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
		if (vertices[1][Y] <= vertices[2][Y]) {
			v0 = 1;
			// case 1 ,0 ,2 / 1, 2, 0
			v1 = vertices[0][Y] <= vertices[2][Y] ? 0 : 2;
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
		float tempV0 = vertices[v0][i];
		float tempV1 = vertices[v1][i];
		float tempV2 = vertices[v2][i];
		float tempN0 = normals[v0][i];
		float tempN1 = normals[v1][i];
		float tempN2 = normals[v2][i];
		vertices[0][i] = tempV0;
		vertices[1][i] = tempV1;
		vertices[2][i] = tempV2;
		normals[0][i] = tempN0;
		normals[1][i] = tempN1;
		normals[2][i] = tempN2;
		if (i < 2) {
			float uv0 = uv_list[v0][i];
			float uv1 = uv_list[v1][i];
			float uv2 = uv_list[v2][i];
			uv_list[0][i] = uv0;
			uv_list[1][i] = uv1;
			uv_list[2][i] = uv2;
		}
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
	// specular_componet: vertices[0,1,2] has seperate component.
	GzColor specular_component[3], diffuse_component[3], amibient_component[3], color[3];
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			specular_component[j][i] = 0;
			diffuse_component[j][i] = 0;
			amibient_component[j][i] = 0;
			color[j][i] = 0;
		}
	}


	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < numlights; i++) {
			GzCoord eyeLookat = { 0, 0, -1.0f };
			GzCoord R;
			float nDotL, nDotE;

			// for every vertex j, we calculate it seperately.

			nDotL =
				normals[j][0] * (lights[i]).direction[0]
				+ normals[j][1] * (lights[i]).direction[1]
				+ normals[j][2] * (lights[i]).direction[2];
			nDotE =
				normals[j][0] * eyeLookat[0]
				+ normals[j][1] * eyeLookat[1]
				+ normals[j][2] * eyeLookat[2];

			// for each single vertex.
			if (nDotL * nDotE > 0) {
				// it must be same direction so that the light can be seen
					// we first calculate things related to specular component.
					// R = 2(N*L)*N - L
					//  nomals[vertex matter][k(x, y, z direction)]
				for (int k = 0; k < 3; k++) {
					R[k] = 2.0f * nDotL * normals[j][k] - (lights[i]).direction[k];
				}
				float lengthR = (float)sqrt((double)(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
				for (int k = 0; k < 3; k++) {
					R[k] /= lengthR;
				}
				float rDotE = R[0] * eyeLookat[0] + R[1] * eyeLookat[1] + R[2] * eyeLookat[2];
				rDotE = rDotE > 1.0f ? 1.0 : rDotE;
				rDotE = rDotE < 0 ? 0 : rDotE;
				// add every light source one by one

				for (int k = 0; k < 3; k++) {
					if (interp_mode == GZ_COLOR && tex_fun != NULL){
						Ks[k] = 1.0f;
						Ka[k] = 1.0f;
						Kd[k] = 1.0f;
					}
					specular_component[j][k] += Ks[k] * lights[i].color[k] * pow(rDotE, spec);
					// the inside and out side teapot color should be same, so we need to inverse the
					// negative value.
					if (nDotL > 0 && nDotE > 0) {
						diffuse_component[j][k] += Kd[k] *
							(normals[j][0] * (lights[i]).direction[0]
								+ normals[j][1] * (lights[i]).direction[1]
								+ normals[j][2] * (lights[i]).direction[2]) * (lights[i]).color[k];
					}
					else {
						diffuse_component[j][k] += Kd[k] *
							(-1.0f * normals[j][0] * (lights[i]).direction[0]
								- 1.0f * normals[j][1] * (lights[i]).direction[1]
								- 1.0f * normals[j][2] * (lights[i]).direction[2]) * (lights[i]).color[k];
					}
					
				}
			}

		}
		for (int k = 0; k < 3; k++) {
			amibient_component[j][k] += Ka[k] * ambientlight.color[k];
		}
	}

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			color[j][i] = specular_component[j][i] + diffuse_component[j][i] + amibient_component[j][i];
			color[j][i] = color[j][i] > 1 ? 1 : color[j][i];
			color[j][i] = color[j][i] < 0 ? 0 : color[j][i];
		}
	}
	// get the color plane.
	// we first generate two vector x12, y12, x13m y13
	float x12 = vertices[1][0] - vertices[0][0];
	float y12 = vertices[1][1] - vertices[0][1];
	float x13 = vertices[2][0] - vertices[0][0];
	float y13 = vertices[2][1] - vertices[0][1];


	// we first generate red color plane.
	float redZ12 = color[1][0] - color[0][0];
	float redZ13 = color[2][0] - color[0][0];
	// AX + BY + CZ + D = 0
	float RedA = (y12 * redZ13) - (redZ12 * y13);
	float RedB = -((x12 * redZ13) - (redZ12 * x13));
	float RedC = (x12 * y13) - (y12 * x13);
	float RedD = -(RedA * vertices[0][0] + RedB * vertices[0][1] + RedC * color[0][0]);


	// generate green color plane:
	float greenZ12 = color[1][1] - color[0][1];
	float greenZ13 = color[2][1] - color[0][1];
	// AX + BY + CZ + D = 0
	float greenA = (y12 * greenZ13) - (greenZ12 * y13);
	float greenB = -((x12 * greenZ13) - (greenZ12 * x13));
	float greenC = (x12 * y13) - (y12 * x13);
	float greenD = -(greenA * vertices[0][0] + greenB * vertices[0][1] + greenC * color[0][1]);

	// generate blue color plane:
	float blueZ12 = color[1][2] - color[0][2];
	float blueZ13 = color[2][2] - color[0][2];
	// AX + BY + CZ + D = 0
	float blueA = (y12 * blueZ13) - (blueZ12 * y13);
	float blueB = -((x12 * blueZ13) - (blueZ12 * x13));
	float blueC = (x12 * y13) - (y12 * x13);
	float blueD = -(blueA * vertices[0][0] + blueB * vertices[0][1] + blueC * color[0][2]);


	// we also need to find the normal (X, Y, Z)
	// first interpolate normal(X,y z);
	float normalX_12 = normals[1][0] - normals[0][0];
	float normalX_13 = normals[2][0] - normals[0][0];
	float normalX_A = (y12 * normalX_13) - (normalX_12 * y13);
	float normalX_B = -((x12 * normalX_13) - (normalX_12 * x13));
	float normalX_C = (x12 * y13) - (y12 * x13);
	float normalX_D = -1.0f * (normalX_A * vertices[0][0] + normalX_B * vertices[0][1] + normalX_C * normals[0][0]);

	// interpolate normal(x,Y,z)

	float normalY_12 = normals[1][1] - normals[0][1];
	float normalY_13 = normals[2][1] - normals[0][1];
	float normalY_A = (y12 * normalY_13) - (normalY_12 * y13);
	float normalY_B = -((x12 * normalY_13) - (normalY_12 * x13));
	float normalY_C = (x12 * y13) - (y12 * x13);
	float normalY_D = -1.0f * (normalY_A * vertices[0][0] + normalY_B * vertices[0][1] + normalY_C * normals[0][1]);

	// // interpolate normal(x,y,Z)

	float normalZ_12 = normals[1][2] - normals[0][2];
	float normalZ_13 = normals[2][2] - normals[0][2];
	float normalZ_A = (y12 * normalZ_13) - (normalZ_12 * y13);
	float normalZ_B = -((x12 * normalZ_13) - (normalZ_12 * x13));
	float normalZ_C = (x12 * y13) - (y12 * x13);
	float normalZ_D = -1.0f * (normalZ_A * vertices[0][0] + normalZ_B * vertices[0][1] + normalZ_C * normals[0][2]);

	/*****************************************
	*	interpolate parameter P 
	*	We need to find our U,V(i, j) in the perspective space.
	*   We first need to get UV(v1, v2, v3) in the correspondent perspective space.
	*	then Interpolate it.
	*	
	******************************************/
	
	GzCoord p_pespective[3];
	for (int i = 0; i < 3; i++) {
		float vz_screen = vertices[i][Z];
		float vz_prime = vz_screen / (Z_MAX - vz_screen);
		p_pespective[i][U] = uv_list[i][U]/(vz_prime + 1);
		p_pespective[i][V] = uv_list[i][V]/(vz_prime + 1);
	}

	float pu_12 = p_pespective[1][U] - p_pespective[0][U];
	float pu_13 = p_pespective[2][U] - p_pespective[0][U];
	float pu_A = (y12 * pu_13) - (pu_12 * y13);
	float pu_B = -((x12 * pu_13) - (pu_12 * x13));
	float pu_C = (x12 * y13) - (y12 * x13);
	float pu_D = -1.0f * (pu_A * vertices[0][0] + pu_B * vertices[0][1] + pu_C * p_pespective[0][U]);

	float pv_12 = p_pespective[1][V] - p_pespective[0][V];
	float pv_13 = p_pespective[2][V] - p_pespective[0][V];
	float pv_A = (y12 * pv_13) - (pv_12 * y13);
	float pv_B = -((x12 * pv_13) - (pv_12 * x13));
	float pv_C = (x12 * y13) - (y12 * x13);
	float pv_D = -1.0f * (pv_A * vertices[0][0] + pv_B * vertices[0][1] + pv_C * p_pespective[0][V]);
	
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
					if (interp_mode == GZ_FLAT) {
						GzIntensity redIntensity = ctoi(flatcolor[0]);
						GzIntensity greenIntensity = ctoi(flatcolor[1]);
						GzIntensity blueIntensity = ctoi(flatcolor[2]);
						// Call GzPut to push the pixel to pixelbuffer.
						GzPut(i, j, redIntensity, greenIntensity, blueIntensity, 0, z);
					}
					else if (interp_mode == GZ_COLOR) {
						GzColor intensity;
						intensity[0] = -(RedA * (float)i + RedB * (float)j + RedD) / RedC;
						intensity[1] = -(greenA * (float)i + greenB * (float)j + greenD) / greenC;
						intensity[2] = -(blueA * (float)i + blueB * (float)j + blueD) / blueC;

						GzColor texture_color;
						if (tex_fun != NULL) {
							GzTextureIndex currentUV;
							float vzPrimeInterp = (float)z / ((float)MAXINT - (float)z);
							currentUV[U] = -1.0f * (pu_A * (float)i + pu_B * (float)j + pu_D) / pu_C;
							currentUV[V] = -1.0f * (pv_A * (float)i + pv_B * (float)j + pv_D) / pv_C;

							currentUV[U] = currentUV[U] * (vzPrimeInterp + 1.0f);
							currentUV[V] = currentUV[V] * (vzPrimeInterp + 1.0f);

							int status = tex_fun(currentUV[U], currentUV[V], texture_color);
							if (status) {
								return GZ_FAILURE;
							}

							for (int m = 0; m < 3; m++) {
								intensity[m] *= texture_color[m];
							}
						}

						GzIntensity redIntensity = ctoi(intensity[0]);
						GzIntensity greenIntensity = ctoi(intensity[1]);
						GzIntensity blueIntensity = ctoi(intensity[2]);
						GzPut(i, j, redIntensity, greenIntensity, blueIntensity, 0, z);
					}
					else if (interp_mode == GZ_NORMALS) {
						GzColor intensity;
						GzCoord normal;
						normal[0] = -1.0f * (normalX_A * (float)i + normalX_B * (float)j + normalX_D) / normalX_C;
						normal[1] = -1.0f * (normalY_A * (float)i + normalY_B * (float)j + normalY_D) / normalY_C;
						normal[2] = -1.0f * (normalZ_A * (float)i + normalZ_B * (float)j + normalZ_D) / normalZ_C;
						

						// interpolate uv, and gets it color
						GzColor texture_color;
						if (tex_fun != NULL) {
							GzTextureIndex currentUV;
							float vzPrimeInterp = (float)z / ((float)MAXINT - (float)z);
							currentUV[U] = -1.0f * (pu_A * (float)i + pu_B * (float)j + pu_D) / pu_C;
							currentUV[V] = -1.0f * (pv_A * (float)i + pv_B * (float)j + pv_D) / pv_C;

							currentUV[U] = currentUV[U] * (vzPrimeInterp + 1.0f);
							currentUV[V] = currentUV[V] * (vzPrimeInterp + 1.0f);

							int status = tex_fun(currentUV[U], currentUV[V], texture_color);
							if (status) {
								return GZ_FAILURE;
							}

							for (int m = 0; m < 3; m++) {
								Kd[m] = texture_color[m];
								Ka[m] = texture_color[m];
							}
						}

						GzColor specular, diffuse, ambient;
						// initialize component.
						for (int k = 0; k < 3; k++) {
							specular[k] = 0;
							diffuse[k] = 0;
							ambient[k] = 0;
						}
						for (int i = 0; i < numlights; i++) {
							GzCoord eyeLookat = { 0, 0, -1.0f };
							GzCoord R;
							float nDotL, nDotE;

							// for every vertex j, we calculate it seperately.
							nDotL =
								normal[0] * (lights[i]).direction[0]
								+ normal[1] * (lights[i]).direction[1]
								+ normal[2] * (lights[i]).direction[2];
							nDotE =
								normal[0] * eyeLookat[0]
								+ normal[1] * eyeLookat[1]
								+ normal[2] * eyeLookat[2];

							// for each single vertex.
							if (nDotL * nDotE > 0) {

								// we first calculate things related to specular component.
								// R = 2(N*L)*N - L
								//  nomals[vertex matter][k(x, y, z direction)]
								for (int k = 0; k < 3; k++) {
									R[k] = 2.0f * nDotL * normal[k] - (lights[i]).direction[k];
								}
								float lengthR = (float)sqrt((double)(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]));
								for (int k = 0; k < 3; k++) {
									R[k] /= lengthR;
								}


								float rDotE = R[0] * eyeLookat[0] + R[1] * eyeLookat[1] + R[2] * eyeLookat[2];
								rDotE = rDotE > 1.0f ? 1.0 : rDotE;
								rDotE = rDotE < 0 ? 0 : rDotE;

								// add every light source one by one
								for (int k = 0; k < 3; k++) {
									specular[k] += Ks[k] * lights[i].color[k] * pow(rDotE, spec);

									if (nDotL > 0 && nDotE > 0) {
										diffuse[k] += Kd[k] *
											(normal[0] * (lights[i]).direction[0]
												+ normal[1] * (lights[i]).direction[1]
												+ normal[2] * (lights[i]).direction[2]) * (lights[i]).color[k];
									}
									else {
										diffuse[k] += Kd[k] *
											(-1.0f * normal[0] * (lights[i]).direction[0]
												- 1.0f * normal[1] * (lights[i]).direction[1]
												- 1.0f * normal[2] * (lights[i]).direction[2]) * (lights[i]).color[k];
									}
								}
							}
						}

						for (int m = 0; m < 3; m++) {
							ambient[m] += Ka[m] * ambientlight.color[m];
						}

						for (int k = 0; k < 3; k++) {
							intensity[k] = specular[k] + diffuse[k] + ambient[k];
							intensity[k] = intensity[k] > 1.0 ? 1.0 : intensity[k];
							intensity[k] = intensity[k] < 0 ? 0 : intensity[k];
						}


						GzIntensity redIntensity = ctoi(intensity[0]);
						GzIntensity greenIntensity = ctoi(intensity[1]);
						GzIntensity blueIntensity = ctoi(intensity[2]);

						GzPut(i, j, redIntensity, greenIntensity, blueIntensity, 0, z);
					}


				}

			}
		}
	}
	return GZ_SUCCESS;
}


