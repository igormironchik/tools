
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
			MainWindow::tr( "Ctrl+Q" ), qApp, &QApplication::quit );
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
