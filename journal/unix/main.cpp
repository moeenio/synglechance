/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * main.cpp
 */

#include <string>

#include <QApplication>

#include "journal.h"
#include "niko.h"

std::string getDocumentsPath() {
	#ifdef _WIN32
		WCHAR path[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path);
		return std::string(w32_fromWide(path));
	#elif defined __APPLE__
		return std::string(getenv("HOME")) + "/Documents";
	#elif defined __linux__
		return std::string(xdg_user_dir_lookup("DOCUMENTS"));
	#else
		#error OS is unsupported!
	#endif
}

std::string getPipePath(bool nikoMode) {
	#ifdef _WIN32
		return std::string("\\\\.\\pipe\\oneshot-journal-to-game");
	#else
		std::string path = (nikoMode ? "/.oneshot-niko-pipe" : "/.oneshot-pipe");
		return std::string(getenv("HOME")) + path;
	#endif
}

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	Niko niko;
	Journal journal;

    if (argc > 1) {
		// do Niko
	} else {
        journal.show();
	}

	return app.exec();
}
