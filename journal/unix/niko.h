/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.h
 */

#pragma once

#include <boost/filesystem.hpp>
#include <QApplication>
#include <QLabel>
#include <QScreen>
#include <QThread>
#include <QWidget>

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
		void start(QPoint pos);
	private:
		int getFrame();
		void update();

		QApplication *app;
		fs::path imagePath;
		fs::path pipePath;
		QPixmap frames[3];

		QSize screenSize;
		QPoint startPos;
		QPoint pos;

		QLabel label = QLabel(this);
		AnimationTimer *timer = new AnimationTimer(pipePath, this);
};
