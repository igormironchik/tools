
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

// Qt include.
#include <QCoreApplication>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>
#include <QProcess>

// QtArg include.
#include <QtArg/Arg>
#include <QtArg/Help>
#include <QtArg/CmdLine>
#include <QtArg/Exceptions>
#include <QtArg/MultiArg>


int main( int argc, char ** argv )
{
	QStringList authorName;
	QString beforeDate;
	QString afterDate;

	try {
		QtMultiArg a( QLatin1Char( 'a' ), QLatin1String( "author" ),
			QLatin1String( "Author's name. Can be defined more then one time "
				"to specify different authors of the selection." ), true, true );
		QtArg after( QLatin1String( "after" ),
			QLatin1String( "Show stat for commits more recent "
						   "than a specific date. Use git format of the date." ),
				false, true );
		QtArg before( QLatin1String( "before" ),
			QLatin1String( "Show stat for commits older than "
						   "a specific date. Use git format of the date." ),
			false, true );

		QtArgHelp help;
		help.printer()->setExecutableName( argv[ 0 ] );
		help.printer()->setProgramDescription( QLatin1String( "Show statistics "
			"for Git repository." ) );

		QtArgCmdLine cmd( argc, argv );

		cmd.addParseable( a );
		cmd.addParseable( help );
		cmd.addParseable( before );
		cmd.addParseable( after );

		cmd.parse();

		authorName = a.values();

		if( before.isDefined() )
			beforeDate = before.value();

		if( after.isDefined() )
			afterDate = after.value();
	}
	catch( const QtArgHelpHasPrintedEx & )
	{
		return 0;
	}
	catch( const QtArgBaseException & x )
	{
		qDebug() << x.whatAsQString();

		return 1;
	}

	QCoreApplication app( argc, argv );

	QProcess p;

	QStringList args;
	args << QLatin1String( "log" );

	if( !beforeDate.isEmpty() )
		args << QString( "--before=" ) + beforeDate;

	if( !afterDate.isEmpty() )
		args << QString( "--after=" ) + afterDate;

	args << QLatin1String( "--shortstat" );

	p.start( QLatin1String( "git" ), args );

	p.waitForFinished();

	QString data = QString::fromLocal8Bit( p.readAll() );

	QTextStream outStream( stdout );

	QList< QRegExp > author;

	foreach( const QString & a, authorName )
		author.append( QRegExp( QString( "^Author: " ) +
			a + QLatin1String( ".*$" ) ) );

	const QRegExp date(
		QLatin1String( "^Date:\\s+(\\w+)\\s+(\\w+)\\s+(\\d+)"
					   "\\s+\\d+:\\d+:\\d+\\s+(\\d+).*$" ) );
	const QRegExp insReg(
		QLatin1String( "^.*(\\d+) insertion.*$" ) );
	const QRegExp delReg(
		QLatin1String( "^.*(\\d+) deletion.*$" ) );

	QStringList lines = data.split( QLatin1Char( '\n' ) );

	int d = 0;
	int y = 0;
	int ins = 0;
	int del = 0;
	int totalAdded = 0;
	int totalDeleted = 0;
	int totalDays = 0;

	QString weekDay;
	QString month;

	while( !lines.isEmpty() )
	{
		QString line = lines.takeFirst();

		foreach( const QRegExp & r, author )
			if( r.exactMatch( line ) )
			{
				if( !lines.isEmpty() )
				{
					line = lines.takeFirst();

					if( date.indexIn( line ) != -1 )
					{
						const QString tmpWeekDay = date.cap( 1 );
						const QString mTmp = date.cap( 2 );
						const int tmp = date.cap( 3 ).toInt();
						const int yTmp = date.cap( 4 ).toInt();

						if( d )
						{
							if( d != tmp || y != yTmp || month != mTmp )
							{
								outStream << weekDay << QLatin1Char( ' ' )
									<< month << QLatin1Char( ' ' )
									<< d << QLatin1Char( ' ' ) << y
									<< QLatin1String( " added " )
									<< ins
									<< QLatin1String( " deleted " )
									<< del << endl;

								totalAdded += ins;
								totalDeleted += del;
								++totalDays;

								d = tmp;
								y = yTmp;
								month = mTmp;
								weekDay = tmpWeekDay;

								ins = 0;
								del = 0;
							}
						}
						else
						{
							d = tmp;
							y = yTmp;
							month = mTmp;
							weekDay = tmpWeekDay;
						}

						while( !lines.isEmpty() )
						{
							line = lines.takeFirst();

							bool matched = false;

							if( insReg.indexIn( line ) != -1 )
							{
								ins += insReg.cap( 1 ).toInt();

								matched = true;
							}

							if( delReg.indexIn( line ) != -1 )
							{
								del += delReg.cap( 1 ).toInt();

								matched = true;
							}

							if( matched )
								break;
						}
					}
					else
						break;
				}
			}
	}

	if( d )
	{
		outStream << weekDay << QLatin1Char( ' ' )
			<< month << QLatin1Char( ' ' )
			<< d << QLatin1Char( ' ' ) << y
			<< QLatin1String( " added " )
			<< ins
			<< QLatin1String( " deleted " )
			<< del << endl;

		totalAdded += ins;
		totalDeleted += del;
		++totalDays;
	}

	outStream << QLatin1String( "----------" ) << endl
		<< QLatin1String( "Total:" ) << endl
		<< QLatin1String( "----------" ) << endl
		<< QLatin1String( "Added " ) << totalAdded
		<< QLatin1String( " line(s)" ) << endl
		<< QLatin1String( "Deleted " ) << totalDeleted
		<< QLatin1String( " line(s)" ) << endl
		<< totalDays <<  QLatin1String( " day(s)" ) << endl;

	return 0;
}
