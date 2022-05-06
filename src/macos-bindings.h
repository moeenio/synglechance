//
// SyngleChance Engine - macos-bindings.h
// ©2018 Vinyl Da.i'gyu-Kazotetsu.  You may use this code for anything you'd like.
// https://www.queengoob.org
//

#pragma once

#include <string>

namespace macOS {
	void CacheCurrentBackground();
	void ChangeBackground(std::string imageURL, double red, double green, double blue);
	void ResetBackground();
    void Beep();
}