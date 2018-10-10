#include "WidgetMessageList.h"

#include <QGridLayout>

#include <QDebug>
#include "Logger.h"


WidgetMessageList::WidgetMessageList(QWidget *parent) : QWidget(parent)
{
    listView = new QListView(this);
    pushButtonClearList = new QPushButton(this);
    pushButtonClearList->setText("clear list");

    // Create a layout and add widget
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(listView   , 0 ,0, 1, 2);
    layout->addWidget(pushButtonClearList, 1, 1);

    // Create model
    model = new QStringListModel(this);

    // Make data
    QStringList List;
    // Populate our model
    model->setStringList(List);
    // Glue model and view together
    listView->setModel(model);


    // connect
    connect(pushButtonClearList, &QPushButton::clicked,
            this, &WidgetMessageList::on_pushButtonClearList_clicked );
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
        listView->scrollTo(index);
    }
}

void WidgetMessageList::on_pushButtonClearList_clicked()
{
    model->setStringList( QStringList{} );
//    model->removeRows( 0, model->rowCount() );
}
