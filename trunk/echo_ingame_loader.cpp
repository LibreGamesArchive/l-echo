// echo_ingame_loader.cpp

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

#include <cstdlib>
#include <cstring>
#include <iostream>
//POSIX
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <echo_error.h>
#include <echo_stage.h>
#include <echo_loader.h>
#include <echo_ingame_loader.h>

char* echo_merge(const char* arg1, const char* arg2)
{
	char* ret = new char[strlen(arg1) + strlen(arg2) + 2];
	CHKPTR(ret);
	strcat(strcat(strcpy(ret, arg1),  "/"), arg2);
	return(ret);
}

int is_dir(const char* dir, const char* fname)
{
	if(!strcmp(fname, ".."))
		return(1);
	char* merged = echo_merge(dir, fname);
	int ret = is_dir(merged);
	delete merged;
	return(ret);
}

int is_dir(const char* fname)
{
	int fd = open(fname, O_RDONLY);
	struct stat *file_stat = new(struct stat);
	CHKPTR(file_stat);
	int exists = !fstat(fd, file_stat);
	close(fd);
	if(exists)
	{
		int ret = S_ISDIR(file_stat->st_mode);
		delete file_stat;
		return(ret);
	}
	delete file_stat;
	return(0);
}

static char* cmp_dir;

static int cmp(const void* arg1v, const void* arg2v)
{
	const char* arg1 = *((char**)arg1v);
	const char* arg2 = *((char**)arg2v);
	//std::cout << "cmp: " <<  arg1v << ", " << arg2v<< std::endl;
	int arg1isdir = is_dir(cmp_dir, arg1), arg2isdir = is_dir(cmp_dir, arg2);
	if(arg1isdir && !arg2isdir)
		return(-1);
	if(!arg1isdir && arg2isdir)
		return(1);
	return(strcmp(arg1, arg2));
}

echo_files* get_files(const char* dirname)
{
	int fd = open(dirname, O_RDONLY);
	struct stat dir_stat;
	int exists = !fstat(fd, &dir_stat);
	close(fd);
	if(exists)
	{
		DIR* dir = opendir(dirname);
		echo_files* ret = new(echo_files);
		ret->num_files = 0;
		ret->current_dir = const_cast<char*>(dirname);
		dirent* each_ent;
		while(each_ent = readdir(dir))
		{
			//std::cout << "each_ent->d_name: " << each_ent->d_name << std::endl;
			if(strcmp(each_ent->d_name, ".") && strcmp(each_ent->d_name, ".."))
				ret->num_files++;
		}
		//std::cout << "each_ent: " << each_ent << std::endl;
		ret->num_files++;
		ret->file_names = new char*[ret->num_files];
		CHKPTR(ret->file_names);
		ret->file_names[0] = "..";
		
		int each = 1;
		
		rewinddir(dir);
		while(each_ent = readdir(dir))
		{
			/*
			fname = new char[strlen(dirname) + strlen(each_ent->d_name) + 1];
			ret->file_names[each] = strcat(strcpy(fname, dirname)
				, strcmp(".", each_ent->d_name) ? each_ent->d_name : "..");
			// */
			if(strcmp(each_ent->d_name, ".") && strcmp(each_ent->d_name, ".."))
			{
				ret->file_names[each] = new char[strlen(each_ent->d_name) + 1];
				CHKPTR(ret->file_names[each]);
				strcpy(ret->file_names[each], each_ent->d_name);
				each++;
			}
		}
		//std::cout << "num_files + each: " << ret->num_files << ", " << each << std::endl;
		closedir(dir);
		
		cmp_dir = ret->current_dir;
		//std::cout << "cmp_dir: " << cmp_dir << std::endl;
		qsort(ret->file_names, ret->num_files, sizeof(char*), cmp);
		return(ret);
	}
	return(NULL);
}

void dump_files(echo_files* files)
{
	int each = 0;
	std::cout << "files->num_files: " << files->num_files << std::endl;
	while(each < files->num_files)
	{
		std::cout << "files: " << files->file_names[each] << std::endl;
		each++;
	}
}

