/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * journal.cpp
 */

#include "journal.h"

WatchPipe::WatchPipe(std::string pipePath, QWidget *parent) : QThread(parent) {
	// changeImage = pyqtSignal(str)
}

void WatchPipe::run() {
	// while True:
	// 	self.changeImage.emit("default")
	// 	while not os.path.exists(self.pipe): time.sleep(0.1)

	// 	pipe = open(self.pipe, "r")
	// 	pipe.flush()

	// 	was_nondefault = False

	// 	while os.path.exists(self.pipe): # Make sure the file still exists and wasn't cleaned up by SyngleChance
	// 		message = os.read(pipe.fileno(), 256)
	// 		if len(message) > 0:
	// 			m = message.decode()
	// 			if m != "default_en":
	// 				was_nondefault = True
	// 			self.changeImage.emit(m)
	// 		else:
	// 			try:
	// 				st = os.stat(self.pipe)
	// 				if st.st_size == 0 and was_nondefault:
	// 					self.changeImage.emit("CLOSE")
	// 			except FileNotFoundError:
	// 				pass

	// 			time.sleep(0.05)
}

CloseButton::CloseButton(QWidget *parent) : QAbstractButton(parent) {
	// def loadBMP(img):
	// 	pixmap = QPixmap(img)
	// 	mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt.MaskMode.MaskInColor)
	// 	pixmap.setMask(mask)
	// 	return pixmap
	
	// self.parent = parent
	// self.pixmap = loadBMP(os.path.join(img_path, "close.bmp"))
	// self.pixmap_hover = loadBMP(os.path.join(img_path, "close_hover.bmp"))

	// self.hovering = False

	// self.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

	// self.move(self.getXPos(), 0)
}

QSize CloseButton::sizeHint() {
	// return self.pixmap.size()
}

void CloseButton::paintEvent(QPaintEvent *e) {
	// painter = QPainter(self)
	// painter.drawPixmap(
	// 	event.rect(), self.pixmap_hover if self.hovering else self.pixmap
	// )
}

void CloseButton::enterEvent(QEvent *e) {
	// self.hovering = True
	// self.update()
}

void CloseButton::leaveEvent(QEvent *e) {
	// self.hovering = False
	// self.update()
}

void CloseButton::mouseReleaseEvent(QMouseEvent *e) {
	// self.parent.app.quit()
}

int CloseButton::getXPos() {
	// left_close = False
	// if sys.platform == "darwin":
	// 	left_close = True
	// elif sys.platform == "linux":
	// 	try:
	// 		o = os.popen("gsettings get org.gnome.desktop.wm.preferences button-layout").read()
	// 		if "close" in o.split(":")[0]:
	// 			left_close = True
	// 	except:
	// 		# If this fails, don't worry about it
	// 		pass

	// return left_close ? 0 : 800-24;
}

Journal::Journal(QApplication *app, QWidget *parent) : QWidget(parent) {
	this->app = app;
	this->changeImage("default");

	this->setWindowFlags(
		Qt::FramelessWindowHint |
		Qt::NoDropShadowWindowHint
	);
	this->setAttribute(Qt::WA_TranslucentBackground);

	this->setMouseTracking(true);
	this->setWindowTitle(" ");
	this->setMinimumSize(800, 600);
	this->setMaximumSize(800, 600);
	this->setGeometry(0, 0, 800, 600);
}

void Journal::mousePressEvent(QMouseEvent *e) {
	// self.mousedown = True
	// self.mousedownpos = event.pos()
}

void Journal::mouseReleaseEvent(QMouseEvent *e) {
	// self.mousedown = False
}

void Journal::mouseMoveEvent(QMouseEvent *e) {
	// if event.buttons() == Qt.MouseButton.LeftButton:
	// 	pos = event.pos()
	// 	frameGm = self.frameGeometry()
	// 	self.setGeometry(frameGm.x() + pos.x() - self.mousedownpos.x(), frameGm.y() + pos.y() - self.mousedownpos.y(), 800, 600)
}

void Journal::changeImage(std::string image) {
	// if image == "CLOSE":
	// 	self.app.quit()
	// 	return

	// name, lang = image.split('_', 1)

	// if name == 'default' or name == 'save' or name == 'final':
	// 	self.close_button.show()
	// else:
	// 	self.close_button.hide()

	// if lang == 'en':
	// 	img = os.path.join(img_path, '{}.bmp'.format(name))
	// if not "_" in image: return
	// else:
	// 	img = os.path.join(img_path, lang.upper(), '{}.bmp'.format(name))
	
	// if not os.path.exists(img):
	// 	return

	// def loadBMP(img):
	// 	pixmap = QPixmap(img)
	// 	mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt.MaskMode.MaskInColor)
	// 	pixmap.setMask(mask)
	// 	return pixmap

	// pixmap = loadBMP(img)
	// self.label.setPixmap(pixmap)
}

