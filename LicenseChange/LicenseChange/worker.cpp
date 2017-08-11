
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

// LicenseChange include.
#include "worker.hpp"
#include "textedit.hpp"
#include "utils.hpp"

// Qt include.
#include <QTextDocument>
#include <QTextCursor>
#include <QList>
#include <QMutex>
#include <QMutexLocker>

// C++ include.
#include <algorithm>


//
// WorkerPrivate
//

class WorkerPrivate {
public:
	WorkerPrivate( Worker * parent, const QStringList & files,
		QTextDocument * oldLicense, QTextDocument * newLicense,
		bool searchForOneLicense, bool caseSensitive )
		:	m_files( files )
		,	m_searchForOneLicense( searchForOneLicense )
		,	m_caseSensitive( caseSensitive )
		,	m_oldLicense( oldLicense )
		,	m_newLicense( newLicense->toPlainText() )
		,	m_terminateRequested( false )
		,	m_isLicenseCorrect( false )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Files.
	QStringList m_files;
	//! Search for one occurrence of license only?
	bool m_searchForOneLicense;
	//! Case sensitive search.
	bool m_caseSensitive;
	//! To replace.
	QList< Statement > m_toReplace;
	//! Old license.
	QTextDocument * m_oldLicense;
	//! New license.
	QString m_newLicense;
	//! Mutex.
	QMutex m_mutex;
	//! Terminate requestde.
	bool m_terminateRequested;
	//! Is license correct?
	bool m_isLicenseCorrect;
	//! Parent.
	Worker * q;
}; // class WorkerPrivate


void
WorkerPrivate::init()
{
	QTextCursor c( m_oldLicense );

	const QString data = m_oldLicense->toPlainText();

	QString word;

	if( !data.isEmpty() && data.at( 0 ).isSpace() )
		m_toReplace.append( Statement::SkipFirstSpaces );

	for( int i = 0; i < data.length(); ++i )
	{
		const QChar ch = data.at( i );

		if( ch == c_r )
		{
			if( i + 1 < data.length() && data.at( i + 1 ) == c_n )
				++i;

			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word,
					( m_caseSensitive ? word : word.toLower() ) ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else if( ch == c_n )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word,
					( m_caseSensitive ? word : word.toLower() ) ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else if( ch == c_or )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word,
					( m_caseSensitive ? word : word.toLower() ) ) );

				m_isLicenseCorrect = true;

				word.clear();
			}

			c.setPosition( i + 1 );

			QTextImageFormat fmt = c.charFormat().toImageFormat();

			if( fmt.name() == c_skipLine )
				m_toReplace.append( Statement::SkipLine );
			else if( fmt.name() == c_skipWord )
				m_toReplace.append( Statement::SkipWord );
		}
		else if( ch.isSpace() )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word,
					( m_caseSensitive ? word : word.toLower() ) ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else
			word.append( ch );
	}

	if( !word.isEmpty() )
	{
		m_toReplace.append( Statement( Statement::Word,
			( m_caseSensitive ? word : word.toLower() ) ) );

		m_isLicenseCorrect = true;
	}

	m_oldLicense = Q_NULLPTR;
}


//
// Worker
//

Worker::Worker( const QStringList & files, QTextDocument * oldLicense,
	QTextDocument * newLicense, bool searchForOneLicense,
	bool caseSensitive, QObject * parent )
	:	QThread( parent )
	,	d( new WorkerPrivate( this, files, oldLicense, newLicense,
			searchForOneLicense, caseSensitive ) )
{
	d->init();
}

Worker::~Worker()
{
}

void
Worker::stopJob()
{
	QMutexLocker lock( &d->m_mutex );

	d->m_terminateRequested = true;
}

void
Worker::run()
{
	if( d->m_isLicenseCorrect )
	{
		QStringList filesWithoutLicense;
		int i = 1;
		int foundCount = 0;

		foreach( const QString & fileName, d->m_files )
		{
			QString data;

			{
				QFile file( fileName );

				FileCloser fc( file, QFile::ReadOnly );

				data = file.readAll();
			}

			Words words = splitData( data, !d->m_caseSensitive );

			QList< LicensePos > found;

			for( int i = 0; i < words.size(); ++i )
			{
				LicensePos pos = findLicense( words, d->m_toReplace, i );

				if( pos.m_start != -1 )
				{
					found.append( pos );

					--i;

					if( d->m_searchForOneLicense )
						break;
				}
				else
					break;
			}

			if( !found.isEmpty() )
			{
				++foundCount;

				std::reverse( found.begin(), found.end() );

				foreach( const LicensePos & p, found )
				{
					data.replace( p.m_start, p.m_end - p.m_start,
						d->m_newLicense );
				}

				QFile file( fileName );

				FileCloser fc( file, QFile::WriteOnly | QFile::Truncate );

				file.write( data.toUtf8() );
			}
			else
				filesWithoutLicense.append( fileName );

			emit processedFile( i );

			++i;

			{
				QMutexLocker lock( &d->m_mutex );

				if( d->m_terminateRequested )
					break;
			}
		}

		{
			QMutexLocker lock( &d->m_mutex );

			if( d->m_terminateRequested )
				emit terminated();
			else
				emit done( foundCount, d->m_files.count(), filesWithoutLicense );
		}
	}
	else
		emit errorInOldLicense();
}
