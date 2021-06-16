/*
 * Hydrogen
 * Copyright(c) 2002-2008 by Alex >Comix< Cominu [comix@users.sourceforge.net]
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef LCDDisplay_H
#define LCDDisplay_H


#include <QtGui>
#include <QtWidgets>
#include <QLineEdit>

#include <core/Object.h>
#include <core/Preferences.h>

#include "WidgetWithScalableFont.h"

#include <vector>

class LCDDisplay : public QLineEdit, protected WidgetWithScalableFont<6, 8, 9>, public H2Core::Object
{
    H2_OBJECT
	Q_OBJECT

public:
	LCDDisplay( QWidget* pParent, QSize size);
	~LCDDisplay();

	// void setText( float fValue );

	void setRedFont();
	void setDefaultFont();

public slots:
	void onPreferencesChanged( bool bAppearanceOnly );

private:
	void updateFont();
	QSize m_size;

	bool m_bEntered;

	/** Used to detect changed in the font*/
	QString m_sLastUsedFontFamily;
	/** Used to detect changed in the font*/
	H2Core::Preferences::FontSize m_lastUsedFontSize;
	
	virtual void paintEvent( QPaintEvent *ev );
	virtual void enterEvent( QEvent *ev );
	virtual void leaveEvent( QEvent *ev );
};

#endif