// echo_ingame_loader.h

/*
    This file is part of L-Echo.

    L-Echo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    L-Echo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with L-Echo.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <echo_stage.h>
#include <echo_loader.h>

#ifndef __ECHO_INTERNAL_FILES__
#define __ECHO_INTERNAL_FILES__
typedef struct
{
	char* current_dir;
	char** file_names;
	int num_files;
#ifdef ARM9
	int num_dir;
#endif
} echo_files;
#endif

STATUS delete_echo_files(echo_files* files);

#ifndef ARM9
int is_dir(const char* dir, const char* fname);
int is_dir(const char* fname);
#endif

char* echo_merge(const char* arg1, const char* arg2);
int is_dir(echo_files* files, int file_index);
echo_files* get_files(const char* dirname);
void dump_files(echo_files* files);

//get the previous directory
STATUS echo_parentdir(const char* path, char** save);
//get the directory of the executable
STATUS echo_execdir(char** save);
STATUS echo_genroot(char** save);
