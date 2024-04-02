
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef LICENSECHANGE__WORKER_HPP__INCLUDED
#define LICENSECHANGE__WORKER_HPP__INCLUDED

// Qt include.
#include <QThread>
#include <QScopedPointer>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE


//
// Worker
//

class WorkerPrivate;

//! Thread that replace license in files.
class Worker
	:	public QThread
{
	Q_OBJECT

signals:
	//! Processed file.
	void processedFile( int num );
	//! Job done.
	void done( int countOfFilesWithChangedLicense,
		int totalFilesCount, const QStringList & filesWithoutLicense );
	//! Error in old license.
	void errorInOldLicense();
	//! Job terminated.
	void terminated();

public:
	Worker( const QStringList & files, QTextDocument * oldLicense,
		QTextDocument * newLicense, bool searchForOneLicense,
		bool caseSensitive, QObject * parent = Q_NULLPTR );
	~Worker();

	//! Terminate job.
	void stopJob();

protected:
	void run() Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( Worker )

	QScopedPointer< WorkerPrivate > d;
}; // class Worker

#endif // LICENSECHANGE__WORKER_HPP__INCLUDED
