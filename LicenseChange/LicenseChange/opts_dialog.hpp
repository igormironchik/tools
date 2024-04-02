
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef LICENSECHANGE__OPTS_DIALOG_HPP__INCLUDED
#define LICENSECHANGE__OPTS_DIALOG_HPP__INCLUDED

// Qt include.
#include <QDialog>
#include <QScopedPointer>


//
// OptsDialog
//

class OptsDialogPrivate;

//! Dialog with search settings.
class OptsDialog
	:	public QDialog
{
	Q_OBJECT

public:
	explicit OptsDialog( QWidget * parent );
	~OptsDialog();

	//! \return Should only one license be changed?
	bool onlyOneLicense() const;

	//! \return Is search case sensitive.
	bool isCaseSensitive() const;

private:
	Q_DISABLE_COPY( OptsDialog )

	QScopedPointer< OptsDialogPrivate > d;
}; // class OptsDialog

#endif // LICENSECHANGE__OPTS_DIALOG_HPP__INCLUDED
