
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017 Igor Mironchik

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

// LicenseChange include.
#include "ui_opts_dialog.h"
#include "opts_dialog.hpp"


//
// OptsDialogPrivate
//

class OptsDialogPrivate {
public:
	OptsDialogPrivate( OptsDialog * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init();

	//! UI.
	Ui::OptsDialog m_ui;
	//! Parent.
	OptsDialog * q;
}; // class OptsDialogPrivate;

void
OptsDialogPrivate::init()
{
	m_ui.setupUi( q );
}


//
// OptsDialog
//

OptsDialog::OptsDialog( QWidget * parent )
	:	QDialog( parent )
	,	d( new OptsDialogPrivate( this ) )
{
	d->init();
}

OptsDialog::~OptsDialog()
{
}
