#ifndef CORE_HPP
#define CORE_HPP

#ifdef WIN32
#include <winsock2.h>
#endif
#include <bson.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

// Usar STDVector para los materiales en vez de eso

class Geometry {
public:
  Geometry(unsigned short grid) {
	this->g = grid;
	geo.resize(g);
	for (short u = 0; u < g; u++) {
	  geo.at(u).resize(g);
	  for (short v = 0; v < g; v++) {
		geo.at(u)[v].resize(g);
	  }
	}
  }
  void SetGrid(unsigned short value, unsigned short x, unsigned short y,
               unsigned short z) {
	geo[x][y][z] = value;
  }
  unsigned short GetGrid(unsigned short x, unsigned short y, unsigned short z) {
	return geo[x][y][z];
  }
  ~Geometry() {
  }
  unsigned short g;

private:
  std::vector<std::vector<std::vector<unsigned short>>> geo;
};

class Material {
public:
  Material() {
	this->name = "NullColour";
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 0.0f;
  }
  Material(std::string name) {
	this->name = name;
	this->r = 1.0f;
	this->g = 1.0f;
	this->b = 0.0f;
  };
  Material(std::string name, float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->name = name;
  };
  // Accept textures in future
  std::string name;

public:
  double r, g, b;
};

typedef std::vector<std::vector<std::vector<Material>>> Material3D;

struct Undo {
  unsigned short value;
  unsigned short x;
  unsigned short y;
  unsigned short z;
  Material material;
};

class Core {
public:
  static Core* Instance();

protected:
  Core();
  ~Core();
  Core(Core const&) = delete;           // Don't Implement.
  void operator=(Core const&) = delete; // Don't implement
public:
  void NewFile(unsigned short gridSize);
  void LoadFile(std::string filename);
  void SaveFile(std::string filename);
  void UpdateGrid(unsigned short value, unsigned short x, unsigned short y,
                  unsigned short z);
  void UpdateMetadata();
  void SetMaterial(Material mat);
  bool ValidateFile();
  bool ExportAsJSON(std::string filename);
  bool ExportAsDAE(std::string filename);
  void Undo();
  Geometry* geo;
  Material3D mat;
  std::string name;
  std::string author;
  unsigned short grid;
  std::vector<struct Undo> undo;

private:
  bson_t kovel;
  static Core* pinstance;
  float r, g, b;
  std::unordered_map<std::string, Material> material;
  Material currentMat;
};

#endif
