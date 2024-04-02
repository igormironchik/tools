
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// LicenseChange include.
#include "utils.hpp"

// C++ include.
#include <algorithm>
#include <iterator>


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

bool operator == ( const Statement & s1, const Statement & s2 )
{
	if( s1.type() == Statement::Word && s2.type() == Statement::Word )
		return s1.word() == s2.word();
	else
		return s1.type() == s2.type();
}

bool operator == ( const WordWithPlace & w1, const WordWithPlace & w2 )
{
	return w1.m_st == w2.m_st;
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

Words splitData( const QString & data, bool toLower )
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
			int start = i;
			int len = 1;

			if( ch == c_r && ( i + 1 ) < data.length() &&
				data.at( i + 1 ) == c_n )
			{
				++i;
				len = 2;
			}

			if( !word.isEmpty() )
			{
				words.append( { Statement( Statement::Word,
						( toLower? word.toLower() : word ) ),
					pos, posWithSpaces } );

				word.clear();
			}

			posWithSpaces = i + 1;

			words.append( { Statement( Statement::LineEnding,
				QString( len, QLatin1Char( '\n' ) ) ), start, start } );
		}
		else if( ch.isSpace() )
		{
			if( !word.isEmpty() )
			{
				words.append( { Statement( Statement::Word,
						( toLower? word.toLower() : word ) ),
					pos, posWithSpaces } );

				word.clear();

				posWithSpaces = i;
			}
		}
	}

	if( !word.isEmpty() )
		words.append( { Statement( Statement::Word,
				( toLower? word.toLower() : word ) ),
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

	// Find first real word.
	for( int i = 0; i < license.count(); ++i )
	{
		if( license.at( i ).type() == Statement::Word )
		{
			firstWord = i;

			break;
		}
	}

	auto first = words.cbegin();
	first += idx;

	auto it = std::find( first, words.cend(), WordWithPlace{
		license.at( firstWord ), -1, -1 } );

	while( it != words.cend() )
	{
		idx = std::distance( words.cbegin(), it );

		bool found = true;
		int wp = idx;

		for( int j = firstWord; j < license.count(); ++j )
		{
			if( license.at( j ).type() == Statement::Word )
			{
				// Skip line endings.
				while( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
				{
					++wp;
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
				bool skipped = false;

				if( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
				{
					++wp;

					skipped = true;
				}

				while( wp < words.count() &&
					words.at( wp ).m_st.type() != Statement::LineEnding )
				{
					++wp;

					skipped = true;
				}

				if( ( wp >= words.count() && j < license.count() - 1 ) ||
					!skipped )
				{
					found = false;

					break;
				}

				if( wp < words.count() - 1 &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
						--wp;
			}
			else if( license.at( j ).type() == Statement::SkipWord )
			{
				// Skip line endings.
				while( wp < words.count() &&
					words.at( wp ).m_st.type() == Statement::LineEnding )
				{
					++wp;
				}
			}

			if( j < license.count() - 1 )
				++wp;
		}

		// If license was found.
		if( found )
		{
			if( wp == words.count() )
				--wp;

			int x = idx;

			if( firstWord > 0 )
			{
				for( int i = firstWord - 1; i >= 0; --i )
				{
					if( license.at( i ).type() == Statement::SkipWord )
					{
						while( x > 0 &&
							words.at( x - 1 ).m_st.type() != Statement::Word )
						{
							--x;
						}

						--x;
					}
					else if( license.at( i ).type() == Statement::SkipLine )
					{
						--x;

						while( x > 0 &&
							words.at( x - 1 ).m_st.type() != Statement::LineEnding )
						{
							--x;
						}
					}
				}

				if( x < 0 )
					x = 0;
			}

			res = { ( license.at( 0 ).type() == Statement::SkipFirstSpaces ||
				license.at( 0 ).type() == Statement::SkipLine ?
					words.at( x ).m_posWithSpaces : words.at( x ).m_pos ),
				words.at( wp ).m_pos + words.at( wp ).m_st.word().length() };

			idx = wp + 1;

			break;
		}

		if( wp >= words.count() )
			break;

		it = std::find( ++it, words.cend(), WordWithPlace{
			license.at( firstWord ), -1, -1 } );
	}

	return res;
} // findLicense
