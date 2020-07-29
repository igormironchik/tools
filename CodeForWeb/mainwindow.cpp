
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2016 Igor Mironchik

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// CodeForWeb include.
#include "mainwindow.hpp"

// Qt include.
#include <QTextCursor>
#include <QMimeData>
#include <QMenuBar>
#include <QApplication>


//
// TextEdit
//

TextEdit::TextEdit( QWidget * parent )
	:	QTextEdit( parent )
{
	setTabStopDistance( 40 );
}

TextEdit::~TextEdit()
{
}

QMimeData *
TextEdit::createMimeDataFromSelection() const
{
	QString sel = textCursor().selectedText();

	sel.replace( QLatin1Char( '\t' ), QLatin1String( "  " ) );
	sel.replace( QLatin1Char( '&' ), QLatin1String( "&amp;" ) );
	sel.replace( QLatin1Char( '<' ), QLatin1String( "&lt;" ) );
	sel.replace( QLatin1Char( '>' ), QLatin1String( "&gt;" ) );
	sel.replace( QChar( 0x2029 ), QLatin1String( "\n" ) );

	auto * mime = new QMimeData;
	mime->setText( sel );

	return mime;
}


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	explicit MainWindowPrivate( MainWindow * parent )
		:	q( parent )
		,	m_edit( 0 )
	{
	}

	//! Init.
	void init();

	//! Parent.
	MainWindow * q;
	//! Text edit.
	TextEdit * m_edit;
}; // class MainWindowPrivate

void
MainWindowPrivate::init()
{
	m_edit = new TextEdit( q );
	m_edit->setPlaceholderText( MainWindow::tr( "Paste your code here "
		"and then copy it..." ) );

	q->setCentralWidget( m_edit );

	q->resize( 800, 600 );

	q->menuBar()->addMenu( MainWindow::tr( "&File" ) )->addAction(
		QIcon( ":/img/application-exit.png" ), MainWindow::tr( "Quit" ),
			qApp, SLOT( quit() ), MainWindow::tr( "Ctrl+Q" ) );
}


//
// MainWindow
//

MainWindow::MainWindow()
	:	d( new MainWindowPrivate( this ) )
{
	d->init();
}

MainWindow::~MainWindow()
{
}
