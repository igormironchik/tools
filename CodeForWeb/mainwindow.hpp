
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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

public:
	explicit TextEdit( QWidget * parent = 0 );
	~TextEdit();

protected:
	QMimeData * createMimeDataFromSelection() const Q_DECL_OVERRIDE;
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

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow

#endif // CODEFORWEB__MAINWINDOW_HPP__INCLUDED
