
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef MAGNIFIER__ZOOMWINDOW_HPP__INCLUDED
#define MAGNIFIER__ZOOMWINDOW_HPP__INCLUDED

// Qt include.
#include <QMainWindow>
#include <QScopedPointer>


//
// ZoomWindow
//

class ZoomWindowPrivate;

//! Main window.
class ZoomWindow final
	:	public QMainWindow
{
	Q_OBJECT

public:
	ZoomWindow( const QPixmap & p, QWidget * parent = nullptr );
	~ZoomWindow() override;

private slots:
	//! Save.
	void save();
	//! Copy.
	void copy();

protected:
	void closeEvent( QCloseEvent * e ) override;

private:
	friend class ZoomWindowPrivate;

	Q_DISABLE_COPY( ZoomWindow )

	QScopedPointer< ZoomWindowPrivate > d;
}; // class ZoomWindow

#endif // MAGNIFIER__ZOOMWINDOW_HPP__INCLUDED
