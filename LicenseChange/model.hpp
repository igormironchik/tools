
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

#ifndef LICENSECHANGE__MODEL_HPP__INCLUDED
#define LICENSECHANGE__MODEL_HPP__INCLUDED

// Qt include.
#include <QFileSystemModel>
#include <QScopedPointer>


//
// Model
//

class ModelPrivate;

//! File system model with check boxes.
class Model Q_DECL_FINAL
	:	public QFileSystemModel
{
	Q_OBJECT

signals:
	//! Loading of directories started.
	void loadingStarted();
	//! Loading of directories finished.
	void loadingFinished();

public:
	Model( QObject * parent = Q_NULLPTR );
	~Model();

	//! Check item and its children if they exist.
	void check( const QModelIndex & index, bool checked = true,
		bool topLevel = true );
	//! Clear model.
	void clear();

	QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const
		Q_DECL_OVERRIDE;
	bool setData( const QModelIndex & idx, const QVariant & value,
		int role = Qt::EditRole ) Q_DECL_OVERRIDE;
	Qt::ItemFlags flags( const QModelIndex & index ) const
		Q_DECL_OVERRIDE;

public slots:
	//! Stop loading directories.
	void stopLoading();

private slots:
	//! Directory loaded.
	void pathReady( const QString & path );

private:
	friend class ModelPrivate;

	Q_DISABLE_COPY( Model )

	QScopedPointer< ModelPrivate > d;
}; // class Model

#endif // LICENSECHANGE__MODEL_HPP__INCLUDED
