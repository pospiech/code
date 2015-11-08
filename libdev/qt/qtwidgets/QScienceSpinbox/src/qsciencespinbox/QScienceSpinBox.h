/**
 * QScienceSpinBox
 *
 *  Copyright 2015 by Matthias Pospiech <matthias@pospiech.eu>
 *
 * This file is part of some open source application.
 *
 * QScienceSpinBox is free software: you can redistribute
 * it and/or modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * QScienceSpinBox is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QScienceSpinBox. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __QScienceSpinBox_H__
#define __QScienceSpinBox_H__

#include <QDoubleSpinBox>
#include <QString>

#include <QScopedPointer>

class QScienceSpinBoxPrivate;

class QScienceSpinBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    QScienceSpinBox(QWidget * parent = 0);
    ~QScienceSpinBox();

    int decimals() const;
    void setDecimals(int value);

    QString textFromValue ( double value ) const;
    double valueFromText ( const QString & text ) const;


protected:
    //const QScopedPointer<QScienceSpinBoxPrivate> d_ptr;

private:
    //Q_DECLARE_PRIVATE(QScienceSpinBox)

private:
    QScienceSpinBoxPrivate* d;

private:
    // overwritten virtual function of QAbstractSpinBox
    QValidator::State validate(QString &text, int &pos) const;
    void fixup(QString &input) const;
    void stepBy(int steps);

public slots:
    void stepDown();
    void stepUp();

};

#endif
