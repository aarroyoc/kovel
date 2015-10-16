#ifndef CORE_HPP
#define CORE_HPP

#include <bson.h>
#include <iostream>
#include <fstream>

class Geometry{
	public:
		Geometry(unsigned short grid){
			this->g=grid;
			geo=(unsigned short***) malloc(grid*sizeof(unsigned short**));
			for(short i=0;i<grid;i++){
				geo[i]=(unsigned short**) malloc(grid*sizeof(unsigned short*));
				for(short j=0;j<grid;j++){
					geo[i][j]=(unsigned short*) malloc(grid*sizeof(unsigned short));
					for(short k=0;k<grid;k++){
						geo[i][j][k]=(unsigned short)0;
					}
				}
			}
		}
		void SetGrid(unsigned short value,unsigned short x, unsigned short y, unsigned short z){
			geo[x][y][z]=value;
		}
		unsigned short GetGrid(unsigned short x, unsigned short y, unsigned short z){
			return geo[x][y][z];
		}
		~Geometry(){
			for(short i=0;i<this->g;i++){
				for(short j=0;j<this->g;j++){
					free(geo[i][j]);
				}
				free(geo[i]);
			}
			free(geo);
		}
		unsigned short g;
	private:
		unsigned short*** geo;
};

class Core{
	public:
		static Core* Instance();
	protected:
		Core();
		~Core();
		Core(Core const&) = delete;              // Don't Implement.
        void operator=(Core const&) = delete; // Don't implement
    public:
		void NewFile(unsigned short);
		void LoadFile(std::string filename);
		void SaveFile();
		void UpdateGrid(unsigned short value, unsigned short x, unsigned short y, unsigned short z);
		void UpdateMetadata();
		Geometry* geo;
	private:
		bson_t kovel;
		unsigned short grid;
		static Core* pinstance;

};

#endif
