
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

// Qt include.
#include <QApplication>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QClipboard>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QWindow>
#include <QMouseEvent>
#include <QCursor>

#include <QDebug>


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	MainWindowPrivate( MainWindow * parent )
		:	m_pressed( false )
		,	m_overrided( false )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! Capture.
	QPixmap capture();
	//! Copy image to clipboard.
	void copy( const QPixmap & p );
	//! Scale.
	void scale( QPainter & p, const QImage & img, int factor );

	//! Type of the handler.
	enum class HandlerType {
		TopLeft,
		Control,
		Top,
		TopRight,
		Right,
		BottomRight,
		Bottom,
		BottomLeft,
		Left,
		Unknown
	}; // enum class HandlerType

	//! \return Type of the handler by the pos.
	HandlerType handlerType( const QPoint & pos );

	//! Position of the mouse cursor.
	QPoint m_pos;
	//! Is mouse button pressed?
	bool m_pressed;
	//! Is cursor overrided?
	bool m_overrided;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

static const int c_dim = 20;
static const int c_delta = 1;
static const QSize c_minSize = QSize(
	c_dim * 5 + c_delta * 3, c_dim * 3 + c_delta * 5 );

void
MainWindowPrivate::init()
{
	q->setAutoFillBackground( false );

	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = q->windowHandle() )
		screen = window->screen();

	const int w = 150;
	const int h = 150;

	q->resize( w, h );

	if( screen )
	{
		q->move( screen->geometry().width() / 2 - w / 2,
			screen->geometry().height() / 2 - h / 2 );
	}

	q->setAttribute( Qt::WA_TranslucentBackground );

	q->setMouseTracking( true );

	q->setMinimumSize( c_minSize );

//	if( QSystemTrayIcon::isSystemTrayAvailable() )
//	{
//		m_sysTray = true;

//		QSystemTrayIcon * sysTray = new QSystemTrayIcon( q );

//		QIcon icon( ":/img/app_256x256.png" );
//		icon.addFile( ":/img/app_128x128.png" );
//		icon.addFile( ":/img/app_64x64.png" );
//		icon.addFile( ":/img/app_48x48.png" );
//		icon.addFile( ":/img/app_32x32.png" );
//		icon.addFile( ":/img/app_22x22.png" );
//		icon.addFile( ":/img/app_16x16.png" );

//		sysTray->setIcon( icon );

//		QMenu * ctx = new QMenu( q );

//		ctx->addAction( QIcon( ":/img/app_22x22.png" ),
//			MainWindow::tr( "Show" ), q, &MainWindow::showWindow );

//		ctx->addSeparator();

//		ctx->addAction( QIcon( ":/img/help-about.png" ),
//			MainWindow::tr( "About" ), q, &MainWindow::about );

//		ctx->addAction( QIcon( ":/img/help-contents.png" ),
//			MainWindow::tr( "Help" ), q, &MainWindow::help );

//		ctx->addAction( QIcon( ":/img/qt.png" ),
//			MainWindow::tr( "About Qt" ), q, &MainWindow::aboutQt );

//		ctx->addSeparator();

//		ctx->addAction( QIcon( ":/img/configure.png" ),
//			MainWindow::tr( "Settings" ), q, &MainWindow::settings );

//		ctx->addSeparator();

//		ctx->addAction(
//			QIcon( ":/img/application-exit.png" ),
//			MainWindow::tr( "Quit" ), QApplication::instance(),
//			&QApplication::quit );

//		sysTray->setContextMenu( ctx );

//		sysTray->show();
//	}
//	else
//		m_sysTray = false;
}

QPixmap
MainWindowPrivate::capture()
{
	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = q->windowHandle() )
		screen = window->screen();

	return ( screen ? screen->grabWindow( 0, q->pos().x(), q->pos().y(),
		q->width(), q->height() ) : QPixmap() );
}

void
MainWindowPrivate::copy( const QPixmap & p )
{
	QApplication::clipboard()->setPixmap( p );
}

void
MainWindowPrivate::scale( QPainter & p, const QImage & img, int factor )
{
	for( int i = 0; i < img.width(); ++i )
	{
		for( int j = 0; j < img.height(); ++j )
		{
			const QColor c = img.pixelColor( i, j );

			p.setPen( c );

			p.setBrush( c );

			p.drawRect( i * factor, j * factor, factor, factor );
		}
	}
}

MainWindowPrivate::HandlerType
MainWindowPrivate::handlerType( const QPoint & pos )
{
	if( QRect( c_delta, c_delta, c_dim, c_dim ).contains( pos ) )
		return HandlerType::TopLeft;
	else if( QRect( q->width() - c_dim - c_delta, c_delta, c_dim, c_dim )
				.contains( pos ) )
		return HandlerType::TopRight;
	else if( QRect( c_delta, q->height() - c_dim - c_delta, c_dim, c_dim )
				.contains( pos ) )
		return HandlerType::BottomLeft;
	else if( QRect( q->width() - c_dim - c_delta, q->height() - c_dim - c_delta,
			c_dim, c_dim ).contains( pos ) )
		return HandlerType::BottomRight;
	else if( QRect( c_delta, q->height() / 2 - c_dim / 2, c_dim, c_dim )
				.contains( pos ) )
		return HandlerType::Left;
	else if( QRect( q->width() / 2 - c_dim / 2, c_delta, c_dim, c_dim )
				.contains( pos ) )
		return HandlerType::Top;
	else if( QRect( q->width() / 2 - c_dim / 2, q->height() - c_dim - c_delta,
			c_dim, c_dim ).contains( pos ) )
		return HandlerType::Bottom;
	else if( QRect( q->width() - c_dim - c_delta, q->height() / 2 - c_dim / 2,
			c_dim, c_dim ).contains( pos ) )
		return HandlerType::Right;
	else if( QRect( c_dim + c_delta * 2, c_delta, c_dim, c_dim - c_delta * 4 )
				.contains( pos ) )
		return HandlerType::Control;
	else
		return HandlerType::Unknown;
}


//
// MainWindow
//

MainWindow::MainWindow()
	:	QWidget( Q_NULLPTR,
			Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint |
			Qt::WindowStaysOnTopHint )
	,	d( new MainWindowPrivate( this ) )
{
	d->init();
}

MainWindow::~MainWindow()
{
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

	dlg.setText( tr( "" ) );

	dlg.exec();
}

void
MainWindow::aboutQt()
{
	QMessageBox::aboutQt( this );
}

void
MainWindow::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	p.setPen( QPen( Qt::red, 2 ) );

	p.setBrush( Qt::NoBrush );

	p.drawRect( c_dim / 2 + c_delta, c_dim / 2 + c_delta,
		width() - c_dim - c_delta * 2, height() - c_dim - c_delta * 2 );

	p.setBrush( Qt::red );

	p.drawPie( QRect( c_delta, c_delta, c_dim, c_dim ),
		0, 270 * 16 );

	p.drawPie( QRect( width() - c_dim - c_delta, c_delta, c_dim, c_dim ),
		180 * 16, -270 * 16 );

	p.drawPie( QRect( c_delta, height() - c_dim - c_delta, c_dim, c_dim ),
		90 * 16, 270 * 16 );

	p.drawPie( QRect( width() - c_dim - c_delta,
		height() - c_dim - c_delta, c_dim, c_dim ),
		90 * 16, -270 * 16 );

	p.drawPie( QRect( c_delta, height() / 2 - c_dim / 2, c_dim, c_dim ),
		90 * 16, 180 * 16 );

	p.drawPie( QRect( width() / 2 - c_dim / 2, c_delta, c_dim, c_dim ),
		0, 180 * 16 );

	p.drawPie( QRect( width() / 2 - c_dim / 2, height() - c_dim - c_delta,
		c_dim, c_dim ),
		0, -180 * 16 );

	p.drawPie( QRect( width() - c_dim - c_delta, height() / 2 - c_dim / 2,
		c_dim, c_dim ),
		90 * 16, -180 * 16 );

	p.drawPie( QRect( c_dim + c_delta * 2, c_delta, c_dim, c_dim - c_delta * 4 ),
		0, 180 * 16 );
}

void
MainWindow::mousePressEvent( QMouseEvent * e )
{
	d->m_pos = e->pos();

	d->m_pressed = true;

	e->accept();
}

void
MainWindow::mouseMoveEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		const QPoint delta = e->pos() - d->m_pos;

		switch( d->handlerType( e->pos() ) )
		{
			case MainWindowPrivate::HandlerType::TopLeft :
			{
				const QSize s( size() - QSize( delta.x(), delta.y() ) );

				if( s.width() > c_minSize.width() ||
					s.height() > c_minSize.height() )
				{
					move( pos() + delta );

					resize( size() - QSize( delta.x(), delta.y() ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::TopRight :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::BottomLeft :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::BottomRight :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::Left :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::Top :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::Bottom :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::Right :
			{

			}
				break;

			case MainWindowPrivate::HandlerType::Control :
			{

			}
				break;

			default :
			{
				d->m_overrided = false;

				QApplication::restoreOverrideCursor();
			}
		}
	}
	else
	{
		switch( d->handlerType( e->pos() ) )
		{
			case MainWindowPrivate::HandlerType::TopLeft :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeFDiagCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::TopRight :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeBDiagCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::BottomLeft :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeBDiagCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::BottomRight :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeFDiagCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Left :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeHorCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Top :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeVerCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Bottom :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeVerCursor ) );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Right :
			{
				if( !d->m_overrided )
				{
					d->m_overrided = true;

					QApplication::setOverrideCursor(
						QCursor( Qt::SizeHorCursor ) );
				}
			}
				break;

			default :
			{
				if( d->m_overrided )
				{
					d->m_overrided = false;

					QApplication::restoreOverrideCursor();
				}
			}
		}
	}

	e->accept();
}

void
MainWindow::mouseReleaseEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		if( d->handlerType( e->pos() ) ==
			MainWindowPrivate::HandlerType::Control )
		{

		}

		d->m_pressed = false;
	}

	e->accept();
}

void
MainWindow::leaveEvent( QEvent * event )
{
	if( d->m_overrided )
	{
		d->m_overrided = false;

		QApplication::restoreOverrideCursor();
	}

	event->accept();
}
