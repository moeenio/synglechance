/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * niko.cpp
 */

#include "niko.h"

AnimationTimer::AnimationTimer(std::string pipePath, QWidget *parent) : QThread(parent) {
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

Niko::Niko(QApplication *app, fs::path imagePath, QWidget *parent) : QWidget(parent), app(app), imagePath(imagePath) {
	// self.app, self.thread = kwargs["app"], kwargs["thread"]
	// self.screen_height = kwargs["screen_height"]
	// del kwargs["screen_height"], kwargs["app"], kwargs["thread"]

	// super().__init__(*args, **kwargs)

	// self.setWindowFlags(
	// 	Qt.WindowType.FramelessWindowHint |
	// 	Qt.WindowType.NoDropShadowWindowHint |
	// 	Qt.WindowType.WindowStaysOnTopHint
	// )
	// self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)
	// self.setMinimumSize(48, 64)
	// self.setMaximumSize(48, 64)

	// def loadBMP(img):
	// 	pixmap = QPixmap(img)
	// 	mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt.MaskMode.MaskInColor)
	// 	pixmap.setMask(mask)
	// 	return pixmap

	// self.frames = [
	// 	loadBMP(os.path.join(img_path, "niko{}.bmp".format(n))) for n in range(1,4)
	// ]

	// self.label = QLabel(self)
	// self.label.setPixmap(self.frames[1])
}

void Niko::start(int x, int y) {
	// self.x = x + 8
	// self.y = y
	// self.start_y = y

	// self.show()
}

int Niko::getFrame() {
	// if ((self.y - self.start_y) % 32 >= 16): return 1
	// if ((self.y - self.start_y) % 64 >= 32): return 0
	// else: return 2
}

void Niko::update() {
	// self.label.setPixmap(self.frames[self.getFrame()])
	// self.y += 2
	// if self.y > self.screen_height:
	// 	self.app.quit()
	// 	return
	// elif self.y > self.screen_height - 64:
	// 	self.setMinimumSize(48, self.screen_height - self.y)
	// 	self.resize(48, self.screen_height - self.y)
	// self.move(self.x, self.y)
}
