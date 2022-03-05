/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.h
 */

#include <QWidget>
#include <QThread>

class AnimationTimer : public QThread {
	Q_OBJECT
	public:
		AnimationTimer(QObject *parent = nullptr);
		void run();
};

class Niko : public QWidget {
	Q_OBJECT
	public:
		Niko(QObject *parent = nullptr);
		void start(int x, int y);
		int getFrame();
		void update();
};