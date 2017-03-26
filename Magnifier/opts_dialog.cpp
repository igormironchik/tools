
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

// Qt include.
#include <QSpinBox>
#include <QDialogButtonBox>


//
// OptsDialogPrivate
//

class OptsDialogPrivate {
public:
	OptsDialogPrivate( Cfg & cfg, OptsDialog * parent )
		:	m_cfg( cfg )
		,	q( parent )
	{
	}

	//! Init.
	void init();

	//! Cfg.
	Cfg & m_cfg;
	//! UI.
	Ui::OptsDialog m_ui;
	//! Parent.
	OptsDialog * q;
}; // class OptsDialogPrivate;

void
OptsDialogPrivate::init()
{
	m_ui.setupUi( q );

	m_ui.m_height->setValue( m_cfg.height() );
	m_ui.m_width->setValue( m_cfg.width() );
	m_ui.m_scale->setValue( m_cfg.scale() );

	OptsDialog::connect( m_ui.m_btns, &QDialogButtonBox::accepted,
		q, &OptsDialog::accepted );
}


//
// OptsDialog
//

OptsDialog::OptsDialog( Cfg & cfg, QWidget * parent )
	:	QDialog( parent )
	,	d( new OptsDialogPrivate( cfg, this ) )
{
	d->init();
}

OptsDialog::~OptsDialog()
{
}

void
OptsDialog::accepted()
{
	d->m_cfg.setHeight( d->m_ui.m_height->value() );
	d->m_cfg.setWidth( d->m_ui.m_width->value() );
	d->m_cfg.setScale( d->m_ui.m_scale->value() );

	accept();
}
