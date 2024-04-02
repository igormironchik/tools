
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

// LicenseChange include.
#include "textedit.hpp"


//
// TextEditPrivate
//

class TextEditPrivate {
public:
	explicit TextEditPrivate( TextEdit * parent )
		:	q( parent )
	{
	}

	//! Init.
	void init();

	//! Parent.
	TextEdit * q;
}; // class TextEditPrivate

void
TextEditPrivate::init()
{
	QImage skipLine( QLatin1String( ":/img/code-function.png" ) );

	q->document()->addResource( QTextDocument::ImageResource,
		QUrl( c_skipLine ), QVariant( skipLine ) );

	QImage skipWord( QLatin1String( ":/img/code-variable.png" ) );

	q->document()->addResource( QTextDocument::ImageResource,
		QUrl( c_skipWord ), QVariant( skipWord ) );
}


//
// TextEdit
//

TextEdit::TextEdit( QWidget * parent )
	:	QTextEdit( parent )
	,	d( new TextEditPrivate( this ) )
{
	d->init();
}

TextEdit::~TextEdit()
{
}

void
TextEdit::insertSkipLine()
{
	QTextCursor c = textCursor();
	c.insertImage( c_skipLine );
}

void
TextEdit::insertSkipWord()
{
	QTextCursor c = textCursor();
	c.insertImage( c_skipWord );
}

void
TextEdit::focusInEvent( QFocusEvent * event )
{
	emit focusReceived();

	QTextEdit::focusInEvent( event );
}

void
TextEdit::focusOutEvent( QFocusEvent * event )
{
	emit focusLost();

	QTextEdit::focusOutEvent( event );
}
