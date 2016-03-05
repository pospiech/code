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

/** DLL Creation Defintion */
#include <QtCore/QtGlobal>
#ifdef QSCIENCESPINBOX_DLL

#ifdef QSCIENCESPINBOX_MAKEDLL     // create a DLL library
#define QSCIENCESPINBOX_EXPORT Q_DECL_EXPORT
#else                                   // use a DLL library
#define QSCIENCESPINBOX_EXPORT Q_DECL_IMPORT
#endif

#endif // QSCIENCESPINBOX_DLL

#ifndef QSCIENCESPINBOX_EXPORT
#define QSCIENCESPINBOX_EXPORT
#endif


#include <QDoubleSpinBox>
#include <QString>
#include <QScopedPointer>

class QScienceSpinBoxPrivate;

class QSCIENCESPINBOX_EXPORT QScienceSpinBox : public QDoubleSpinBox
{
    Q_OBJECT

public:
    QScienceSpinBox(QWidget * parent = 0);
    virtual ~QScienceSpinBox();

    int decimals() const;
    void setDecimals(int value);

    QString textFromValue ( double value ) const;
    double valueFromText ( const QString & text ) const;

public slots:

    void stepDown();
    void stepUp();

protected:
    const QScopedPointer<QScienceSpinBoxPrivate> d_ptr;

private:
    // declares QScienceSpinBoxPrivate d_func()
    // requires d_ptr
    Q_DECLARE_PRIVATE(QScienceSpinBox)

private:
    // overwritten virtual function of QAbstractSpinBox
    QValidator::State validate(QString &text, int &pos) const;
    void fixup(QString &input) const;
    void stepBy(int steps);

};

#endif
