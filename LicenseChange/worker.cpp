
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

// Qt include.
#include <QTextDocument>
#include <QTextCursor>
#include <QList>


//
// Statement
//

//! Statement in the license or text file.
class Statement Q_DECL_FINAL {
public:
	//! Type of the statement.
	enum Type {
		//! Word.
		Word,
		//! Skip line.
		SkipLine,
		//! Skip word.
		SkipWord,
		//! Line ending.
		LineEnding
	}; // enum Type

	Statement( Type type, const QString & word = QString() )
		:	m_type( type )
		,	m_word( word )
	{
	}

	//! \return Type.
	Type type() const
	{
		return m_type;
	}

	//! \return Word.
	const QString & word() const
	{
		return m_word;
	}

private:
	//! Type.
	Type m_type;
	//! Word.
	QString m_word;
}; // class Statement


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
	QList< Statement > m_toReplace;
	//! Old license.
	QTextDocument * m_oldLicense;
	//! New license.
	QString m_newLicense;
	//! Parent.
	Worker * q;
}; // class WorkerPrivate

static const QChar c_r = QLatin1Char( '\r' );
static const QChar c_n = QLatin1Char( '\n' );
static const QChar c_or = QChar::ObjectReplacementCharacter;

void
WorkerPrivate::init()
{
	QTextCursor c( m_oldLicense );

	const QString data = m_oldLicense->toPlainText();

	QString word;

	for( int i = 0; i < data.length(); ++i )
	{
		const QChar ch = data.at( i );

		if( ch == c_r )
		{
			if( i + 1 < data.length() && data.at( i + 1 ) == c_n )
				++i;

			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word, word ) );

				word.clear();
			}

			m_toReplace.append( Statement::LineEnding );
		}
		else if( ch == c_n )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word, word ) );

				word.clear();
			}

			m_toReplace.append( Statement::LineEnding );
		}
		else if( ch == c_or )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word, word ) );

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
				m_toReplace.append( Statement( Statement::Word, word ) );

				word.clear();
			}
		}
		else
			word.append( ch );
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
