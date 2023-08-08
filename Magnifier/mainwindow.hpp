
/*!
	\file

	\author Igor Mironchik (igor.mironchik at gmail dot com).

	Copyright (c) 2017-2023 Igor Mironchik

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

#pragma once

// Qt include.
#include <QWidget>
#include <QToolButton>
#include <QBitmap>
#include <QFrame>
#include <QAbstractButton>
#include <QComboBox>


class MainWindow;

//
// ResizeHandle
//

//! Resize handle.
class ResizeHandle
	:	public QFrame
{
	Q_OBJECT

public:
	enum Orientation {
		Horizontal = 0,
		Vertical,
		TopLeftBotomRight,
		BottomLeftTopRight
	};

	ResizeHandle( Orientation o, bool withMove, QWidget * parent, MainWindow * obj );
	~ResizeHandle() override = default;

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

protected:
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;
	void mouseMoveEvent( QMouseEvent * e ) override;

private:
	void handleMouseMove( QMouseEvent * e );

private:
	Q_DISABLE_COPY( ResizeHandle )

	MainWindow * m_obj = nullptr;
	Orientation m_orient = Orientation::Horizontal;
	bool m_leftButtonPressed = false;
	bool m_withMove = false;
	QPointF m_pos = { 0.0, 0.0 };
}; // class ResizeHandle


//
// Title
//

//! Title widget.
class TitleWidget
	:	public QFrame
{
	Q_OBJECT

public:
	TitleWidget( QWidget * parent, MainWindow * obj );
	~TitleWidget() override = default;

protected:
	void mousePressEvent( QMouseEvent * e ) override;
	void mouseReleaseEvent( QMouseEvent * e ) override;
	void mouseMoveEvent( QMouseEvent * e ) override;

private:
	void handleMouseMove( QMouseEvent * e );

private:
	Q_DISABLE_COPY( TitleWidget )

	MainWindow * m_obj = nullptr;
	bool m_leftButtonPressed = false;
	QPointF m_pos = { 0.0, 0.0 };
}; // class TitleWidget


//
// CloseButton
//

//! Close button.
class CloseButton
	:	public QAbstractButton
{
	Q_OBJECT

public:
	explicit CloseButton( QWidget * parent );
	~CloseButton() override = default;

	QSize sizeHint() const override;

protected:
	void paintEvent( QPaintEvent * e ) override;
	void enterEvent( QEnterEvent * event ) override;
	void leaveEvent( QEvent * event ) override;

private:
	Q_DISABLE_COPY( CloseButton )

	bool m_hovered = false;
	QPixmap m_activePixmap;
	QPixmap m_inactivePixmap;
}; // class CloseButton


class EventMonitor;


//
// MainWindow
//

//! Main window.
class MainWindow
	:	public QWidget
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow() override = default;

protected:
	void resizeEvent( QResizeEvent * e ) override;

private slots:
	void onMagnify();

private:
	Q_DISABLE_COPY( MainWindow )

	TitleWidget * m_title = nullptr;
	QWidget * m_recordArea = nullptr;
	QWidget * m_c = nullptr;
	QToolButton * m_recordButton = nullptr;
	CloseButton * m_closeButton = nullptr;
	QComboBox * m_mult = nullptr;
	QBitmap m_mask;
};// class MainWindow
