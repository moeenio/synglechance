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
#include <QLabel>
#include <QPoint>
#include <QThread>
#include <QWidget>

namespace fs = boost::filesystem;

class WatchPipe : public QThread {
	Q_OBJECT
	public:
		WatchPipe(std::string pipePath, QWidget *parent = nullptr);
		void run();
};

class CloseButton : public QAbstractButton {
	Q_OBJECT
	public:
		CloseButton(QWidget *parent = nullptr);
		QSize sizeHint();
	private:
		void paintEvent(QPaintEvent *e);
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);

		int getXPos();
};

class Journal : public QWidget {
	Q_OBJECT
	
public:
	Journal(QApplication *app, QWidget *parent = nullptr);
private:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

	void changeImage(std::string image);

	QApplication *app;
	fs::path imagePath;
	bool mouseDown = false;
	QPoint mousePos = QPoint(0, 0);
	QLabel label = QLabel(this);
	CloseButton *closeButton;
};
