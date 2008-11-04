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
#ifdef WIN32
	#include <windows.h>
#endif
//POSIX
#ifdef ARM9
	#include <sys/dir.h>
#else
	#include <dirent.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <echo_debug.h>
#include <echo_error.h>
#include <echo_stage.h>
#include <echo_loader.h>
#include <echo_ingame_loader.h>

int delete_echo_files(echo_files* files)
{
	if(files != NULL)
	{
		//if(files->current_dir != NULL)
		//	delete[] files->current_dir;
		if(files->file_names != NULL)
		{
			int each = 1;	//0 is ALWAYS constant ".."
			while(each < files->num_files)
			{
				char* del = files->file_names[each];
				if(del != NULL)
					delete[] del;
				each++;
			}
			delete[] files->file_names;
		}
		delete files;
		return(WIN);
	}
	return(FAIL);
}

char* echo_merge(const char* arg1, const char* arg2)
{
	char* ret = new char[strlen(arg1) + strlen(arg2) + 2];
	CHKPTR(ret);
#ifdef ECHO_WIN
	char* slash = strrchr(arg1, '\\');
#else
	char* slash = strrchr(arg1, '/');
#endif
	if(slash && slash == arg1 + strlen(arg1) - 1)
		strcat(strcpy(ret, arg1), arg2);
	else
#ifdef ECHO_WIN
		strcat(strcat(strcpy(ret, arg1),  "\\"), arg2);
#else
		strcat(strcat(strcpy(ret, arg1),  "/"), arg2);
#endif
	return(ret);
}

#ifndef ECHO_NDS
int is_dir(const char* dir, const char* fname)
{
	if(!strcmp(fname, ".."))
		return(1);
	char* merged = echo_merge(dir, fname);
	int ret = is_dir(merged);
	delete[] merged;
	return(ret);
}

int is_dir(const char* fname)
{
#ifdef ECHO_WIN
	DWORD attrs = GetFileAttributes(fname);
	return(attrs != INVALID_FILE_ATTRIBUTES 
		&& (attrs & FILE_ATTRIBUTE_DIRECTORY));	//exists and is directory
#else
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
#endif
}

int is_dir(echo_files* files, int file_index)
{
	return(is_dir(files->current_dir, files->file_names[file_index]));
}

static const char* cmp_dir;

static int cmp(const void* arg1v, const void* arg2v)
{
	const char* arg1 = *((char**)arg1v);
	const char* arg2 = *((char**)arg2v);
	int arg1isdir = is_dir(cmp_dir, arg1), arg2isdir = is_dir(cmp_dir, arg2);
	if(arg1isdir && !arg2isdir)
		return(-1);
	if(!arg1isdir && arg2isdir)
		return(1);
	return(strcmp(arg1, arg2));
}

echo_files* get_files(const char* dirname)
{
#ifdef WIN32
	WIN32_FIND_DATA find_data;
	HANDLE find = FindFirstFile(dirname, &find_data);
	int exists = (find != INVALID_HANDLE_VALUE);
	FindClose(find);
#else
	int fd = open(dirname, O_RDONLY);
        struct stat dir_stat;
        int exists = !fstat(fd, &dir_stat);
        close(fd);
#endif
	if(exists)
	{
                DIR* dir = opendir(dirname);
                echo_files* ret = new(echo_files);
                ret->num_files = 0;
                ret->current_dir = const_cast<char*>(dirname);
                dirent* each_ent;
                while((each_ent = readdir(dir)) != NULL)
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
                while((each_ent = readdir(dir)) != NULL)
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
#else
int is_dir(echo_files* files, int file_index)
{
	return(file_index < files->num_dir);
}

static int cmp(const void* arg1v, const void* arg2v)
{
	return(strcmp(*((char**)arg1v), *((char**)arg2v)));
}

echo_files* get_files(const char* dirname)
{
	DIR_ITER* dir = diropen(dirname);
	if(dir)
	{
		echo_files* ret = new(echo_files);
		ret->num_files = 0;
		ret->current_dir = const_cast<char*>(dirname);
		ret->num_dir = 1;
		
		struct stat each_ent;
		char* filename = new char[MAXPATHLEN];
		while(dirnext(dir, filename, &each_ent) == 0)
		{
			//ECHO_PRINT("fname: %s\n", filename);
			if(strcmp(filename, ".") && strcmp(filename, ".."))
			{
				ret->num_files++;
				if(S_ISDIR(each_ent.st_mode))
					ret->num_dir++;
			}
		}
		ret->num_files++;
		ret->file_names = new char*[ret->num_files];
		CHKPTR(ret->file_names);
		ret->file_names[0] = "..";
		
		int each_dir = 1, each_file = ret->num_dir, each = 1;
		dirreset(dir);
		while(dirnext(dir, filename, &each_ent) == 0)
		{
			if(strcmp(filename, ".") && strcmp(filename, ".."))
			{
				if(S_ISDIR(each_ent.st_mode))
				{
					each = each_dir;
					each_dir++;
				}
				else
				{
					each = each_file;
					each_file++;
				}
				ret->file_names[each] = new char[strlen(filename) + 1];
				CHKPTR(ret->file_names[each]);
				memset(ret->file_names[each], '.'
					, strlen(ret->file_names[each]));
				strcpy(ret->file_names[each], filename);
			}
		}
		
		delete[] filename;
		dirclose(dir);
		
		//ECHO_PRINT("num_dir: %i\n", ret->num_dir);
		//cmp_dir = ret->current_dir;
		qsort(ret->file_names, ret->num_dir, sizeof(char*), cmp);
		qsort(ret->file_names + ret->num_dir
			, ret->num_files - ret->num_dir, sizeof(char*), cmp);
		//dump_files(ret);
		return(ret);
	}
	return(NULL);
}
#endif

void dump_files(echo_files* files)
{
	int each = 0;
	ECHO_PRINT("files->num_files: %i\n", files->num_files);
	while(each < files->num_files)
	{
		ECHO_PRINT("files: %s\n", files->file_names[each]);
		each++;
	}
}

