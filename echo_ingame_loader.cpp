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

#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_stage.h>
#include <echo_loader.h>
#include <echo_ingame_loader.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

#ifdef ECHO_WIN
	#include <windows.h>
#endif
//POSIX
#ifdef ECHO_NDS
	#include <sys/dir.h>
#else
	#include <dirent.h>
#endif

#ifdef ECHO_PC
	#include <libgen.h>
#endif
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

STATUS echo_genroot(char** save)
{
	if(save != NULL)
	{
		*save = new char[2];
		CHKPTR(*save);
		(*save)[0] = '/';
		(*save)[1] = '\0';
		return(WIN);
	}
	return(FAIL);
}

STATUS echo_execdir(char** save)
{
	if(save != NULL)
	{
#ifdef 	ECHO_NDS
		return(echo_genroot(save));
#elif	ECHO_WIN
		char exe[MAX_PATH];
		GetModuleFileName(NULL, exe, MAX_PATH);
		return(echo_parentdir(exe, save));
#elif	ECHO_OSX
#else
		//copied from l-portable
		
		//step 1: get the directory of this binary
		//you can do that on the command line by: readlink /proc/$PID/exe
		pid_t pid = getpid();
		/*
			print it into a string
			since pid is an int (in gcc):
				length = strlen("/proc//exe") (the other stuff) 
							+ log(2^(sizeof(int) * 8)) (the number of digits)
							+ 1 (for null char)
				length = 10 + sizeof(int) * 8 * log(2) + 1
				length = 11 + 2.408 * sizeof(int)
			ceiling function it just to be sure...
		*/
		//just curious...
		ECHO_PRINT("sizeof int: %i\n", sizeof(int));
		//debug print out
		const int pid_str_len = (int)ceil(11 + (int)ceil(2.408 * sizeof(int)));
		ECHO_PRINT("sizeof pid_str: %i\n", pid_str_len);
		char pid_str[pid_str_len];
		//and finally print it into the string
		sprintf(pid_str, "/proc/%d/exe", pid);
		//print out your results
		ECHO_PRINT("PID_STR: %s\n", pid_str);
		//copied from: https://www.securecoding.cert.org/confluence/display/seccode/POS30-C.+Use+the+readlink()+function+properly
		char buf[FILENAME_MAX];
		//actually do the readlink, and return the length
		ssize_t len = readlink(pid_str, buf, sizeof(buf) - 1);
		//if it actully followed the link
		if(len != -1)
		{
			//insert the null char
			buf[len] = '\0';
			//debug print
			ECHO_PRINT("BINARY LOCATION: %s\n", buf);
			//return the directory the program is in
			return(echo_parentdir(buf, save));
		}
		else
		{
			//get the current directory anyways
			char* pwd = getenv("PWD");
			*save = new char[strlen(pwd) + 1];
			CHKPTR(*save);
			strcpy(*save, pwd);
			return(WIN);
		}
#endif
	}
	return(FAIL);
}

STATUS echo_parentdir(const char* path, char** save)
{
	if(path != NULL && save != NULL)
	{
#ifdef ECHO_NDS
		//if not root
		if(strcmp(path, "/"))
		{
			//index of last '/' character
			const int len =  strrchr(path, '/') - path;
			//not root
			if(len > 0)
			{
				*save = new char[len + 1];
				CHKPTR(*save);
				//cut off last '/' and after
				strncpy(*save, path, len);
				return(WIN);
			}
		}
		//if going to root (len == 0), is at root (!strcmp(path, "/")), or error (len < 0), write root anyways
		return(echo_genroot(save));
#else
		const char* dir = dirname(const_cast<char*>(path));
		*save = new char[strlen(dir) + 1];
		CHKPTR(*save);
		strcpy(*save, dir);
		//ECHO_PRINT("save: %s\n", *save);
		return(WIN);
#endif
	}
	return(FAIL);
}

STATUS delete_echo_files(echo_files* files)
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
		if(files->current_dir != NULL)
			delete[] files->current_dir;
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
                                memset(ret->file_names[each], 0
									, strlen(each_ent->d_name) + 1);
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
				memset(ret->file_names[each], 0
					, strlen(filename) + 1);
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

