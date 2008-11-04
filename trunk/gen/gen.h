//gen.h

#define PI					3.141592f
#define LATS				6
#define NDS_LATS			2
#define ANGLE_INCR			PI / LATS
#define NDS_ANGLE_INCR		PI / NDS_LATS

#define floattov16(n)       ((short int)((n) * (1 << 12)))

#define PRINT_STATS			printf("//stats: LATS: %i, ANGLE_INCR: %f, NDS_LATS: %i, NDS_ANGLE_INCR: %f\n", \
								LATS, ANGLE_INCR, NDS_LATS, NDS_ANGLE_INCR)

void print_pt(float x, float y, float z)
{
	printf("\t\tglVertex3v16(%i, %i, %i);\n", floattov16(x), floattov16(y), floattov16(z));
	//printf("\t\tglVertex3f(%.3ff, %.3ff, %.3ff);\n", x, y, z);
}
