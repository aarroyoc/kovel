/**
 * @file cli.cpp
 * @brief Entry point for Command Line kovel interface
 */

#include <iostream>
#include <algorithm>
#include <sstream>
#include "../core/core.hpp"
#include "../core/version.hpp"

char* getCmdOption(char** begin, char** end, const std::string& option) {
  char** itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
	return *itr;
  }
  return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option) {
  return std::find(begin, end, option) != end;
}

void ShowHelp() {
  /* Show help */
  std::cout << std::endl;
  std::cout << "HELP" << std::endl;
  std::cout << "-h\tShow help" << std::endl;
  std::cout << "-i\tInput file" << std::endl;
  std::cout << "-x\tExport as [dae,json]" << std::endl;
  std::cout << "-o\tOut file" << std::endl;
  std::cout << std::endl;
  std::cout << "Example (export as COLLADA DAE)" << std::endl;
  std::cout << "kovelcli -i File.kvl -x dae -o File.dae" << std::endl;
}

int main(int argc, char** argv) {

  std::cout << "==============================" << std::endl;
  std::cout << "KOVEL " << KOVEL_VERSION << std::endl;
  std::cout << "(C) " << KOVEL_COPYRIGHT << std::endl;
  std::cout << "Help: " << KOVEL_URL << std::endl;
  std::cout << "==============================" << std::endl;

  /* Parse command line options, show help, version and run exporter */

  if (cmdOptionExists(argv, argv + argc, "-h")) {
	ShowHelp();
	return 0;
  }

  char* input = getCmdOption(argv, argv + argc, "-i");
  char* exportAs = getCmdOption(argv, argv + argc, "-x");
  char* output = getCmdOption(argv, argv + argc, "-o");

  if (input && exportAs && output) {
	Core* core = Core::Instance();
	std::cout << "Processing file: " << input << std::endl;
	core->LoadFile(input);

#ifdef WIN32
	char* folder = std::getenv("TMP");
#else
	char* folder = std::getenv("TMPDIR");
	if (folder == 0) {
	  folder = (char*)"/tmp";
	}
#endif
	std::stringstream path;
	path << folder;
	path << "/KovelExport.tmp.kvl";
	core->SaveFile(path.str());
	if (!core->ValidateFile()) {
	  std::cerr << "Kovel file is not valid!" << std::endl;
	  return 2;
	}
	if (strcmp(exportAs, "dae") == 0) {
	  core->ExportAsDAE(output);
	} else if (strcmp(exportAs, "json") == 0) {
	  core->ExportAsJSON(output);
	} else {
	  std::cerr << "You must specify export as 'dae' or 'json'!" << std::endl;
	  return 3;
	}
  } else {
	ShowHelp();
	return 1;
  }

  return 0;
}
