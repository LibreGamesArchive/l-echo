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

/// Various echo libraries
#include <echo_platform.h>
#include <echo_debug.h>
#include <echo_error.h>
#include <echo_stage.h>
#include <echo_loader.h>
#include <echo_ingame_loader.h>

/// Standard libraries
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cmath>

/// Windows functions, because some POSIX functions inexplicitly stop working in Windows =|
#ifdef ECHO_WIN
	#include <windows.h>
#endif

/// POSIX, for directories
#ifdef ECHO_NDS
	#include <sys/dir.h>
#else
	#include <dirent.h>
#endif

/// Functions for base directory names
#ifdef ECHO_PC
	#include <libgen.h>
#endif

/// Types and stuff
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/** Generate a new string at "save" that is equal to "/"; used because
 * paths have to be dynamically allocated.  Delete after use.
 * @param save Pointer to where to put the string; remember to delete it after use
 */
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
/** Get the executable's directory, if possible:\n
 * Windows and Linux - Works\n
 * OSX - Current directory\n
 * NDS - Just root\n
 * @param save Where to put the directory
 */
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
		return(echo_currentdir(save));
#else
		/// copied from l-portable
		/// Step 1: get the directory of this binary
		/// You can do that on the command line by: readlink /proc/$PID/exe
		pid_t pid = getpid();
		/**
			print it into a string
			since pid is an int (in gcc):
				length = strlen("/proc//exe") (the other stuff) 
							+ log(2^(sizeof(int) * 8)) (the number of digits)
							+ 1 (for null char)
				length = 10 + sizeof(int) * 8 * log(2) + 1
				length = 11 + 2.408 * sizeof(int)
			ceiling function it just to be sure...
		*/
		/// Debug print out
		const int pid_str_len = (int)ceil(11 + (int)ceil(2.408 * sizeof(int)));
		char pid_str[pid_str_len];
		/// And finally print it into the string
		sprintf(pid_str, "/proc/%d/exe", pid);
		/// Copied from: https://www.securecoding.cert.org/confluence/display/seccode/POS30-C.+Use+the+readlink()+function+properly
		char buf[FILENAME_MAX];
		/// Actually do the readlink, and return the length
		ssize_t len = readlink(pid_str, buf, sizeof(buf) - 1);
		/// If it actully followed the link
		if(len != -1)
		{
			/// Insert the null char
			buf[len] = '\0';
			/// Return the directory the program is in
			return(echo_parentdir(buf, save));
		}
		else
		{
			/// Get the current directory anyways
			return(echo_currentdir(save));
		}
#endif
	}
	return(FAIL);
}
/** Gets the prefs filename: \n
 * Win/Lin/Mac - echo_execdir() + "prefs.xml"\n
 * NDS - "/apps/n-echo/prefs.xml"\n
 * @param save Where to put the file name
 */
STATUS echo_prefsfile(char** save)
{
	if(save != NULL)
	{
#ifdef ECHO_NDS
		*save = new char[strlen("/apps/n-echo/prefs.xml") + 1];
		CHKPTR(*save);
		strcpy(*save, "/apps/n-echo/prefs.xml");
		return(WIN);
#else
		char** path = new(char*);
		CHKPTR(path);
		if(echo_execdir(path) == WIN)
		{
			*save = echo_merge(*path, "prefs.xml");
			delete path;
			return(WIN);
		}
		delete path;
#endif
	}
	return(FAIL);
}
/** Get the current directory; not neccessarily the executable's directory
 * @param save Where to save the current directory
 */
STATUS echo_currentdir(char** save)
{
	if(save != NULL)
	{
		/// getenv is best because we don't have to worry about overflow
		char* pwd = getenv("PWD");
		/// Copy results over
		*save = new char[strlen(pwd) + 1];
		CHKPTR(*save);
		strcpy(*save, pwd);
		return(WIN);
	}
	return(FAIL);
}
/** Get the parent directory of the path
 * @param path The path
 * @param save Where to save the parent directory
 */
STATUS echo_parentdir(const char* path, char** save)
{
	if(path != NULL && save != NULL)
	{
#ifdef ECHO_NDS
		/// If not root
		if(strcmp(path, "/"))
		{
			/// Index of last '/' character
			const int len =  strrchr(path, '/') - path;
			/// The path may have been "/apps", so return root
			if(len > 0)
			{
				*save = new char[len + 1];
				CHKPTR(*save);
				/// Cut off last '/' and after
				strncpy(*save, path, len);
				return(WIN);
			}
		}
		/// If going to root (len == 0), is at root (!strcmp(path, "/")), or error (len < 0), write root anyways
		return(echo_genroot(save));
#else
		const char* dir = dirname(const_cast<char*>(path));
		/// Copy it over
		*save = new char[strlen(dir) + 1];
		CHKPTR(*save);
		strcpy(*save, dir);
		return(WIN);
#endif
	}
	return(FAIL);
}
/** Delete every string in the structure, and the structure itself
 * @param files The structure to be deallocated
 */
STATUS delete_echo_files(echo_files* files)
{
	if(files != NULL)
	{
		if(files->file_names != NULL)
		{
			/// 0 is ALWAYS static string "..", which we don't want to delete
			int each = 1;
			/// Delete every string
			while(each < files->num_files)
			{
				char* del = files->file_names[each];
				if(del != NULL)
					delete[] del;
				each++;
			}
			/// Delete string array
			delete[] files->file_names;
		}
		/// Delete the current directory string
		if(files->current_dir != NULL)
			delete[] files->current_dir;
		/// Delete the structure itself
		delete files;
		return(WIN);
	}
	return(FAIL);
}
/** Correctly merges the filenames
 * @param arg1 Directory
 * @param arg2 File
 * @return The right file name.
 */
char* echo_merge(const char* arg1, const char* arg2)
{
	/// Fill up later
	char* ret = NULL;
#ifdef ECHO_WIN
	/// Windows uses the backslash
	const char* slash = strrchr(arg1, '\\');
#else
	/// All other systems uses slash
	const char* slash = strrchr(arg1, '/');
#endif
	/// If the slash/backslash exists and is the last character...
	if(slash != NULL && slash == arg1 + strlen(arg1) - 1)
	{
		/// Besides tehe string lengths, 1 char is for the null-char
		ret = new char[strlen(arg1) + strlen(arg2) + 1];
		CHKPTR(ret);
		/// Just concat it
		strcat(strcpy(ret, arg1), arg2);
	}
	/// If there is no slash/backslash or it's not the last character, we need to append one
	else
	{
		/// Besides the string lengths, 1 char is for the slash or backslash, and another char is for the null-char
		ret = new char[strlen(arg1) + strlen(arg2) + 2];
		CHKPTR(ret);
#ifdef ECHO_WIN
		strcat(strcat(strcpy(ret, arg1),  "\\"), arg2);
#else
		strcat(strcat(strcpy(ret, arg1),  "/"), arg2);
#endif
	}
	return(ret);
}

#ifndef ECHO_NDS
	/** Is the combined filename a directory?
	 * @param dir Directory
	 * @param fname
	 */
	int is_dir(const char* dir, const char* fname)
	{
		/// If it equals "..", then return true
		if(!strcmp(fname, ".."))
			return(true);
		/// Merge the filenames
		const char* merged = echo_merge(dir, fname);
		/// Ask if it's correct
		const int ret = is_dir(merged);
		/// delete the merge
		delete[] merged;
		return(ret);
	}
	/** Is the given filename a directory?
	 * @param fname Absolute path
	 */
	int is_dir(const char* fname)
	{
#ifdef ECHO_WIN
		/// Get the attribute flags
		const DWORD attrs = GetFileAttributes(fname);
		/// Exists and is directory
		return(attrs != INVALID_FILE_ATTRIBUTES 
			&& (attrs & FILE_ATTRIBUTE_DIRECTORY));
#else
		/// Get the file descriptor
		const int fd = open(fname, O_RDONLY);
		/// Get a new file structure
		struct stat* file_stat = new(struct stat);
		CHKPTR(file_stat);
		/// File up the file descriptor
		const int exists = !fstat(fd, file_stat);
		/// Don't need the file descriptor anymore
		close(fd);
		if(exists)
		{
			const int ret = S_ISDIR(file_stat->st_mode);
			/// Delete the file structure first
			delete file_stat;
			/// Return our result
			return(ret);
		}
		/// Delete the file structure first
		delete file_stat;
		/// It doesn't exist, so return false
		return(false);
#endif
	}
	/** Is the file at "file_index" a directory?
	 * @param files File strucutre containing the file name to check
	 * @param file_index Which filename to check
	 */
	int is_dir(echo_files* files, int file_index)
	{
		/// Just check the current_dir + the filename in question
		return(is_dir(files->current_dir, files->file_names[file_index]));
	}
	/// A state variable; put the current_dir of the file structure here before sorting
	static const char* cmp_dir;
	/// Sorting the file names; if the file is a directory, then it should be on top
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
	/** Get the file structure at dirname
	 * @param dirname Directory to check
	 * @return Echo_file structure, with the file names of that directory
	 */
	echo_files* get_files(const char* dirname)
	{
		/// Make sure the directory exists first
#ifdef ECHO_WIN
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
		/// It exists...
		if(exists)
		{
			/// Open the directory
			DIR* dir = opendir(dirname);
			/// Get a new structure
			echo_files* ret = new(echo_files);
			CHKPTR(ret);
			/// Fill out the fields
			ret->num_files = 0;
			ret->current_dir = const_cast<char*>(dirname);
			/// Go through each file
			dirent* each_ent;
			while((each_ent = readdir(dir)) != NULL)
			{
				/// Count every file (except ".")
				if(strcmp(each_ent->d_name, "."))
					ret->num_files++;
			}
			/// Initialize the filename array
			ret->file_names = new char*[ret->num_files];
			CHKPTR(ret->file_names);
			/// "..", or previous directory, is always first
			ret->file_names[0] = "..";
			/// Initialize the index
			int each = 1;
			/// Rewind the directory back
			rewinddir(dir);
			/// Fo through each file again
			while((each_ent = readdir(dir)) != NULL)
			{
				/// Copy every filename over (except "." and "..", which is already copied)
				if(strcmp(each_ent->d_name, ".") && strcmp(each_ent->d_name, ".."))
				{
					/// New string
					ret->file_names[each] = new char[strlen(each_ent->d_name) + 1];
					CHKPTR(ret->file_names[each]);
					/// Null the string
					memset(ret->file_names[each], 0
						, strlen(each_ent->d_name) + 1);
					/// Copy the name over
					strcpy(ret->file_names[each], each_ent->d_name);
					/// Increment index
					each++;
				}
			}
			/// Finally close the directory
			closedir(dir);
			/// Set the compare directory
			cmp_dir = ret->current_dir;
			/// Sort our structure
			qsort(ret->file_names, ret->num_files, sizeof(char*), cmp);
			/// Return the structure
			return(ret);
		}
		/// Doesn't exist -> return NULL
		return(NULL);
	}
#else
	/** Is the file at "file_index" a directory?
	 * @param files File strucutre containing the file name to check
	 * @param file_index Which filename to check
	 */
	int is_dir(echo_files* files, int file_index)
	{
		return(file_index < files->num_dir);
	}
	/// The directories and the files are sorted seperately, so just strcmp it
	static int cmp(const void* arg1v, const void* arg2v)
	{
		return(strcmp(*((char**)arg1v), *((char**)arg2v)));
	}
	/** Get the file structure at dirname
	 * @param dirname Directory to check
	 * @return Echo_file structure, with the file names of that directory
	 */
	echo_files* get_files(const char* dirname)
	{
		/// Open the directory
		DIR_ITER* dir = diropen(dirname);
		/// It exists...
		if(dir != NULL)
		{
			/// New file structure
			echo_files* ret = new(echo_files);
			CHKPTR(ret);
			
			/// Set fields
			ret->num_files = 0;
			ret->current_dir = const_cast<char*>(dirname);
			ret->num_dir = 0;
			
			/// Set stats
			struct stat each_ent;
			/// Buffer to save file names
			char* filename = new char[MAXPATHLEN];
			CHKPTR(filename);
			while(dirnext(dir, filename, &each_ent) == 0)
			{
				/// If the file is not "."
				if(strcmp(filename, "."))
				{
					/// Increment number of files (both directories and regular files)
					ret->num_files++;
					/// If it's a directory...
					if(S_ISDIR(each_ent.st_mode))
						/// ...increment the number of directories
						ret->num_dir++;
				}
			}
			/// Make a new array
			ret->file_names = new char*[ret->num_files];
			CHKPTR(ret->file_names);
			/// First file always ".."
			ret->file_names[0] = "..";
			/** each_dir is the directory index (1 because ".." is already filled)
			 * each_file is the regular file index
			 * each is one of them
			 */
			int each_dir = 1, each_file = ret->num_dir, each = 1;
			/// Rewind back
			dirreset(dir);
			/// For each file entry
			while(dirnext(dir, filename, &each_ent) == 0)
			{
				/// If it's not "." or ".."
				if(strcmp(filename, ".") && strcmp(filename, ".."))
				{
					/// If it's a directory...
					if(S_ISDIR(each_ent.st_mode))
					{
						/// Use the directory index
						each = each_dir;
						each_dir++;
					}
					/// Else, it's a reg. file...
					else
					{
						/// Use the reg. file index
						each = each_file;
						each_file++;
					}
					/// Copy the file name over
					ret->file_names[each] = new char[strlen(filename) + 1];
					CHKPTR(ret->file_names[each]);
					memset(ret->file_names[each], 0
						, strlen(filename) + 1);
					strcpy(ret->file_names[each], filename);
				}
			}
			/// Delete the buffer
			delete[] filename;
			/// Close the directory
			dirclose(dir);
			/// Sort the directories
			qsort(ret->file_names, ret->num_dir, sizeof(char*), cmp);
			/// Sort the reg. files
			qsort(ret->file_names + ret->num_dir
				, ret->num_files - ret->num_dir, sizeof(char*), cmp);
			/// Return files
			return(ret);
		}
		return(NULL);
	}
#endif
/** Print out the files
 * @param files File structure to print out
 */
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

