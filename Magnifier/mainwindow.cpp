
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

// Magnifier include.
#include "mainwindow.hpp"
#include "constants.hpp"
#include "zoomwindow.hpp"

// Qt include.
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QPixmap>
#include <QScreen>
#include <QPainter>
#include <QImage>
#include <QDir>
#include <QWindow>
#include <QMouseEvent>
#include <QCursor>
#include <QEventLoop>
#include <QTimer>
#include <QContextMenuEvent>

#include <QDebug>


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	explicit MainWindowPrivate( MainWindow * parent )
		:	m_pressed( false )
		,	m_overrided( false )
		,	q( parent )
	{
	}

	//! Init.
	void init();
	//! Capture.
	QPixmap capture();
	//! Scale.
	void scale( QPainter & p, const QImage & img, int factor );
    //! Show menu.
	void showMenu( const QPoint & pos );

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
	//! Move delta.
	QPoint m_moveDelta;
	//! Parent.
	MainWindow * q;
}; // class MainWindowPrivate

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
}

QPixmap
MainWindowPrivate::capture()
{
	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = q->windowHandle() )
		screen = window->screen();

	return ( screen ? screen->grabWindow( 0,
			q->pos().x() + c_windowOffset, q->pos().y() + c_windowOffset,
			q->width() - c_windowOffset * 2, q->height() - c_windowOffset * 2 ) :
		QPixmap() );
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

void
MainWindowPrivate::showMenu( const QPoint & pos )
{
	QMenu menu( q );

	menu.addAction( QIcon( ":/img/zoom-in.png" ),
		MainWindow::tr( "2x" ), q, &MainWindow::x2 );

	menu.addAction( QIcon( ":/img/zoom-in.png" ),
		MainWindow::tr( "3x" ), q, &MainWindow::x3 );

	menu.addAction( QIcon( ":/img/zoom-in.png" ),
		MainWindow::tr( "5x" ), q, &MainWindow::x5 );

	menu.addSeparator();

	menu.addAction( QIcon( ":/img/help-about.png" ),
		MainWindow::tr( "About" ), q, &MainWindow::about );

	menu.addAction( QIcon( ":/img/help-contents.png" ),
		MainWindow::tr( "Help" ), q, &MainWindow::help );

	menu.addAction( QIcon( ":/img/qt.png" ),
		MainWindow::tr( "About Qt" ), q, &MainWindow::aboutQt );

	menu.addSeparator();

	menu.addAction(
		QIcon( ":/img/application-exit.png" ),
		MainWindow::tr( "Quit" ), QApplication::instance(),
		&QApplication::quit );

	menu.exec( pos );
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

	dlg.setText( tr( "Select a region to magnify. "
		"Launch menu and select zoom action you need." ) );

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
	if( e->button() == Qt::LeftButton )
	{
		d->m_pos = e->pos();

		d->m_pressed = true;

		d->m_moveDelta = QPoint( 0, 0 );
	}

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

					resize( s );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::TopRight :
			{
				const QSize s( size() + QSize( delta.x(), -delta.y() ) );

				d->m_pos = QPoint( e->pos().x(), d->m_pos.y() );

				if( s.width() > c_minSize.width() ||
					s.height() > c_minSize.height() )
				{
					move( pos().x(), pos().y() + delta.y() );

					resize( s );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::BottomLeft :
			{
				const QSize s( size() + QSize( -delta.x(), delta.y() ) );

				d->m_pos = QPoint( d->m_pos.x(), e->pos().y() );

				if( s.width() > c_minSize.width() ||
					s.height() > c_minSize.height() )
				{
					move( pos().x() + delta.x(), pos().y() );

					resize( s );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::BottomRight :
			{
				const QSize s( size() + QSize( delta.x(), delta.y() ) );

				d->m_pos = e->pos();

				if( s.width() > c_minSize.width() ||
					s.height() > c_minSize.height() )
				{
					resize( s );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Left :
			{
				const int w = width() - delta.x();

				if( w > c_minSize.width() )
				{
					move( pos().x() + delta.x(), pos().y() );

					resize( w, height() );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Top :
			{
				const int h = height() - delta.y();

				if( h > c_minSize.height() )
				{
					move( pos() + QPoint( 0, delta.y() ) );

					resize( width(), h );
				}
			}
				break;

			case MainWindowPrivate::HandlerType::Bottom :
			{
				const int h = height() + delta.y();

				d->m_pos = QPoint( d->m_pos.x(), e->pos().y() );

				if( h > c_minSize.height() )
					resize( width(), h );
			}
				break;

			case MainWindowPrivate::HandlerType::Right :
			{
				const int w = width() + delta.x();

				d->m_pos = QPoint( e->pos().x(), d->m_pos.y() );

				if( w > c_minSize.width() )
					resize( w, height() );
			}
				break;

			case MainWindowPrivate::HandlerType::Control :
			{
				d->m_moveDelta += QPoint( qAbs( delta.x() ), qAbs( delta.y() ) );

				move( pos() + delta );
			}
				break;

			default :
				break;
		}
	}
	else
	{
		switch( d->handlerType( e->pos() ) )
		{
			case MainWindowPrivate::HandlerType::BottomRight :
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

			case MainWindowPrivate::HandlerType::Right :
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
			MainWindowPrivate::HandlerType::Control &&
				d->m_moveDelta.manhattanLength() <= 3 )
		{
			d->showMenu( e->globalPos() );
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

void
MainWindow::contextMenuEvent( QContextMenuEvent * e )
{
	d->showMenu( e->globalPos() );

	e->accept();
}

void
MainWindow::x2()
{
	QEventLoop loop;
	QTimer::singleShot( 1000, &loop, &QEventLoop::quit );
	loop.exec();

	QApplication::processEvents();

	const QPixmap tmp = d->capture();

	QPixmap pixmap( tmp.width() * 2, tmp.height() * 2 );

	QPainter p( &pixmap );

	d->scale( p, tmp.toImage(), 2 );

	ZoomWindow * w = new ZoomWindow( pixmap );

	w->move( pos() - QPoint( c_dim, c_dim ) );

	w->show();
}

void
MainWindow::x3()
{
	QEventLoop loop;
	QTimer::singleShot( 1000, &loop, &QEventLoop::quit );
	loop.exec();

	QApplication::processEvents();

	const QPixmap tmp = d->capture();

	QPixmap pixmap( tmp.width() * 3, tmp.height() * 3 );

	QPainter p( &pixmap );

	d->scale( p, tmp.toImage(), 3 );

	ZoomWindow * w = new ZoomWindow( pixmap );

	w->move( pos() - QPoint( c_dim, c_dim ) );

	w->show();
}

void
MainWindow::x5()
{
	QEventLoop loop;
	QTimer::singleShot( 1000, &loop, &QEventLoop::quit );
	loop.exec();

	QApplication::processEvents();

	const QPixmap tmp = d->capture();

	QPixmap pixmap( tmp.width() * 5, tmp.height() * 5 );

	QPainter p( &pixmap );

	d->scale( p, tmp.toImage(), 5 );

	ZoomWindow * w = new ZoomWindow( pixmap );

	w->move( pos() - QPoint( c_dim, c_dim ) );

	w->show();
}
