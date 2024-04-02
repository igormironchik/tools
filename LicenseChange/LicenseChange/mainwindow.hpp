
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
	void jobDone( int found, int total,
		const QStringList & filesWithoutLicense );
	//! Thread finished.
	void threadFinished();
	//! Job terminated.
	void jobTerminated();
	//! Error in old license.
	void errorInOldLicense();
	//! Cancel job.
	void cancelJob();

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
