/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.cpp
 */

#include "niko.h"

AnimationTimer::AnimationTimer(fs::path pipePath, QWidget *parent) : PipeWatcher(pipePath, parent) {}

void AnimationTimer::contentsChanged(std::string msg) {
	std::size_t comma = msg.find(",");

	if (comma == std::string::npos) {
		// If no comma, ignore
		return;
	}

	int x = std::stoi(msg.substr(0, comma)), y = std::stoi(msg.substr(comma+1));
	emit startAnimation(QPoint(x, y));

	while (!this->isInterruptionRequested()) {
		emit nextFrame();
		this->msleep(1000/60);
	}
}

Niko::Niko(QApplication *app, fs::path imagePath, fs::path pipePath, QWidget *parent) : QWidget(parent), app(app), screenSize(app->primaryScreen()->size()), imagePath(imagePath), pipePath(pipePath) {
	this->setWindowFlags(
		Qt::FramelessWindowHint |
		Qt::NoDropShadowWindowHint |
		Qt::WindowStaysOnTopHint
	);
	this->setAttribute(Qt::WA_TranslucentBackground);
	this->setMinimumSize(48, 64);
	this->setMaximumSize(48, 64);

	for (int i = 0; i < 3; i++) {
		std::string fileName = "niko" + std::to_string(i+1) + ".bmp";
		fs::path filePath = imagePath / fileName;
		this->frames[i] = loadPixmap(filePath);
	}

	this->label.setPixmap(this->frames[0]);

	connect(this->timer, &AnimationTimer::startAnimation, this, &Niko::start);
	connect(this->timer, &AnimationTimer::nextFrame, this, &Niko::nextFrame);
	this->timer->start();
}

void Niko::start(QPoint pos) {
	this->startPos = pos;
	this->pos = pos;

	// X must be offset to align properly
	this->pos.setX(pos.x() + 8);

	this->show();
}

void Niko::nextFrame() {
	this->label.setPixmap(this->frames[this->getFrame()]);
	this->pos.setY(this->pos.y() + 2);

	if (this->pos.y() > this->screenSize.height()) {
		// Once Niko's walked off the screen, quit the app
		this->timer->stop();
		this->app->quit();
	} else if (this->pos.y() > this->screenSize.height() - 64) {
		// Cut off parts outside screen bounds
		int diff = this->screenSize.height() - this->pos.y();
		this->setMinimumSize(48, diff);
		this->resize(48, diff);
	};

	this->move(this->pos);
	this->update();
}

void Niko::closeEvent(QCloseEvent *e) {
	this->timer->stop();
	e->accept();
}

int Niko::getFrame() {
	if ((this->pos.y() - this->startPos.y()) % 32 >= 16) {
		return 1;
	}
	if ((this->pos.y() - this->startPos.y()) % 64 >= 32) {
		return 0;
	}
	return 2;
}
