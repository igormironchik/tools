
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

// Qt include.
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QClipboard>
#include <QPainter>
#include <QFileDialog>
#include <QMenuBar>
#include <QWidget>
#include <QCloseEvent>


//
// Magnified
//

class Magnified final
	:	public QWidget
{
public:
	Magnified( const QPixmap & p, QWidget * parent )
		:	QWidget( parent )
		,	m_p( p )
	{
		setAutoFillBackground( false );
		setMinimumSize( p.size() );
	}

	QSize sizeHint() const override
	{
		return m_p.size();
	}

	const QPixmap & pixmap() const
	{
		return m_p;
	}

protected:
	void paintEvent( QPaintEvent * e ) override
	{
		QPainter p( this );

		p.drawPixmap( ( width() - m_p.width() ) / 2,
			( height() - m_p.height() ) / 2,
			m_p.width(), m_p.height(), m_p );
	}

private:
	QPixmap m_p;
}; // class Magnified


//
// ZoomWindowPrivate
//

class ZoomWindowPrivate {
public:
	ZoomWindowPrivate( const QPixmap & p, ZoomWindow * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init( const QPixmap & p );

	//! Central widget.
	Magnified * m_c = nullptr;
	//! Parent.
	ZoomWindow * q;
}; // class ZoomWindowPrivate

void
ZoomWindowPrivate::init( const QPixmap & p )
{
	auto file = q->menuBar()->addMenu( ZoomWindow::tr( "&File" ) );

	file->addAction( QIcon( ":/img/document-save.png" ),
		ZoomWindow::tr( "Save As" ), q, &ZoomWindow::save );
	file->addAction( QIcon( ":/img/edit-copy.png" ),
		ZoomWindow::tr( "Copy to Clipboard" ), q,
		&ZoomWindow::copy );
	file->addSeparator();
	file->addAction( QIcon( ":/img/application-exit.png" ),
		ZoomWindow::tr( "Quit" ), QApplication::instance(),
		&QApplication::quit );

	m_c = new Magnified( p, q );
	q->setCentralWidget( m_c );
}


//
// ZoomWindow
//

ZoomWindow::ZoomWindow( const QPixmap & p, QWidget * parent )
	:	QMainWindow( parent, Qt::WindowStaysOnTopHint )
	,	d( new ZoomWindowPrivate( p, this ) )
{
	d->init( p );
}

ZoomWindow::~ZoomWindow()
{
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
	QString fileName =
		QFileDialog::getSaveFileName( this, tr( "Select File Name..." ),
			QString(), tr( "PNG (*.png)" ) );

	if( !fileName.isEmpty() )
	{
		if( !fileName.endsWith( QLatin1String( ".png" ), Qt::CaseInsensitive ) )
			fileName.append( QLatin1String( ".png" ) );

		d->m_c->pixmap().save( fileName );
	}
}

void
ZoomWindow::copy()
{
	QApplication::clipboard()->setPixmap( d->m_c->pixmap() );
}
