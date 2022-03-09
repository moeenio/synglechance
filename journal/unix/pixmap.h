/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * pixmap.h
 */

#pragma once

#include <string>

#include <boost/filesystem.hpp>
#include <QBitmap>
#include <QColor>
#include <QPixmap>

namespace fs = boost::filesystem;

QPixmap loadPixmap(fs::path filePath);
