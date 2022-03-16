/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * journal.h
 */

#pragma once

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <QAbstractButton>
#include <QApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QHoverEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPoint>
#include <QWidget>

#include "pixmap.h"
#include "pipe.h"

namespace fs = boost::filesystem;

class Journal;

class WatchPipe : public PipeWatcher {
	Q_OBJECT
	public:
		WatchPipe(fs::path pipePath, QWidget *parent = nullptr);
		void contentsChanged(std::string msg) override;

	signals:
		void changeImage(std::string image);
		void quitApp();

	private:
		bool hasChanged = false;
};

class CloseButton : public QAbstractButton {
	Q_OBJECT
	public:
		CloseButton(fs::path imagePath, Journal *parent = nullptr);
		QSize sizeHint();
		void moveButton(std::string page = "default");
	private:
		void paintEvent(QPaintEvent *e);
		bool event(QEvent *e);

		Journal *parent;
		fs::path imagePath;

		QPixmap pixmap;
		QPixmap pixmapHover;
		bool hovering = false;
};

class Journal : public QWidget {
	Q_OBJECT
	
	public:
		Journal(QApplication *app, fs::path imagePath, fs::path pipePath, QWidget *parent = nullptr);

	public slots:
		void changeImage(std::string image);
		void quitApp();

	private:
		void closeEvent(QCloseEvent *e);
		void mousePressEvent(QMouseEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
		void mouseMoveEvent(QMouseEvent *e);

		QApplication *app;
		fs::path imagePath;
		fs::path pipePath;
		std::string currentImage;
		bool mouseDown = false;
		QPoint mouseDownPos = QPoint(0, 0);
		QLabel label = QLabel(this);
		CloseButton *closeButton = new CloseButton(imagePath, this);
		WatchPipe *pipe = new WatchPipe(pipePath, this);
};
