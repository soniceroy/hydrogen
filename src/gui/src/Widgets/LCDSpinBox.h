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

#ifndef LCDSPINBOX_H
#define LCDSPINBOX_H

#include <QtGui>
#include <QDoubleSpinBox>

#include <core/Object.h>
#include <core/Preferences.h>

/** Custom spin box.
 *
 * The QDoubleSpinBox is used for both integer and float values
 * instead of using QSpinBox as a base for the former. This is done to
 * keep the code concise. All internal values will be handled as
 * floats and only textFromValue() makes a difference.
 *
 * The LCDSpinBox::Kind was introduced to handle custom constraints
 * for the widgets concerning with the pattern size in the
 * PatternEditorPanel.
 *
 * Updating the font family in QDoubleSpinBox is not supported and
 * changing the font size (both via setFont()) yields erratic
 * results.
 */
/** \ingroup docGUI docWidgets*/
class LCDSpinBox : public QDoubleSpinBox, public H2Core::Object<LCDSpinBox>
{
    H2_OBJECT(LCDSpinBox)
	Q_OBJECT

public:

	enum class Type {
		Int,
		Double
	};

	enum class Kind {
		/** Behaves like QDoubleSpinBox.*/
		Default,
		/** The minimum value - a fractional one - can only be reached
		 * by entering it using the keyboard. up/down keys as well as
		 * mouse wheel increment the number by one and will stay at
		 * integer values per default.*/
		PatternSizeNumerator,
		/** Only a limited number of values is allowed.*/
		PatternSizeDenominator
	};

	LCDSpinBox( QWidget *pParent, QSize size, Type type, double fMin = 0.0, double fMax = 1.0 );
	~LCDSpinBox();

	void setKind( Kind kind );
	QValidator::State validate( QString &text, int &pos ) const;

public slots:
	void onPreferencesChanged( H2Core::Preferences::Changes changes );

signals:
	void slashKeyPressed();
	
private:
	void updateStyleSheet();
	double nextValueInPatternSizeDenominator( bool bUp, bool bAccelerated );
	QSize m_size;
	Type m_type;
	Kind m_kind;
	
	bool m_bEntered;

	virtual QString textFromValue( double fValue ) const override;
	virtual double valueFromText( const QString& sText ) const override;	
	virtual void paintEvent( QPaintEvent *ev ) override;
	virtual void enterEvent( QEvent *ev ) override;
	virtual void leaveEvent( QEvent *ev ) override;
	virtual void wheelEvent( QWheelEvent *ev ) override;
	virtual void keyPressEvent( QKeyEvent *ev ) override;
	virtual bool event( QEvent* ev ) override;
};

inline void LCDSpinBox::setKind( Kind kind ) {
	m_kind = kind;
}
#endif
