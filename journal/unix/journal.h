/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * journal.h
 */

#pragma once

#include <string>

#include <QWidget>
#include <QAbstractButton>
#include <QThread>

class WatchPipe : public QThread {
	Q_OBJECT
	public:
		WatchPipe(QObject *parent = nullptr);
		void run();
};

class CloseButton : public QAbstractButton {
	Q_OBJECT
	public:
		CloseButton(QObject *parent = nullptr);
		QSize sizeHint();
	private:
		void paintEvent(QPaintEvent *e);
		void enterEvent(QEvent *e);
		void leaveEvent(QEvent *e);
		void mouseReleaseEvent(QMouseEvent *e);
};

class Journal : public QWidget {
	Q_OBJECT
	
public:
	Journal(QWidget *parent = nullptr);
private:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void changeImage(std::string image);
};
