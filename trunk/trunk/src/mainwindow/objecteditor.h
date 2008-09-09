/***************************************************************************
 *   Copyright (C) 2008 by Nils Schimmelmann   *
 *   nschimme@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 ***************************************************************************/

#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QDialog>

#include "ui_objecteditor.h"
#include "main.h"
#include "list.h"

enum TabIndexName   { TAB_ALIAS = 0, TAB_ACTION, TAB_GROUP, TAB_VAR, TAB_MARK, TAB_BIND };

#define COMPLETE_REBUILD true
#define UPDATE false

class Wrapper;

class ObjectEditor : public QDialog, public Ui::ObjectEditor
{
  Q_OBJECT

  public:
    ObjectEditor(Wrapper* wrapper, QWidget *parent = 0);

  private:
    void loadAliasTab();
    void loadActionTab();
    void loadGroupTab();
    void loadVariableTab();
    void loadMarkTab();
    void loadBindTab();

    Wrapper* wrapper;
    QHash<QString, bool> groupHash;
    QHash<QString, QTreeWidgetItem*> aliasGroupHash, actionGroupHash; // contains group items
    QHash<QTreeWidgetItem*, aliasnode*> aliasHash;
    QHash<QTreeWidgetItem*, actionnode*> actionHash;
    //QHash<QString, QString> variableStringHash, variableIntegerHash;
    QHash<QTreeWidgetItem*, keynode*> bindHash;
    QHash<QTreeWidgetItem*, marknode*> markHash;

    bool addingNewAlias, addingNewAction, addingNewGroup, addingNewVariable, addingNewMark;

    QTreeWidgetItem* addAliasNode(aliasnode*);
    bool isCurrentAlias(const char*);
    void updateAliasTable();
    void deleteAlias(QTreeWidgetItem*);

    QTreeWidgetItem* addActionNode(actionnode*);
    bool isCurrentAction(const char*);
    void updateActionTable();
    void deleteAction(QTreeWidgetItem*);
    void toggleAction(bool);

    void updateVariableTable(bool completeRebuild);
    void deleteVariable(QTreeWidgetItem*);

    bool isCurrentMark(const char*);
    void updateMarkTable();

  private slots:
    void tabChanged(int);
    void buttonBoxClicked(QAbstractButton *);

    void aliasAddClicked();
    void aliasRemoveClicked();
    void aliasNameEditingFinished();
    void aliasCommandEditingFinished() { updateAliasTable(); }
    void aliasGroupActivated(int) { updateAliasTable(); }
    void aliasItemClicked(QTreeWidgetItem*, int);

    void actionAddClicked();
    void actionRemoveClicked();
    void actionLabelEditingFinished();
    void actionPatternEditingFinished() { updateActionTable(); }
    void actionCommandEditingFinished()  { updateActionTable(); }
    void actionTypeActivated(int)  { updateActionTable(); }
    void actionGroupActivated(int)  { updateActionTable(); }
    void actionCheckBoxClicked();
    void actionItemClicked(QTreeWidgetItem*, int);

    void groupAddClicked();
    void groupRemoveClicked();
    void groupNameEditingFinished();
    void groupCheckBoxClicked();
    void groupItemClicked(QTreeWidgetItem*, int);

    void variableAddClicked();
    void variableRemoveClicked();
    void variableNameEditingFinished();
    void variableValueEditingFinished();
    void variableTypeActivated(int);
    void variableItemClicked(QTreeWidgetItem*, int);

    void markAddClicked();
    void markRemoveClicked();
    void markPatternEditingFinished() { updateMarkTable(); }
    void markBoldClicked() { updateMarkTable(); }
    void markBlinkClicked() { updateMarkTable(); }
    void markInverseClicked() { updateMarkTable(); }
    void markUnderlineClicked() { updateMarkTable(); }
    void markForegroundColorActivated(int)  { updateMarkTable(); }
    void markBackgroundColorActivated(int)  { updateMarkTable(); }
    void markItemClicked(QTreeWidgetItem* item, int column);

    void bindDetectClicked();
};

#endif
