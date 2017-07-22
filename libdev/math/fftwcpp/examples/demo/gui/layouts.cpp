#include "layouts.h"
#include <QWidget>
#include <QLayoutItem>


void clearLayout(QLayout* layout, bool deleteWidgets)
{
    while (QLayoutItem* child = layout->takeAt(0))
    {
        // delete widget in layout
        if (deleteWidgets)
        {
            if (QWidget* widget = child->widget())
                widget->deleteLater();
        }
        // recursive delete further layouts
        if (QLayout* childLayout = child->layout())
            clearLayout(childLayout, deleteWidgets);
        // delete layout
        delete child;
    }
}
