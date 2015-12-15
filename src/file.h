#ifndef file_h
#define file_h

#include "path.h"

class file: public path
{
	private:
		std::string path_name;

	public:
		file(std::string path_name);

		void copy(std::string destination);
		void move(std::string destination);
		void remove();
};

#endif
