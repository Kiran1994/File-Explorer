#include "command_line_interface.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

void command_line_interface::close()
{
	std::cout << "Goodbye" << std::endl;
	exit(0);
}

std::string command_line_interface::convert_to_full_path_name(std::string path_name)
{
	if(path::is_full_path_name(path_name)) return path_name;
	else return this->current_directory->get_path() + path_name;
}

void command_line_interface::execute_copy_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 3)
	{
		std::string source_path = this->convert_to_full_path_name(arg[1]);

		try
		{
			std::string dest_path = this->convert_to_full_path_name(arg[2]);

			if(path::is_file(source_path))
			{
				file source_file(source_path);
				source_file.copy(dest_path);
			}
			else if(path::is_directory(source_path))
			{
				directory source_dir(source_path);
				source_dir.copy(dest_path);
			}
			else
			{
				std::cout << "path does not exist" << std::endl;
			}

			this->refresh();
		}
		catch(const char *msg)
		{
			std::cout << msg << std::endl;
		}
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_create_folder_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 2)
	{
		if(!this->current_directory->create_sub_dir(arg[1])) std::cout << create_dir_fail << std::endl;
		this->refresh();
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_list_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 1)
	{
		std::vector<std::string> content = this->current_directory->get_directory_content();

		for(int i = 0;i < content.size();i++) if(content[i][0] != '.') std::cout << content[i] << "\t";
		std::cout << std::endl;
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_move_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 3)
	{
		std::string source_path = this->convert_to_full_path_name(arg[1]);
		try
		{
			std::string dest_path = this->convert_to_full_path_name(arg[2]);

			if(path::is_file(source_path))
			{
				file source_file(source_path);
				source_file.move(dest_path);
			}
			else if(path::is_directory(source_path))
			{
				directory source_dir(source_path);
				source_dir.move(dest_path);
			}
			else
			{
				std::cout << "path does not exist" << std::endl;
			}

			this->refresh();
		}
		catch(const char *msg)
		{
			std::cout << msg << std::endl;
		}
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_open_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 2)
	{
		if(arg[1] == ".") return;
		else if(arg[1] == "..") this->up();
		else
		{
			std::string path_name = this->convert_to_full_path_name(arg[1]);

			if(path::is_file(path_name))
			{
				this->open_file(path_name);
			}
			else if(path::is_directory(path_name))
			{
				directory *prev_directory = this->current_directory;
				if(this->open_dir(arg[1])) delete prev_directory;
				else current_directory = prev_directory;
			}
			else
			{
				std::cout << "path does not exist" << std::endl;
			}
		}
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_remove_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 2)
	{
		std::string path_to_delete = this->convert_to_full_path_name(arg[1]);
		try
		{
			if(path::is_file(path_to_delete))
			{
				file file_to_del(path_to_delete);
				file_to_del.remove();
			}
			else if(path::is_directory(path_to_delete))
			{
				directory dir_to_del(path_to_delete);
				dir_to_del.remove();
			}
			else
			{
				std::cout << "path does not exist" << std::endl;
			}

			this->refresh();
		}
		catch(const char *msg)
		{
			std::cout << msg << std::endl;
		}
	}
}

void command_line_interface::execute_rename_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 3)
	{
		if(directory::is_not_path_name(arg[2])) this->execute_move_sequence(arg);
		else std::cout << invalid_name << std::endl;
	}
	else std::cout << invalid_args << std::endl;
}

void command_line_interface::execute_search_sequence(std::vector<std::string> arg)
{
	if(arg.size() == 2)
	{
		if(directory::is_not_path_name(arg[1]))
		{
			std::vector<std::string> result;
			this->current_directory->search(arg[1], result);

			std::cout << "Search results:" << std::endl;
			for(int i = 0;i < result.size();i++) std::cout << result[i] << std::endl;
		}
		else std::cout << invalid_keyword << std::endl;
	}
	else std::cout << invalid_args << std::endl;
}

bool command_line_interface::open_dir(std::string dir_name)
{
	std::string path_name;

	path_name = this->convert_to_full_path_name(dir_name);

	try
	{
		directory *new_directory = new directory(path_name);
		this->current_directory = new_directory;

		return true;
	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
		return false;
	}
}

void command_line_interface::open_file(std::string file_name)
{
	std::string command = "xdg-open " + file_name;
	system(command.c_str());
}

void command_line_interface::parse(std::string str, std::vector<std::string> &arg)
{
	int first_char = -1, pos = 0;
	while(1)
	{
		pos = str.find(' ', first_char + 1);
		if(pos == std::string::npos)
		{
			arg.push_back(str.substr(first_char + 1, str.length()));
			return;
		}
		arg.push_back(str.substr(first_char + 1, pos - first_char - 1));
		first_char = pos;
	}
}

void command_line_interface::process(std::string command)
{
	std::vector<std::string> arg;

	parse(command, arg);

	if(arg[0] == "copy") this->execute_copy_sequence(arg);
	else if(arg[0] == "delete") this->execute_remove_sequence(arg);
	else if(arg[0] == "list") this->execute_list_sequence(arg);
	else if(arg[0] == "make_dir") this->execute_create_folder_sequence(arg);
	else if(arg[0] == "move") this->execute_move_sequence(arg);
	else if(arg[0] == "open") this->execute_open_sequence(arg);
	else if(arg[0] == "quit")
	{
		if(arg.size() == 1) this->close();
		else std::cout << invalid_args << std::endl;
	}
	else if(arg[0] == "rename") this->execute_rename_sequence(arg);
	else if(arg[0] == "search") this->execute_search_sequence(arg);
	else if(arg[0] == "up")
	{
		if(arg.size() == 1) this->up();
		else std::cout << invalid_args << std::endl;
	}
	else std::cout << "Invalid command" << std::endl;
}

void command_line_interface::refresh()
{
	std::string path_name = this->current_directory->get_path();
	delete this->current_directory;
	this->open_dir(path_name);
}

void command_line_interface::up()
{
	if(this->current_directory->get_path() == "/") return;

	std::string current_path = this->current_directory->get_path();
	std::string parent_path = current_path.substr(0, current_path.rfind('/', current_path.length() - 2) + 1);

	delete this->current_directory;
	this->current_directory = new directory(parent_path);
}

command_line_interface::command_line_interface(std::string path)
{
	try
	{
		this->current_directory = new directory(path);
	}
	catch(const char *msg)
	{
		std::cout << msg << std::endl;
		exit(1);
	}

	this->run();
}

command_line_interface::~command_line_interface()
{
	std::cout << "destructor" << std::endl;
	delete this->current_directory;
}

void command_line_interface::run()
{
	std::string command;

	while(1)
	{
		std::cout << "File explorer <" << this->current_directory->get_path() << ">: ";
		getline(std::cin, command);
		process(command);
	}
}
