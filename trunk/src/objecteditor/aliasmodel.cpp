/****************************************************************************
 **
 ** Copyright (C) 2005-2008 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the documentation of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#include <QtGui>

#include "aliasitem.h"
#include "aliasmodel.h"

#include "cmd2.h"
#include "cmd.h"
#include "utils.h"
#include "list.h"

AliasModel::AliasModel(QList<aliasnode*> &list, QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  rootData << "Name" << "Command";
  rootItem = new AliasItem(rootData);
  setupModelData(list, rootItem);
}

AliasModel::~AliasModel()
{
  delete rootItem;
}

int AliasModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<AliasItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant AliasModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  AliasItem *item = static_cast<AliasItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags AliasModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
}

QVariant AliasModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return rootItem->data(section);

  return QVariant();
}

QModelIndex AliasModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  AliasItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<AliasItem*>(parent.internalPointer());

  AliasItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex AliasModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  AliasItem *childItem = static_cast<AliasItem*>(index.internalPointer());
  AliasItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int AliasModel::rowCount(const QModelIndex &parent) const
{
  AliasItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<AliasItem*>(parent.internalPointer());

  return parentItem->childCount();
}

void AliasModel::setupModelData(QList<aliasnode*> &list, AliasItem *parent)
{
  QList<AliasItem*> parents;
  QList<int> indentations;
  parents << parent;
  indentations << 0;
  /*
  int number = 0;

  while (number < lines.count()) {
    int position = 0;
    while (position < lines[number].length()) {
      if (lines[number].mid(position, 1) != " ")
        break;
      position++;
    }

    QString lineData = lines[number].mid(position).trimmed();

    if (!lineData.isEmpty()) {
             // Read the column data from the rest of the line.
      QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
      QList<QVariant> columnData;
      for (int column = 0; column < columnStrings.count(); ++column)
        columnData << columnStrings[column];

      if (position > indentations.last()) {
                 // The last child of the current parent is now the new parent
                 // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
          parents << parents.last()->child(parents.last()->childCount()-1);
          indentations << position;
        }
      } else {
        while (position < indentations.last() && parents.count() > 0) {
          parents.pop_back();
          indentations.pop_back();
        }
      }

             // Append a new item to the current parent's list of children.
      parents.last()->appendChild(new AliasItem(columnData, parents.last()));
    }

    number++;
  }
  */
}
