#include <stdio.h>

#define floattov16(n)        ((short int)((n) * (1 << 12)))

int print_id(int id);

int main()
{
	float each = -2 * 0.166f;
	printf("#ifdef ARM9\n");
	printf("\t#define SET_ID(id)	gfx_set_polyID(id)\n");
	printf("#else\n");
	printf("\t#define SET_ID(id)\n");
	printf("#endif\n\n");
	printf("void draw_stairs()\n{\n");
	printf("#ifndef ECHO_NDS\n");
	printf("\tglBegin(GL_QUAD_STRIP);\n");
	while(each <= 0.5f)
	{
		printf("\t\tglVertex3f(0.5f, %.3ff, %.3ff);\n", each, each);
		printf("\t\tglVertex3f(-0.5f, %.3ff, %.3ff);\n", each, each);
		printf("\t\tglVertex3f(0.5f, %.3ff, %.3ff);\n", each - 0.166f, each);
		printf("\t\tglVertex3f(-0.5f, %.3ff, %.3ff);\n", each - 0.166f, each);
		each += 0.166f;
	}
	printf("\t\tglVertex3f(0.5f, 0.5f, 0.5f);\n");
	printf("\t\tglVertex3f(-0.5f, 0.5f, 0.5f);\n");
	printf("\tglEnd();\n");
	printf("#else\n");
	each = -2 * 0.166f;
	int id = 16;
	while(each <= 0.5f)
	{
		id = print_id(id);
		/*
		draw_rect(0.5f, each, each, 0.5f, each - 0.166f, each
			, -0.5f, each - 0.166f, each, -0.5f, each, each);
		// */
		printf("\tdraw_rectv16(%i, %i, %i, %i, %i, %i\n\t\t, %i, %i, %i, %i, %i, %i);\n"
				, floattov16(0.5f), floattov16(each), floattov16(each)
				, floattov16(0.5f), floattov16(each - 0.166f), floattov16(each)
				, floattov16(-0.5f), floattov16(each - 0.166f), floattov16(each)
				, floattov16(-0.5f), floattov16(each), floattov16(each));
		id = print_id(id);
		/*
		draw_rect(0.5f, each - 0.166f, each - 0.166f, 0.5f, each - 0.166f, each
			, -0.5f, each - 0.166f, each, -0.5f, each - 0.166f, each - 0.166f);
		// */
		//*
		printf("\tdraw_rectv16(%i, %i, %i, %i, %i, %i\n\t\t, %i, %i, %i, %i, %i, %i);\n"
				, floattov16(0.5f), floattov16(each - 0.166f), floattov16(each - 0.166f)
				, floattov16(0.5f), floattov16(each - 0.166f), floattov16(each)
				, floattov16(-0.5f), floattov16(each - 0.166f), floattov16(each)
				, floattov16(-0.5f), floattov16(each - 0.166f), floattov16(each - 0.166f));
		// */
		
		each += 0.166f;
	}
	printf("#endif\n}\n");
}

int print_id(int id)
{
	printf("\tSET_ID(%i);\n", id);
	return(id == 18 ? 16 : id + 1);
}
