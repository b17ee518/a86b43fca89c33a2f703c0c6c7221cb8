#ifndef CUSTOMTABLEDELEGATE_H
#define CUSTOMTABLEDELEGATE_H

#include <QStyledItemDelegate>
#include <QTableWidgetItem>
#include <QPen>

// bit
enum {
	CustomTableDelegate_Left = 1,
	CustomTableDelegate_Right = 2,
	CustomTableDelegate_Top = 4,
	CustomTableDelegate_Bottom = 8,
};

class CustomTableDelegate : public QStyledItemDelegate
{
public:
    CustomTableDelegate(QTableWidget* tableWidget);
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
    QPen _gridPen;

    QBrush _orangeBrush;
    QBrush _yellowBrush;
    QBrush _greenBrush;
};


#endif // CUSTOMTABLEDELEGATE_H
