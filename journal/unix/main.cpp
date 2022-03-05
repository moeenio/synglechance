/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * main.cpp
 */

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include <QApplication>

#include "journal.h"
#include "niko.h"

std::string getPipePath(bool nikoMode) {
	#ifdef _WIN32
		return std::string("\\\\.\\pipe\\oneshot-journal-to-game");
	#else
		std::string path = (nikoMode ? "/.oneshot-niko-pipe" : "/.oneshot-pipe");
		return std::string(getenv("HOME")) + path;
	#endif
}

std::string checkSaveProgress() {
	std::string documentsPath;
	#ifdef _WIN32
		WCHAR path[MAX_PATH];
		SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path);
		documentsPath = std::string(path);
	#elif defined __APPLE__
		documentsPath = std::string(getenv("HOME")) + "/Documents";
	#elif defined __linux__
		documentsPath = std::string(xdg_user_dir_lookup("DOCUMENTS"));
	#else
		#error OS is unsupported!
	#endif

	std::string filePath = documentsPath + "/OneShot/save_progress.oneshot";

	if (access(filePath.c_str(), F_OK) != -1) {
		std::ifstream file(filePath, std::ios::in | std::ios::binary);

		std::string search("HeyNoxidHeresTheLanguage[");
		std::string line;

		while(std::getline(file, line)) {
			std::size_t searchResult = line.find(search);
			if (searchResult != std::string::npos) {
				line = line.substr(line.find(search)+search.length());
				std::string lang = line.substr(0, line.length()-1);
				file.close();
				return lang == "en_US" ? std::string("en") : lang;
			}
		}

		file.close();
	}

	return "";
}

int doNiko(int argc, char *argv[]) {
	QApplication app(argc, argv);
	Niko niko(&app);

	niko.show();

	return app.exec();
}

int doJournal(int argc, char *argv[]) {
	QApplication app(argc, argv);
	Journal journal(&app);

	journal.show();

	return app.exec();
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
		return doNiko(argc, argv);
	}

    return doJournal(argc, argv);
}
