
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
#include "zoomwindow.hpp"
#include "constants.hpp"

// Qt include.
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QClipboard>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>

#include <QDebug>


//
// ZoomWindowPrivate
//

class ZoomWindowPrivate {
public:
	ZoomWindowPrivate( const QPixmap & p, ZoomWindow * parent )
		:	m_pixmap( p )
		,	m_pressed( false )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Show context menu.
	void showMenu( const QPoint & pos );

	//! Pixmap.
	QPixmap m_pixmap;
	//! Position of the mouse cursor.
	QPoint m_pos;
	//! Move delta.
	QPoint m_moveDelta;
	//! Is mouse button pressed?
	bool m_pressed;
	//! Parent.
	ZoomWindow * q;
}; // class ZoomWindowPrivate

void
ZoomWindowPrivate::init()
{
	q->setAutoFillBackground( false );

	q->resize( q->sizeHint() );

	q->setAttribute( Qt::WA_TranslucentBackground );
}

void
ZoomWindowPrivate::showMenu( const QPoint & pos )
{
	QMenu menu( q );

	menu.addAction( QIcon( ":/img/document-save.png" ),
		ZoomWindow::tr( "Save As" ), q, &ZoomWindow::save );

	menu.addAction( QIcon( ":/img/edit-copy.png" ),
		ZoomWindow::tr( "Copy to Clipboard" ), q,
		&ZoomWindow::copy );

	menu.addSeparator();

	menu.addAction( QIcon( ":/img/dialog-close.png" ),
		ZoomWindow::tr( "Close Window" ), q, &QWidget::close );

	menu.addSeparator();

	menu.addAction(
		QIcon( ":/img/application-exit.png" ),
		ZoomWindow::tr( "Quit" ), QApplication::instance(),
		&QApplication::quit );

	menu.exec( pos );
}


//
// ZoomWindow
//

ZoomWindow::ZoomWindow( const QPixmap & p, QWidget * parent )
	:	QWidget( parent,
			Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint |
			Qt::WindowStaysOnTopHint )
	,	d( new ZoomWindowPrivate( p, this ) )
{
	d->init();
}

ZoomWindow::~ZoomWindow()
{
}

QSize
ZoomWindow::sizeHint() const
{
	return QSize( d->m_pixmap.width() + 4, d->m_pixmap.height() + 4 );
}

void
ZoomWindow::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	p.setClipRect( QRect( 0, 0, sizeHint().width(), sizeHint().height() ) );

	p.drawPixmap( 2, 2, d->m_pixmap.width(),
		d->m_pixmap.height(), d->m_pixmap );

	p.setPen( QPen( Qt::darkGreen, 2 ) );

	p.setBrush( Qt::NoBrush );

	p.drawRect( 1, 1, sizeHint().width() - 2, sizeHint().height() - 2 );
}

void
ZoomWindow::mousePressEvent( QMouseEvent * e )
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
ZoomWindow::mouseMoveEvent( QMouseEvent * e )
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
ZoomWindow::mouseReleaseEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		if( d->m_moveDelta.manhattanLength() <= 3 )
			d->showMenu( e->globalPos() );

		d->m_pressed = false;
	}

	e->accept();
}

void
ZoomWindow::closeEvent( QCloseEvent * e )
{
	deleteLater();

	e->accept();
}

void
ZoomWindow::contextMenuEvent( QContextMenuEvent * e )
{
	d->showMenu( e->globalPos() );

	e->accept();
}

void
ZoomWindow::save()
{
	QString fileName =
		QFileDialog::getSaveFileName( this, tr( "Select File Name..." ),
			QString(), tr( "PNG (*.png)" ) );

	if( !fileName.isEmpty() )
	{
		if( !fileName.endsWith( QLatin1String( ".png" ), Qt::CaseInsensitive ) )
			fileName.append( QLatin1String( ".png" ) );

		d->m_pixmap.save( fileName );
	}
}

void
ZoomWindow::copy()
{
	QApplication::clipboard()->setPixmap( d->m_pixmap );
}
