
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

	//! Type of the handler.
	enum class HandlerType {
		Control,
		Unknown
	}; // enum class HandlerType

	//! \return Type of the handler by the pos.
	HandlerType handlerType( const QPoint & pos );

	//! Pixmap.
	QPixmap m_pixmap;
	//! Position of the mouse cursor.
	QPoint m_pos;
	//! Is mouse button pressed?
	bool m_pressed;
	//! Move delta.
	QPoint m_moveDelta;
	//! Parent.
	ZoomWindow * q;
}; // class ZoomWindowPrivate

void
ZoomWindowPrivate::init()
{
	q->setAutoFillBackground( false );

	q->resize( m_pixmap.width() + c_windowOffset * 2,
		m_pixmap.height() + c_windowOffset * 2 );

	q->setAttribute( Qt::WA_TranslucentBackground );
}

ZoomWindowPrivate::HandlerType
ZoomWindowPrivate::handlerType( const QPoint & pos )
{
	if( QRect( c_dim + c_delta * 2, c_delta, c_dim, c_dim - c_delta * 4 )
				.contains( pos ) )
		return HandlerType::Control;
	else
		return HandlerType::Unknown;
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

void
ZoomWindow::paintEvent( QPaintEvent * )
{
	QPainter p( this );

	p.drawPixmap( c_windowOffset, c_windowOffset, d->m_pixmap.width(),
		d->m_pixmap.height(), d->m_pixmap );

	p.setPen( QPen( Qt::darkGreen, 2 ) );

	p.setBrush( Qt::NoBrush );

	p.drawRect( c_dim / 2 + c_delta, c_dim / 2 + c_delta,
		width() - c_dim - c_delta * 2, height() - c_dim - c_delta * 2 );

	p.setBrush( Qt::darkGreen );

	p.drawPie( QRect( c_dim + c_delta * 2, c_delta, c_dim, c_dim - c_delta * 4 ),
		0, 180 * 16 );
}

void
ZoomWindow::mousePressEvent( QMouseEvent * e )
{
	d->m_pos = e->pos();

	d->m_pressed = true;

	d->m_moveDelta = QPoint( 0, 0 );

	e->accept();
}

void
ZoomWindow::mouseMoveEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		const QPoint delta = e->pos() - d->m_pos;

		if( d->handlerType( e->pos() ) ==
			ZoomWindowPrivate::HandlerType::Control )
		{
			d->m_moveDelta += QPoint( qAbs( delta.x() ), qAbs( delta.y() ) );

			move( pos() + delta );
		}
	}

	e->accept();
}

void
ZoomWindow::mouseReleaseEvent( QMouseEvent * e )
{
	if( d->m_pressed )
	{
		if( d->handlerType( e->pos() ) ==
			ZoomWindowPrivate::HandlerType::Control &&
				d->m_moveDelta.manhattanLength() <= 3 )
		{
			QMenu menu( this );

			menu.addAction( QIcon( ":/img/document-save.png" ),
				tr( "Save As" ), this, &ZoomWindow::save );

			menu.addAction( QIcon( ":/img/edit-copy.png" ),
				tr( "Copy to Clipboard" ), this,
				&ZoomWindow::copy );

			menu.addSeparator();

			menu.addAction( QIcon( ":/img/dialog-close.png" ),
				tr( "Close Window" ), this, &QWidget::close );

			menu.addSeparator();

			menu.addAction(
				QIcon( ":/img/application-exit.png" ),
				tr( "Quit" ), QApplication::instance(),
				&QApplication::quit );

			menu.exec( e->globalPos() );
		}

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
ZoomWindow::save()
{

}

void
ZoomWindow::copy()
{
	QApplication::clipboard()->setPixmap( d->m_pixmap );
}
