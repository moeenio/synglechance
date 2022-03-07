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
#include <QPoint>
#include <QLabel>

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
	bool mouseDown = false;
	QPoint mousePos = QPoint(0, 0);
	QLabel label = QLabel(this);
	CloseButton *closeButton;
};
