#include "MainWindow.h"


MainWindow::MainWindow( QWidget* parent /*= 0*/, Qt::WindowFlags flags /*= 0*/ ) : QMainWindow(parent, flags)
{
    setupUi(this);
    connect(scienceSpinBox, SIGNAL(valueChanged(double)), this,  SLOT(on_pushButton_clicked()));

    lineEdit->setText("120.497e-4");
    scienceSpinBox->setValue( lineEdit->text().toDouble() );
}


MainWindow::~MainWindow()
{
}



void MainWindow::on_pushButton_clicked(bool checked)
{
    QString str = QString::number( scienceSpinBox->value(), 'f', 1000);
    textEdit->setPlainText(str);
    QString str2 = QString::number( scienceSpinBox->value(), 'e',     scienceSpinBox->decimals());
    lineEditValue->setText(str2);
}
void MainWindow::on_pushButtonSet_clicked(bool checked)
{
    QString text = lineEdit->text();
    // replace german , notation with .
    // a better approach would be to use Qlocale, but then one
    // needs to know which convension is used
    text.replace(",", ".");
    scienceSpinBox->setValue( text.toDouble() );
}
