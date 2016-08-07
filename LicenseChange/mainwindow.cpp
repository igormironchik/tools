
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
#include "proxy.hpp"

// Qt include.
#include <QApplication>
#include <QMenuBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>
#include <QLineEdit>
#include <QDir>
#include <QFileSystemModel>
#include <QToolBar>
#include <QAction>
#include <QTextDocument>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>


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
		,	m_proxy( Q_NULLPTR )
		,	m_model( Q_NULLPTR )
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

		m_model = new QFileSystemModel( this );
		m_model->setNameFilters( QStringList() << "*.c"
			<< "*.h" << "*.cpp" << "*.hpp" << "*.cc" << "*.cxx" << "*.hxx" );
		m_model->setRootPath( QDir::rootPath() );
		m_model->setReadOnly( true );

		m_proxy = new CheckableProxyModel( this );
		m_proxy->setSourceModel( m_model );

		m_view->setModel( m_proxy );

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
		m_newLicense->setPlaceholderText( tr( "Put new license here." ) );

		vb2->addWidget( m_newLicense );

		sp2->addWidget( m_oldLicense );
		sp2->addWidget( m_newLicense );

		l->addWidget( w2 );

		sp1->addWidget( w1 );
		sp1->addWidget( w2 );
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
	//! Proxy model.
	CheckableProxyModel * m_proxy;
	//! Files model.
	QFileSystemModel * m_model;
}; // class CentralWidget


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_centralWidget( Q_NULLPTR )
		,	m_run( Q_NULLPTR )
		,	m_regexp( Q_NULLPTR )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Central widget.
	CentralWidget * m_centralWidget;
	//! Run action.
	QAction * m_run;
	//! Insert regexp action.
	QAction * m_regexp;
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

	QToolBar * tool = new QToolBar( MainWindow::tr( "Tools" ), q );

	m_run = tool->addAction( QIcon( ":/img/arrow-right.png" ),
		MainWindow::tr( "Run" ), q, &MainWindow::run );
	m_run->setShortcutContext( Qt::ApplicationShortcut );
	m_run->setShortcut( QKeySequence( "Ctrl+R" ) );
	m_run->setEnabled( false );

	m_regexp = tool->addAction( QIcon( ":/img/code-function.png" ),
		MainWindow::tr( "Insert Regular Expression" ),
		q, &MainWindow::insertRegexp );
	m_regexp->setShortcutContext( Qt::ApplicationShortcut );
	m_regexp->setShortcut( QKeySequence( "Ctrl+X" ) );
	m_regexp->setEnabled( false );

	q->addToolBar( Qt::TopToolBarArea, tool );

	MainWindow::connect( m_centralWidget->m_filter, &QLineEdit::textChanged,
		q, &MainWindow::nameFiltersChanged );
	MainWindow::connect( m_centralWidget->m_oldLicense, &TextEdit::focusReceived,
		q, &MainWindow::oldLicenseReceivedFocus );
	MainWindow::connect( m_centralWidget->m_oldLicense, &TextEdit::focusLost,
		q, &MainWindow::oldLicenseLostFocus );
	MainWindow::connect( m_centralWidget->m_oldLicense, &TextEdit::textChanged,
		q, &MainWindow::oldLicenseChanged );
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

void
MainWindow::run()
{
	const QStringList files = checkedFiles();
}

QStringList
MainWindow::checkedFiles()
{
	QScopedPointer< CheckableProxyModelState > state(
		d->m_centralWidget->m_proxy->checkedState() );

	QModelIndexList lindexes;

	state->checkedLeafSourceModelIndexes( lindexes );

	QModelIndexList bindexes;

	state->checkedBranchSourceModelIndexes( bindexes );

	QModelIndexList ulidx;

	state->uncheckedLeafSourceModelIndexes( ulidx );

	QModelIndexList ubidx;

	state->uncheckedBranchSourceModelIndexes( ubidx );

	QStringList ubpath;

	foreach( const QModelIndex & i, ubidx )
		ubpath.append( d->m_centralWidget->m_model->filePath( i ) );

	const QStringList filter = d->m_centralWidget->m_filter->text()
		.split( QLatin1Char( ' ' ), QString::SkipEmptyParts );

	QStringList res;

	foreach( const QModelIndex & i, bindexes )
		checkedFiles( d->m_centralWidget->m_model->filePath( i ),
			res, bindexes, lindexes, ubpath, ulidx, filter );

	QList< QRegExp > regexp;

	foreach( const QString & f, filter )
	{
		QRegExp r;
		r.setPatternSyntax( QRegExp::Wildcard );
		r.setPattern( f );

		regexp.append( r );
	}

	foreach( const QModelIndex & i, lindexes )
	{
		const QString file = d->m_centralWidget->m_model->filePath( i );
		const QFileInfo info( file );

		foreach( const QRegExp & r, regexp )
		{
			if( r.exactMatch( info.fileName() ) )
				res.append( file );
		}
	}

	foreach( const QModelIndex & i, ulidx )
		res.removeOne( d->m_centralWidget->m_model->filePath( i ) );

	return res;
}

void
MainWindow::checkedFiles( const QString & path, QStringList & res,
	const QModelIndexList & cb, const QModelIndexList & cl,
	const QStringList & ub, const QModelIndexList & ul,
	const QStringList & filter )
{
	QDir dir( path );

	const QFileInfoList names = dir.entryInfoList( filter,
		QDir::NoDotAndDotDot | QDir::AllEntries );

	foreach( const QFileInfo & i, names )
	{
		if( i.isDir() )
		{
			const QString p = i.absolutePath();

			if( !ub.contains( p ) )
				checkedFiles( p, res, cb, cl, ub, ul, filter );
		}
		else
			res.append( i.absoluteFilePath() );
	}
}

void
MainWindow::insertRegexp()
{

}

void
MainWindow::nameFiltersChanged( const QString & filter )
{
	const QStringList names = filter.split( QLatin1Char( ' ' ),
		QString::SkipEmptyParts );

	d->m_centralWidget->m_model->setNameFilters( names );
}

void
MainWindow::oldLicenseReceivedFocus()
{
	d->m_regexp->setEnabled( true );
}

void
MainWindow::oldLicenseLostFocus()
{
	d->m_regexp->setEnabled( false );
}

void
MainWindow::oldLicenseChanged()
{
	enableDisableRunButton();
}

void
MainWindow::enableDisableRunButton()
{
	if( !d->m_centralWidget->m_oldLicense->document()->isEmpty() )
		d->m_run->setEnabled( true );
	else
		d->m_run->setEnabled( false );
}
