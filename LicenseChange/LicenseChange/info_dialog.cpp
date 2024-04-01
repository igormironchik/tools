
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2022 Igor Mironchik

	Permission is hereby granted, free of charge, to any person
	obtaining a copy of this software and associated documentation
	files (the "Software"), to deal in the Software without
	restriction, including without limitation the rights to use,
	copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the
	Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
	OTHER DEALINGS IN THE SOFTWARE.
*/

// Widgets include.
#include "info_dialog.hpp"
#include "ui_info_dialog.h"

// Qt include.
#include <QLabel>
#include <QStyle>
#include <QDialogButtonBox>
#include <QPushButton>


//
// InfoDialogPrivate
//

class InfoDialogPrivate {
public:
	InfoDialogPrivate()
	{
	}

	void init( InfoDialog * parent, const QString & title,
		const QString & msg, const QString & details )
	{
		q = parent;
		ui.setupUi( q );
		q->setWindowTitle( title );
		const auto i = q->style()->standardIcon( QStyle::SP_MessageBoxInformation, nullptr, q );
		const int iconSize = q->style()->pixelMetric( QStyle::PM_MessageBoxIconSize, nullptr, q );
		const qreal dpr = q->devicePixelRatio();
		ui.pixmap->setPixmap( i.pixmap( QSize( iconSize, iconSize ), dpr ) );
		ui.msg->setText( msg );
		ui.details->setPlainText( details );
		ui.details->hide();
		showHide = ui.buttonBox->addButton( InfoDialog::tr( "Show Details..." ),
			QDialogButtonBox::ActionRole );
		InfoDialog::connect( showHide, &QPushButton::clicked,
			[this]()
			{
				if( this->ui.details->isVisible() )
				{
					this->showHide->setText( InfoDialog::tr( "Show Details..." ) );
					this->ui.details->hide();
				}
				else
				{
					this->showHide->setText( InfoDialog::tr( "Hide Details..." ) );
					this->ui.details->show();
				}
			} );

		q->setSizeGripEnabled( true );
		q->adjustSize();
		
		auto pp = static_cast< QWidget* > ( q->parent() );
		
		q->resize( std::max( q->width(), pp->width() / 2 ), std::max( q->height(), pp->height() / 2 ) );
		q->move( pp->width() / 2 - q->width() / 2, pp->height() / 2 - q->height() / 2 );
	}

	//! Parent.
	InfoDialog * q = nullptr;
	//! Ui.
	Ui::InfoDialog ui;
	//! Show/hide button.
	QPushButton * showHide = nullptr;
}; // class InfoDialogPrivate



//
// InfoDialog
//

InfoDialog::InfoDialog( const QString & title, const QString & msg,
	const QString & details, QWidget * parent )
	:	QDialog( parent )
	,	d( new InfoDialogPrivate )
{
	d->init( this, title, msg, details );
}

InfoDialog::~InfoDialog()
{
}
