
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

// LicenseChange include.
#include "mainwindow.hpp"
#include "textedit.hpp"
#include "model.hpp"

// Qt include.
#include <QApplication>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QLineEdit>
#include <QDir>
#include <QProgressBar>
#include <QToolButton>
#include <QVariantAnimation>


//
// DirectoryLoadingProgress
//

DirectoryLoadingProgress::DirectoryLoadingProgress( QWidget * parent )
	:	QWidget( parent )
	,	m_progress( Q_NULLPTR )
	,	m_anim( Q_NULLPTR )
{
	QHBoxLayout * l = new QHBoxLayout( this );

	m_progress = new QProgressBar( this );
	m_progress->setMinimum( 0 );
	m_progress->setMaximum( 100 );
	l->addWidget( m_progress );

	m_anim = new QVariantAnimation( this );
	m_anim->setKeyValueAt( 0.0, 0 );
	m_anim->setKeyValueAt( 0.5, 75 );
	m_anim->setKeyValueAt( 1.0, 0 );
	m_anim->setDuration( 1500 );
	m_anim->setLoopCount( -1 );

	QToolButton * b = new QToolButton( this );
	b->setIconSize( QSize( 16, 16 ) );
	b->setIcon( QIcon( ":/img/dialog-cancel.png" ) );
	l->addWidget( b );

	connect( b, &QToolButton::clicked,
		this, &DirectoryLoadingProgress::cancel );
	connect( m_anim, &QVariantAnimation::valueChanged,
		this, &DirectoryLoadingProgress::animValueChanged );

}

DirectoryLoadingProgress::~DirectoryLoadingProgress()
{
}

void
DirectoryLoadingProgress::start()
{
	m_anim->start();

	show();
}

void
DirectoryLoadingProgress::finish()
{
	m_anim->stop();

	hide();
}

void
DirectoryLoadingProgress::cancel()
{
	finish();

	emit cancelLoading();
}

void
DirectoryLoadingProgress::animValueChanged( const QVariant & value )
{
	m_progress->setValue( value.toInt() );
}


//
// CentralWidget
//

class CentralWidget
	:	public QWidget
{
public:
	CentralWidget( QWidget * parent = Q_NULLPTR )
		:	QWidget( parent )
		,	m_view( Q_NULLPTR )
		,	m_oldLicense( Q_NULLPTR )
		,	m_newLicense( Q_NULLPTR )
		,	m_filter( Q_NULLPTR )
		,	m_model( Q_NULLPTR )
		,	m_progress( Q_NULLPTR )
	{
		init();
	}

	~CentralWidget()
	{
	}

private:
	//! Init.
	void init()
	{
		QHBoxLayout * l = new QHBoxLayout( this );
		l->setSpacing( 0 );

		QWidget * w1 = new QWidget( this );

		QVBoxLayout * vb1 = new QVBoxLayout( w1 );
		vb1->setContentsMargins( 0, 0, 0, 0 );

		m_filter = new QLineEdit( w1 );
		m_filter->setText( tr( "*.c *.h *.cpp *.hpp *.cc *.cxx *.hxx" ) );
		vb1->addWidget( m_filter );

		m_view = new QTreeView( w1 );
		vb1->addWidget( m_view );

		m_model = new Model( this );
		m_model->setNameFilters( QStringList() << "*.c"
			<< "*.h" << "*.cpp" << "*.hpp" << "*.cc" << "*.cxx" << "*.hxx" );
		m_model->setRootPath( QDir::rootPath() );
		m_model->setReadOnly( true );

		m_view->setModel( m_model );

		m_progress = new DirectoryLoadingProgress( w1 );
		vb1->addWidget( m_progress );
		m_progress->finish();

		l->addWidget( w1 );

		QSplitter * sp1 = new QSplitter( Qt::Horizontal, this );

		l->addWidget( sp1 );

		QWidget * w2 = new QWidget( this );

		QVBoxLayout * vb2 = new QVBoxLayout( w2 );
		vb2->setContentsMargins( 0, 0, 0, 0 );

		m_oldLicense = new TextEdit( w2 );
		m_oldLicense->setPlaceholderText( tr( "Put old license here. "
			"You can use regular expressions here, to insert regular expression "
			"use tool bar." ) );

		vb2->addWidget( m_oldLicense );

		QSplitter * sp2 = new QSplitter( Qt::Vertical, w2 );

		vb2->addWidget( sp2 );

		m_newLicense = new TextEdit( w2 );
		m_newLicense->setPlaceholderText( tr( "Insert new license here." ) );

		vb2->addWidget( m_newLicense );

		sp2->addWidget( m_oldLicense );
		sp2->addWidget( m_newLicense );

		l->addWidget( w2 );

		sp1->addWidget( w1 );
		sp1->addWidget( w2 );

		connect( m_progress, &DirectoryLoadingProgress::cancelLoading,
			m_model, &Model::stopLoading );
		connect( m_model, &Model::loadingStarted,
			m_progress, &DirectoryLoadingProgress::start );
		connect( m_model, &Model::loadingFinished,
			m_progress, &DirectoryLoadingProgress::finish );
	}

public:
	//! Tree view.
	QTreeView * m_view;
	//! Old license.
	TextEdit * m_oldLicense;
	//! New license.
	TextEdit * m_newLicense;
	//! Filter.
	QLineEdit * m_filter;
	//! Model.
	Model * m_model;
	//! Progress.
	DirectoryLoadingProgress * m_progress;
}; // class CentralWidget


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_centralWidget( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Central widget.
	CentralWidget * m_centralWidget;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

void
MainWindowPrivate::init()
{
	q->menuBar()->addMenu( MainWindow::tr( "&File" ) )->addAction(
		QIcon( ":/img/application-exit.png" ), MainWindow::tr( "Quit" ),
			qApp, SLOT( quit() ), MainWindow::tr( "Ctrl+Q" ) );

	m_centralWidget = new CentralWidget( q );

	q->setCentralWidget( m_centralWidget );
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
