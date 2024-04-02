
/*
	SPDX-FileCopyrightText: 2016-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: GPL-3.0-or-later
*/

#ifndef LICENSECHANGE__TEXTEDIT_HPP__INCLUDED
#define LICENSECHANGE__TEXTEDIT_HPP__INCLUDED

// Qt include.
#include <QTextEdit>
#include <QScopedPointer>


static const QString c_skipLine = QLatin1String( "data://skip-line.png" );
static const QString c_skipWord = QLatin1String( "data://skip-word.png" );


//
// TextEdit
//

class TextEditPrivate;

//! Text editor.
class TextEdit Q_DECL_FINAL
	:	public QTextEdit
{
	Q_OBJECT

signals:
	//! Focus received.
	void focusReceived();
	//! Focus lost.
	void focusLost();

public:
	explicit TextEdit( QWidget * parent = Q_NULLPTR );
	~TextEdit();

public slots:
	//! Insert skip line.
	void insertSkipLine();
	//! Insert skip word.
	void insertSkipWord();

protected:
	void focusInEvent( QFocusEvent * event ) Q_DECL_OVERRIDE;
	void focusOutEvent( QFocusEvent * event ) Q_DECL_OVERRIDE;

private:
	Q_DISABLE_COPY( TextEdit )

	QScopedPointer< TextEditPrivate > d;
}; // class TextEdit

#endif // LICENSECHANGE__TEXTEDIT_HPP__INCLUDED
