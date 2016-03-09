
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


int main( int argc, char ** argv )
{
	QString authorName;
	QString beforeDate;
	QString afterDate;

	try {
		QtArg a( QLatin1Char( 'a' ), QLatin1String( "author" ),
			QLatin1String( "Author's name." ), true, true );
		QtArg after( QLatin1String( "after" ),
			QLatin1String( "Show stat for commits more recent "
						   "than a specific date." ), false, true );
		QtArg before( QLatin1String( "before" ),
			QLatin1String( "Show stat for commits older than "
						   "a specific date." ), false, true );

		QtArgHelp help;

		QtArgCmdLine cmd( argc, argv );

		cmd.addParseable( a );
		cmd.addParseable( help );
		cmd.addParseable( before );
		cmd.addParseable( after );

		cmd.parse();

		authorName = a.value();

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
	const QRegExp author( QString( "^Author: " ) +
		authorName + QLatin1String( ".*$" ) );

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

	QString weekDay;
	QString month;

	while( !lines.isEmpty() )
	{
		QString line = lines.takeFirst();

		if( author.exactMatch( line ) )
		{
			if( !lines.isEmpty() )
			{
				line = lines.takeFirst();

				if( date.indexIn( line ) != -1 )
				{
					weekDay = date.cap( 1 );
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

							d = tmp;
							y = yTmp;
							month = mTmp;

							ins = 0;
							del = 0;
						}
					}
					else
					{
						d = tmp;
						y = yTmp;
						month = mTmp;
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
		outStream << weekDay << QLatin1Char( ' ' )
			<< month << QLatin1Char( ' ' )
			<< d << QLatin1Char( ' ' ) << y
			<< QLatin1String( " added " )
			<< ins
			<< QLatin1String( " deleted " )
			<< del << endl;

	return 0;
}
