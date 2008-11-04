#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gen.h>

void print_sphere_pt(double r, float theta, float phi);

int main(int argc, char *argv[])
{
	PRINT_STATS;
	if(argc >= 2)
	{
		double r = strtod(argv[1], NULL);
		printf("void draw_sphere()\n{\n");
		printf("#ifndef ECHO_NDS\n");
			printf("\tglutSolidSphere(%.3ff, %i, %i);\n", r, LATS, LATS);
		printf("#else\n");
		printf("\tglBegin(GL_QUAD_STRIP);\n");		//10 vertices
			print_pt(-r, r, -r);
			print_pt(-r, -r, -r);
			print_pt(r, r, -r);
			print_pt(r, -r, -r);
			print_pt(r, r, r);
			print_pt(r, -r, r);
			print_pt(-r, r, r);
			print_pt(-r, -r, r);
			print_pt(-r, r, -r);
			print_pt(-r, -r, -r);
		printf("\tglEnd();\n");
		/*
		float theta = 0;
		while(theta <= PI * 2)
		{
			float phi = 0;
			printf("\tglBegin(GL_QUAD_STRIP);\n");
			while(phi <= PI)
			{
				print_sphere_pt(r, theta, phi);
				print_sphere_pt(r, theta + NDS_ANGLE_INCR, phi);
				phi += NDS_ANGLE_INCR;
			}
			printf("\tglEnd();\n");
			theta += NDS_ANGLE_INCR;
		}
		// */
		printf("#endif\n");
		printf("}\n");
	}
	else
	{
		printf("please give the radius as an argument...\n");
	}
}

void print_sphere_pt(double r, float theta, float phi)
{
	printf("\t\tglVertex3v16(%i, %i, %i);\n"
			, floattov16(r * (cos(theta) * sin(phi)))
			, floattov16(r * (sin(theta) * sin(phi)))
			, floattov16(r * (cos(phi))));
}
