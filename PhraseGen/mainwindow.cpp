
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// PhraseGen include.
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

// Qt include.
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QList>
#include <QChar>
#include <QLineEdit>
#include <QClipboard>
#include <QApplication>

// C++ include.
#include <random>
#include <algorithm>


namespace PhraseGen {

//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	explicit MainWindowPrivate( MainWindow * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init();

	//! Ui.
	Ui::MainWindow m_ui;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

void
MainWindowPrivate::init()
{
	m_ui.setupUi( q );

	QObject::connect( m_ui.m_btn, &QPushButton::clicked,
		q, &MainWindow::generate );
	QObject::connect( m_ui.m_copy, &QPushButton::clicked,
		q, &MainWindow::copy );
	QObject::connect( m_ui.m_digits, &QCheckBox::stateChanged,
		q, &MainWindow::checkbox );
	QObject::connect( m_ui.m_lower, &QCheckBox::stateChanged,
		q, &MainWindow::checkbox );
	QObject::connect( m_ui.m_upper, &QCheckBox::stateChanged,
		q, &MainWindow::checkbox );
	QObject::connect( m_ui.m_special, &QCheckBox::stateChanged,
		q, &MainWindow::checkbox );
}


//
// MainWindow
//

//! Main window.
MainWindow::MainWindow()
	:	d( new MainWindowPrivate( this ) )
{
	d->init();

	generate();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::generate()
{
	static std::random_device rd;
	static std::mt19937 gen( rd() );

	int upper = 0;
	int lower = 0;
	int digits = 0;
	int special = 0;

	{
		int length = d->m_ui.m_length->value();

		std::uniform_int_distribution< int > ud( 1, length / 3 );

		if( d->m_ui.m_upper->isChecked() )
			upper = ud( gen );

		std::uniform_int_distribution< int > ld( 1, length / 3 );

		if( d->m_ui.m_lower->isChecked() )
			lower = ld( gen );

		std::uniform_int_distribution< int > dd( 1, length / 3 );

		if( d->m_ui.m_digits->isChecked() )
			digits = dd( gen );

		std::uniform_int_distribution< int > sd( 1, length / 3 );

		if( d->m_ui.m_special->isChecked() )
			special = sd( gen );

		length -= ( upper + lower + digits + special );

		if( length > 0 )
		{
			if( lower > 0 )
				lower += length;
			else if( special > 0 )
				special += length;
			else if( digits > 0 )
				digits += length;
			else if( upper > 0 )
				upper += length;
		}
	}

	QList< QChar > data;

	static std::uniform_int_distribution< int > ud( 65, 90 );

	for( int i = 0; i < upper; ++i )
		data.append( QChar( ud( gen ) ) );

	static std::uniform_int_distribution< int > ld( 97, 122 );

	for( int i = 0; i < lower; ++i )
		data.append( QChar( ld( gen ) ) );

	static std::uniform_int_distribution< int > dd( 48, 57 );

	for( int i = 0; i < digits; ++i )
		data.append( QChar( dd( gen ) ) );

	static std::uniform_int_distribution< int > sd( 33, 47 );

	for( int i = 0; i < special; ++i )
		data.append( QChar( sd( gen ) ) );

	std::shuffle( data.begin(), data.end(), gen );

	QString phrase;

	for( int i = 0; i < data.size(); ++i )
		phrase.append( data.at( i ) );

	d->m_ui.m_phrase->setText( phrase );
}

void
MainWindow::checkbox( int )
{
	if( !d->m_ui.m_upper->isChecked() &&
		!d->m_ui.m_lower->isChecked() &&
		!d->m_ui.m_special->isChecked() &&
		!d->m_ui.m_digits->isChecked() )
			d->m_ui.m_btn->setEnabled( false );
	else
		d->m_ui.m_btn->setEnabled( true );
}

void
MainWindow::copy()
{
	QApplication::clipboard()->setText( d->m_ui.m_phrase->text() );
}

} /* namespace PhraseGen */
