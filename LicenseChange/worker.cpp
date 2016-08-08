
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
#include <QFile>


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
		LineEnding,
		//! Skip first spaces.
		SkipFirstSpaces
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
		,	m_isLicenseCorrect( false )
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
	//! Is license correct?
	bool m_isLicenseCorrect;
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
				m_toReplace.append( Statement( Statement::Word, word ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else if( ch == c_n )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word, word ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else if( ch == c_or )
		{
			if( !word.isEmpty() )
			{
				m_toReplace.append( Statement( Statement::Word, word ) );

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
				m_toReplace.append( Statement( Statement::Word, word ) );

				m_isLicenseCorrect = true;

				word.clear();
			}
		}
		else
			word.append( ch );
	}

	if( !word.isEmpty() )
	{
		m_toReplace.append( Statement( Statement::Word, word ) );

		m_isLicenseCorrect = true;
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


//
// FileCloser
//

class FileCloser Q_DECL_FINAL {
public:
	FileCloser( QFile & file, QFile::OpenMode mode )
		:	m_file( file )
	{
		m_file.open( mode );
	}

	~FileCloser()
	{
		m_file.close();
	}

private:
	//! File.
	QFile & m_file;
}; // class FileCloser


//
// WordWithPlace
//

struct WordWithPlace Q_DECL_FINAL {
	//! Statement.
	Statement m_st;
	//! Postion of the word.
	int m_pos;
	//! Poistion of the word with previous spaces.
	int m_posWithSpaces;
}; // struct WordWithPlace

using Words = QList< WordWithPlace >;


Words splitData( const QString & data )
{
	Words words;

	QString word;

	int pos = 0;
	int posWithSpaces = 0;

	for( int i = 0; i < data.length(); ++i )
	{
		const QChar ch = data.at( i );

		if( ch.isSpace() )
		{
			if( !word.isEmpty() )
			{
				words.append( { Statement( Statement::Word, word ),
					pos, posWithSpaces } );

				word.clear();

				posWithSpaces = i;
			}
		}
		else if( ch != c_r && ch != c_n )
		{
			if( word.isEmpty() )
				pos = i;

			word.append( ch );
		}
		else
		{
			if( ch == c_r && ( i + 1 ) < data.length() &&
				data.at( i + 1 ) == c_n )
					++i;

			words.append( { Statement::LineEnding, -1, -1 } );
		}
	}

	if( !word.isEmpty() )
		words.append( { Statement( Statement::Word, word ),
			pos, posWithSpaces } );

	return words;
} // splitData


//
// LicensePos
//

struct LicensePos Q_DECL_FINAL {
	//! Start.
	int m_start;
	//! End.
	int m_end;
}; // struct LicensePos


LicensePos findLicense( const Words & words, const QList< Statement > & license,
	int & idx )
{
	LicensePos res{ -1, -1 };

	int firstWord = 0;

	for( int i = 0; i < license.count(); ++i )
	{
		if( license.at( i ).type() == Statement::Word ||
			license.at( i ).type() == Statement::SkipWord )
		{
			firstWord = i;

			break;
		}
	}

	for( int i = idx; i < words.count(); ++i )
	{
		bool found = true;
		int wp = i;

		for( int j = firstWord; j < license.count(); ++j )
		{
			if( license.at( j ).type() == Statement::Word )
			{
				while( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
						++wp;

				if( wp >= words.count() )
				{
					found = false;

					break;
				}

				if( license.at( j ).word() != words.at( wp ).m_st.word() )
				{
					found = false;

					break;
				}
			}
			else if( license.at( j ).type() == Statement::SkipLine )
			{
				while( wp < words.count() &&
					words.at( wp ).m_st.type() != Statement::LineEnding )
						++wp;

				if( wp >= words.count() && j < license.count() )
				{
					found = false;

					break;
				}
			}

			++wp;
		}

		if( found )
		{
			if( wp == words.count() )
				--wp;

			int x = i;

			if( firstWord > 0 )
			{
				int lines = 0;

				if( words.at( i - 1 ).m_st.type() == Statement::LineEnding )
					lines = -1;

				x = i - 1;

				for( ; x >= 0; --x )
				{
					if( words.at( x ).m_st.type() == Statement::LineEnding )
						++lines;

					if( lines == firstWord )
						break;
				}

				++x;
			}

			res = { ( license.at( 0 ).type() == Statement::SkipFirstSpaces ?
				words.at( x ).m_posWithSpaces : words.at( x ).m_pos ),
				words.at( wp ).m_pos + words.at( wp ).m_st.word().length() };
		}
	}

	return res;
} // findLicense

void
Worker::run()
{
	int i = 1;

	foreach( const QString & fileName, d->m_files )
	{
		if( d->m_isLicenseCorrect )
		{
			QString data;

			{
				QFile file( fileName );

				FileCloser fc( file, QFile::ReadOnly );

				data = file.readAll();
			}

			Words words = splitData( data );

			QList< LicensePos > found;

			for( int i = 0; i < words.size(); ++i )
			{
				LicensePos pos = findLicense( words, d->m_toReplace, i );

				if( pos.m_start != -1 )
					found.append( pos );
			}
		}

		emit processedFile( i );

		++i;
	}
}
