/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * pipe.cpp
 */

#include "pipe.h"
#include <fstream>

PipeWatcher::PipeWatcher(fs::path pipePath, QWidget *parent) : QThread(parent), pipePath(pipePath) {}

void PipeWatcher::run() {
	while (!this->isInterruptionRequested()) {
		// Create empty file
		std::ofstream pipeCreate(this->pipePath.native(), std::ios::trunc);
		pipeCreate.close();

		std::streampos currentPos = 0, lastPos = 0;

		while (fs::exists(pipePath) && !this->isInterruptionRequested()) {
			// While pipe exists, get contents
			std::ifstream pipe(this->pipePath.native(), std::ios::ate);

			currentPos = pipe.tellg();
			if (currentPos != lastPos) {
				pipe.seekg(lastPos);
				lastPos = currentPos;
				char *buf = new char[lastPos];
				pipe.read(buf, lastPos);
				this->contentsChanged(std::string(buf));
			}

			pipe.close();
			this->usleep(50);
		}

		this->usleep(50);
	}
}

void PipeWatcher::stop() {
	this->requestInterruption();
	this->wait(); // Wait for watch thread finish
	fs::remove(this->pipePath); // Delete pipe file
}

void PipeWatcher::contentsChanged(std::string msg) {
	// Override
}
