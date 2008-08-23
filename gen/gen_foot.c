#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI		3.141592f
#define LATS		8
#define ANGLE_INCR	PI / LATS
#define X_RADIUS	0.075f
#define Y_RADIUS	0.15f
#define Z_RADIUS	0.2f
#define TOP		0
#define BOTTOM		PI / 2

#define floattov16(n)        ((short int)((n) * (1 << 12)))

void print_sphere_pt(float theta, float phi);
void print_sphere_pt_nds(float theta, float phi);

int main()
{
	printf("void draw_foot()\n{\n");
	printf("#ifndef ECHO_NDS\n");
	float theta = 0;
	while(theta < PI)
	{
		float phi = TOP, u = 0;
		printf("\tglBegin(GL_QUAD_STRIP);\n");
		while(phi <= BOTTOM)
		{
			print_sphere_pt(theta, phi);
			print_sphere_pt(theta + ANGLE_INCR, phi);
			phi += ANGLE_INCR;
		}
		printf("\tglEnd();\n");
		theta += ANGLE_INCR;
	}
	printf("#else\n");
	theta = PI / 2;
	while(theta < PI * 3 / 2)
	{
		float phi = TOP, u = 0;
		printf("\tglBegin(GL_QUAD_STRIP);\n");
		while(phi <= BOTTOM)
		{
			print_sphere_pt_nds(theta, phi);
			print_sphere_pt_nds(theta + ANGLE_INCR, phi);
			phi += ANGLE_INCR;
		}
		printf("\tglEnd();\n");
		theta += ANGLE_INCR;
	}
	printf("#endif\n");
	printf("}\n");
}

void print_sphere_pt(float theta, float phi)
{
	printf("\t\tglVertex3f(%.3ff, %.3ff, %.3ff);\n"
			, X_RADIUS * (cos(theta) * sin(phi))
			, Y_RADIUS * (cos(phi))
			, Z_RADIUS * (sin(theta) * sin(phi)));
}

void print_sphere_pt_nds(float theta, float phi)
{
	printf("\t\tglVertex3v16(%i, %i, %i);\n"
			, floattov16(X_RADIUS * (cos(theta) * sin(phi)))
			, floattov16(Y_RADIUS * (cos(phi)))
			, floattov16(Z_RADIUS * (sin(theta) * sin(phi))));
}

