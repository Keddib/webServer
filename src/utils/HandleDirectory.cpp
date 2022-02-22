#include "../Includes.hpp"
#include "../MACROS.hpp"
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

std::string getFileEntry(std::string PATH, char *name, bool isDir)
{
	// <a href="../">../</a> 	date _ size

	if (name[0] == '.' && name[1] == '\0')
		return "\n";
	int size(0);
	struct stat result;
	PATH += name;
	std::string Nname(name);
	Nname += (isDir? "/" : "");
	char buf[100];
	bzero(buf, 100);
	if (stat(PATH.c_str(), &result) == 0)
	{
		size = result.st_size;
		time_t mod_time = result.st_mtime;
		struct tm * curtime = std::gmtime( &mod_time );
		std::strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M", curtime);
	}
	std::string entry("<a href=\"");
	entry += Nname;
	entry += std::string("\">") + Nname + "</a>\t\t";
	if (!strcmp(name, ".."))
		return entry + '\n';
	entry += buf;
	entry += std::string("\t\t") + (isDir? "_" : to_string(size));
	entry += '\n';
	return entry;
}

std::string ListDirectory(const std::string &Dir, const std::string &name)
{
	//("</h1><hr><pre>"
	DIR *direcroty;
	struct dirent *ent;
	std::string x(Dir1);
	x += name + "</h1><hr><pre>";

	if ((direcroty = opendir (Dir.c_str())) != NULL)
	{
		/* print all the files and directories within directory */
		while ( (ent = readdir(direcroty) ) != NULL)
		{

		 	x+= getFileEntry(Dir, ent->d_name, ent->d_type == DT_DIR );
		}
		closedir (direcroty);
	}
	x += Dir2;
	/* could not open directory */
	return x;
}
