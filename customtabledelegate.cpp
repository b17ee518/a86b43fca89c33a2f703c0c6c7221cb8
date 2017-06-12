#include "customtabledelegate.h"

#include <QStyledItemDelegate>
#include <QPainter>

CustomTableDelegate::CustomTableDelegate(QTableWidget* tableWidget)
{
	//    int gridHint = tableWidget->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
	//    QColor gridColor = static_cast<QRgb>(gridHint);
	QColor gridColor(45, 45, 48);
	_gridPen = QPen(gridColor, 0, tableWidget->gridStyle());

	_orangeBrush = QBrush(QColor(0xff, 0x99, 0, 0x30));
	_yellowBrush = QBrush(QColor(0xff, 0xff, 0, 0x30));
	_greenBrush = QBrush(QColor(0, 0xff, 0, 0x30));
}

void CustomTableDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{

	QPen oldPen = painter->pen();
	QBrush oldBrush = painter->brush();

	painter->setPen(_gridPen);

	int d = index.data(Qt::UserRole).toInt();
	int progress = -1;
	bool bYellow = false;
	int flag = d;

	// 0 00000 00 => yellowflag progress flag

	if (d >= 10000000)
	{
		bYellow = true;
	}

	if (d >= 100)
	{
		progress = (d - (bYellow ? 10000000 : 0)) / 100;
		flag = d % 100;
	}

	if (flag & CustomTableDelegate_Top)
	{
		painter->drawLine(option.rect.topLeft(), option.rect.topRight());
	}
	if (flag & CustomTableDelegate_Bottom)
	{
		painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
	}
	if (flag & CustomTableDelegate_Left)
	{
		painter->drawLine(option.rect.topLeft(), option.rect.bottomLeft());
	}
	if (flag & CustomTableDelegate_Right)
	{
		painter->drawLine(option.rect.topRight(), option.rect.bottomRight());
	}

	if (progress >= 0)
	{
		if (bYellow)
		{
			if (progress == 10000)
			{
				painter->setBrush(_greenBrush);
			}
			else
			{
				painter->setBrush(_yellowBrush);
			}
		}
		else
		{
			painter->setBrush(_orangeBrush);
		}
		QRect rect;
		rect.setTop(option.rect.top());
		rect.setBottom(option.rect.bottom());
		int left = option.rect.left();
		int right = option.rect.right();
		rect.setLeft(left);
		rect.setRight((right - left) * 2 * progress / 10000 + left);
		painter->fillRect(rect, painter->brush());
	}

	painter->setPen(oldPen);
	painter->setBrush(oldBrush);

	QStyledItemDelegate::paint(painter, option, index);
}
