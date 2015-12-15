#include "directory.h"

#include <cstdlib>
#include <dirent.h>

void directory::set_directory_content()
{
	DIR *ptr_to_dir = opendir(this->path_name.c_str());
	if(ptr_to_dir == NULL)
	{
		std::string error = this->path_name + " does not exist";
		throw error.c_str();
	}

	struct dirent *dir_info = readdir(ptr_to_dir);
	while(dir_info != NULL)
	{
		this->directory_content.push_back(dir_info->d_name);
		dir_info = readdir(ptr_to_dir);
	}

	closedir(ptr_to_dir);
}

directory::directory(std::string path_name)
{
	this->path_name = path_name;

	try
	{
		this->set_directory_content();
	}
	catch(const char *msg)
	{
		throw msg;
	}
	if(path_name[path_name.length() - 1] != '/') this->path_name += "/";	//The last charecter of path_str of directory will always be '/'
}

bool directory::contains(std::string file_or_folder_name)
{
	for(int i = 0;i < this->directory_content.size();i++) if(directory_content[i] == file_or_folder_name) return true;
	return false;
}

void directory::copy(std::string destination)
{
	std::string command = "cp -r ";

	command += this->path_name + " " + destination;
	system(command.c_str());
}

bool directory::create_sub_dir(std::string dir_name)
{
	if(path::is_not_path_name(dir_name) && !this->contains(dir_name))
	{
		std::string command = "mkdir " + this->path_name + dir_name;
		system(command.c_str());
		return true;
	}
	else return false;
}

std::vector<std::string> directory::get_directory_content()
{
	return this->directory_content;
}

std::string directory::get_path()
{
	return this->path_name;
}

std::string directory::get_path_of_content(std::string content_name)
{
	return this->path_name + content_name;
}

directory directory::get_sub_directory(std::string dir_name)
{
	std::string new_dir_path = this->path_name + dir_name;
	try
	{
		directory new_dir(new_dir_path);
		return new_dir;
	}
	catch(const char *msg)
	{
		throw msg;
	}
}

void directory::move(std::string destination)
{
	std::string command = "mv ";

	command += this->path_name + " " + destination;
	system(command.c_str());
}

void directory::remove()
{
	std::string command = "rm -r ";

	command += this->path_name;
	system(command.c_str());
}

void directory::search(std::string keyword, std::vector<std::string> &result)
{
	// DFS for the file is conducted in a view to minimise the resources consumed during the search
	for(int i = 0;i < directory_content.size();i++) 
	{
		if(directory_content[i][0] != '.')
		{
			try
			{
				if(!path::is_file(this->get_path_of_content(directory_content[i])))
				{
					directory sub_dir = get_sub_directory(directory_content[i]);
					sub_dir.search(keyword, result);
				}
				if(directory_content[i] == keyword) result.push_back(this->path_name + directory_content[i]);
			}
			catch(const char *msg)
			{
				continue;
			}
		}
	}
}
