#include "core.hpp"
#include "version.hpp"

Core* Core::pinstance = 0;

Core* Core::Instance() {
  if (pinstance == 0) {
	pinstance = new Core;
  }
  return pinstance;
}

Core::Core() {
  this->NewFile(5);
}

Core::~Core() {
  // delete geo;
}

void Core::NewFile(unsigned short g) {
  this->grid = g;
  // delete geo;
  geo = new Geometry(g);
  material.clear();
  undo.clear();
  mat.resize(g);
  for (short u = 0; u < g; u++) {
	mat.at(u).resize(g);
	for (short v = 0; v < g; v++) {
	  mat.at(u)[v].resize(g);
	}
  }
  name = "";
  author = "";
  SetMaterial(currentMat);
}

void Core::LoadFile(std::string filename) {
  std::ifstream file(filename, std::ios::binary);
  file.seekg(0, std::ios::end);
  std::streamsize size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(size);
  if (file.read((char*)(buffer.data()), size)) {
	bson_t* doc;
	doc = bson_new_from_data(buffer.data(), size);

	short x = 0, y = 0, z = 0;

	bson_iter_t iter;
	bson_iter_t materials;
	bson_iter_t materialBSON;
	bson_iter_t childX;
	bson_iter_t childY;
	bson_iter_t childZ;
	bson_iter_t name;

	bson_iter_t metadata;
	bson_iter_t countGrid;

	unsigned short loadedGridSize = 0;
	bson_iter_init(&metadata, doc);
	bson_iter_find_descendant(&metadata, "voxels", &countGrid);
	bson_iter_recurse(&metadata, &countGrid);
	while (bson_iter_next(&countGrid)) {
	  loadedGridSize++;
	}
	this->NewFile(loadedGridSize);

	// READ METADATA AND GRID SIZE

	bson_iter_init(&iter, doc);

	bson_iter_find_descendant(&iter, "metadata", &name);
	bson_iter_recurse(&iter, &name);
	while (bson_iter_next(&name)) {
	  if (strcmp(bson_iter_key(&name), "name") == 0) {
		uint32_t length;
		this->name = bson_iter_utf8(&name, &length);
	  }
	  if (strcmp(bson_iter_key(&name), "creator") == 0) {
		uint32_t length;
		this->author = bson_iter_utf8(&name, &length);
	  }
	}

	// Materials
	bson_iter_find_descendant(&iter, "materials", &materials);
	bson_iter_recurse(&iter, &materials);
	material.clear();
	while (bson_iter_next(&materials)) {
	  std::string matName = bson_iter_key(&materials);
	  Material mat(matName);
	  bson_iter_recurse(&materials, &materialBSON);
	  while (bson_iter_next(&materialBSON)) {
		if (strcmp(bson_iter_key(&materialBSON), "red") == 0) {
		  mat.r = bson_iter_double(&materialBSON);
		}
		if (strcmp(bson_iter_key(&materialBSON), "green") == 0) {
		  mat.g = bson_iter_double(&materialBSON);
		}
		if (strcmp(bson_iter_key(&materialBSON), "blue") == 0) {
		  mat.b = bson_iter_double(&materialBSON);
		}
	  }
	  material.insert({matName, mat});
	}

	// Voxels
	bson_iter_find_descendant(&iter, "voxels", &childX);
	bson_iter_recurse(&iter, &childX);
	while (bson_iter_next(&childX)) {
	  bson_iter_recurse(&childX, &childY);
	  while (bson_iter_next(&childY)) {
		bson_iter_recurse(&childY, &childZ);
		while (bson_iter_next(&childZ)) {
		  if (bson_iter_type(&childZ) == BSON_TYPE_NULL) {
			this->geo->SetGrid(0, x, y, z);
		  }
		  if (bson_iter_type(&childZ) == BSON_TYPE_DOCUMENT) {
			this->geo->SetGrid(1, x, y, z);
			bson_iter_t voxelObject;
			bson_iter_recurse(&childZ, &voxelObject);
			while (bson_iter_next(&voxelObject)) {
			  if (strcmp(bson_iter_key(&voxelObject), "material") == 0) {
				uint32_t length;
				std::string matName = bson_iter_utf8(&voxelObject, &length);
				mat[x][y][z] = material[matName];
			  }
			}
		  }
		  z++;
		}
		z = 0;
		y++;
	  }
	  y = 0;
	  x++;
	}
  }
}

void Core::SaveFile(std::string filename) {
  // Dump grid to File
  bson_init(&kovel);

  // METADATA
  bson_t metadata;
  bson_append_document_begin(&kovel, "metadata", -1, &metadata);
  BSON_APPEND_UTF8(&metadata, "name", this->name.c_str());
  BSON_APPEND_UTF8(&metadata, "creator", this->author.c_str());
  BSON_APPEND_UTF8(&metadata, "date", "");
  BSON_APPEND_UTF8(&metadata, "version", KOVEL_VERSION);
  bson_append_document_end(&kovel, &metadata);

  // MATERIALS

  bson_t materials;
  bson_append_document_begin(&kovel, "materials", -1, &materials);

  for (auto kv : material) {
	bson_t materialBSON;
	Material mat = kv.second;
	bson_append_document_begin(&materials, mat.name.c_str(), -1, &materialBSON);
	BSON_APPEND_DOUBLE(&materialBSON, "red", mat.r);
	BSON_APPEND_DOUBLE(&materialBSON, "green", mat.g);
	BSON_APPEND_DOUBLE(&materialBSON, "blue", mat.b);
	bson_append_document_end(&materials, &materialBSON);
  }

  bson_append_document_end(&kovel, &materials);

  // VOXELS
  bson_t voxels;
  bson_append_array_begin(&kovel, "voxels", -1, &voxels);
  for (int i = 0; i < this->grid; i++) {
	bson_t voxelY;
	bson_append_array_begin(&voxels, "VoxelY", -1, &voxelY);
	for (int j = 0; j < this->grid; j++) {
	  bson_t voxelZ;
	  bson_append_array_begin(&voxelY, "VoxelZ", -1, &voxelZ);
	  for (int k = 0; k < this->grid; k++) {
		bson_t voxelObject;
		if (this->geo->GetGrid(i, j, k) == 1) {
		  bson_append_document_begin(&voxelZ, "VoxelObject", -1, &voxelObject);
		  bson_append_utf8(&voxelObject, "material", -1,
		                   mat[i][j][k].name.c_str(), -1);
		  bson_append_document_end(&voxelZ, &voxelObject);
		} else {
		  bson_append_null(&voxelZ, "NULL", -1);
		}
	  }
	  bson_append_array_end(&voxelY, &voxelZ);
	}

	bson_append_array_end(&voxels, &voxelY);
  }
  bson_append_array_end(&kovel, &voxels);

  // Validate and Write
  if (this->ValidateFile()) {
	std::ofstream outfile;
	outfile.open(filename, std::ios::binary | std::ios::out);

	outfile.write((char*)bson_get_data(&kovel), kovel.len);

	outfile.close();
  } else {
	std::cerr << "ERROR: Kovel file was badformed" << std::endl;
  }
}

bool Core::ValidateFile() {
  size_t offset;
  if (bson_validate(&kovel, BSON_VALIDATE_UTF8_ALLOW_NULL, &offset)) {
	return true;
  } else {
	return false;
  }
}

bool Core::ExportAsJSON(std::string filename) {
  if (this->ValidateFile()) {
	std::ofstream outfile;
	outfile.open(filename, std::ios::out);
	outfile << bson_as_json(&kovel, NULL);
	outfile.close();
	return true;
  } else {
	return false;
  }
}

bool Core::ExportAsDAE(std::string filename) {
  std::ofstream ss;
  ss.open(filename, std::ios::out);
  ss << "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
  ss << "<COLLADA xmlns=\"http://www.collada.org/2005/11/COLLADASchema\" "
        "version=\"1.4.1\">";
  ss << "<asset>";
  ss << "<contributor>";
  ss << "<author>";
  ss << "Kovel User";
  ss << "</author>";
  ss << "<authoring_tool>";
  ss << "Kovel " << KOVEL_VERSION;
  ss << "</authoring_tool>";
  ss << "</contributor>";
  ss << "<up_axis>";
  ss << "Y_UP";
  ss << "</up_axis>";
  ss << "<unit name=\"meter\" meter=\"1\"/>";
  ss << "</asset>";

  ss << "<library_geometries>";
  ss << "<geometry id=\"Cube-mesh\" name=\"Cube\">";
  ss << "<mesh>";
  ss << "<source id=\"Cube-mesh-positions\">";
  ss << "<float_array id=\"Cube-mesh-positions-array\" count=\"24\">1 1 -1 1 "
        "-1 -1 -1 -0.9999998 -1 -0.9999997 1 -1 1 0.9999995 1 0.9999994 "
        "-1.000001 1 -1 -0.9999997 1 -1 1 1</float_array>";
  ss << "<technique_common>";
  ss << "<accessor source=\"#Cube-mesh-positions-array\" count=\"8\" "
        "stride=\"3\">";
  ss << "<param name=\"X\" type=\"float\"/>";
  ss << "<param name=\"Y\" type=\"float\"/>";
  ss << "<param name=\"Z\" type=\"float\"/>";
  ss << "</accessor>";
  ss << "</technique_common>";
  ss << "</source>";
  ss << "<source id=\"Cube-mesh-normals\">";
  ss << "<float_array id=\"Cube-mesh-normals-array\" count=\"36\">0 0 -1 0 0 1 "
        "1 -5.96046e-7 3.27825e-7 -4.76837e-7 -1 0 -1 2.38419e-7 -1.19209e-7 "
        "2.08616e-7 1 0 0 0 -1 0 0 1 1 0 -2.38419e-7 0 -1 -4.76837e-7 -1 "
        "2.38419e-7 -1.49012e-7 2.68221e-7 1 2.38419e-7</float_array>";
  ss << "<technique_common>";
  ss << "<accessor source=\"#Cube-mesh-normals-array\" count=\"12\" "
        "stride=\"3\">";
  ss << "<param name=\"X\" type=\"float\"/>";
  ss << "<param name=\"Y\" type=\"float\"/>";
  ss << "<param name=\"Z\" type=\"float\"/>";
  ss << "</accessor>";
  ss << "</technique_common>";
  ss << "</source>";
  ss << "<vertices id=\"Cube-mesh-vertices\">";
  ss << "<input semantic=\"POSITION\" source=\"#Cube-mesh-positions\"/>";
  ss << "</vertices>";
  ss << "<polylist material=\"Material-material\" count=\"12\">";
  ss << "<input semantic=\"VERTEX\" source=\"#Cube-mesh-vertices\" "
        "offset=\"0\"/>";
  ss << "<input semantic=\"NORMAL\" source=\"#Cube-mesh-normals\" "
        "offset=\"1\"/>";
  ss << "<vcount>3 3 3 3 3 3 3 3 3 3 3 3 </vcount>";
  ss << "<p>0 0 1 0 2 0 7 1 6 1 5 1 4 2 5 2 1 2 5 3 6 3 2 3 2 4 6 4 7 4 0 5 3 "
        "5 7 5 3 6 0 6 2 6 4 7 7 7 5 7 0 8 4 8 1 8 1 9 5 9 2 9 3 10 2 10 7 10 "
        "4 11 0 11 7 11</p>";
  ss << "</polylist>";
  ss << "</mesh>";
  ss << "</geometry>";
  ss << "</library_geometries>";

  ss << "<library_effects>";
  for (auto kv : material) {
	Material mat = kv.second;
	std::string str = mat.name;
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	ss << "<effect id=\"Material-effect-" << str.c_str() << "\">";
	ss << "<profile_COMMON>";
	ss << "<technique sid=\"common\">";
	ss << "<phong>";
	ss << "<emission>";
	ss << "<color sid=\"emission\">0 0 0 1</color>";
	ss << "</emission>";
	ss << "<ambient>";
	ss << "<color sid=\"ambient\">0 0 0 1</color>";
	ss << "</ambient>";
	ss << "<diffuse>";
	ss << "<color sid=\"diffuse\">" << mat.r << " " << mat.g << " " << mat.b
	   << " 1</color>";
	ss << "</diffuse>";
	ss << "<specular>";
	ss << "<color sid=\"specular\">0.5 0.5 0.5 1</color>";
	ss << "</specular>";
	ss << "<shininess>";
	ss << "<float sid=\"shininess\">50</float>";
	ss << "</shininess>";
	ss << "<index_of_refraction>";
	ss << "<float sid=\"index_of_refraction\">1</float>";
	ss << "</index_of_refraction>";
	ss << "</phong>";
	ss << "</technique>";
	ss << "</profile_COMMON>";
	ss << "</effect>";
  }
  ss << "</library_effects>";

  ss << "<library_materials>";
  for (auto kv : material) {
	Material mat = kv.second;
	std::string str = mat.name;
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	ss << "<material id=\"Material-" << str.c_str() << "\" name=\"Material\">";
	ss << "<instance_effect url=\"#Material-effect-" << str.c_str() << "\"/>";
	ss << "</material>";
  }
  ss << "</library_materials>";

  ss << "<library_visual_scenes>";
  ss << "<visual_scene id=\"Scene\" name=\"Scene\">";
  for (int i = 0; i < this->grid; i++) {
	for (int j = 0; j < this->grid; j++) {
	  for (int k = 0; k < this->grid; k++) {
		if (this->geo->GetGrid(i, j, k) == 1) {
		  ss << "<node>";
		  ss << "<matrix sid=\"transform\">";
		  ss << "1 0 0 " << i * 2 << " 0 1 0 " << j * 2 << " 0 0 1 " << k * 2
		     << " 0 0 0 1";
		  ss << "</matrix>";

		  ss << "<instance_geometry url=\"#Cube-mesh\" name=\"Voxel\">";
		  ss << "<bind_material>";
		  ss << "<technique_common>";
		  std::string str = mat[i][j][k].name;
		  str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
		  ss << "<instance_material symbol=\"Material-" << str.c_str()
		     << "\" target=\"#Material-" << str.c_str() << "\"/>";
		  ss << "</technique_common>";
		  ss << "</bind_material>";
		  ss << "</instance_geometry>";
		  ss << "</node>";
		}
	  }
	}
  }
  ss << "</visual_scene>";
  ss << "</library_visual_scenes>";

  ss << "<scene>";
  ss << "<instance_visual_scene url=\"#Scene\" />";
  ss << "</scene>";
  ss << "</COLLADA>";

  ss.close();

  return true;
}

void Core::UpdateMetadata() {
}

void Core::Undo() {
  if (undo.size() < 1) {
	return;
  }
  struct Undo u = undo.back();
  undo.pop_back();
  geo->SetGrid(u.value, u.x, u.y, u.z);
  mat[u.x][u.y][u.z] = u.material;
}

void Core::UpdateGrid(unsigned short value, unsigned short x, unsigned short y,
                      unsigned short z) {
  // Save state to Undo

  struct Undo u;
  u.value = geo->GetGrid(x, y, z);
  u.x = x;
  u.y = y;
  u.z = z;
  u.material = mat[x][y][z];
  undo.push_back(u);

  // Modify Geo
  geo->SetGrid(value, x, y, z);
  mat[x][y][z] = currentMat;
}

void Core::SetMaterial(Material mat) {
  // Check if already exists, otherwise, push it
  std::string matName = mat.name;
  if (material.find(matName) == material.end()) {
	// Doesn't exist
	material.insert({matName, mat});
  }

  // Find and change to current material
  currentMat = material[matName];
}
