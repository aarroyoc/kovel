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
	/*bson_t metadata;
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
				bson_append_null(&voxelZ,"NULL",-1);*/
				/*
				bson_append_document_begin(&voxelZ,"VoxelObject",-1,&voxelObject);
				bson_append_document_end(&voxelZ,&voxelObject);*/
			/*}
			bson_append_array_end(&voxelY,&voxelZ);
		}
		
		bson_append_array_end(&voxels,&voxelY);
	}
	bson_append_array_end(&kovel,&voxels);
	
	
	
	// Update UI
	char *str;
	str = bson_as_json (&kovel, NULL);
	printf ("%s\n", str);*/
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
		
		short x=0,y=0,z=0;
		delete this->geo;
		this->geo=new Geometry(5); // Maybe read first GRID Size
		
		bson_iter_t iter;
		bson_iter_t materials;
		bson_iter_t materialBSON;
		bson_iter_t childX;
		bson_iter_t childY;
		bson_iter_t childZ;
		
		bson_iter_init(&iter,doc);
		
		
		// Materials
		bson_iter_find_descendant(&iter,"materials",&materials);
		bson_iter_recurse(&iter,&materials);
		material.clear();
		while(bson_iter_next(&materials)){
			std::string matName=bson_iter_key(&materials);
			Material mat(matName);
			bson_iter_recurse(&materials,&materialBSON);
			while(bson_iter_next(&materialBSON)){
				if(strcmp(bson_iter_key(&materialBSON),"red") == 0){
					mat.r=bson_iter_double(&materialBSON);
				}
				if(strcmp(bson_iter_key(&materialBSON),"green") == 0){
					mat.g=bson_iter_double(&materialBSON);
				}
				if(strcmp(bson_iter_key(&materialBSON),"blue") == 0){
					mat.b=bson_iter_double(&materialBSON);
				}
			}
			material.insert({matName,mat});
		}
		
		// Voxels
		bson_iter_find_descendant(&iter,"voxels",&childX);
		bson_iter_recurse(&iter,&childX);
		while(bson_iter_next(&childX)){
			bson_iter_recurse(&childX,&childY);
			while(bson_iter_next(&childY)){
				bson_iter_recurse(&childY,&childZ);
				while(bson_iter_next(&childZ)){
					if(bson_iter_type(&childZ) == BSON_TYPE_NULL){
						this->geo->SetGrid(0,x,y,z);
					}
					if(bson_iter_type(&childZ) == BSON_TYPE_DOCUMENT){
						this->geo->SetGrid(1,x,y,z);
						bson_iter_t voxelObject;
						bson_iter_recurse(&childZ,&voxelObject);
						while(bson_iter_next(&voxelObject)){
							if(strcmp(bson_iter_key(&voxelObject),"material") == 0){
								uint32_t length;
								std::string matName=bson_iter_utf8(&voxelObject,&length);
								mat[x][y][z]=material[matName];
							}
						}
					}
					z++;
				}
				z=0;
				y++;
			}
			y=0;
			x++;
		}
		
		
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
	
	bson_t materials;
	bson_append_document_begin(&kovel,"materials",-1,&materials);
	

	for(auto kv : material) {
		bson_t materialBSON;
		Material mat=kv.second;
		bson_append_document_begin(&materials,mat.name.c_str(),-1,&materialBSON);
		BSON_APPEND_DOUBLE(&materialBSON,"red",mat.r);
		BSON_APPEND_DOUBLE(&materialBSON,"green",mat.g);
		BSON_APPEND_DOUBLE(&materialBSON,"blue",mat.b);
		bson_append_document_end(&materials,&materialBSON);
	} 
	
	bson_append_document_end(&kovel,&materials);
	
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
				if(this->geo->GetGrid(i,j,k)==1){
					bson_append_document_begin(&voxelZ,"VoxelObject",-1,&voxelObject);
					bson_append_utf8(&voxelObject,"material",-1,mat[i][j][k].name.c_str(),-1);
					bson_append_document_end(&voxelZ,&voxelObject);
				}else{
					bson_append_null(&voxelZ,"NULL",-1);
				}
			}
			bson_append_array_end(&voxelY,&voxelZ);
		}
		
		bson_append_array_end(&voxels,&voxelY);
	}
	bson_append_array_end(&kovel,&voxels);
	
	// Validate and Write
	if(this->ValidateFile()){
		std::ofstream outfile;
		outfile.open(filename,std::ios::binary | std::ios::out);
	
		outfile.write((char*)bson_get_data(&kovel),kovel.len);
	
		outfile.close();
	}else{
		std::cerr << "ERROR: Kovel file was badformed" << std::endl;
	}

}

bool Core::ValidateFile()
{
	size_t offset;
	if(bson_validate(&kovel,BSON_VALIDATE_UTF8_ALLOW_NULL,&offset)){
		return true;
	}else{
		return false;
	}
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
