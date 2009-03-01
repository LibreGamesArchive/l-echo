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

#include "echo_platform.h"
#include "echo_stage.h"
#include "echo_loader.h"

#ifndef __ECHO_INTERNAL_FILES__
#define __ECHO_INTERNAL_FILES__
/** @brief A list of files;\n
 * Used for menus and loading.
 */
struct echo_files
{
	/// The current directory; ALWAYS dynamically allocated
	char* current_dir;
	/// Array of files names; this array and file names are always ALWAYS dynamically allocated
	char** file_names;
	/// The number of file names contained in file_names.
	int num_files;
#ifdef ECHO_NDS
	/** The NDS can't use stat for some reason (at loeast, when I wrote this 
	 * loader), so the directories are placed at the top when loading.
	 * num_dir is the number of directories there are, and thus the index
	 * to the first file.
	 */
	int num_dir;
#endif
};
#endif

/** Delete every string in the structure, and the structure itself
 * @param files The structure to be deallocated
 */
STATUS delete_echo_files(echo_files* files);

/// NDS doesn't have support for stat (at least, when I first wrote this)
#ifndef ECHO_NDS
	/** Is the combined filename a directory?
	 * @param dir Directory
	 * @param fname Relative path
	 */ 
	int is_dir(const char* dir, const char* fname);
	/** Is the given filename a directory?
	 * @param fname Absolute path
	 */
	int is_dir(const char* fname);
#endif

/** Correctly merges the filenames
 * @param arg1 Directory
 * @param arg2 Relative path
 * @return The right file name.
 */
char* echo_merge(const char* arg1, const char* arg2);
/** Is the file at "file_index" a directory?
 * @param files File strucutre containing the file name to check
 * @param file_index Which filename to check
 */
int is_dir(echo_files* files, int file_index);
/** Get the file structure at dirname
 * @param dirname Directory to check
 * @return Echo_file structure, with the file names of that directory
 */
echo_files* get_files(const char* dirname);
/** Print out the files
 * @param files File structure to print out
 */
void dump_files(echo_files* files);
/** Get the parent directory of the path
 * @param path The path
 * @param save Where to save the parent directory
 */
STATUS echo_parentdir(const char* path, char** save);
/** Get the executable's directory, if possible:\n
 * Windows and Linux - Works\n
 * OSX - Current directory\n
 * NDS - Just root\n
 * @param save Where to put the directory
 */
STATUS echo_execdir(char** save);
/** Gets the prefs filename: \n
 * Win/Lin/Mac - echo_execdir() + "prefs.xml"\n
 * NDS - "/apps/n-echo/prefs.xml"\n
 * @param save Where to put the file name
 */
STATUS echo_prefsfile(char** save);
/** Generate a new string at "save" that is equal to "/"; used because
 * paths have to be dynamically allocated.  Delete after use.
 * @param save Pointer to where to put the string; remember to delete it after use
 */
STATUS echo_genroot(char** save);
/** Get the current directory; not neccessarily the executable's directory
 * @param save Where to save the current directory
 */
STATUS echo_currentdir(char** save);
