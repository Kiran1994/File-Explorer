#include "path.h"

#include <sys/stat.h>
#include <sys/types.h>

bool path::is_file(std::string path_name)
{
	struct stat buf;
	stat(path_name.c_str(), &buf);
	return S_ISREG(buf.st_mode);
}

bool path::is_directory(std::string path_name)
{
	struct stat buf;
	stat(path_name.c_str(), &buf);
	return S_ISDIR(buf.st_mode);
}

bool path::is_full_path_name(std::string path_name)
{
	return path_name[0] == '/';
}

bool path::is_not_path_name(std::string str)
{
	for(int i = 0;i < str.length();i++) if(str[i] == '/') return false;
	return true;
}
