
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
