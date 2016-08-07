
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

// Qt include.
#include <QTextDocument>
#include <QTextCursor>
#include <QList>
#include <QRegExp>


//
// WorkerPrivate
//

class WorkerPrivate {
public:
	WorkerPrivate( Worker * parent, const QStringList & files,
		QTextDocument * oldLicense, QTextDocument * newLicense )
		:	m_files( files )
		,	m_oldLicense( oldLicense )
		,	m_newLicense( newLicense->toPlainText() )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Files.
	QStringList m_files;
	//! To replace.
	QStringList m_toReplace;
	//! Regular expressions.
	QList< QRegExp > m_regs;
	//! Old license.
	QTextDocument * m_oldLicense;
	//! New license.
	QString m_newLicense;
	//! Parent.
	Worker * q;
}; // class WorkerPrivate

void
WorkerPrivate::init()
{
	QTextCursor c( m_oldLicense );
	c.movePosition( QTextCursor::Start );

	const QString data = m_oldLicense->toPlainText();
	QString regexp;
	QString word;

	c.movePosition( QTextCursor::NextCharacter );

	while( !c.atEnd() )
	{
		if( c.charFormat().background().color() == Qt::blue )
		{
			regexp.append( data.at( c.position() - 1 ) );

			if( !word.isEmpty() )
			{
				m_toReplace.append( word );

				word.clear();
			}
		}
		else
		{
			const QChar ch = data.at( c.position() - 1 );

			if( !ch.isSpace() )
				word.append( ch );
			else if( !word.isEmpty() )
			{
				m_toReplace.append( word );

				word.clear();
			}

			if( !regexp.isEmpty() )
			{
				m_regs.append( QRegExp( regexp ) );

				m_toReplace.append( QString() );

				regexp.clear();
			}
		}

		c.movePosition( QTextCursor::NextCharacter );
	}

	if( c.charFormat().background().color() == Qt::blue )
		regexp.append( data.at( c.position() - 1 ) );
	else
		word.append( data.at( c.position() - 1 ) );

	if( !word.isEmpty() )
		m_toReplace.append( word );

	if( !regexp.isEmpty() )
	{
		m_regs.append( QRegExp( regexp ) );

		m_toReplace.append( QString() );
	}

	m_oldLicense = Q_NULLPTR;
}


//
// Worker
//

Worker::Worker( const QStringList & files, QTextDocument * oldLicense,
	QTextDocument * newLicense, QObject * parent )
	:	QThread( parent )
	,	d( new WorkerPrivate( this, files, oldLicense, newLicense ) )
{
	d->init();
}

Worker::~Worker()
{
}

void
Worker::run()
{

}
