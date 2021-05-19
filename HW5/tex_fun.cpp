/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

	for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
		fread(pixel, sizeof(pixel), 1, fd);
	
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }
  
/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */

   //clamp uv to 0~1
  u = u > 1.0 ? 1.0 : u;
  u = u < 0 ? 0 : u;
  v = v > 1.0 ? 1.0 : v;
  v = v < 0 ? 0 : v;

  // 
  u = u * (float)(xs - 1);
  v = v * (float)(ys - 1);

  int x = (int)floor(u);
  int y = (int)floor(v);


  float s = u - (float)x;
  float t = v - (float)y;

    //	. . . . 
	//	. 0 1 .
	//  . 2 3 .
	//  . . . .
	

  GzColor corner_color[4];
  int indexX[] = {0, 1, 0, 1};
  int indexY[] = { 0, 0, 1, 1 };
  for (int j = 0; j < 4; j++) {
	  for (int i = 0; i < 3; i++) {
		  corner_color[j][i] = image[(y + indexY[j]) * xs + x + indexX[j]][i];
	  }
  }
  // notice the order of its four corners...
  for (int i = 0; i < 3; i++) {
	  color[i] = s * t * corner_color[3][i] + (1 - s) * t * corner_color[2][i] +
		  s * (1 - t) * corner_color[1][i] + (1 - s)* (1 - t)* corner_color[0][i];
  }

  return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	int xs = 250;
	int ys = 250;
	// render if even then colored, if odd then black.
	//clamp uv to 0~1
	u = u > 1.0 ? 1.0 : u;
	u = u < 0 ? 0 : u;
	v = v > 1.0 ? 1.0 : v;
	v = v < 0 ? 0 : v;

	// 
	u = u * (int)(xs - 1);
	v = v * (int)(ys - 1);

	bool xeven = (int)u / 25 % 2 == 0 ? 1 : 0;
	bool yeven = (int)v / 25 % 2 == 0 ? 1 : 0;
	if ((xeven && yeven) || (!xeven && !yeven)) {
		for (int i = 0; i < 3; i++) {
			color[i] = 1;
		}
	}
	else {
		for (int i = 0; i < 3; i++) {
			color[i] = 0;
		}
	}

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

