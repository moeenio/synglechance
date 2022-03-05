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

	// XXX Implement the rest of me!

	// if os.path.exists(save_path):
			// with open(save_path, 'rb') as save:
				// save.seek(-8, os.SEEK_END)
				// lang = save.read().decode('utf-8')
				// lang = lang[lang.find('[') + 1:lang.find(']')]
				// if lang == 'en_US': lang = 'en'
				// journal.changeImage('save_' + lang)
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
