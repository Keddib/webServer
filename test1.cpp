
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include  <dirent.h>
#include <vector>

#include <libgen.h>

std::string lookForIndexInDirectory(const std::string &Dir, const std::vector<std::string> &indexes, int *error)
{
	DIR *direcroty;
	struct dirent *ent;
	int inx = indexes.size() - 1;
	std::string found;
	if ((direcroty = opendir (Dir.c_str())) != NULL)
	{
		*error = 0;
		/* print all the files and directories within directory */
		while ( (ent = readdir(direcroty) ) != NULL)
		{
			int inx1 = inx;
			while (inx1 >= 0)
			{
				if (indexes[inx1] == ent->d_name)
				{
					found = indexes[inx1];
					inx--;
				}
				inx1--;
			}
		}
		closedir (direcroty);
	}
	/* could not open directory */
	else if (errno == EACCES)
		*error = 2;
	else
		*error = 1;
	return found;
}

int main(int argc, char **argv)
{
	std::vector<std::string> vec {};



	return 0;
}


// int main(int argc, char **argv)
// {
	// std::string Dir(argv[0]);
	// DIR *direcroty;
	// std::string s1("/tmp/lol/exrp");
	// char base[1000];
	// basename_r(s1.c_str(), base);
	// std::cout << base <<"|||\n";
	// struct dirent *ent;
	// if ((direcroty = opendir (Dir.c_str())) != NULL)
	// {
		// std::cout << "hello\n";
		// /* print all the files and directories within directory */
		// while ( (ent = readdir(direcroty) ) != NULL)
		// {
			// printf ("%s\n", ent->d_name);
			// if (DT_DIR == ent->d_type)
				// std::cout << ent->d_type << "D\n";
			// if (DT_REG == ent->d_type)
				// std::cout << ent->d_type << "F\n";
//
			// std::cout << ent->d_ino << '\n';
			// std::cout << ent->d_namlen << '\n';
			// std::cout << ent->d_reclen << '\n';
		// }
		// closedir (direcroty);
	// }
	// else if (errno == ENOENT)
		// std::cout << "error : " << std::strerror(errno) << '\n';
	// else if(errno == EACCES)
		// std::cout << "error : " << std::strerror(errno) << '\n';
	// else
		// std::cout << "error: " << std::strerror(errno) << '\n';
	// return 0;
// }
//
