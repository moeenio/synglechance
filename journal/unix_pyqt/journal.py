# -*- coding: utf-8 -*-

from pathlib import Path
import os, sys, time

from PyQt6.QtCore import Qt, QEvent, QThread, pyqtSignal, QRect, QRectF, QTimer, QPoint
from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QAbstractButton
from PyQt6.QtGui import QIcon, QPixmap, QPainter, QCursor, QColor

def get_documents_path():
	if sys.platform == 'win32':
		import ctypes, ctypes.wintypes
		buff = ctypes.create_unicode_buffer(ctypes.wintypes.MAX_PATH)
		ctypes.windll.shell32.SHGetFolderPathW(None, CSIDL_PERSONAL, None, SHGFP_TYPE_CURRENT, buff)
		return os.path.join(buff.value, 'My Games')
	elif sys.platform == 'linux':
		from gi.repository import GLib
		return GLib.get_user_special_dir(GLib.UserDirectory.DIRECTORY_DOCUMENTS)
	else:
		return os.path.expanduser('~/Documents')

def get_pipe_path(mode='journal'):
	if sys.platform == 'win32':
		return '\\\\.\\pipe\\oneshot-journal-to-game'
	else:
		if mode == 'niko':
			return os.path.expanduser('~/.oneshot-niko-pipe')
		return os.path.expanduser('~/.oneshot-pipe')

# Get whether to place close button on left or right
left_close = False
if sys.platform == 'darwin':
	left_close = True
elif sys.platform == 'linux':
	try:
		o = os.popen('gsettings get org.gnome.desktop.wm.preferences button-layout').read()
		if 'close' in o.split(':')[0]:
			left_close = True
	except:
		# If this fails, don't worry about it
		pass

def get_img_path():
	if getattr(sys, 'frozen', False) and hasattr(sys, '_MEIPASS'):
		base_path = Path(sys._MEIPASS)
		if sys.platform == 'darwin':
			base_path = Path(base_path, '..', '..', '..')
	else:
		# XXX Not supposed to be run without PyInstaller bundle
		# img_path = Path(__file__, '..', 'images').resolve()
		base_path = Path(Path.home(), "Library/Application Support/Steam/SteamApps/common/OneShot").resolve()
	return Path(base_path, 'Graphics', 'Journal').resolve()

img_path = get_img_path()

def loadBMP(img):
	pixmap = QPixmap(img)
	mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt.MaskMode.MaskInColor)
	pixmap.setMask(mask)
	return pixmap



class PipeThread(QThread):
	def __init__(self, *args, **kwargs):
		self.pipe = kwargs['pipe']
		del kwargs['pipe']
		super().__init__(*args, **kwargs)

class WatchPipe(PipeThread):
	changeImage = pyqtSignal(str)

	def run(self):
		while True:
			self.changeImage.emit('default_en')
			while not os.path.exists(self.pipe): time.sleep(0.1)

			pipe = open(self.pipe, 'r')
			pipe.flush()

			was_nondefault = False

			while os.path.exists(self.pipe): # Make sure the file still exists and wasn't cleaned up by SyngleChance
				message = os.read(pipe.fileno(), 256)
				if len(message) > 0:
					m = message.decode()
					if m != 'default_en':
						was_nondefault = True
					self.changeImage.emit(m)
				else:
					try:
						st = os.stat(self.pipe)
						if st.st_size == 0 and was_nondefault:
							self.changeImage.emit('CLOSE')
					except FileNotFoundError:
						pass

					time.sleep(0.05)

class AnimationTimer(PipeThread):
	next_frame = pyqtSignal()
	start_animation = pyqtSignal(int, int)

	def run(self):
		while True:
			while not os.path.exists(self.pipe): time.sleep(0.1)

			pipe = open(self.pipe, 'r')
			pipe.flush()

			while os.path.exists(self.pipe): # Make sure the file still exists and wasn't cleaned up by SyngleChance
				message = os.read(pipe.fileno(), 256)
				if len(message) > 0:
					m = message.decode()
					if not ',' in m: pass
					last_line = m.splitlines()[-1]
					x, y = last_line.split(',')
					self.start_animation.emit(int(x), int(y))

					while True:
						self.next_frame.emit()
						time.sleep(1.0 / 60)
					
				time.sleep(0.05)

class CloseButton(QAbstractButton):
	def __init__(self, parent=None):
		super().__init__(parent)

		self.parent = parent
		self.pixmap = loadBMP(os.path.join(img_path, 'close.bmp'))
		self.pixmap_hover = loadBMP(os.path.join(img_path, 'close_hover.bmp'))

		self.hovering = False
	
		self.setCursor(QCursor(Qt.CursorShape.PointingHandCursor))

	def sizeHint(self):
		return self.pixmap.size()

	def paintEvent(self, event):
		painter = QPainter(self)
		painter.drawPixmap(
			event.rect(), self.pixmap_hover if self.hovering else self.pixmap
		)

	def enterEvent(self, event):
		self.hovering = True
		self.update()

	def leaveEvent(self, event):
		self.hovering = False
		self.update()

	def mouseReleaseEvent(self, event):
		self.parent.app.quit()

class Journal(QWidget):
	def __init__(self, *args, **kwargs):
		self.app = kwargs['app']
		del kwargs['app']

		super().__init__(*args, **kwargs)

		self.mousedown = False
		self.mousedownpos = QPoint(0, 0)

		self.label = QLabel(self)

		self.close_button = CloseButton(self)
		if not left_close: self.close_button.move(800-24, 0)

		self.changeImage('default_en')

		self.setWindowFlags(
			Qt.WindowType.FramelessWindowHint |
			Qt.WindowType.NoDropShadowWindowHint
		)

		self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)
		self.setMouseTracking(True)
		self.setWindowTitle(' ')
		self.setMinimumSize(800, 600)
		self.setMaximumSize(800, 600)
		self.setGeometry(0, 0, 800, 600)

		self.show()

	def mousePressEvent(self, event):
		self.mousedown = True
		self.mousedownpos = event.pos()

	def mouseReleaseEvent(self, event):
		self.mousedown = False

	def mouseMoveEvent(self, event):
		if event.buttons() == Qt.MouseButton.LeftButton:
			pos = event.pos()
			frameGm = self.frameGeometry()
			self.setGeometry(frameGm.x() + pos.x() - self.mousedownpos.x(), frameGm.y() + pos.y() - self.mousedownpos.y(), 800, 600)

	def changeImage(self, image):
		if image == 'CLOSE':
			self.app.quit()
			return
		if not '_' in image: return

		name, lang = image.split('_', 1)

		if name == 'default' or name == 'save' or name == 'final':
			self.close_button.show()
		else:
			self.close_button.hide()

		if lang == 'en':
			img = os.path.join(img_path, '{}.bmp'.format(name))
		else:
			img = os.path.join(img_path, lang.upper(), '{}.bmp'.format(name))
		
		if not os.path.exists(img):
			return

		pixmap = loadBMP(img)
		self.label.setPixmap(pixmap)

class Niko(QWidget):
	def __init__(self, *args, **kwargs):
		self.app, self.thread = kwargs['app'], kwargs['thread']
		self.screen_height = kwargs['screen_height']
		del kwargs['screen_height'], kwargs['app'], kwargs['thread']

		super().__init__(*args, **kwargs)

		self.setWindowFlags(
			Qt.WindowType.FramelessWindowHint |
			Qt.WindowType.NoDropShadowWindowHint |
			Qt.WindowType.WindowStaysOnTopHint
		)
		self.setAttribute(Qt.WidgetAttribute.WA_TranslucentBackground)
		self.setMinimumSize(48, 64)
		self.setMaximumSize(48, 64)

		self.frames = [
			loadBMP(os.path.join(img_path, 'niko{}.bmp'.format(n))) for n in range(1,4)
		]

		self.label = QLabel(self)
		self.label.setPixmap(self.frames[1])

	def start(self, x, y):
		self.x = x + 8
		self.y = y
		self.start_y = y

		self.show()

	def getFrame(self):
		if ((self.y - self.start_y) % 32 >= 16): return 1
		if ((self.y - self.start_y) % 64 >= 32): return 0
		else: return 2

	def update(self):
		self.label.setPixmap(self.frames[self.getFrame()])
		self.y += 2
		if self.y > self.screen_height:
			self.app.quit()
			return
		elif self.y > self.screen_height - 64:
			self.setMinimumSize(48, self.screen_height - self.y)
			self.resize(48, self.screen_height - self.y)
		self.move(self.x, self.y)

if __name__ == '__main__':
	app = QApplication(sys.argv)

	pipe_path = get_pipe_path()
	if len(sys.argv) == 2 and sys.argv[1] == 'niko':
		# "Niko-leaves-the-screen" mode.
		pipe_path = get_pipe_path('niko')
		thread = AnimationTimer(pipe = pipe_path)

		niko = Niko(screen_height = app.primaryScreen().size().height(), app = app, thread = thread)

		thread.start_animation.connect(niko.start)
		thread.next_frame.connect(niko.update)
		thread.start()

	else:
		# Author's Journal mode.
		journal = Journal(app = app)
		save_path = os.path.join(get_documents_path(), 'Oneshot', 'save_progress.oneshot')
		if os.path.exists(save_path):
			with open(save_path, 'rb') as save:
				save.seek(-8, os.SEEK_END)
				lang = save.read().decode('utf-8')
				lang = lang[lang.find('[') + 1:lang.find(']')]
				if lang == 'en_US': lang = 'en'
				journal.changeImage('save_' + lang)
		thread = WatchPipe(pipe = pipe_path)
		thread.changeImage.connect(journal.changeImage)
		thread.start()

	if not os.path.exists(pipe_path):
		pipe_file = open(pipe_path, 'w+')
		pipe_file.close()

	app.exec()

	try:
		os.remove(pipe_path)
	except:
		# Most likely due to the file being in use, ignore.
		pass
