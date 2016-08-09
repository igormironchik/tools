
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

#ifndef LICENSECHANGE__MAINWINDOW_HPP__INCLUDED
#define LICENSECHANGE__MAINWINDOW_HPP__INCLUDED

// Qt include.
#include <QMainWindow>
#include <QScopedPointer>
#include <QWidget>
#include <QModelIndexList>


//
// MainWindow
//

class MainWindowPrivate;

//! Main window.
class MainWindow Q_DECL_FINAL
	:	public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

private slots:
	//! Run.
	void run();
	//! Skip line.
	void skipLine();
	//! Skip word.
	void skipWord();
	//! Name filters changed.
	void nameFiltersChanged( const QString & filter );
	//! Old license received focus.
	void oldLicenseReceivedFocus();
	//! Old license lost focus.
	void oldLicenseLostFocus();
	//! Old license changed.
	void oldLicenseChanged();
	//! File was processed.
	void fileProcessed( int num );
	//! Job done.
	void jobDone( int found, int total );
	//! Thread finished.
	void threadFinished();
	//! Error in old license.
	void errorInOldLicense();

private:
	//! Enable/disable run button.
	void enableDisableRunButton();
	//! \return List of checked files.
	QStringList checkedFiles();
	//! \return List of checked files.
	void checkedFiles( const QString & path, QStringList & res,
		const QModelIndexList & cb, const QModelIndexList & cl,
		const QStringList & ub, const QModelIndexList & ul,
		const QStringList & filter );

private:
	friend class MainWindowPrivate;

	Q_DISABLE_COPY( MainWindow )

	QScopedPointer< MainWindowPrivate > d;
}; // class MainWindow

#endif // LICENSECHANGE__MAINWINDOW_HPP__INCLUDED
