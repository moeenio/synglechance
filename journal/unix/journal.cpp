/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * journal.cpp
 */

#include "journal.h"

WatchPipe::WatchPipe(fs::path pipePath, QWidget *parent) : QThread(parent), pipePath(pipePath) {}

void WatchPipe::run() {
	while (!this->isInterruptionRequested()) {
		emit changeImage("default");
		bool nondefault = false;

		// Create empty file
		fs::ofstream pipeCreate(this->pipePath, std::ios::trunc);
		pipeCreate.close();

		std::streampos currentPos = 0, lastPos = 0;

		while (fs::exists(pipePath) && !this->isInterruptionRequested()) {
			// While pipe exists, get contents
			fs::ifstream pipe(this->pipePath, std::ios::ate);

			currentPos = pipe.tellg();
			if (currentPos != lastPos) {
				pipe.seekg(lastPos);
				lastPos = currentPos;
				char *buf = new char[lastPos];
				pipe.read(buf, lastPos);
				std::string msg(buf); // Need casting to fix .find() bug

				if (msg.find("default") != std::string::npos) {
					nondefault = true;
				}
				emit changeImage(msg);
			} else if (currentPos == 0 && nondefault) {
				// If the pipe is emptied and we showed journal pages, close
				emit quitApp();
			}

			pipe.close();
			this->usleep(50);
		}

		this->usleep(50);
	}
}

void WatchPipe::stop() {
	this->requestInterruption();
	this->wait(); // Wait for watch thread finish
	fs::remove(this->pipePath); // Delete pipe file
}

CloseButton::CloseButton(fs::path imagePath, Journal *parent) : QAbstractButton(parent), imagePath(imagePath), parent(parent) {
	this->setAttribute(Qt::WA_Hover, true);

	this->pixmap = loadPixmap(imagePath / "close.bmp");
	this->pixmapHover = loadPixmap(imagePath / "close_hover.bmp");

	this->setCursor(QCursor(Qt::PointingHandCursor));
	this->moveButton();
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
				this->parent->quitApp();
			}
			break;
		default:
			break;
	}

	return QWidget::event(e);
}

void CloseButton::moveButton(std::string page) {
	bool leftClose = false;

	#ifdef __APPLE__
		// Close button is always left on macOS
		leftClose = true;
	#elif defined(__linux__)
		std::array<char, 128> buffer;
		std::string result;

		// XXX Only supports GNOME right now
		FILE* pipe = popen("gsettings get org.gnome.desktop.wm.preferences button-layout", "r");
		if (pipe) {
			while (fgets(buffer.data(), 128, pipe) != NULL) {
				// Read the data from the pipe
				result += buffer.data();
			}
			pclose(pipe);

			std::size_t colon = result.find(":");
			if (colon == std::string::npos) {
				result = result.substr(0, colon);
			}

			if (result.find("close")) {
				leftClose = true;
			}
		}
	#endif

	if (page == "final") {
		this->move(QPoint(leftClose ? 198 : 630-24, 30));
	} else if (page == "save") {
		this->move(leftClose ? QPoint(8, 16) : QPoint(790-24, 40));
	} else {
		this->move(QPoint(leftClose ? 0 : 800-24, 0));
	}
}

Journal::Journal(QApplication *app, fs::path imagePath, fs::path pipePath, QWidget *parent) : QWidget(parent), app(app), imagePath(imagePath), pipePath(pipePath) {
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

	connect(this->pipe, &WatchPipe::changeImage, this, &Journal::changeImage);
	connect(this->pipe, &WatchPipe::quitApp, this, &Journal::quitApp);
	this->pipe->start();
}

void Journal::closeEvent(QCloseEvent *e) {
	this->pipe->stop();
	e->accept();
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
		this->quitApp();
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
		lang = image.substr(underscore+1);

		if (lang.rfind("en", 0) == 0) {
			// If en_US, en_UK, etc., set to default
			lang = "";
		}

		boost::to_upper(lang);
	}

	if (this->currentImage == imgName) {
		// Don't update if image is the same
		return;
	}

	this->currentImage = imgName;

	// Show close button when applicable
	if (imgName == "default" || imgName == "save" || imgName == "final") {
		this->closeButton->show();
		this->closeButton->moveButton(imgName);
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

void Journal::quitApp() {
	this->pipe->stop();
	this->app->quit();
}

