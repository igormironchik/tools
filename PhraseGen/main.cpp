
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// Qt include.
#include <QApplication>
#include <QIcon>

// PhraseGen include.
#include "mainwindow.hpp"


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QIcon appIcon( ":/img/app_256x256.png" );
	appIcon.addFile( ":/img/app_128x128.png" );
	appIcon.addFile( ":/img/app_64x64.png" );
	appIcon.addFile( ":/img/app_48x48.png" );
	appIcon.addFile( ":/img/app_32x32.png" );
	appIcon.addFile( ":/img/app_22x22.png" );
	appIcon.addFile( ":/img/app_16x16.png" );
	app.setWindowIcon( appIcon );

	PhraseGen::MainWindow w;
	w.show();

	return app.exec();
}
