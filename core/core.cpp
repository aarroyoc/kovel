#include "core.hpp"


Core* Core::pinstance = 0;

Core* Core::Instance ()
{
  if (pinstance == 0)
  {
    pinstance = new Core;
  }
  return pinstance;
}

Core::Core()
{
	this->NewFile(5);
	geo=new Geometry(5);
	mat.clear();
	mat.resize(5);
	for(short u=0;u<5;u++){
		mat.at(u).resize(5);
		for(short v=0;v<5;v++){
			mat.at(u)[v].resize(5);
		}
	}
}

Core::~Core()
{
	delete geo;
}

void Core::NewFile(unsigned short g)
{
	this->grid=g;
	delete geo;
	geo=new Geometry(g);
	bson_init(&kovel);
	
	// METADATA
	bson_t metadata;
	bson_append_document_begin(&kovel,"metadata",-1,&metadata);
	BSON_APPEND_UTF8(&metadata,"name","Untitled");
	BSON_APPEND_UTF8(&metadata,"creator","Unspecified");
	BSON_APPEND_UTF8(&metadata,"date","");
	BSON_APPEND_UTF8(&metadata,"version","1.0.0");
	bson_append_document_end(&kovel,&metadata);
	
	
	// MATERIALS
	
	
	// VOXELS
	bson_t voxels;
	bson_append_array_begin(&kovel,"voxels",-1,&voxels);
	for(int i=0;i<this->grid;i++){
		bson_t voxelY;
		bson_append_array_begin(&voxels,"VoxelY",-1,&voxelY);
		for(int j=0;j<this->grid;j++){
			bson_t voxelZ;
			bson_append_array_begin(&voxelY,"VoxelZ",-1,&voxelZ);
			for(int k=0;k<this->grid;k++){
				bson_t voxelObject;
				bson_append_null(&voxelZ,"NULL",-1);
				/*
				bson_append_document_begin(&voxelZ,"VoxelObject",-1,&voxelObject);
				bson_append_document_end(&voxelZ,&voxelObject);*/
			}
			bson_append_array_end(&voxelY,&voxelZ);
		}
		
		bson_append_array_end(&voxels,&voxelY);
	}
	bson_append_array_end(&kovel,&voxels);
	
	
	
	// Update UI
	char *str;
	str = bson_as_json (&kovel, NULL);
	printf ("%s\n", str);
}

void Core::LoadFile(std::string filename)
{
	std::ifstream file(filename, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	std::vector<uint8_t> buffer(size);
	if (file.read((char*)(buffer.data()), size))
	{
		bson_t* doc;
		doc=bson_new_from_data(buffer.data(),size);
		
		// DO REAL PROCESSING HERE
		
		
		char *str;
		str = bson_as_json (doc, NULL);
		printf ("%s\n", str);
	}

}

void Core::SaveFile(std::string filename)
{
	// Dump grid to File
	bson_init(&kovel);
	
	// METADATA
	bson_t metadata;
	bson_append_document_begin(&kovel,"metadata",-1,&metadata);
	BSON_APPEND_UTF8(&metadata,"name","Untitled");
	BSON_APPEND_UTF8(&metadata,"creator","Unspecified");
	BSON_APPEND_UTF8(&metadata,"date","");
	BSON_APPEND_UTF8(&metadata,"version","1.0.0");
	bson_append_document_end(&kovel,&metadata);
	
	
	// MATERIALS
	
	// DO MATERIALS
	
	// VOXELS
	bson_t voxels;
	bson_append_array_begin(&kovel,"voxels",-1,&voxels);
	for(int i=0;i<this->grid;i++){
		bson_t voxelY;
		bson_append_array_begin(&voxels,"VoxelY",-1,&voxelY);
		for(int j=0;j<this->grid;j++){
			bson_t voxelZ;
			bson_append_array_begin(&voxelY,"VoxelZ",-1,&voxelZ);
			for(int k=0;k<this->grid;k++){
				bson_t voxelObject;
				//bson_append_null(&voxelZ,"NULL",-1);
				BSON_APPEND_INT32(&voxelZ,"Voxel",this->geo->GetGrid(i,j,k));
				/*
				bson_append_document_begin(&voxelZ,"VoxelObject",-1,&voxelObject);
				bson_append_document_end(&voxelZ,&voxelObject);*/
			}
			bson_append_array_end(&voxelY,&voxelZ);
		}
		
		bson_append_array_end(&voxels,&voxelY);
	}
	bson_append_array_end(&kovel,&voxels);
	
	// Validate
	size_t offset;
	if(bson_validate(&kovel,BSON_VALIDATE_UTF8_ALLOW_NULL,&offset)){
		std::cout << "KOVEL - Save file validation: OK" << std::endl;
	}
	
	// WRITE FILE
	
	std::ofstream outfile;
	outfile.open(filename,std::ios::binary | std::ios::out);
	
	outfile.write((char*)bson_get_data(&kovel),kovel.len);
	
	outfile.close();
}

void Core::UpdateMetadata()
{
	
}

void Core::UpdateGrid(unsigned short value,unsigned short x, unsigned short y, unsigned short z)
{
	// Modify Geo
	geo->SetGrid(value,x,y,z);
	mat[x][y][z]=currentMat;
}

void Core::SetMaterial(Material mat)
{
	// Check if already exists, otherwise, push it
	std::string matName=mat.name;
	if(material.find(matName) == material.end()){
		// Doesn't exist
		material.insert({matName,mat});
	}
	
	// Find and change to current material
	currentMat=material[matName];
}
