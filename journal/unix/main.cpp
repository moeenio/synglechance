/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * main.cpp
 */

#include "journal.h"
#include "niko.h"

#include <QApplication>

int main(int argc, char *argv[]) {
	QApplication App(argc, argv);

	if (argc > 1) {
		// do Niko
	} else {
		// do journal
	}

	return App.exec();
}
