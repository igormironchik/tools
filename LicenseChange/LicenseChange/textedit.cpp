
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
