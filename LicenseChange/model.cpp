
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
#include "model.hpp"

// Qt include.
#include <QMap>
#include <QPersistentModelIndex>
#include <QApplication>


//
// ModelPrivate
//

class ModelPrivate {
public:
	ModelPrivate( Model * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init();

	//! Checked indexes.
	QMap< QString, bool > m_map;
	//! Indexes of directories that are loading now.
	QList< QString > m_loading;
	//! Parent.
	Model * q;
}; // class ModelPrivate

void
ModelPrivate::init()
{
	Model::connect( q, &QFileSystemModel::directoryLoaded,
		q, &Model::pathReady );
}


//
// Model
//

Model::Model( QObject * parent )
	:	QFileSystemModel( parent )
	,	d( new ModelPrivate( this ) )
{
	d->init();
}

Model::~Model()
{
}

void
Model::check( const QModelIndex & idx, bool checked, bool topLevel )
{
	if( topLevel )
	{
		emit loadingStarted();

		QCoreApplication::processEvents();
	}

	d->m_map[ filePath( idx ) ] = checked;

	emit dataChanged( idx, idx );

	bool stop = true;

	if( isDir( idx ) )
	{
		if( canFetchMore( idx ) )
		{
			stop = false;

			d->m_loading.append( filePath( idx ) );

			fetchMore( idx );
		}
		else
		{
			const int count = rowCount( idx );

			for( int r = 0; r < count; ++r )
				check( index( r, 0, idx ), checked, false );
		}
	}

	if( topLevel && stop )
		emit loadingFinished();
}

void
Model::clear()
{
	const QStringList indexes = d->m_map.keys();

	d->m_map.clear();

	foreach( const QString & path, indexes )
	{
		const QModelIndex idx = index( path );

		emit dataChanged( idx, idx );
	}
}

void
Model::stopLoading()
{
	d->m_loading.clear();
}

QVariant
Model::data( const QModelIndex & index, int role ) const
{
	if( role == Qt::CheckStateRole && index.column() == 0 )
	{
		const QString path = filePath( index );
		if( d->m_map.contains( path ) )
			return ( d->m_map[ path ] ? Qt::Checked : Qt::Unchecked );
		else
			return Qt::Unchecked;
	}
	else
		return QFileSystemModel::data( index, role );
}

bool
Model::setData( const QModelIndex & idx, const QVariant & value, int role )
{
	if( idx.column() == 0  && role == Qt::CheckStateRole )
	{
		check( idx,
			( static_cast< Qt::CheckState > ( value.toInt() ) == Qt::Checked ?
				true : false ) );

		return true;
	}
	else
		return QFileSystemModel::setData( idx, value, role );
}

Qt::ItemFlags
Model::flags( const QModelIndex & index ) const
{
	if( index.column() == 0 )
		return QFileSystemModel::flags( index ) | Qt::ItemIsUserCheckable;
	else
		return QFileSystemModel::flags( index );
}

void
Model::pathReady( const QString & path )
{
	const QModelIndex idx = index( path );

	if( idx.isValid() && d->m_loading.contains( path ) )
	{
		d->m_loading.removeOne( path );

		check( idx, true, false );
	}

	if( d->m_loading.isEmpty() )
		emit loadingFinished();
}
