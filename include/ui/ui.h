#pragma once

#if defined WIN32 || defined MINGW
	// I hate Windows
	#define WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>
#endif

#include "ui/UIExporter.h"
#include "ui/UIPicker.h"
#include "ui/UIMainWindow.h"

#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qstylefactory.h>
#include <QtGui/QPalette>

#include <cstdlib>

int main(int argc, char** argv);
