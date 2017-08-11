
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

#ifndef CODEFORWEB__MAINWINDOW_HPP__INCLUDED
#define CODEFORWEB__MAINWINDOW_HPP__INCLUDED

// Qt include.
#include <QMainWindow>
#include <QScopedPointer>
#include <QTextEdit>


//
// TextEdit
//

//! Text edit.
class TextEdit
	:	public QTextEdit
{
	Q_OBJECT

signals:
	//! Copy.
	void copyTriggered();

public:
	explicit TextEdit( QWidget * parent = 0 );
	~TextEdit();

protected:
	void keyPressEvent( QKeyEvent * e ) Q_DECL_OVERRIDE;
}; // class TextEdit


//
// MainWindow
//

class MainWindowPrivate;

//! Main window.
class MainWindow
	:	public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:
	//! Copy.
	void copy();

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow

#endif // CODEFORWEB__MAINWINDOW_HPP__INCLUDED
