/**
 * @file app.cpp
 * @brief Entry point for Haiku native app
 */

#include "../../core/core.hpp"
#include "../../core/version.hpp"
#include "window.hpp"

#include <Application.h>

class KovelApp : public BApplication {
public:
  KovelWindow* window;
  KovelApp() : BApplication("application/x-kovel-voxel") {
	window = new KovelWindow();
	window->Show();
  }
  void AboutRequested() {
	BString aboutText = "";
	aboutText << "Kovel " << KOVEL_VERSION << "\n";
	aboutText << "\n";
	aboutText << "(C) " << KOVEL_COPYRIGHT << "\n";
	aboutText << "Homepage: " << KOVEL_URL << "\n";
	aboutText << "Licensed under the GNU GPL v2 license\n";

	BAlert* about = new BAlert("About", aboutText, "Thanks");
	about->Go();
  }
  void LoadFile(char* filename) {
	window->LoadFile(filename);
  }
};

int main(int argc, char** argv) {
  KovelApp app;
  if (argc == 2) {
	app.LoadFile(argv[1]);
  }
  app.Run();
  return 0;
}
