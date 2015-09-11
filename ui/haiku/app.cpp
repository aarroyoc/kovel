/**
 * @file app.cpp
 * @brief Entry point for Haiku native app
 */

class KovelApp : public BApplication{
	KovelApp() : BApplication("application/x-kovel-voxel"){
		KovelWindow* window=new KovelWindow();
		window->Show();
	}
	void	AboutRequested(){
		BString aboutText="";
		aboutText << "Kovel " << KOVEL_VERSION << "\n";
		aboutText << "\n";
		aboutText << "(C) " << KOVEL_COPYRIGHT << "\n";
		aboutText << "Homepage: " << KOVEL_URL << "\n";
		aboutText << "Licensed under the GNU GPL v2 license\n";

		BAlert* about=new BAlert("About",aboutText,"Thanks");
		about->Go();
	}
};

int main(int argc, char** argv)
{
	KovelApp app;
	app.Run();
	return 0;
}
