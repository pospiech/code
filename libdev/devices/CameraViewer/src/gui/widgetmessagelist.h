#ifndef WIDGETMESSAGELIST_H
#define WIDGETMESSAGELIST_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

class WidgetMessageList : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetMessageList(QWidget *parent = nullptr);
    ~WidgetMessageList();

signals:

public slots:
    void addString(const QString & message);
    void on_pushButtonClearList_clicked();

private:
    QListView * listView;
    QPushButton * pushButtonClearList;
    QStringListModel *model;
};

#endif // WIDGETMESSAGELIST_H
