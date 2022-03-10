/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.cpp
 */

#include "niko.h"

AnimationTimer::AnimationTimer(fs::path pipePath, QWidget *parent) : QThread(parent) {
	// next_frame = pyqtSignal()
	// start_animation = pyqtSignal(int, int)
}

void AnimationTimer::run() {
	// while True:
	// 	while not os.path.exists(self.pipe): time.sleep(0.1)

	// 	pipe = open(self.pipe, "r")
	// 	pipe.flush()

	// 	while os.path.exists(self.pipe): # Make sure the file still exists and wasn't cleaned up by SyngleChance
	// 		message = os.read(pipe.fileno(), 256)
	// 		if len(message) > 0:
	// 			m = message.decode()
	// 			if not "," in m: pass
	// 			last_line = m.splitlines()[-1]
	// 			x, y = last_line.split(",")
	// 			self.start_animation.emit(int(x), int(y))

	// 			while True:
	// 				self.next_frame.emit()
	// 				time.sleep(1.0 / 60)
				
	// 		time.sleep(0.05)
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
}

void Niko::start(QPoint pos) {
	this->startPos = pos;
	this->pos = pos;

	// X must be offset to align properly
	this->pos.setX(pos.x() + 8);

	this->show();
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

void Niko::update() {
	this->label.setPixmap(this->frames[this->getFrame()]);
	this->pos.setY(this->pos.y() + 2);

	if (this->pos.y() > this->screenSize.height()) {
		// Once Niko's walked off the screen, quit the app
		this->app->quit();
	} else if (this->pos.y() > this->screenSize.height() - 64) {
		// Cut off parts outside screen bounds
		int diff = this->screenSize.height() - this->pos.y();
		this->setMinimumSize(48, diff);
		this->resize(48, diff);
	};

	this->move(this->pos);
}
