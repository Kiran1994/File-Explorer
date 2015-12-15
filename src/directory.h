#ifndef directory_h
#define directory_h

#include "path.h"

#include <vector>

class directory: public path
{
	private:
		std::string path_name;
		std::vector<std::string> directory_content;

		void set_directory_content();

	public:
		directory(std::string path_name);

		bool contains(std::string file_or_folder_name);
		void copy(std::string destination);
		bool create_sub_dir(std::string dir_name);
		std::vector<std::string> get_directory_content();
		directory get_sub_directory(std::string dir_name);
		std::string get_path();
		std::string get_path_of_content(std::string content_name);
		void move(std::string destination);
		void remove();
		void search(std::string keyword, std::vector<std::string> &result);
};

#endif
