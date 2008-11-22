#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gen.h>

#define X_RADIUS	0.075f
#define Y_RADIUS	FOOT_HEIGHT
#define Z_RADIUS	0.2f
#define TOP			0
#define BOTTOM		PI / 2


void print_sphere_pt(float theta, float phi);
void print_sphere_pt_nds(float theta, float phi);

int main()
{
	PRINT_STATS;
	printf("void draw_foot()\n{\n");
	printf("#ifndef ECHO_NDS\n");
	float theta = 0;
	while(theta <= PI)
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
	printf("\tglBegin(GL_QUAD_STRIP);\n");	//12 verts
		print_pt(-X_RADIUS, 0, 0);
		print_pt(-X_RADIUS, 0, Z_RADIUS);
		print_pt(-X_RADIUS, Y_RADIUS, 0);
		print_pt(-X_RADIUS, Y_RADIUS, Z_RADIUS);
		print_pt(X_RADIUS, Y_RADIUS, 0);
		print_pt(X_RADIUS, Y_RADIUS, Z_RADIUS);
		print_pt(X_RADIUS, 0, 0);
		print_pt(X_RADIUS, 0, Z_RADIUS);
	printf("\tglEnd();\n");
	printf("\tglBegin(GL_QUADS);\n");
		print_pt(-X_RADIUS, 0, Z_RADIUS);
		print_pt(-X_RADIUS, Y_RADIUS, Z_RADIUS);
		print_pt(X_RADIUS, Y_RADIUS, Z_RADIUS);
		print_pt(X_RADIUS, 0, Z_RADIUS);
	printf("\tglEnd();\n");
	printf("#endif\n");
	printf("}\n");
}

void print_sphere_pt(float theta, float phi)
{
	printf("\t\tglVertex3f(%.3ff, %.3ff, %.3ff);\n"
			, -X_RADIUS * (cos(theta) * sin(phi))
			, Y_RADIUS * (cos(phi))
			, Z_RADIUS * (sin(theta) * sin(phi)));
}

