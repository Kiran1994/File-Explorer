#include "file.h"

#include <cstdlib>

file::file(std::string path_name)
{
	this->path_name = path_name;
}

void file::copy(std::string destination)
{
	std::string command = "cp ";

	command += this->path_name + " " + destination;
	system(command.c_str());
}

void file::move(std::string destination)
{
	std::string command = "mv ";

	command += this->path_name + " " + destination;
	system(command.c_str());
}

void file::remove()
{
	std::string command = "rm ";

	command += this->path_name;
	system(command.c_str());
}
