
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
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
	void initTestCase()
	{
		m_lic1.append( Statement::SkipFirstSpaces );
		m_lic1.append( { Statement::Word, QLatin1String( "Best" ) } );
		m_lic1.append( { Statement::Word, QLatin1String( "license." ) } );

		m_lic2.append( Statement::SkipFirstSpaces );
		m_lic2.append( { Statement::Word, QLatin1String( "Best" ) } );
		m_lic2.append( Statement::SkipWord );
		m_lic2.append( { Statement::Word, QLatin1String( "license." ) } );

		m_lic3.append( Statement::SkipLine );
		m_lic3.append( { Statement::Word, QLatin1String( "Best" ) } );
		m_lic3.append( Statement::SkipWord );
		m_lic3.append( { Statement::Word, QLatin1String( "license." ) } );
		m_lic3.append( Statement::SkipLine );

		m_lic4.append( Statement::SkipLine );
		m_lic4.append( Statement::SkipWord );
		m_lic4.append( { Statement::Word, QLatin1String( "license." ) } );
		m_lic4.append( Statement::SkipLine );

		m_lic5.append( Statement::SkipLine );
		m_lic5.append( Statement::SkipLine );
		m_lic5.append( Statement::SkipWord );
		m_lic5.append( Statement::SkipWord );
		m_lic5.append( { Statement::Word, QLatin1String( "license." ) } );
		m_lic5.append( Statement::SkipLine );
	}

	void test_splitData()
	{
		const QString data = QLatin1String(
			// 0 - 12
			"word1 word2\r"
			// 12 - 25
			"word3 word4\r\n"
			// 25 - 37
			"word5 word6\n"
			// 37 - 44
			" word7\n"
			// 44 - 45
			"\n"
			// 45 - 50
			"word8" );

		Words w = splitData( data );

		QCOMPARE( w.count(), 13 );

		QCOMPARE( w.at( 0 ).m_pos, 0 );
		QCOMPARE( w.at( 0 ).m_posWithSpaces, 0 );
		QCOMPARE( w.at( 0 ).m_st.word(), QLatin1String( "word1" ) );
		QCOMPARE( w.at( 0 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 1 ).m_pos, 6 );
		QCOMPARE( w.at( 1 ).m_posWithSpaces, 5 );
		QCOMPARE( w.at( 1 ).m_st.word(), QLatin1String( "word2" ) );
		QCOMPARE( w.at( 1 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 2 ).m_pos, 11 );
		QCOMPARE( w.at( 2 ).m_posWithSpaces, 11 );
		QCOMPARE( w.at( 2 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 3 ).m_pos, 12 );
		QCOMPARE( w.at( 3 ).m_posWithSpaces, 12 );
		QCOMPARE( w.at( 3 ).m_st.word(), QLatin1String( "word3" ) );
		QCOMPARE( w.at( 3 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 4 ).m_pos, 18 );
		QCOMPARE( w.at( 4 ).m_posWithSpaces, 17 );
		QCOMPARE( w.at( 4 ).m_st.word(), QLatin1String( "word4" ) );
		QCOMPARE( w.at( 4 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 5 ).m_pos, 23 );
		QCOMPARE( w.at( 5 ).m_posWithSpaces, 23 );
		QCOMPARE( w.at( 5 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 6 ).m_pos, 25 );
		QCOMPARE( w.at( 6 ).m_posWithSpaces, 25 );
		QCOMPARE( w.at( 6 ).m_st.word(), QLatin1String( "word5" ) );
		QCOMPARE( w.at( 6 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 7 ).m_pos, 31 );
		QCOMPARE( w.at( 7 ).m_posWithSpaces, 30 );
		QCOMPARE( w.at( 7 ).m_st.word(), QLatin1String( "word6" ) );
		QCOMPARE( w.at( 7 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 8 ).m_pos, 36 );
		QCOMPARE( w.at( 8 ).m_posWithSpaces, 36 );
		QCOMPARE( w.at( 8 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 9 ).m_pos, 38 );
		QCOMPARE( w.at( 9 ).m_posWithSpaces, 37 );
		QCOMPARE( w.at( 9 ).m_st.word(), QLatin1String( "word7" ) );
		QCOMPARE( w.at( 9 ).m_st.type(), Statement::Word );

		QCOMPARE( w.at( 10 ).m_pos, 43 );
		QCOMPARE( w.at( 10 ).m_posWithSpaces, 43 );
		QCOMPARE( w.at( 10 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 11 ).m_pos, 44 );
		QCOMPARE( w.at( 11 ).m_posWithSpaces, 44 );
		QCOMPARE( w.at( 11 ).m_st.type(), Statement::LineEnding );

		QCOMPARE( w.at( 12 ).m_pos, 45 );
		QCOMPARE( w.at( 12 ).m_posWithSpaces, 45 );
		QCOMPARE( w.at( 12 ).m_st.word(), QLatin1String( "word8" ) );
		QCOMPARE( w.at( 12 ).m_st.type(), Statement::Word );
	}

	void test_license1()
	{
		const QString data = QLatin1String(
			// 0 - 3
			"/*\n"
			// 3 - 18
			"\tBest license.\n"
			// 18 - 21
			"*/\n"
			// 21 - 22
			"\n"
			// 22 - 42
			"#include <iostream>\n"
			// 42 - 43
			"\n"
			// 43 - 48
			"Best\n"
			// 48 - 56
			"license." );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic1, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 2 );

		QCOMPARE( pos.at( 0 ).m_start, 3 );
		QCOMPARE( pos.at( 0 ).m_end, 17 );

		QCOMPARE( pos.at( 1 ).m_start, 43 );
		QCOMPARE( pos.at( 1 ).m_end, 56 );
	}

	void test_license2()
	{
		const QString data = QLatin1String(
			// 0 - 3
			"/*\n"
			// 3 - 23
			"\tBest\never\nlicense.\n"
			// 23 - 26
			"*/\n"
			// 26 - 27
			"\n"
			// 27 - 47
			"#include <iostream>\n"
			// 47 - 48
			"\n"
			// 48 - 53
			"Best\n"
			// 53 - 61
			"license." );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic2, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 1 );

		QCOMPARE( pos.at( 0 ).m_start, 3 );
		QCOMPARE( pos.at( 0 ).m_end, 22 );
	}

	void test_license3()
	{
		const QString data = QLatin1String(
			// 0 - 3
			"/*\n"
			// 3 - 11
			"License\n"
			// 11 - 31
			"\tBest\never\nlicense.\n"
			// 31 - 39
			"License\n"
			// 39 - 42
			"*/\n"
			// 42 - 43
			"\n"
			// 43 - 63
			"#include <iostream>\n"
			// 63 - 64
			"\n"
			// 64 - 74
			"Best\never\n"
			// 74 - 82
			"license." );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic3, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 1 );

		QCOMPARE( pos.at( 0 ).m_start, 3 );
		QCOMPARE( pos.at( 0 ).m_end, 38 );
	}

	void test_license4()
	{
		const QString data = QLatin1String(
			// 0 - 3
			"/*\n"
			// 3 - 11
			"License\n"
			// 11 - 31
			"\tBest\never\nlicense.\n"
			// 31 - 39
			"License\n"
			// 39 - 42
			"*/\n"
			// 42 - 43
			"\n"
			// 43 - 63
			"#include <iostream>\n"
			// 63 - 64
			"\n"
			// 64 - 74
			"Best\never\n"
			// 74 - 83
			"license.\n" );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic4, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 2 );

		QCOMPARE( pos.at( 0 ).m_start, 11 );
		QCOMPARE( pos.at( 0 ).m_end, 38 );

		QCOMPARE( pos.at( 1 ).m_start, 64 );
		QCOMPARE( pos.at( 1 ).m_end, 83 );
	}

	void test_license5()
	{
		const QString data = QLatin1String(
			// 0 - 3
			"/*\n"
			// 3 - 11
			"License\n"
			// 11 - 31
			"\tBest\never\nlicense.\n"
			// 31 - 39
			"License\n"
			// 39 - 42
			"*/\n"
			// 42 - 43
			"\n"
			// 43 - 63
			"#include <iostream>\n"
			// 63 - 64
			"\n"
			// 64 - 74
			"Best\never\n"
			// 74 - 83
			"license.\n" );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic5, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 2 );

		QCOMPARE( pos.at( 0 ).m_start, 0 );
		QCOMPARE( pos.at( 0 ).m_end, 38 );

		QCOMPARE( pos.at( 1 ).m_start, 43 );
		QCOMPARE( pos.at( 1 ).m_end, 83 );
	}

	void test_license6()
	{
		const QString data = QLatin1String(
			"Best" );

		Words w = splitData( data );

		QList< LicensePos > pos;

		for( int i = 0; i < w.count(); ++i )
		{
			LicensePos p = findLicense( w, m_lic1, i );

			if( p.m_start != -1 )
			{
				pos.append( p );

				--i;
			}
			else
				break;
		}

		QCOMPARE( pos.count(), 0 );
	}

private:
	QList< Statement > m_lic1;
	QList< Statement > m_lic2;
	QList< Statement > m_lic3;
	QList< Statement > m_lic4;
	QList< Statement > m_lic5;
}; // class BoolScalarTest

QTEST_MAIN( Utils )

#include "main.moc"
