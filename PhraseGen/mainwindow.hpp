
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

#ifndef PHRASEGEN__MAINWINDOW_HPP__INCLUDED
#define PHRASEGEN__MAINWINDOW_HPP__INCLUDED

// Qt include.
#include <QWidget>
#include <QScopedPointer>


namespace PhraseGen {

//
// MainWindow
//

class MainWindowPrivate;

//! Main window.
class MainWindow
	:	public QWidget
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:
	//! Generate.
	void generate();
	//! Checkbox clicked.
	void checkbox( int );
	//! Copy.
	void copy();

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow

} /* namespace PhraseGen */

#endif // PHRASEGEN__MAINWINDOW_HPP__INCLUDED
