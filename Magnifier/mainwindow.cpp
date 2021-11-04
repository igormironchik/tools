
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
#include <QWheelEvent>


//
// MainWindowPrivate
//

class MainWindowPrivate {
public:
	explicit MainWindowPrivate( MainWindow * parent )
		:	m_pressed( false )
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
	void showMenu( const QPointF & pos );

	//! Position of the mouse cursor.
	QPoint m_pos;
	//! Move delta.
	QPoint m_moveDelta;
	//! Is mouse button pressed?
	bool m_pressed;
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

	const int w = c_startSize.width();
	const int h = c_startSize.height();

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
			q->pos().x() + 2, q->pos().y() + 2,
			q->width() - 4, q->height() - 4 ) :
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

void
MainWindowPrivate::showMenu( const QPointF & pos )
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

	menu.exec( pos.toPoint() );
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

	dlg.setWindowTitle( tr( "Help of Magnifier" ) );
	dlg.setText( tr( "Select a region to magnify. "
		"Launch menu and select zoom action you need.\n"
		"You can use scrolling to resize region.\n"
		"Use scrolling with Shift and Control "
		"to resize in one direction." ) );

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

	p.drawRect( 1, 1, width() - 2, height() - 2 );
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

		d->m_moveDelta += QPoint( qAbs( delta.x() ), qAbs( delta.y() ) );

		move( pos() + delta );
	}

	e->accept();
}

void
MainWindow::mouseReleaseEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		if( d->m_moveDelta.manhattanLength() <= 3 )
			d->showMenu( e->globalPosition() );

		d->m_pressed = false;
	}

	e->accept();
}

void
MainWindow::contextMenuEvent( QContextMenuEvent * e )
{
	d->showMenu( e->globalPos() );

	e->accept();
}

void
MainWindow::wheelEvent( QWheelEvent * e )
{
	const bool up = e->angleDelta().y() > 0;

	QScreen * screen = QApplication::primaryScreen();

	if( const QWindow * window = windowHandle() )
		screen = window->screen();

	const int maxWidth = screen->availableSize().width();
	const int maxHeight = screen->availableSize().height();
	const int minWidth = c_minSize.width();
	const int minHeight = c_minSize.height();

	if( up )
	{
		QPoint p = pos();

		if( p.x() > 0 && !( e->modifiers() & Qt::ShiftModifier ) )
			p.setX( p.x() - 1 );

		if( p.y() > 0 && !( e->modifiers() & Qt::ControlModifier ) )
			p.setY( p.y() - 1 );

		move( p );

		QSize s = size();

		if( s.width() < maxWidth && !( e->modifiers() & Qt::ShiftModifier )  )
			s.setWidth( s.width() + 2 );

		if( s.height() < maxHeight && !( e->modifiers() & Qt::ControlModifier ) )
			s.setHeight( s.height() + 2 );

		resize( s );
	}
	else
	{
		QPoint p = pos();

		QSize s = size();

		if( s.width() > minWidth && !( e->modifiers() & Qt::ShiftModifier ) )
		{
			p.setX( p.x() + 1 );
			s.setWidth( s.width() - 2 );
		}

		if( s.height() > minHeight && !( e->modifiers() & Qt::ControlModifier ) )
		{
			p.setY( p.y() + 1 );
			s.setHeight( s.height() - 2 );
		}

		move( p );
		resize( s );
	}

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

	w->move( pos() );

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

	w->move( pos() );

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

	w->move( pos() );

	w->show();
}
