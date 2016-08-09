
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
#include "utils.hpp"


//
// Statement
//

Statement::Statement( Type type, const QString & word )
	:	m_type( type )
	,	m_word( word )
{
}

Statement::Type
Statement::type() const
{
	return m_type;
}

const QString &
Statement::word() const
{
	return m_word;
}


//
// FileCloser
//

FileCloser::FileCloser( QFile & file, QFile::OpenMode mode )
	:	m_file( file )
{
	m_file.open( mode );
}

FileCloser::~FileCloser()
{
	m_file.close();
}


//
// splitData
//

Words splitData( const QString & data )
{
	Words words;

	QString word;

	int pos = 0;
	int posWithSpaces = 0;

	for( int i = 0; i < data.length(); ++i )
	{
		const QChar ch = data.at( i );

		if( ch != c_r && ch != c_n && !ch.isSpace() )
		{
			if( word.isEmpty() )
				pos = i;

			word.append( ch );
		}
		else if( ch == c_r || ch == c_n )
		{
			if( ch == c_r && ( i + 1 ) < data.length() &&
				data.at( i + 1 ) == c_n )
					++i;

			if( !word.isEmpty() )
			{
				words.append( { Statement( Statement::Word, word ),
					pos, posWithSpaces } );

				word.clear();
			}

			posWithSpaces = i + 1;

			words.append( { Statement::LineEnding, -1, -1 } );
		}
		else if( ch.isSpace() )
		{
			if( !word.isEmpty() )
			{
				words.append( { Statement( Statement::Word, word ),
					pos, posWithSpaces } );

				word.clear();

				posWithSpaces = i;
			}
		}
	}

	if( !word.isEmpty() )
		words.append( { Statement( Statement::Word, word ),
			pos, posWithSpaces } );

	return words;
} // splitData


//
// findLicense
//

LicensePos findLicense( const Words & words, const QList< Statement > & license,
	int & idx )
{
	LicensePos res{ -1, -1 };

	int firstWord = 0;

	for( int i = 0; i < license.count(); ++i )
	{
		if( license.at( i ).type() == Statement::Word ||
			license.at( i ).type() == Statement::SkipWord ||
			license.at( i ).type() == Statement::SkipFirstSpaces )
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
				{
					++wp;

					if( j <= ( license.at( 0 ).type() !=
						Statement::SkipFirstSpaces ? firstWord : firstWord + 1 ) )
							++i;
				}

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
				if( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
						++wp;

				while( wp < words.count() &&
					words.at( wp ).m_st.type() != Statement::LineEnding )
						++wp;

				if( wp >= words.count() && j < license.count() )
				{
					found = false;

					break;
				}

				if( words.at( wp ).m_st.type() == Statement::LineEnding )
					--wp;
			}
			else if( license.at( j ).type() == Statement::SkipWord )
			{
				while( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
				{
					++wp;

					if( j <= ( license.at( 0 ).type() !=
						Statement::SkipFirstSpaces ? firstWord : firstWord + 1 ) )
							++i;
				}
			}

			if( license.at( j ).type() != Statement::SkipFirstSpaces &&
				j < license.count() - 1 )
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

			idx = wp + 1;

			break;
		}

		if( wp >= words.count() )
			break;
	}

	return res;
} // findLicense
