/*
 * OneShot Journal Qt reimplementation for Unix systems
 * Written by Vinyl Da.i'gyu-Kazotetsu (Gooborg Studios), © 2022.
 *
 * pipe.h
 */

#pragma once

#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <QThread>
#include <QWidget>

namespace fs = boost::filesystem;

class PipeWatcher : public QThread {
	public:
		PipeWatcher(fs::path pipePath, QWidget *parent = nullptr);
		void run();
		void stop();

		virtual void contentsChanged(std::string message);

	protected:
		fs::path pipePath;
};
