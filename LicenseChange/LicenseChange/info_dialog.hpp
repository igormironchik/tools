
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

// Qt include.
#include <QDialog>
#include <QWidget>


//
// InfoDialog
//

class InfoDialogPrivate;

//! Information dialog.
class InfoDialog
	:	public QDialog
{
	Q_OBJECT

public:
	InfoDialog( const QString & title, const QString & msg, const QString & details,
		QWidget * parent = nullptr );
	~InfoDialog() override;

private:
	QScopedPointer< InfoDialogPrivate > d;

	Q_DISABLE_COPY( InfoDialog )
}; // class InfoDialog
