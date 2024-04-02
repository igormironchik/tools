
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
