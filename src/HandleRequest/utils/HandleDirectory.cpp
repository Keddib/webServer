#include "../utils.hpp"
#include  <dirent.h>


std::string lookForIndexInDirectory(const std::string &Dir, const std::vector<std::string> &indexes, int &error)
{
	DIR *direcroty;
	struct dirent *ent;
	int inx = indexes.size() - 1;
	std::string found;
	if ((direcroty = opendir (Dir.c_str())) != NULL)
	{
		error = 0;
		/* print all the files and directories within directory */
		while ( (ent = readdir(direcroty) ) != NULL)
		{
			if (ent->d_type != DT_REG && ent->d_type != DT_LNK)
				continue;
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
		error = 2;
	else
		error = 1;
	return found;
}

std::string ListDirectory(const std::string &Dir, const std::string &name)
{
	DIR *direcroty;
	struct dirent *ent;
	if ((direcroty = opendir (Dir.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ( (ent = readdir(direcroty) ) != NULL)
		{
			printf ("%s\n", ent->d_name);
		}
		closedir (direcroty);
	}
	/* could not open directory */
	return name;
}
