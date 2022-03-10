/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.h
 */

#pragma once

#include <boost/filesystem.hpp>
#include <QWidget>
#include <QThread>

#include "pixmap.h"

namespace fs = boost::filesystem;

class AnimationTimer : public QThread {
	Q_OBJECT
	public:
		AnimationTimer(fs::path pipePath, QWidget *parent = nullptr);
		void run();
	private:
		fs::path pipePath;
};

class Niko : public QWidget {
	Q_OBJECT
	public:
		Niko(QApplication *app, fs::path imagePath, fs::path pipePath, QWidget *parent = nullptr);
		void start(int x, int y);
	private:
		int getFrame();
		void update();

		QApplication *app;
		fs::path imagePath;
		fs::path pipePath;
		AnimationTimer *timer = new AnimationTimer(pipePath, this);
};