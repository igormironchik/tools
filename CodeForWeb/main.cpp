
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// CodeForWeb include.
#include "mainwindow.hpp"

// Qt include.
#include <QApplication>


int main( int argc, char ** argv )
{
	QApplication app( argc, argv );

	QIcon appIcon( ":/img/Icon_48x48.png" );
	appIcon.addFile( ":/img/Icon_32x32.png" );
	appIcon.addFile( ":/img/Icon_22x22.png" );
	appIcon.addFile( ":/img/Icon_16x16.png" );
	app.setWindowIcon( appIcon );

	MainWindow w;
	w.show();

	return app.exec();
}
