#ifndef CORE_HPP
#define CORE_HPP

#include <bson.h>
#include <iostream>
#include <fstream>

class Core{
	public:
		Core();
		void NewFile(unsigned short);
		void LoadFile(std::string filename);
		void SaveFile();
		void UpdateGrid();
		void UpdateMetadata();
	private:
		bson_t kovel;
		unsigned short grid;
};

#endif
