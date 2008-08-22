#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI		3.141592f
#define LATS		8
#define ANGLE_INCR	PI / LATS
#define RADIUS		0.3f
#define HEIGHT		0.9f
#define FRUST_HEIGHT	0.5f
#define RAD_DIV_HEIGHT	0.333f
#define CONE_INCR	0.05f

#define floattov16(n)        ((short int)((n) * (1 << 12)))

void print_sphere_pt(float theta, float phi);
void print_cone_pt(float theta, float u);
void print_sphere_pt_nds(float theta, float phi);
void print_cone_pt_nds(float theta, float u);

int main()
{
	printf("void draw_head()\n{\n");
	printf("#ifndef ECHO_NDS\n");
	/*
		printf("\tglutSolidSphere(%.3ff, %i, %i);\n", RADIUS, LATS, LATS);
		printf("\tglPushMatrix();\n");
			printf("\t\tglRotatef(90, 1, 0, 0);\n");
			printf("\t\tglutSolidCone(%.3ff, %.3ff, %i, %i);\n", RADIUS, HEIGHT, LATS, LATS);
		printf("\tglPopMatrix();\n");
	// */
	//*
	float theta = 0;
	while(theta <= PI * 2)
	{
		float phi = 0, u = 0;
		printf("\tglBegin(GL_QUAD_STRIP);\n");
		while(phi <= PI / 2)
		{
			print_sphere_pt(theta, phi);
			print_sphere_pt(theta + ANGLE_INCR, phi);
			phi += ANGLE_INCR;
		}
		while(u < FRUST_HEIGHT)
		{
			print_cone_pt(theta, u);
			print_cone_pt(theta + ANGLE_INCR, u);
			u += CONE_INCR;
		}
		printf("\tglEnd();\n");
		theta += ANGLE_INCR;
	}
	// */
	printf("#else\n");
	//float theta = 0;
	theta = 0;
	while(theta <= PI * 2)
	{
		float phi = 0, u = 0;
		printf("\tglBegin(GL_QUAD_STRIP);\n");
		while(phi <= PI / 2)
		{
			print_sphere_pt_nds(theta, phi);
			print_sphere_pt_nds(theta + ANGLE_INCR, phi);
			phi += ANGLE_INCR;
		}
		while(u < FRUST_HEIGHT)
		{
			print_cone_pt_nds(theta, u);
			print_cone_pt_nds(theta + ANGLE_INCR, u);
			u += CONE_INCR;
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
			, RADIUS * (cos(theta) * sin(phi))
			, RADIUS * (cos(phi))
			, RADIUS * (sin(theta) * sin(phi)));
}

void print_cone_pt(float theta, float u)
{
	printf("\t\tglVertex3f(%.3ff, %.3ff, %.3ff);\n"
			, RAD_DIV_HEIGHT * cos(theta) * (HEIGHT - u)
			, -u
			, RAD_DIV_HEIGHT * sin(theta) * (HEIGHT - u));
}

void print_sphere_pt_nds(float theta, float phi)
{
	printf("\t\tglVertex3v16(%i, %i, %i);\n"
			, floattov16(RADIUS * (cos(theta) * sin(phi)))
			, floattov16(RADIUS * (cos(phi)))
			, floattov16(RADIUS * (sin(theta) * sin(phi))));
}

void print_cone_pt_nds(float theta, float u)
{
	printf("\t\tglVertex3v16(%i, %i, %i);\n"
			, floattov16(RAD_DIV_HEIGHT * cos(theta) * (HEIGHT - u))
			, floattov16(-u)
			, floattov16(-RAD_DIV_HEIGHT * sin(theta) * (HEIGHT - u)));
}

