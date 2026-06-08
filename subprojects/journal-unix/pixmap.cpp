/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * pixmap.cpp
 */

#include "pixmap.h"

QPixmap loadPixmap(fs::path filePath) {
	QPixmap pixmap = QPixmap(filePath.c_str());
	QBitmap mask = pixmap.createMaskFromColor(QColor(0, 255, 0), Qt::MaskInColor);
	pixmap.setMask(mask);
	return pixmap;
}