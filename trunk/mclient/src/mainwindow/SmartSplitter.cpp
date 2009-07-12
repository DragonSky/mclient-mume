#include "SmartSplitter.h"

#include <QDebug>

SmartSplitter::SmartSplitter(QWidget *parent)
  : QSplitter(parent) {
}

SmartSplitter::SmartSplitter(Qt::Orientation orientation, QWidget *parent)
  : QSplitter(orientation, parent) {
}

SmartSplitter::~SmartSplitter() {
}

void SmartSplitter::addWidget(QWidget *widget) {
  QSplitter::addWidget(widget);
  qDebug() << "* SmartSplitter added" << widget
	   << "to index" << indexOf(widget);

  connect(widget, SIGNAL(resizeSplitter(QWidget*)),
	  SLOT(resizeWidget(QWidget*)));

}

void SmartSplitter::insertWidget(int index, QWidget *widget) {
  QSplitter::insertWidget(index, widget);
  
  qDebug() << "* SmartSplitter added" << widget
	   << "to index" << indexOf(widget);
  
  connect(widget, SIGNAL(resizeSplitter(QWidget*)),
	  SLOT(resizeWidget(QWidget*)));
}

void SmartSplitter::resizeWidget(QWidget *widget) {
  qDebug() << "* SmartSplitter: resizing" << indexOf(widget)
	   << widget->size().height();
  qDebug() << "* SmartSplitter size:" << this->size().height();
  int pos = closestLegalPosition(size().height() -
				 handleWidth() -
				 widget->size().height(),
				 indexOf(widget));
  qDebug() << "* SmartSplitter is moving to" << pos;
  moveSplitter(pos, indexOf(widget));
}
