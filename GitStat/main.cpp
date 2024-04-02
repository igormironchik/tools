
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
#include <QRegularExpression>
#include <QDebug>
#include <QProcess>
#include <QCommandLineOption>
#include <QCommandLineParser>


int main( int argc, char ** argv )
{
	QCoreApplication app( argc, argv );
	
	
	QStringList authorName;
	QString beforeDate;
	QString afterDate;
	
	QCommandLineParser parser;
	
	QCommandLineOption authorOpt( QStringList() << QStringLiteral( "a" )
			<< QStringLiteral( "author" ),
		QStringLiteral( "Author's name. Can be defined more then "
			"one time to specify different authors of the selection." ),
		QStringLiteral( "name or email" ) );
	parser.addOption( authorOpt );
	
	QCommandLineOption bDateOpt( QStringLiteral( "before" ),
		QStringLiteral( "Show stat for commits older than "
			"a specific date. Use git format of the date." ),
		QStringLiteral( "date" ) );
	parser.addOption( bDateOpt );
	
	QCommandLineOption aDateOpt( QStringLiteral( "after" ),
		QStringLiteral( "Show stat for commits more recent "
			"than a specific date. Use git format of the date." ),
		QStringLiteral( "date" ) );
	parser.addOption( aDateOpt );
	
	parser.addHelpOption();
	parser.setApplicationDescription( QStringLiteral(
		"Maybe useful git statistics for the given author" ) );
	
	parser.process( app );
	
	if( !parser.isSet( QStringLiteral( "author" ) ) )
	{
		qDebug() << "Please define author.";
		
		qDebug() << parser.helpText();
		
		return 1;
	}
	else
		authorName = parser.values( QStringLiteral( "author" ) );
	
	if( parser.isSet( QStringLiteral( "before" ) ) )
		beforeDate = parser.value( QStringLiteral( "before" ) );

	if( parser.isSet( QStringLiteral( "after" ) ) )
		afterDate = parser.value( QStringLiteral( "after" ) );

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

	QList< QRegularExpression > author;

	foreach( const QString & a, authorName )
		author.append( QRegularExpression( QString( "^Author: " ) +
			a + QLatin1String( ".*$" ) ) );

	const QRegularExpression date(
		QLatin1String( "^Date:\\s+(\\w+)\\s+(\\w+)\\s+(\\d+)"
					   "\\s+\\d+:\\d+:\\d+\\s+(\\d+).*$" ) );
	const QRegularExpression insReg(
		QLatin1String( "^.*(\\d+) insertion.*$" ) );
	const QRegularExpression delReg(
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

		foreach( const auto & r, author )
		{
			const auto match = r.match( line );

			if( match.hasMatch() )
			{
				if( !lines.isEmpty() )
				{
					line = lines.takeFirst();

					const auto dateMatch = date.match( line );

					if( dateMatch.hasMatch() )
					{
						const QString tmpWeekDay = dateMatch.captured( 1 );
						const QString mTmp = dateMatch.captured( 2 );
						const int tmp = dateMatch.captured( 3 ).toInt();
						const int yTmp = dateMatch.captured( 4 ).toInt();

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
									<< del << Qt::endl;

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

							const auto insMatch = insReg.match( line );

							if( insMatch.hasMatch() )
							{
								ins += insMatch.captured( 1 ).toInt();

								matched = true;
							}

							const auto delMatch = delReg.match( line );

							if( delMatch.hasMatch() )
							{
								del += delMatch.captured( 1 ).toInt();

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
	}

	if( d )
	{
		outStream << weekDay << QLatin1Char( ' ' )
			<< month << QLatin1Char( ' ' )
			<< d << QLatin1Char( ' ' ) << y
			<< QLatin1String( " added " )
			<< ins
			<< QLatin1String( " deleted " )
			<< del << Qt::endl;

		totalAdded += ins;
		totalDeleted += del;
		++totalDays;
	}

	outStream << QLatin1String( "----------" ) << Qt::endl
		<< QLatin1String( "Total:" ) << Qt::endl
		<< QLatin1String( "----------" ) << Qt::endl
		<< QLatin1String( "Added " ) << totalAdded
		<< QLatin1String( " line(s)" ) << Qt::endl
		<< QLatin1String( "Deleted " ) << totalDeleted
		<< QLatin1String( " line(s)" ) << Qt::endl
		<< totalDays <<  QLatin1String( " day(s)" ) << Qt::endl;

	return 0;
}
