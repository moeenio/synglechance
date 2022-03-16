/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.h
 */

#pragma once

#include <boost/filesystem.hpp>
#include <QApplication>
#include <QCloseEvent>
#include <QLabel>
#include <QScreen>
#include <QThread>
#include <QWidget>

#include "pixmap.h"
#include "pipe.h"

namespace fs = boost::filesystem;

class AnimationTimer : public PipeWatcher {
	Q_OBJECT
	public:
		AnimationTimer(fs::path pipePath, QWidget *parent = nullptr);
		void contentsChanged(std::string msg) override;

	signals:
		void nextFrame();
		void startAnimation(QPoint startPos);
};

class Niko : public QWidget {
	Q_OBJECT
	public:
		Niko(QApplication *app, fs::path imagePath, fs::path pipePath, QWidget *parent = nullptr);

	public slots:
		void start(QPoint pos);
		void nextFrame();

	private:
		void closeEvent(QCloseEvent *e);
		int getFrame();

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
