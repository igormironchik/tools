
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

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
#include "cfg.hpp"
#include "opts_dialog.hpp"

// Qt include.
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QStatusBar>
#include <QStandardPaths>
#include <QFileInfo>
#include <QPixmap>
#include <QScreen>
#include <QClipboard>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QWindow>
#include <QSystemTrayIcon>

// QtConfFile include.
#include <QtConfFile/Utils>
#include <QtConfFile/Exceptions>


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_sysTray( false )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! Read cfg.
	void readCfg();
	//! Save cfg.
	void saveCfg();
	//! Capture.
	void capture();
	//! Copy to clipboard.
	void copy();
	//! Scale.
	void scale( QPainter & p );

	//! Cfg.
	Cfg m_cfg;
	//! Pixmap.
	QPixmap m_pixmap;
	//! Image.
	QImage m_img;
	//! Position of the mouse cursor.
	QPoint m_pos;
	//! Is system tray available.
	bool m_sysTray;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

static const QString c_appCfgFileName = QLatin1String( "/Magnifier.cfg" );

void
MainWindowPrivate::init()
{
	readCfg();

	q->resize( m_cfg.width(), m_cfg.height() );

	if( QSystemTrayIcon::isSystemTrayAvailable() )
	{
		m_sysTray = true;

		QSystemTrayIcon * sysTray = new QSystemTrayIcon( q );

		QIcon icon( ":/img/app_256x256.png" );
		icon.addFile( ":/img/app_128x128.png" );
		icon.addFile( ":/img/app_64x64.png" );
		icon.addFile( ":/img/app_48x48.png" );
		icon.addFile( ":/img/app_32x32.png" );
		icon.addFile( ":/img/app_22x22.png" );
		icon.addFile( ":/img/app_16x16.png" );

		sysTray->setIcon( icon );

		QMenu * ctx = new QMenu( q );

		ctx->addAction( QIcon( ":/img/app_22x22.png" ),
			MainWindow::tr( "Show" ), q, &MainWindow::showWindow );

		ctx->addSeparator();

		ctx->addAction( QIcon( ":/img/help-about.png" ),
			MainWindow::tr( "About" ), q, &MainWindow::about );

		ctx->addAction( QIcon( ":/img/help-contents.png" ),
			MainWindow::tr( "Help" ), q, &MainWindow::help );

		ctx->addAction( QIcon( ":/img/qt.png" ),
			MainWindow::tr( "About Qt" ), q, &MainWindow::aboutQt );

		ctx->addSeparator();

		ctx->addAction( QIcon( ":/img/configure.png" ),
			MainWindow::tr( "Settings" ), q, &MainWindow::settings );

		ctx->addSeparator();

		ctx->addAction(
			QIcon( ":/img/application-exit.png" ),
			MainWindow::tr( "Quit" ), QApplication::instance(),
			&QApplication::quit );

		sysTray->setContextMenu( ctx );

		sysTray->show();
	}
	else
		m_sysTray = false;
}

void
MainWindowPrivate::readCfg()
{
	static const QString cfgFolder =
		QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

	const QString cfgFileName = cfgFolder + c_appCfgFileName;

	if( QFileInfo::exists( cfgFileName ) )
	{
		try {
			TagCfg tag;

			QtConfFile::readQtConfFile( tag, cfgFileName,
				QTextCodec::codecForName( "UTF-8" ) );

			m_cfg = tag.getCfg();
		}
		catch( const QtConfFile::Exception & )
		{
		}
	}
}

void
MainWindowPrivate::saveCfg()
{
	static const QString cfgFolder =
		QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation );

	const QString cfgFileName = cfgFolder + c_appCfgFileName;

	QFileInfo info( cfgFileName );

	QDir dir( info.path() );

	if( !dir.exists() )
	{
		if( !dir.mkpath( info.path() ) )
		{
			QMessageBox::warning( q,
				MainWindow::tr( "Unable to create folder..." ),
				MainWindow::tr( "Unable to create folder for the configuration files. "
					"Path: \"%1\"." ).arg( info.path() ) );

			return;
		}
	}

	try {
		TagCfg tag( m_cfg );

		QtConfFile::writeQtConfFile( tag, cfgFileName,
			QTextCodec::codecForName( "UTF-8" ) );
	}
	catch( const QtConfFile::Exception & x )
	{
		QMessageBox::warning( q,
			MainWindow::tr( "Unable to Save Configuration..." ),
			MainWindow::tr( "Unable to save configuration.\n"
				"%1" ).arg( x.whatAsQString() ) );
	}
}

void
MainWindowPrivate::capture()
{
	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = q->windowHandle() )
		screen = window->screen();

	if( !screen )
		return;

	const int x = q->pos().x();
	const int y = q->pos().y();

	m_pixmap = screen->grabWindow( 0, 100, 100,
		m_cfg.width() / m_cfg.scale() + 1,
		m_cfg.height() / m_cfg.scale() + 1 );

	m_pixmap.save( "tmp.png" );

//	QPixmap p = screen->grabWindow( 0, x, y,
//		m_cfg.width() / m_cfg.scale() + 1,
//		m_cfg.height() / m_cfg.scale() + 1 );

//	p.save( "tmp.png" );

	m_img = m_pixmap.toImage();
}

void
MainWindowPrivate::copy()
{
	QPixmap pixmap( m_img.width() * m_cfg.scale(),
		m_img.height() * m_cfg.height() );

	QPainter p( &pixmap );

	scale( p );

	QApplication::clipboard()->setPixmap( pixmap );
}

void
MainWindowPrivate::scale( QPainter & p )
{
	const int s = m_cfg.scale();

	for( int i = 0; i < m_img.width(); ++i )
	{
		for( int j = 0; j < m_img.height(); ++j )
		{
			const QColor c = m_img.pixelColor( i, j );

			p.setPen( c );

			p.setBrush( c );

			p.drawRect( i * s, j * s, s, s );
		}
	}
}


//
// MainWindow
//

MainWindow::MainWindow()
	:	QWidget( Q_NULLPTR, Qt::FramelessWindowHint )
	,	d( new MainWindowPrivate( this ) )
{
	d->init();
}

MainWindow::~MainWindow()
{
}

void
MainWindow::showWindow()
{
	show();
}

void
MainWindow::about()
{
	QMessageBox::about( this, tr( "About Magnifier" ),
		tr( "Magnifier.\n\n"
			"Author - Igor Mironchik (igor.mironchik at gmail dot com).\n\n"
			"Copyright (c) 2017 Igor Mironchik.\n\n"
			"Licensed under GNU GPL 3.0." ) );
}

void
MainWindow::help()
{
	QMessageBox dlg( this );

	dlg.setText( tr( "To hide Magnifier just press any key on keyboard. "
		"To move Magnifier drag window. On hide event scaled screenshot will be "
		"available in the system clipboard." ) );

	dlg.exec();
}

void
MainWindow::aboutQt()
{
	QMessageBox::aboutQt( this );
}

void
MainWindow::settings()
{
	OptsDialog dlg( d->m_cfg, this );

	if( QDialog::Accepted == dlg.exec() )
	{
		resize( d->m_cfg.width(), d->m_cfg.height() );

		d->capture();

		update();

		d->saveCfg();
	}
}

void
MainWindow::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	d->scale( p );

	p.setPen( Qt::black );

	p.drawRect( 0, 0, width() - 1, height() - 1 );
}

void
MainWindow::keyPressEvent( QKeyEvent * e )
{
	d->capture();

	d->copy();

	hide();

	if( !d->m_sysTray )
		QApplication::quit();

	e->accept();
}

void
MainWindow::mousePressEvent( QMouseEvent * e )
{
	d->m_pos = e->pos();

	e->accept();
}

void
MainWindow::mouseMoveEvent( QMouseEvent * e )
{
	const QPoint delta = e->pos() - d->m_pos;

	move( pos() + delta );

	e->accept();
}

void
MainWindow::showEvent( QShowEvent * e )
{
	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = windowHandle() )
		screen = window->screen();

	e->accept();

	if( !screen )
		return;

	move( screen->geometry().width() / 2 - d->m_cfg.width() / 2,
		screen->geometry().height() / 2 - d->m_cfg.height() / 2 );
}

void
MainWindow::moveEvent( QMoveEvent * e )
{
	d->capture();

	update();

	e->accept();
}
