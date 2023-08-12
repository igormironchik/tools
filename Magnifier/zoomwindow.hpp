
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
