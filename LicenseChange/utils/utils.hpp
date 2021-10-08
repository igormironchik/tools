
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

#ifndef LICENSECHANGE__UTILS_HPP__INCLUDED
#define LICENSECHANGE__UTILS_HPP__INCLUDED

// Qt include.
#include <QString>
#include <QFile>


static const QChar c_r = QLatin1Char( '\r' );
static const QChar c_n = QLatin1Char( '\n' );
static const QChar c_or = QChar::ObjectReplacementCharacter;


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

	Statement( Type type, const QString & word = QString() );

	//! \return Type.
	Type type() const;

	//! \return Word.
	const QString & word() const;

private:
	//! Type.
	Type m_type;
	//! Word.
	QString m_word;
}; // class Statement


bool operator == ( const Statement & s1, const Statement & s2 );


//
// FileCloser
//

class FileCloser Q_DECL_FINAL {
public:
	FileCloser( QFile & file, QFile::OpenMode mode );
	~FileCloser();

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
	qsizetype m_pos;
	//! Poistion of the word with previous spaces.
	qsizetype m_posWithSpaces;
}; // struct WordWithPlace

using Words = QList< WordWithPlace >;


bool operator == ( const WordWithPlace & w1, const WordWithPlace & w2 );


//
// splitData
//

Words splitData( const QString & data, bool toLower = false );


//
// LicensePos
//

struct LicensePos Q_DECL_FINAL {
	//! Start.
	qsizetype m_start;
	//! End.
	qsizetype m_end;
}; // struct LicensePos


//
// findLicense
//

LicensePos findLicense( const Words & words, const QList< Statement > & license,
	int & idx );

#endif // LICENSECHANGE__UTILS_HPP__INCLUDED
