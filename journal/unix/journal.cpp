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

CloseButton::CloseButton(QApplication *app, fs::path imagePath, QWidget *parent) : QAbstractButton(parent), app(app), imagePath(imagePath) {
	this->setAttribute(Qt::WA_Hover, true);

	this->pixmap = loadPixmap(imagePath / "close.bmp");
	this->pixmapHover = loadPixmap(imagePath / "close_hover.bmp");

	this->setCursor(QCursor(Qt::PointingHandCursor));
	this->move(this->getXPos(), 0);
}

QSize CloseButton::sizeHint() {
	return this->pixmap.size();
}

void CloseButton::paintEvent(QPaintEvent *e) {
	QPainter painter(this);
	painter.drawPixmap(
		this->pixmap.rect(), this->hovering ? this->pixmapHover : this->pixmap
	);
}

bool CloseButton::event(QEvent *e) {
	switch(e->type()) {
		case QEvent::HoverEnter:
			this->hovering = true;
			this->update();
			break;
		case QEvent::HoverLeave:
			this->hovering = false;
			this->update();
			break;
		case QEvent::MouseButtonRelease:
			if (this->rect().contains(static_cast<QMouseEvent*>(e)->pos())) {
				// If we're still hovering over the close button
				this->app->quit();
			}
			break;
		default:
			break;
	}

	return QWidget::event(e);
}

int CloseButton::getXPos() {
	bool leftClose = false;

	#ifdef __APPLE__
		// Close button is always left on macOS
		leftClose = true;
	#elif defined(__linux__)
			// 	try:
	// 		o = os.popen("gsettings get org.gnome.desktop.wm.preferences button-layout").read()
	// 		if "close" in o.split(":")[0]:
	// 			left_close = True
	// 	except:
	// 		# If this fails, don't worry about it
	// 		pass
	#endif

	return leftClose ? 0 : 800-24;
}

Journal::Journal(QApplication *app, fs::path imagePath, QWidget *parent) : QWidget(parent), app(app), imagePath(imagePath) {
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

	this->setCursor(QCursor(Qt::OpenHandCursor));

	// Set to default image
	this->changeImage("default");
}

void Journal::mousePressEvent(QMouseEvent *e) {
	this->mouseDown = true;
	this->mouseDownPos = e->pos();

	if (e->buttons() == Qt::LeftButton) {
		// Set cursor only if left mouse button
		this->setCursor(QCursor(Qt::ClosedHandCursor));
	}
}

void Journal::mouseReleaseEvent(QMouseEvent *e) {
	this->mouseDown = false;
	this->setCursor(QCursor(Qt::OpenHandCursor));
}

void Journal::mouseMoveEvent(QMouseEvent *e) {
	if (!(e->buttons() == Qt::LeftButton && this->mouseDown)) {
		// If not dragging with left mouse button, or if we didn't start dragging, ignore
		return;
	}

	QPoint pos = e->pos();
	QRect frameGm = this->frameGeometry();
	this->setGeometry(frameGm.x() + pos.x() - this->mouseDownPos.x(), frameGm.y() + pos.y() - this->mouseDownPos.y(), 800, 600);
}

void Journal::changeImage(std::string image) {
	// If we're told to close the app, close it
	if (image == "CLOSE") {
		this->app->quit();
	}

	std::string imgName;
	std::string lang = "";

	std::size_t underscore = image.find("_");
	if (underscore == std::string::npos) {
		// If no underscore, assume no locale
		imgName = image;
		lang = "";
	} else {
		imgName = image.substr(0, underscore);
		lang = image.substr(underscore);

		if (lang.rfind("en", 0) == 0) {
			// If en_US, en_UK, etc., set to default
			lang = "";
		}

		boost::to_upper(lang);
	}

	// Show close button when applicable
	if (imgName == "default" || imgName == "save" || imgName == "final") {
		this->closeButton->show();
	} else {
		this->closeButton->hide();
	}

	fs::path imgPath = this->imagePath / lang / (imgName + ".bmp");
	if (!fs::exists(imgPath)) {
		// If the image doesn't exist, ignore
		return;
	}

	QPixmap pixmap = loadPixmap(imgPath);
	this->label.setPixmap(pixmap);
}

