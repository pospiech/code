#include "WidgetMessageList.h"

#include <QGridLayout>

#include <QDebug>
#include "Logger.h"


WidgetMessageList::WidgetMessageList(QWidget *parent) : QWidget(parent)
{
    listView = new QListView(this);
    buttonClear = new QPushButton(this);
    buttonClear->setText("clear list");

    // Create a layout and add widget
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(listView   , 0 ,0, 1, 2);
    layout->addWidget(buttonClear, 1, 1);

    // Create model
    model = new QStringListModel(this);

    // Make data
    QStringList List;
    // Populate our model
    model->setStringList(List);
    // Glue model and view together
    listView->setModel(model);
}

WidgetMessageList::~WidgetMessageList()
{

}

void WidgetMessageList::addString(const QString & message)
{
    // from : https://stackoverflow.com/questions/37412712/how-to-update-a-qstringlistmodel
    if(model->insertRow(model->rowCount())) {
        QModelIndex index = model->index(model->rowCount() - 1, 0);
        model->setData(index, message);
    }
}
