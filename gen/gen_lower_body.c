#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gen.h>

#define X_RADIUS	0.2f
#define Y_RADIUS	0.3f
#define Z_RADIUS	0.15f
#define TOP			PI / 6
#define	BOTTOM		PI * 0.55f
#define NDS_BOTTOM	PI / 6 + ANGLE_INCR

void print_sphere_pt(float theta, float phi);
void print_sphere_pt_nds(float theta, float phi);

int main()
{
	PRINT_STATS;
	printf("void draw_lower_body()\n{\n");
	printf("#ifndef ECHO_NDS\n");
	float theta = 0;
	while(theta <= PI * 2)
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
	printf("\tglBegin(GL_QUAD_STRIP);\n");	//10 verts
		print_pt(-X_RADIUS, -Y_RADIUS * sin(TOP), Z_RADIUS);
		print_pt(-X_RADIUS, Y_RADIUS * sin(BOTTOM), Z_RADIUS);	//front left
		print_pt(X_RADIUS, -Y_RADIUS * sin(TOP), Z_RADIUS);
		print_pt(X_RADIUS, Y_RADIUS * sin(BOTTOM), Z_RADIUS);	//front right
		print_pt(X_RADIUS, -Y_RADIUS * sin(TOP), -Z_RADIUS);
		print_pt(X_RADIUS, Y_RADIUS * sin(BOTTOM), -Z_RADIUS);	//back right
		print_pt(-X_RADIUS, -Y_RADIUS * sin(TOP), -Z_RADIUS);
		print_pt(-X_RADIUS, Y_RADIUS * sin(BOTTOM), -Z_RADIUS);	//back left
		print_pt(-X_RADIUS, -Y_RADIUS * sin(TOP), Z_RADIUS);
		print_pt(-X_RADIUS, Y_RADIUS * sin(BOTTOM), Z_RADIUS);	//front left
	printf("\tglEnd();\n");
	/*
	theta = 0;
	while(theta <= PI * 2)
	{
		float phi = TOP, u = 0;
		printf("\tglBegin(GL_QUAD_STRIP);\n");
		while(phi <= NDS_BOTTOM)
		{
			print_sphere_pt_nds(theta, phi);
			print_sphere_pt_nds(theta + NDS_ANGLE_INCR, phi);
			phi += NDS_ANGLE_INCR;
		}
		printf("\tglEnd();\n");
		theta += NDS_ANGLE_INCR;
	}
	// */
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

