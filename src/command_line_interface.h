#ifndef command_line_interface_h
#define command_line_interface_h

#include "directory.h"
#include "file.h"

#define create_dir_fail "Failed to create new directory"
#define invalid_args "Invalid arguments"
#define invalid_keyword "Invalid search keyword"
#define invalid_name "Invalid name"

class command_line_interface
{
	private:
		directory *current_directory;

		void close();
		std::string convert_to_full_path_name(std::string path_name);
		void execute_copy_sequence(std::vector<std::string> arg);
		void execute_create_folder_sequence(std::vector<std::string> arg);
		void execute_list_sequence(std::vector<std::string> arg);
		void execute_move_sequence(std::vector<std::string> arg);
		void execute_open_sequence(std::vector<std::string> arg);
		void execute_remove_sequence(std::vector<std::string> arg);
		void execute_rename_sequence(std::vector<std::string> arg);
		void execute_search_sequence(std::vector<std::string> arg);
		bool open_dir(std::string dir_name);
		void open_file(std::string file_name);
		void parse(std::string str, std::vector<std::string> &arg);
		void process(std::string command);
		void refresh();
		void up();

	public:
		command_line_interface(std::string path);
		~command_line_interface();

		void run();
};

#endif
