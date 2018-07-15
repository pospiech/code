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

private:
    QListView * listView;
    QPushButton * buttonClear;
    QStringListModel *model;
};

#endif // WIDGETMESSAGELIST_H
