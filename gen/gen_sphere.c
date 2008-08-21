#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI		3.141592f
#define LATS		8
#define ANGLE_INCR	PI / LATS

#define floattov16(n)        ((short int)((n) * (1 << 12)))

void print_pt(double r, float theta, float phi);

int main(int argc, char *argv[])
{
	if(argc >= 2)
	{
		double r = strtod(argv[1], NULL);
		//*
		printf("void draw_sphere()\n{\n");
		printf("#ifndef ECHO_NDS\n");
			printf("\tglutSolidSphere(%.3ff, %i, %i);\n", r, LATS, LATS);
		printf("#else\n");
		float theta = 0;
		while(theta <= PI * 2)
		{
			float phi = 0;
			printf("\tglBegin(GL_QUAD_STRIP);\n");
			while(phi <= PI)
			{
				print_pt(r, theta, phi);
				print_pt(r, theta + ANGLE_INCR, phi);
				//print_pt(r, theta + ANGLE_INCR, phi + ANGLE_INCR);
				//print_pt(r, theta, phi + ANGLE_INCR);
				phi += ANGLE_INCR;
			}
			/*
			print_pt(r, theta, PI);
			print_pt(r, theta + ANGLE_INCR, PI);
			// */
			printf("\tglEnd();\n");
			theta += ANGLE_INCR;
		}
		/*
		float phi = 0;
		while(phi < PI)
		{
			float theta = 0;
			int lower = 1;
			printf("\tglBegin(GL_QUAD_STRIP);\n");
			printf("\t\tglVertex3v16(%i, 0, %i);\n"
					, floattov16(r * (sin(phi)))
					, floattov16(r * (cos(phi))));
			while(theta <= PI * 2)
			{
				if(lower)
				{
					printf("\t\tglVertex3v16(%i, %i, %i);\n"
						, floattov16(r * (cos(theta) * sin(phi + ANGLE_INCR)))
						, floattov16(r * (sin(theta) * sin(phi + ANGLE_INCR)))
						, floattov16(r * (cos(phi + ANGLE_INCR))));
					printf("\t\tglVertex3v16(%i, %i, %i);\n"
						, floattov16(r * (cos(theta + ANGLE_INCR) * sin(phi + ANGLE_INCR)))
						, floattov16(r * (sin(theta + ANGLE_INCR) * sin(phi + ANGLE_INCR)))
						, floattov16(r * (cos(phi + ANGLE_INCR))));
				}
				else
				{
					printf("\t\tglVertex3v16(%i, %i, %i);\n"
						, floattov16(r * (cos(theta) * sin(phi)))
						, floattov16(r * (sin(theta) * sin(phi)))
						, floattov16(r * (cos(phi))));
					printf("\t\tglVertex3v16(%i, %i, %i);\n"
						, floattov16(r * (cos(theta + ANGLE_INCR) * sin(phi)))
						, floattov16(r * (sin(theta + ANGLE_INCR) * sin(phi)))
						, floattov16(r * (cos(phi))));
				}
				theta += ANGLE_INCR;
				lower = !lower;
			}
			printf("\t\tglVertex3v16(%i, 0, %i);\n"
					, floattov16(r * (sin(phi + ANGLE_INCR)))
					, floattov16(r * (cos(phi + ANGLE_INCR))));
			printf("\tglEnd();\n");
			phi += ANGLE_INCR;
		}
		// */
		printf("#endif\n");
		printf("}\n");
		// */
	}
	else
	{
		printf("please give the radius as an argument...\n");
	}
}

void print_pt(double r, float theta, float phi)
{
	/*
	printf("\t\tglVertex3f(%.3ff, %.3ff, %.3ff);\n"
			, r * (cos(theta) * sin(phi))
			, r * (sin(theta) * sin(phi))
			, r * (cos(phi)));
	// */
	//*
	printf("\t\tglVertex3v16(%i, %i, %i);\n"
			, floattov16(r * (cos(theta) * sin(phi)))
			, floattov16(r * (sin(theta) * sin(phi)))
			, floattov16(r * (cos(phi))));
	// */
}
