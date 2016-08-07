
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

#ifndef LICENSECHANGE__TEXTEDIT_HPP__INCLUDED
#define LICENSECHANGE__TEXTEDIT_HPP__INCLUDED

// Qt include.
#include <QPlainTextEdit>
#include <QScopedPointer>


//
// TextEdit
//

class TextEditPrivate;

//! Text editor.
class TextEdit Q_DECL_FINAL
	:	public QPlainTextEdit
{
	Q_OBJECT

signals:
	//! Focus received.
	void focusReceived();
	//! Focus lost.
	void focusLost();

public:
	TextEdit( QWidget * parent = Q_NULLPTR );
	~TextEdit();

protected:
	void focusInEvent( QFocusEvent * event ) Q_DECL_OVERRIDE;
	void focusOutEvent( QFocusEvent * event ) Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( TextEdit )

	QScopedPointer< TextEditPrivate > d;
}; // class TextEdit

#endif // LICENSECHANGE__TEXTEDIT_HPP__INCLUDED
