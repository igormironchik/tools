
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

#ifndef MAGNIFIER__CONSTANTS_HPP__INCLUDED
#define MAGNIFIER__CONSTANTS_HPP__INCLUDED

// Qt include.
#include <QSize>

//! Size of the handler, i.e. diameter.
static const int c_dim = 20;

//! Offset.
static const int c_delta = 25;

//! Minimus size of the window.
static const QSize c_minSize = QSize(
	c_dim * 5 + c_delta * 3, c_dim * 3 + c_delta * 5 );

//! Window offset.
static const int c_windowOffset = c_delta + c_dim / 2 + ( c_dim % 2 ? 1 : 0 ) + 2;

#endif // MAGNIFIER__CONSTANTS_HPP__INCLUDED
