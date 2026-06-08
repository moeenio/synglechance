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
#include <pwd.h>

#include <QApplication>
#include <boost/filesystem.hpp>
#include <physfs.h>

#include "journal.h"
#include "niko.h"

namespace fs = boost::filesystem;

fs::path getImagePath(QApplication *app) {
	fs::path imagePath = app->applicationDirPath().toStdString();

	#ifdef __APPLE__
		imagePath = fs::canonical("../../..", imagePath);
	#endif

	return imagePath / "Graphics/Journal";
}

fs::path getPipePath(bool nikoMode = false) {
	#ifdef _WIN32
		return fs::path("\\\\.\\pipe\\oneshot-journal-to-game");
	#else
		const char *prefPath = PHYSFS_getPrefDir(".", "Oneshot");
		std::string filename = (nikoMode ? ".oneshot-niko-pipe" : ".oneshot-pipe");
		return fs::path(prefPath()) / filename;
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

	fs::path filePath = fs::path(documentsPath) / "OneShot" / "save_progress.oneshot";

	if (fs::exists(filePath)) {
		fs::ifstream file(filePath, std::ios::in | std::ios::binary);

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
	QApplication *app = new QApplication(argc, argv);
	Niko niko(app, getImagePath(app), getPipePath(true));

	niko.show();

	return app->exec();
}

int doJournal(int argc, char *argv[]) {
	QApplication *app = new QApplication(argc, argv);
	Journal journal(app, getImagePath(app), getPipePath());

	journal.show();

	return app->exec();
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
		return doNiko(argc, argv);
	}

    return doJournal(argc, argv);
}
