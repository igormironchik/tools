
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

// Qt include.
#include <QtTest/QtTest>


class Utils
	:	public QObject
{
	Q_OBJECT

private slots:
	void test_splitData()
	{
		const QString data = QLatin1String(
			"word1 word2\r"
			"word3 word4\r\n"
			"word5 word6\n"
			" word7" );

		Words w = splitData( data );

		QCOMPARE( w.count(), 10 );

		QCOMPARE( w.at( 0 ).m_pos, 0 );
		QCOMPARE( w.at( 0 ).m_posWithSpaces, 0 );
		QCOMPARE( w.at( 0 ).m_st.word(), QLatin1String( "word1" ) );
		QCOMPARE( w.at( 0 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 1 ).m_pos, 6 );
		QCOMPARE( w.at( 1 ).m_posWithSpaces, 5 );
		QCOMPARE( w.at( 1 ).m_st.word(), QLatin1String( "word2" ) );
		QCOMPARE( w.at( 1 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 2 ).m_pos, -1 );
		QCOMPARE( w.at( 2 ).m_posWithSpaces, -1 );
		QCOMPARE( w.at( 2 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 3 ).m_pos, 12 );
		QCOMPARE( w.at( 3 ).m_posWithSpaces, 12 );
		QCOMPARE( w.at( 3 ).m_st.word(), QLatin1String( "word3" ) );
		QCOMPARE( w.at( 3 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 4 ).m_pos, 18 );
		QCOMPARE( w.at( 4 ).m_posWithSpaces, 17 );
		QCOMPARE( w.at( 4 ).m_st.word(), QLatin1String( "word4" ) );
		QCOMPARE( w.at( 4 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 5 ).m_pos, -1 );
		QCOMPARE( w.at( 5 ).m_posWithSpaces, -1 );
		QCOMPARE( w.at( 5 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 6 ).m_pos, 25 );
		QCOMPARE( w.at( 6 ).m_posWithSpaces, 25 );
		QCOMPARE( w.at( 6 ).m_st.word(), QLatin1String( "word5" ) );
		QCOMPARE( w.at( 6 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 7 ).m_pos, 31 );
		QCOMPARE( w.at( 7 ).m_posWithSpaces, 30 );
		QCOMPARE( w.at( 7 ).m_st.word(), QLatin1String( "word6" ) );
		QCOMPARE( w.at( 7 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 8 ).m_pos, -1 );
		QCOMPARE( w.at( 8 ).m_posWithSpaces, -1 );
		QCOMPARE( w.at( 8 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 9 ).m_pos, 38 );
		QCOMPARE( w.at( 9 ).m_posWithSpaces, 37 );
		QCOMPARE( w.at( 9 ).m_st.word(), QLatin1String( "word7" ) );
		QCOMPARE( w.at( 9 ).m_st.type(), Statement::Word );
	}

}; // class BoolScalarTest

QTEST_MAIN( Utils )

#include "main.moc"
