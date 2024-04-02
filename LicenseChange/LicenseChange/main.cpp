
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// LicenseChange include.
#include "mainwindow.hpp"

// Qt include.
#include <QApplication>


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QIcon appIcon( ":/img/app_48x48.png" );
	appIcon.addFile( ":/img/app_32x32.png" );
	appIcon.addFile( ":/img/app_22x22.png" );
	appIcon.addFile( ":/img/app_16x16.png" );
	app.setWindowIcon( appIcon );

	MainWindow w;
	w.showMaximized();

	return app.exec();
}
