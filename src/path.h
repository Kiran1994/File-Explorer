#ifndef path_h
#define path_h
  
#include <string>

class path
{
	public:
		static bool is_directory(std::string path_name);
		static bool is_file(std::string path_name);
		static bool is_full_path_name(std::string path_name);
		static bool is_not_path_name(std::string str);

		virtual void copy(std::string destination) = 0;
		virtual void move(std::string destination) = 0;
		virtual void remove() = 0;
};

#endif
