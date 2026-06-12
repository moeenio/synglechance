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
		return fs::path(prefPath) / filename;
	#endif
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
