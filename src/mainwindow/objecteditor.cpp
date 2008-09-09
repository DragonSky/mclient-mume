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

#include <QtGui>

#include "objecteditor.h"
#include "keybinder.h"

#include "main.h"
#include "cmd2.h"
#include "cmd.h"
#include "utils.h"
#include "list.h"
#include "eval.h" // vars
#include "edit.h" // keybinds

ObjectEditor::ObjectEditor(Wrapper *wrapper, QWidget *parent)
  : QDialog(parent), wrapper(wrapper)
{
  setupUi(this);

  groupHash["*"] = true;   // default group is not disable-able
  addingNewAlias = false;
  addingNewAction = false;
  addingNewGroup = false;
  addingNewVariable = false;
  addingNewMark = false;

  /* Standard Object Editor Signals */
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
  connect(buttonBox, SIGNAL(clicked(QAbstractButton *)),
          this, SLOT(buttonBoxClicked(QAbstractButton *)) );

  /* Alias Tab */
  aliasTable->setColumnCount(2);
  aliasTable->setSortingEnabled(true);
  aliasTable->sortItems(0, Qt::AscendingOrder);
  aliasTable->setHeaderLabels(QStringList() << tr("Name") << tr("Command"));
  aliasTable->header()->setResizeMode(1, QHeaderView::Stretch);
  aliasTable->setRootIsDecorated(true);
  aliasTable->setAlternatingRowColors(true);

  connect(aliasAdd, SIGNAL(clicked()), this, SLOT(aliasAddClicked()) );
  connect(aliasRemove, SIGNAL(clicked()), this, SLOT(aliasRemoveClicked()) );
  connect(aliasTable, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          this, SLOT(aliasItemClicked(QTreeWidgetItem*, int)) );
  connect(aliasName, SIGNAL(editingFinished()), this, SLOT(aliasNameEditingFinished()) );
  connect(aliasCommand, SIGNAL(editingFinished()), this, SLOT(aliasCommandEditingFinished()) );
  connect(aliasGroup, SIGNAL(activated(int)), this, SLOT(aliasGroupActivated(int)) );


  /* Action Tab */
  actionTable->setColumnCount(3);
  actionTable->setSortingEnabled(true);
  actionTable->sortItems(0, Qt::AscendingOrder);
  actionTable->setHeaderLabels(QStringList() << tr("Label") << tr("Pattern") << tr("Command"));
  actionTable->setRootIsDecorated(true);
  actionTable->setAlternatingRowColors(true);
  actionType->addItem("default", QVariant(">"));
  actionType->addItem ("regex", QVariant("%"));

  connect(actionAdd, SIGNAL(clicked()), this, SLOT(actionAddClicked()) );
  connect(actionRemove, SIGNAL(clicked()), this, SLOT(actionRemoveClicked()) );
  connect(actionTable, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          this, SLOT(actionItemClicked(QTreeWidgetItem*, int)) );
  connect(actionLabel, SIGNAL(editingFinished()), this, SLOT(actionLabelEditingFinished()) );
  connect(actionPattern, SIGNAL(editingFinished()), this, SLOT(actionPatternEditingFinished()) );
  connect(actionCommand, SIGNAL(editingFinished()), this, SLOT(actionCommandEditingFinished()) );
  connect(actionType, SIGNAL(activated(int)), this, SLOT(actionTypeActivated(int)) );
  connect(actionGroup, SIGNAL(activated(int)), this, SLOT(actionGroupActivated(int)) );
  connect(actionCheckBox, SIGNAL(clicked()), this, SLOT(actionCheckBoxClicked()) );

  /* Group Tab */
  groupTable->setColumnCount(3);
  groupTable->setSortingEnabled(true);
  groupTable->sortItems(0, Qt::AscendingOrder);
  groupTable->setHeaderLabels(QStringList() << tr("Name") << tr("Aliases") << ("Actions"));
  groupTable->setRootIsDecorated(false);
  groupTable->setAlternatingRowColors(true);

  connect(groupAdd, SIGNAL(clicked()), this, SLOT(groupAddClicked()) );
  connect(groupRemove, SIGNAL(clicked()), this, SLOT(groupRemoveClicked()) );
  connect(groupTable, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          this, SLOT(groupItemClicked(QTreeWidgetItem*, int)) );
  connect(groupName, SIGNAL(editingFinished()), this, SLOT(groupNameEditingFinished()) );
  connect(groupCheckBox, SIGNAL(clicked()), this, SLOT(groupCheckBoxClicked()) );

  /* Variable Tab */
  varTable->setColumnCount(2);
  varTable->setSortingEnabled(true);
  varTable->sortItems(0, Qt::AscendingOrder);
  varTable->setHeaderLabels(QStringList() << tr("Name") << tr("Expression"));
  varTable->setRootIsDecorated(false);
  varTable->setAlternatingRowColors(true);
  varType->addItem("string", QVariant("$"));
  varType->addItem("integer", QVariant("@"));

  connect(varAdd, SIGNAL(clicked()), this, SLOT(variableAddClicked()) );
  connect(varRemove, SIGNAL(clicked()), this, SLOT(variableRemoveClicked()) );
  connect(varTable, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
          this, SLOT(variableItemClicked(QTreeWidgetItem*, int)) );
  connect(varName, SIGNAL(editingFinished()), this, SLOT(variableNameEditingFinished()) );
  connect(varValue, SIGNAL(editingFinished()), this, SLOT(variableValueEditingFinished()) );
  connect(varType, SIGNAL(activated(int)), this, SLOT(variableTypeActivated(int)) );

  /* Highlights Tab */
  markTable->setColumnCount(2);
  markTable->setSortingEnabled(true);
  markTable->sortItems(1, Qt::AscendingOrder);
  markTable->setHeaderLabels(QStringList() << tr("Color") << tr("Pattern"));
  markTable->setRootIsDecorated(false);
  markTable->setAlternatingRowColors(true);
  markForegroundColor->addItem("none", QVariant("\0"));
  markForegroundColor->addItem("black", QVariant("black"));
  markForegroundColor->addItem("red", QVariant("red"));
  markForegroundColor->addItem("green", QVariant("green"));
  markForegroundColor->addItem("yellow", QVariant("yellow"));
  markForegroundColor->addItem("blue", QVariant("blue"));
  markForegroundColor->addItem("magenta", QVariant("magenta"));
  markForegroundColor->addItem("cyan", QVariant("cyan"));
  markForegroundColor->addItem("white", QVariant("white"));
  markForegroundColor->addItem("BLACK", QVariant("BLACK"));
  markForegroundColor->addItem("RED", QVariant("RED"));
  markForegroundColor->addItem("GREEN", QVariant("GREEN"));
  markForegroundColor->addItem("YELLOW", QVariant("YELLOW"));
  markForegroundColor->addItem("BLUE", QVariant("BLUE"));
  markForegroundColor->addItem("MAGENTA", QVariant("MAGENTA"));
  markForegroundColor->addItem("CYAN", QVariant("CYAN"));
  markForegroundColor->addItem("WHITE", QVariant("WHITE"));
  markBackgroundColor->addItem("none", QVariant("\0"));
  markBackgroundColor->addItem("black", QVariant("black"));
  markBackgroundColor->addItem("red", QVariant("red"));
  markBackgroundColor->addItem("green", QVariant("green"));
  markBackgroundColor->addItem("yellow", QVariant("yellow"));
  markBackgroundColor->addItem("blue", QVariant("blue"));
  markBackgroundColor->addItem("magenta", QVariant("magenta"));
  markBackgroundColor->addItem("cyan", QVariant("cyan"));
  markBackgroundColor->addItem("white", QVariant("white"));
  markBackgroundColor->addItem("BLACK", QVariant("BLACK"));
  markBackgroundColor->addItem("RED", QVariant("RED"));
  markBackgroundColor->addItem("GREEN", QVariant("GREEN"));
  markBackgroundColor->addItem("YELLOW", QVariant("YELLOW"));
  markBackgroundColor->addItem("BLUE", QVariant("BLUE"));
  markBackgroundColor->addItem("MAGENTA", QVariant("MAGENTA"));
  markBackgroundColor->addItem("CYAN", QVariant("CYAN"));
  markBackgroundColor->addItem("WHITE", QVariant("WHITE"));

  connect(markAdd, SIGNAL(clicked()), this, SLOT(markAddClicked()) );
  connect(markRemove, SIGNAL(clicked()), this, SLOT(markRemoveClicked()) );
  connect(markPattern, SIGNAL(editingFinished()), this, SLOT(markPatternEditingFinished()) );
  connect(markBold, SIGNAL(clicked()), this, SLOT(markBoldClicked()) );
  connect(markBlink, SIGNAL(clicked()), this, SLOT(markBoldClicked()) );
  connect(markInverse, SIGNAL(clicked()), this, SLOT(markInverseClicked()) );
  connect(markUnderline, SIGNAL(clicked()), this, SLOT(markUnderlineClicked()) );
  connect(markForegroundColor, SIGNAL(activated(int)), this, SLOT(markForegroundColorActivated(int)) );
  connect(markBackgroundColor, SIGNAL(activated(int)), this, SLOT(markBackgroundColorActivated(int)) );
  connect(markTable, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(markItemClicked(QTreeWidgetItem*, int)) );

  /* Bind Tab */
  bindTable->setColumnCount(3);
  bindTable->setSortingEnabled(true);
  bindTable->sortItems(0, Qt::AscendingOrder);
  bindTable->setHeaderLabels(QStringList() << tr("Key") << tr("Bind") << tr("Command"));
  bindTable->setRootIsDecorated(false);
  bindTable->setAlternatingRowColors(true);
  
  connect(bindDetect, SIGNAL(clicked()), this, SLOT(bindDetectClicked()) );

  tabChanged(tabWidget->currentIndex());
}

void ObjectEditor::tabChanged(int tabIndex)
{
  switch (tabIndex)
  {
    case TAB_ALIAS:
      loadAliasTab();
      break;
    case TAB_ACTION:
      loadActionTab();
      break;
    case TAB_GROUP:
      loadGroupTab();
      break;
    case TAB_VAR:
      loadVariableTab();
      break;
    case TAB_MARK:
      loadMarkTab();
      break;
    case TAB_BIND:
      loadBindTab();
      break;
    default:
      qDebug("Tab Changed: Unknown tab selected.");
  };
}

void ObjectEditor::buttonBoxClicked(QAbstractButton *button)
{
  qDebug("Button Box: uhm, now what?");
}


  /* GROUP TAB */

void ObjectEditor::loadGroupTab()
{
  int disabledActionCount, actionCount, j;
  QTreeWidgetItem *item;
  QString group;

  loadAliasTab();   // load actions after the aliases to ensure that
  loadActionTab();  // the alias groups are present

  groupTable->clear();

  QHashIterator<QString, QTreeWidgetItem*> i(actionGroupHash);
  // Step through each group in the actions
  while (i.hasNext())
  {
    i.next();
    group = i.key();
    item = i.value();

    disabledActionCount = 0;
    actionCount = item->childCount();
    for (j = 0; j < actionCount; j++)
    {
      actionnode *p = actionHash[item->child(j)];
      if (!p->active)
        disabledActionCount++;
    }

    //qDebug("Current Group: %s %d (-%d)", group.toAscii().data(), actionCount, disabledActionCount);
    if (!aliasGroupHash.contains(group)) // since we know aliases would be disabled then
      groupHash[group] = !(disabledActionCount == actionCount);

    item = new QTreeWidgetItem(groupTable);
    if (i.key().compare("*") == 0)
      item->setText(0, "default");
    else {
      item->setText(0, group);
      item->setCheckState(0, groupHash[group] ? Qt::Checked : Qt::Unchecked);
    }
    item->setText(1, QString("%1").arg(aliasGroupHash[group]->childCount()));

    if (actionCount == 0 || disabledActionCount == 0) item->setText(2, QString("%1").arg(actionCount));
    else if (actionCount == disabledActionCount) item->setText(2, QString("%1 disabled").arg(actionCount));
    else item->setText(2, QString("%1\t(%2 disabled)").arg(actionCount).arg(disabledActionCount));
  }
}

void ObjectEditor::groupAddClicked() {
  addingNewGroup = true;
  groupRemove->setEnabled(false);
  groupAdd->setEnabled(false);
  groupTable->setEnabled(false);
  groupCheckBox->setEnabled(false);
  groupName->clear();
  groupName->setFocus();
  groupCheckBox->setChecked(false);
}

void ObjectEditor::groupRemoveClicked() {
  QTreeWidgetItem *item, *group;
  if (groupTable->selectedItems().count() <= 0)
    return;
  item = groupTable->currentItem();
  QString name = item->text(0);
  int ret = QMessageBox::warning(this, tr("Object Editor"),
                                 tr("Are you sure you want to delete all the aliases\n"
                                     "and actions within the \'%1\' group?").arg(name),
                                    QMessageBox::Yes | QMessageBox::No);
  if (ret == QMessageBox::No)
    return ;
  if (name.compare("default") == 0) name = "*";
  group = actionGroupHash[name];

  QHashIterator<QTreeWidgetItem*, actionnode*> i(actionHash);
  while (i.hasNext()) {
    i.next();
    item = i.key();
    if (item->parent() == group) {
      actionTable->setCurrentItem(item);
      actionRemoveClicked();
    }
  }
  group = aliasGroupHash[name];
  QHashIterator<QTreeWidgetItem*, aliasnode*> j(aliasHash);
  while (j.hasNext()) {
    j.next();
    item = j.key();
    if (item->parent() == group) {
      aliasTable->setCurrentItem(item);
      aliasRemoveClicked();
    }
  }

  // Remove group item from list
  if (name.compare("*") != 0) {
    groupTable->currentItem()->setHidden(true);
    groupHash.remove(name);
    actionGroupHash.remove(name);
    aliasGroupHash.remove(name);
  }
  else
  {
    groupTable->currentItem()->setText(1, "0");
    groupTable->currentItem()->setText(2, "0");
  }
}

void ObjectEditor::groupCheckBoxClicked() { // TODO
  QTreeWidgetItem *item;
  if (groupTable->selectedItems().count() == 0) {
    qDebug("Group CheckBox: No item selected");
    return ;
  }
  item = groupTable->currentItem();
  if (item->text(0).compare("default") == 0) {
    qDebug("Group Name is default, can't disable");
    return ;
  }
  aliasnode *p;
  actionnode *a;
  int active = groupCheckBox->isChecked() ? 1 : 0;

  char *group;
  sprintf(group, "%s", item->text(0).toAscii().data() );
  qDebug("Group Checkbox Change: Changing \"%s\" to %s (%d)", group, active ? "on" : "off", active);

  groupTable->currentItem()->setCheckState(0, active ? Qt::Checked : Qt::Unchecked);
  for( p = sortedaliases; p; p = p -> snext ) {
    if( p -> group && strcmp( p -> group, group ) == 0 ) {
      p -> active = active;
    }
  }
  for( a = wrapper->getActions(); a; a = a -> next ) {
    if( a -> group && strcmp( a -> group, group ) == 0 ) {
      a -> active = active;
    }
  }
  //tabWidget->setFocus();
  loadGroupTab();
}

void ObjectEditor::groupNameEditingFinished() {
  QTreeWidgetItem *item, *group;
  QString powwow_cmd, oldName, active, name = groupName->text();
  actionnode *cp;
  aliasnode *lp;
  char buf[BUFSIZE];

  if (addingNewGroup) {
    item = new QTreeWidgetItem(aliasTable);
    item->setText(0, name);
    aliasGroupHash[name] = item;

    item = new QTreeWidgetItem(actionTable);
    item->setText(0, name);
    actionGroupHash[name] = item;

    groupHash[name] = groupCheckBox->isChecked();
    item = new QTreeWidgetItem(groupTable);
    item->setText(0, name);
    item->setText(1, "0");
    item->setText(2, "0");
    item->setCheckState(0, Qt::Unchecked);

    groupCheckBox->setEnabled(true);
    groupAdd->setEnabled(true);
    groupRemove->setEnabled(true);
    groupTable->setEnabled(true);
    groupTable->setFocus();
    groupTable->setCurrentItem(item);
    groupTable->scrollToItem(item);
  }
  else { // Renaming a group
    if (groupTable->selectedItems().count() <= 0) {
      qDebug("Group Name: Unknown item selected.");
      return;
    }
    item = groupTable->currentItem();
    oldName = item->text(0);

    if (oldName.compare("default") == 0) {
       qDebug("Group Name: Unable to rename default group");
       QMessageBox::information (this,
                                 tr("Object Editor"),
                                 tr("The default group can not be renamed"));
       return ;
    }

    item->setText(0, name);
    groupHash.remove(oldName);
    groupHash[name] = item;
    tabWidget->setFocus();

    group = actionGroupHash[oldName];
    actionGroupHash.remove(oldName);
    QHashIterator<QTreeWidgetItem*, actionnode*> i(actionHash);
    while (i.hasNext()) {
      i.next();
      item = i.key();
      if (item->parent() == group) {
        cp = actionHash[item];
        active = QString("%1").arg(cp->active ? '+' : '-');
        QTextStream(&powwow_cmd) << action_chars[cp->type] << active << cp->label << group_delim << name
            << " " << cp->pattern << "=" << cp->command;
        qDebug("#action %s", powwow_cmd.toAscii().data());
        parse_action(powwow_cmd.toAscii().data(), 0);
      }
      powwow_cmd.clear();
    }
    group = aliasGroupHash[oldName];
    aliasGroupHash.remove(oldName);
    QHashIterator<QTreeWidgetItem*, aliasnode*> j(aliasHash);
    while (j.hasNext()) {
      j.next();
      item = j.key();
      if (item->parent() == group) {
        lp = aliasHash[item];
        escape_specials(buf, lp->name);
        QTextStream(&powwow_cmd) << buf << group_delim << name << "=" << lp->subst;
        qDebug("#alias %s", powwow_cmd.toAscii().data());
        parse_alias(powwow_cmd.toAscii().data());
      }
      powwow_cmd.clear();
    }
  }
}

void ObjectEditor::groupItemClicked(QTreeWidgetItem* item, int) {
  if (groupTable->selectedItems().count() <= 0) {
    qDebug("Group Click: Unknown item selected.");
    return;
  }

  QString name = item->text(0);

  if (!name.compare("default") == 0) {
    groupName->setText(name);
    groupCheckBox->setChecked(item->checkState(0) == Qt::Checked);
  } else {
    groupName->clear();
    groupCheckBox->setChecked(false);
  }
}


 /*      ALIAS TAB */
void ObjectEditor::loadAliasTab()
{
  QTreeWidgetItem *currentGroup;

  aliasGroup->clear();
  aliasTable->clear();

  /* (re?)populate the aliasTable */
  QHashIterator<QString, bool> i(groupHash);
  while (i.hasNext())
  {
    i.next();
    currentGroup = new QTreeWidgetItem();
    currentGroup->setText(0, i.key());
    currentGroup->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    aliasGroupHash[i.key()] = currentGroup;
    aliasTable->addTopLevelItem(currentGroup);
  }
  currentGroup = aliasGroupHash["*"];
  currentGroup->setText(0, "default");

  aliasnode *p;

  for (p = sortedaliases; p; p = p->snext) 
    addAliasNode(p);

  /* generate the groups for the combo */
  aliasGroup->addItem("default", QVariant(QString("\0")) );
  QHashIterator<QString, QTreeWidgetItem*> j(aliasGroupHash);
  while (j.hasNext())
  {
    j.next();
    //j.value()->setExpanded(true); // HACK to fix moving groups

    if (j.key() != "*")
      aliasGroup->addItem(j.key(), QVariant(j.key()));
  }
}

QTreeWidgetItem* ObjectEditor::addAliasNode(aliasnode* p)
{
  QTreeWidgetItem *item, *groupItem;
  char buf[BUFSIZE];

  escape_specials(buf, p->name);
  //QString active = QString("%1").arg(p->active ? "" : "(disabled) ");
  QString name = QString(buf);
  QString group = QString("%1").arg(p->group == NULL ? "*" : p->group);
  QString subst = QString(p->subst);

  if (aliasGroupHash.contains(group)) {
    groupItem = aliasGroupHash[group];
  } else {
    qDebug("Add Alias Node: New group '%s'", group.toAscii().data() );
    groupItem = new QTreeWidgetItem(aliasTable);
    groupItem->setText(0, group);
    aliasGroupHash[group] = groupItem;
    groupHash[group] = p->active;
  }
  if (!p->active) // if one alias is inactive the entire group is
    groupHash[group] = p->active;

  item = new QTreeWidgetItem(groupItem);
  item->setText(0, name);
  item->setText(1, subst);
  //item->setDisabled(!p->active);

  aliasHash[item] = p;
  return item;
}

void ObjectEditor::aliasAddClicked() {
  addingNewAlias = true;

  aliasName->clear();
  aliasCommand->clear();
  aliasName->setFocus();

  aliasTable->setEnabled(false);
  aliasRemove->setEnabled(false);
  aliasAdd->setEnabled(false);
  /*
  aliasCommand->setEnabled(false);
  aliasGroup->setEnabled(false);
  aliasGroupLabel->setEnabled(false);
  */
}


void ObjectEditor::aliasNameEditingFinished() {
  QTreeWidgetItem *item;
  // error checking in the name
  if (aliasName->text().isEmpty() ||
      check_alias(aliasName->text().toAscii().data()) == 1)
  {
    // TODO: add message box
    qDebug("Alias Name: Incorrect name");
    return;
  }

  // remove spaces
  aliasName->setText(aliasName->text().remove(QString(group_delim)) );
  aliasName->setText(aliasName->text().remove(QString(" ")) );

  if (addingNewAlias) {
    //aliasCommand->setEnabled(true);
    aliasCommand->setFocus();
    //aliasName->setEnabled(false);
    //aliasGroup->setEnabled(true);
    //aliasGroupLabel->setEnabled(true);
  }
  else
  {
    // renaming aliases
    if (!isCurrentAlias("Name Editing")) return ;
    item = aliasTable->currentItem();
    deleteAlias(item);
    updateAliasTable();
    aliasHash.remove(item);
  }
}


bool ObjectEditor::isCurrentAlias(const char* source) {
  QTreeWidgetItem *item;
  QString name;

  if (aliasTable->selectedItems().count() == 0)
    return false;

  item = aliasTable->currentItem();
  name = item->text(0);
  QList<QTreeWidgetItem *> list = aliasTable->findItems(name, Qt::MatchRecursive);

  if ((aliasGroupHash.contains(name) || name.compare("default") == 0) && list.size() == 1)
  {
    qDebug("Alias %s: Group \"%s\" selected", source, name.toAscii().data());
    aliasName->clear();
    aliasCommand->clear();
    aliasGroup->setCurrentIndex(aliasGroup->findText("default"));
    return false;
  }

  if (!aliasHash.contains(item))
  {
    qDebug("Alias %s: Unknown item selected", source);
    return false;
  }

  qDebug("Alias %s: \"%s\" selected", source, name.toAscii().data());
  return true;
}

void ObjectEditor::aliasItemClicked(QTreeWidgetItem* item, int column) {
  QString name, group, subst;
  aliasnode *p;
  char buf[BUFSIZE];

  if (!isCurrentAlias("Clicked"))
    return ;

  p = aliasHash[item];
  escape_specials(buf, p->name);
  name = QString(buf);
  group = QString("%1").arg(p->group == NULL ? "default" : p->group);
  subst = QString(p->subst).trimmed();

  aliasName->setText(name);
  aliasCommand->setText(subst);
  aliasCommand->setCursorPosition(0);
  aliasGroup->setCurrentIndex(aliasGroup->findText(group));
}

/* Updates an alias-item from the various user-provided inputs */
void ObjectEditor::updateAliasTable() {
  QTreeWidgetItem *item, *parent;
  aliasnode *p;
  QString name, subst, group, powwow_cmd;

  if (addingNewAlias) {
    if (aliasCommand->text().isEmpty() || aliasName->text().isEmpty()) {
      qDebug("Alias Updater: Unable to add item because of insufficient data.");
      return ;
    }
    qDebug("Alias Updater: Building item...");

  } else {
    if (!isCurrentAlias("Updater"))
      return ;
    item = aliasTable->currentItem();
    //item->setHidden(true); // HACK because items don't like being deleted
                           // instead of moving always create a new item
  }

  // Generate Powwow command
  name = aliasName->text();
  subst = aliasCommand->text();
  group = aliasGroup->itemData(aliasGroup->currentIndex()).toString();
  QTextStream(&powwow_cmd) << name << group_delim << group << "=" <<  subst;
  qDebug("#alias %s", powwow_cmd.toAscii().data());

  parse_alias(powwow_cmd.toAscii().data());
  p = *(lookup_alias(name.toAscii().data()));

  if (addingNewAlias) {
    // Add new item
    item = addAliasNode(p);

    qDebug("Added new item.");
    addingNewAlias = false;
    //aliasName->setEnabled(true);
    aliasTable->setEnabled(true);
    aliasAdd->setEnabled(true);
    aliasRemove->setEnabled(true);
  }
  else
  {
    // Reuse existing item
    aliasHash[item] = p;

    item->setText(0, name);
    item->setText(1, subst);
    parent = item->parent();
    group = group.isEmpty() ? "*" : group;
    int index = parent->indexOfChild(item);
    if (item->parent() != aliasGroupHash[group]) {
      // Update group
      qDebug("Moving item %d from group \"%s\" to \"%s\"",
             index,
             parent->text(0).toAscii().data(),
             group.toAscii().data());
      QTreeWidgetItem *newParent = aliasGroupHash[group];
      QList<QTreeWidgetItem*> children = parent->takeChildren();
      for (int i = 0; i < children.count(); i++)
        if (children.at(i) == item) children.removeAt(i);
      newParent->addChild(item);
      parent->addChildren(children);
      /*
      if ((item = parent->takeChild(index)) == 0)
        qDebug("Error, child wasn't removed");
      parent = aliasGroupHash[group];
      parent->addChild(item);
      */
    }
    aliasHash[item] = p;
  }

  aliasTable->setFocus();
  aliasTable->setCurrentItem(item);
  aliasTable->scrollToItem(item);
}


void ObjectEditor::aliasRemoveClicked() {
  QTreeWidgetItem *parent, *item = aliasTable->currentItem();

  if (!isCurrentAlias("Remove")) return ;

  deleteAlias(item);
  aliasHash.remove(item);
  parent = item->parent();
  parent->removeChild(item);

  aliasName->clear();
  aliasCommand->clear();
  aliasTable->setFocus();
}

void ObjectEditor::deleteAlias(QTreeWidgetItem* item) {
  aliasnode *p = aliasHash[item];
  QString command;
  QTextStream(&command) << p->name << "=";
  qDebug("#alias %s", command.toAscii().data() );
  parse_alias(command.toAscii().data());
}

 
   /*      ACTIONS START     */


void ObjectEditor::loadActionTab()
{
  QTreeWidgetItem *currentGroup;
  QHashIterator<QString, bool> i(groupHash);

  actionGroup->clear();
  actionTable->clear();

  /* (re?)populate the actionTable */
  while (i.hasNext())
  {
    i.next();
    currentGroup = new QTreeWidgetItem(actionTable);
    currentGroup->setText(0, i.key());
    currentGroup->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    actionGroupHash[i.key()] = currentGroup;
  }
  currentGroup = actionGroupHash["*"];
  currentGroup->setText(0, "default");

  actionnode *p;
  for (p = wrapper->getActions(); p; p = p->next)
    addActionNode(p);

  /* populate group combobox */
  actionGroup->addItem("default", QVariant(QString()) );
  QHashIterator<QString, QTreeWidgetItem*> j(actionGroupHash);
  while (j.hasNext())
  {
    j.next();

    if (j.key() != "*")
      actionGroup->addItem(j.key(), QVariant(j.key()));
  }
}

QTreeWidgetItem* ObjectEditor::addActionNode(actionnode* p)
{
  QTreeWidgetItem *item, *currentGroup;

  //QString type = QString("%1").arg(action_chars[p->type] == '>' ? "default" : "regex");
  QString active = QString("%1").arg(p->active ? '+' : '-');
  QString label = QString(p->label);
  QString group = QString("%1").arg(p->group == NULL ? "*" : p->group);
  QString pattern = QString(p->pattern);
  QString command = QString(p->command);

  if (actionGroupHash.contains(group))
    currentGroup = actionGroupHash[group];
  else {
    qDebug("Add Action Node: New group '%s'", group.toAscii().data() );
    currentGroup = new QTreeWidgetItem(actionTable);
    currentGroup->setText(0, group);
    actionGroupHash[group] = currentGroup;
    currentGroup->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicatorWhenChildless);
    groupHash[group] = true; // since we're not sure at this point in time
  }

  item = new QTreeWidgetItem(currentGroup);
  item->setCheckState(0, p->active ? Qt::Checked : Qt::Unchecked);
  item->setText(0, label);
  item->setText(1, pattern);
  item->setText(2, command);
  //item->setText(3, type);

  actionHash[item] = p;
  return item;
}

void ObjectEditor::actionAddClicked() {
  addingNewAction = true;
  
  actionTable->setEnabled(false);
  actionAdd->setEnabled(false);
  actionRemove->setEnabled(false);
  actionLabel->clear();
  actionPattern->clear();
  actionCommand->clear();
  actionCheckBox->setChecked(true);
  
  actionLabel->setFocus();
}

bool ObjectEditor::isCurrentAction(const char* source) {
  QTreeWidgetItem *item;
  QString name;
  bool isCurrent = true;

  if (actionTable->selectedItems().count() == 0)
    return false;

  item = actionTable->currentItem();
  name = item->text(0);
  QList<QTreeWidgetItem *> list = actionTable->findItems(name, Qt::MatchRecursive);

  if ((actionGroupHash.contains(name) || name.compare("default") == 0)  && list.size() == 1)
  {
    qDebug("Action %s: Group \"%s\" selected", source, name.toAscii().data());
    isCurrent = false;
  }
  else if (!actionHash.contains(item))
  {
    qDebug("Action %s: Unknown item selected", source);
    isCurrent = false;
  }

  if (isCurrent) {
    qDebug("Action %s: \"%s\" selected", source, name.toAscii().data());
  } else {
    actionLabel->clear();
    actionCommand->clear();
    actionPattern->clear();
    actionGroup->setCurrentIndex(actionGroup->findText("default"));
    actionType->setCurrentIndex(actionType->findText("default"));
    actionCheckBox->setChecked(false);
  }
  return isCurrent;
}

void ObjectEditor::actionRemoveClicked() {
  QTreeWidgetItem *parent, *item;

  if (!isCurrentAction("Remove")) return;

  item = actionTable->currentItem();
  deleteAction(item);
  actionHash.remove(item);
  parent = item->parent();
  parent->removeChild(item);

  actionLabel->clear();
  actionCommand->clear();
  actionPattern->clear();
  actionTable->setFocus();
  actionCheckBox->setChecked(false);
}

void ObjectEditor::deleteAction(QTreeWidgetItem* item) {
  actionnode *p = actionHash[item];

  QString label = p->label;
  QString command;
  QTextStream(&command) << "<" << label;
  qDebug("#action %s", command.toAscii().data() );
  parse_action(command.toAscii().data(), 0);
}

void ObjectEditor::actionLabelEditingFinished() {
  QTreeWidgetItem *item;
  if (actionLabel->text().isEmpty())
    return ;

  if (addingNewAction) actionPattern->setFocus();
  else {
    // Rename action
    if (!isCurrentAction("Label Editor")) return;
    item = actionTable->currentItem();
    deleteAction(item);
    updateActionTable();
    actionHash.remove(item);
  }
}

void ObjectEditor::actionCheckBoxClicked() { 
  if (addingNewAction) updateActionTable();
  else toggleAction(actionCheckBox->isChecked());
}

void ObjectEditor::toggleAction(bool state) {
  QTreeWidgetItem *item;
  actionnode *p;
  QString label, active, powwow_cmd;

  if (!isCurrentAction("Toggle")) return ;

  item = actionTable->currentItem();
  p = actionHash[item];

  actionCheckBox->setChecked(state);
  item->setCheckState(0, state ? Qt::Checked : Qt::Unchecked);

  active = QString("%1").arg(state ? '+' : '-');
  label = QString(p->label);
  QTextStream(&powwow_cmd) << active << label;
  qDebug("#action %s", powwow_cmd.toAscii().data());
  parse_action(powwow_cmd.toAscii().data(), 0);
}

void ObjectEditor::actionItemClicked(QTreeWidgetItem* item, int column) {
  QString label, group, pattern, command, type;
  actionnode *p;

  if (!isCurrentAction("Clicked")) return ;

  p = actionHash[item];

  label = QString(p->label);
  group = QString("%1").arg(p->group == NULL ? "default" : p->group);
  pattern = QString(p->pattern);
  command = QString(p->command);
  type = QString(action_chars[p->type] == '>' ? "default" : "regex");

  actionLabel->setText(label);
  actionPattern->setText(pattern);
  actionPattern->setCursorPosition(0);
  actionCommand->setText(command);
  actionCommand->setCursorPosition(0);
  actionType->setCurrentIndex(actionType->findText(type));
  actionGroup->setCurrentIndex(actionGroup->findText(group));
  actionCheckBox->setChecked(p->active);

  if (column == 0) toggleAction(item->checkState(0) == Qt::Checked);
}

void ObjectEditor::updateActionTable() {
  QTreeWidgetItem* item;
  actionnode *p;
  QString active, label, pattern, command, type, group, powwow_cmd;

  if (addingNewAction) {
    if (actionCommand->text().isEmpty() ||  actionPattern->text().isEmpty()) {
      qDebug("Action Updater: Unable to add item because of insufficient data.");
      return ;
    }
    qDebug("Action Updater: Building item...");

  } else {
    if (!isCurrentAction("Update")) return;
    item = actionTable->currentItem();

    // Remove previous item
    item->setHidden(true);
  }

  // Generate Powwow command
  label = actionLabel->text();
  active = QString("%1").arg(actionCheckBox->isChecked() ? '+' : '-');
  pattern = actionPattern->text();
  command = actionCommand->text();
  type = actionType->itemData(actionType->currentIndex()).toString();
  group = actionGroup->itemData(actionGroup->currentIndex()).toString();
  QTextStream(&powwow_cmd) << type << active << label << group_delim << group << " " << pattern << "=" << command;
  qDebug("#action %s", powwow_cmd.toAscii().data());

  parse_action(powwow_cmd.toAscii().data(), 0);
  p = *(lookup_action_pattern(pattern.toAscii().data() ));

  // Add new item
  item = addActionNode(p);

  if (addingNewAction) {
    qDebug("Added new item.");
    addingNewAction = false;
    //actionLabel->setEnabled(true);
    actionTable->setEnabled(true);
    actionAdd->setEnabled(true);
    actionRemove->setEnabled(true);
  }

  tabWidget->setFocus();
  actionTable->setCurrentItem(item);
  actionTable->scrollToItem(item);
}


    /* VARIABLE TAB */


void ObjectEditor::loadVariableTab() {
  QTreeWidgetItem *item;
  QString name, value;
  varnode *v;
  int type;
  ptr p = (ptr)0;

  varTable->clear();

  // numbered
  type = 0;
  v = sortednamed_vars[type];
  while (v) {
    item = new QTreeWidgetItem(varTable);
    name = v->name;
    value = QString("%1").arg(v->num);
    item->setText(0, "@" + name);
    item->setText(1, value);
    //numberedVariableHash[name] = value;
    v = v->snext;
  }
  QHashIterator<QString, QString> i(wrapper->getNUMVARs(type));
  while (i.hasNext()) {
    i.next();
    item = new QTreeWidgetItem(varTable);
    item->setText(0, "@" + i.key());
    item->setText(1, i.value());
    //numberedVariableHash[i.key()] = i.value();
  }

  // named
  type = 1;
  v = sortednamed_vars[type];
  while (v) {
    item = new QTreeWidgetItem(varTable);
    p = ptrescape(p, v->str, 0);
    name = v->name;
    value = QString("%1").arg(p ? ptrdata(p): "");
    item->setText(0, "$" + name);
    item->setText(1, value);
    //namedVariableHash[name] = value;
    v = v->snext;
  }
  QHashIterator<QString, QString> j(wrapper->getNUMVARs(type));
  while (j.hasNext()) {
    j.next();
    item = new QTreeWidgetItem(varTable);
    item->setText(0, "$" + j.key());
    item->setText(1, j.value());
    //namedVariableHash[j.key()] = j.value();
  }
}

void ObjectEditor::variableAddClicked() {
  addingNewVariable = true;
  varTable->setEnabled(false);
  varAdd->setEnabled(false);
  varRemove->setEnabled(false);
  varName->setText("");
  varValue->setText("");
  varName->setFocus();
}

void ObjectEditor::variableRemoveClicked() {
  QTreeWidgetItem *item;
  if (varTable->selectedItems().count() <= 0) {
    qDebug("Variable Remove: Unknown item selected.");
    return ;
  }
  item = varTable->currentItem();
  deleteVariable(item);
}

void ObjectEditor::deleteVariable(QTreeWidgetItem* item) {
  QString name, type, powwow_cmd;
  name = item->text(0);

  if (name.contains("$")) {
    name.remove("$");
    type = "$";
  } else {
    name.remove("@");
    type = "@";
  }

  // Delete
  QTextStream(&powwow_cmd) << type << name << "=";
  qDebug("#var %s", powwow_cmd.toAscii().data());

  wrapper_cmd_var(powwow_cmd.toAscii().data());

  varTable->setItemHidden(item, true);
}

void ObjectEditor::variableNameEditingFinished() {
  if (varName->text().isEmpty())
    return;

  char *tmp, *arg = varName->text().toAscii().data();
  char kind;

  kind = *arg;
  if (isalpha(kind) || kind == '_') {
    tmp = arg;
    while (*tmp && (isalnum(*tmp) || *tmp == '_'))
      tmp++;
    if (*tmp) {
      qDebug("Variable Name: Incorrect Name");
      QMessageBox::information (this,
                                tr("Object Editor"),
                                tr("Variable names can include only alphanumerical\n"
                                   "characters"));
      varName->setFocus();
      return;
    }
  }
  updateVariableTable(COMPLETE_REBUILD);
}

void ObjectEditor::variableValueEditingFinished() { updateVariableTable(UPDATE); }
void ObjectEditor::variableTypeActivated(int) { updateVariableTable(COMPLETE_REBUILD); }

void ObjectEditor::variableItemClicked(QTreeWidgetItem* item, int column) {
  if (varTable->selectedItems().count() <= 0) {
    qDebug("Variable Click: Unknown item selected.");
    return;
  }

  if (item->text(0).contains("$")) {
    varName->setText(item->text(0).remove("$"));
    varType->setCurrentIndex(varType->findData(QVariant("$")) );
  } else {
    varName->setText(item->text(0).remove("@"));
    varType->setCurrentIndex(varType->findData(QVariant("@")) );
  }
  varValue->setText(item->text(1));
  varValue->setCursorPosition(0);
}

void ObjectEditor::updateVariableTable(bool completeRebuild) {
  QTreeWidgetItem *item;
  QString name, value, type, powwow_cmd;
  if (addingNewVariable) {
    if (varName->text().isEmpty() ||
        varValue->text().isEmpty()) {
            qDebug("Variable Updater : Insufficient data to add item.");
            return;
        }
        qDebug("Variable Updater: Building item...");
  } else {
    item = varTable->currentItem();
    if (varTable->selectedItems().count() <= 0) {
      qDebug("Variable Updater: Unknown item selected.");
      return ;
    }
    // Remove from table
    varTable->setItemHidden(item, true);
    if (completeRebuild) deleteVariable(item);
  }

  // Rebuild
  name = varName->text();
  value = varValue->text();
  type = varType->itemData(varType->currentIndex()).toString();

  QTextStream(&powwow_cmd) << type << name << "=" << value;
  qDebug("#var %s", powwow_cmd.toAscii().data());

  wrapper_cmd_var(powwow_cmd.toAscii().data());

  // "Add" new item
  // TODO
  loadVariableTab();

  if (addingNewVariable) {
    qDebug("Added new item.");
    addingNewVariable = false;
    varTable->setEnabled(true);
    varAdd->setEnabled(true);
    varRemove->setEnabled(true);
  }

  QList<QTreeWidgetItem *> list = varTable->findItems(name, Qt::MatchRecursive);
  if (!list.isEmpty() && list.count() == 1) {
    item = list.first();
    varTable->setFocus();
    varTable->setCurrentItem(item);
    varTable->scrollToItem(item);
  }
  else qDebug("Unable to match");
}




/*   MARKS TAB */

void ObjectEditor::loadMarkTab() {
  QTreeWidgetItem *item;
  QString pattern, attributes;
  marknode *p;

  markTable->clear();

  for (p = markers; p; p = p->next) {
    pattern = QString("%1%2").arg(p->mbeg ? "^" : "").arg(p->pattern);
    attributes = attr_name(p->attrcode);
    item = new QTreeWidgetItem(markTable);
    item->setText(0, attributes);
    item->setText(1, pattern);
    markHash[item] = p;
  }
}

bool ObjectEditor::isCurrentMark(const char* source) {
  QTreeWidgetItem *item;
  QString name;

  if (markTable->selectedItems().count() == 0)
    return false;

  item = markTable->currentItem();
  name = item->text(1);

  if (!markHash.contains(item))
  {
    qDebug("Mark %s: Unknown item selected", source);
    return false;
  }

  qDebug("Mark %s: \"%s\" selected", source, name.toAscii().data());
  return true;
}

void ObjectEditor::markItemClicked(QTreeWidgetItem* item, int column) {
  QString pattern, attributes, foregroundColor, backgroundColor;
  marknode *p;
  char buf[BUFSIZE];
  QRegExp colorRx("\\b(black|red|green|yellow|blue|magenta|cyan|white)\\b", Qt::CaseInsensitive);
  QStringList colorList;

  if (!isCurrentMark("Clicked"))
    return ;

  p = markHash[item];
  pattern = QString("%1%2").arg(p->mbeg ? "^" : "").arg(p->pattern);
  attributes = attr_name(p->attrcode);

  int pos = 0;
  while ((pos = colorRx.indexIn(attributes, pos)) != -1) {
    colorList << colorRx.cap(1);
    pos += colorRx.matchedLength();
  }

  if (attributes.contains(" on ")) {
    if (colorList.count() == 2) {    // background and foreground color exist
      foregroundColor = colorList.at(0);
      backgroundColor = colorList.at(1);
    }
    else {                           // only background color exists
      foregroundColor = "\0";
      backgroundColor = colorList.at(0);
    }
  }
  else if (colorList.count() == 1) { // only foreground color exists
    foregroundColor = colorList.at(0);
  }
  else {                             // no colors exist
    foregroundColor = "\0";
    backgroundColor = "\0";
  }

  markPattern->setText(pattern);
  markPattern->setCursorPosition(0);
  markBold->setChecked(attributes.contains("bold") ? Qt::Checked : Qt::Unchecked);
  markBlink->setChecked(attributes.contains("blink") ? Qt::Checked : Qt::Unchecked);
  markUnderline->setChecked(attributes.contains("underline") ? Qt::Checked : Qt::Unchecked);
  markInverse->setChecked(attributes.contains("inverse") ? Qt::Checked : Qt::Unchecked);
  markForegroundColor->setCurrentIndex(markForegroundColor->findData(QVariant(foregroundColor)) );
  markBackgroundColor->setCurrentIndex(markBackgroundColor->findData(QVariant(backgroundColor)) );
}

/* Updates an alias-item from the various user-provided inputs */
void ObjectEditor::updateMarkTable() {
  QTreeWidgetItem *item, *parent;
  marknode *p;
  QString pattern, attributes, foregroundColor, backgroundColor, powwow_cmd;

  if (addingNewMark) {
    if (markPattern->text().isEmpty() ||
        (markBold->checkState() == Qt::Unchecked &&
        markBlink->checkState() == Qt::Unchecked &&
        markUnderline->checkState() == Qt::Unchecked &&
        markInverse->checkState() == Qt::Unchecked &&
        markForegroundColor->itemText(markForegroundColor->currentIndex()) == "none" &&
        markBackgroundColor->itemText(markBackgroundColor->currentIndex()) == "none")) {
      qDebug("Mark Updater: Unable to add item because of insufficient data.");
      return ;
    }
    qDebug("Mark Updater: Building item...");

  } else {
    if (!isCurrentMark("Updater"))
      return ;
    item = markTable->currentItem();
  }

  // Generate Powwow command
  pattern = markPattern->text();
  attributes = markBold->checkState() == Qt::Checked ? "bold " : "";
  attributes += markBlink->checkState() == Qt::Checked ? "blink " : "";
  attributes += markUnderline->checkState() == Qt::Checked ? "underline " : "";
  attributes += markInverse->checkState() == Qt::Checked ? "inverse " : "";
  foregroundColor = markForegroundColor->itemData(markForegroundColor->currentIndex()).toString();
  backgroundColor = markBackgroundColor->itemData(markBackgroundColor->currentIndex()).toString();
  QTextStream(&powwow_cmd) << pattern << "=" <<  attributes
      << QString("%1%2").arg(foregroundColor).arg(foregroundColor.isEmpty() ? "" : " ")
      << QString("%1%2").arg(backgroundColor.isEmpty() ? "" : "on ").arg(backgroundColor);
  qDebug("#mark %s", powwow_cmd.toAscii().data());

  parse_mark(powwow_cmd.toAscii().data());
  bool mbeg = pattern.indexOf("^") == 0;
  p = *(lookup_marker(mbeg ? pattern.mid(1).toAscii().data() : pattern.toAscii().data(), mbeg));

  if (addingNewMark) {
    // Add new item
    qDebug("Added new item.");
    item = new QTreeWidgetItem(markTable);
    addingNewMark = false;
    markTable->setEnabled(true);
    markAdd->setEnabled(true);
    markRemove->setEnabled(true);
  }
  markHash[item] = p;
  item->setText(0, attr_name(p->attrcode));
  item->setText(1, QString("%1%2").arg(p->mbeg ? "^" : "").arg(p->pattern));

  markTable->setFocus();
  markTable->setCurrentItem(item);
  markTable->scrollToItem(item);
}

void ObjectEditor::markAddClicked() {
  addingNewMark = true;

  markPattern->clear();
  markPattern->setFocus();
  markBold->setChecked(false);
  markBlink->setChecked(false);
  markInverse->setChecked(false);
  markUnderline->setChecked(false);
  markForegroundColor->setCurrentIndex(markForegroundColor->findText("none"));
  markBackgroundColor->setCurrentIndex(markBackgroundColor->findText("none"));

  markTable->setEnabled(false);
  markRemove->setEnabled(false);
  markAdd->setEnabled(false);
}

void ObjectEditor::markRemoveClicked() {
  QTreeWidgetItem *parent, *item = markTable->currentItem();

  if (!isCurrentMark("Remove")) return ;

  marknode *p = markHash[item];
  QString command;
  QTextStream(&command) << QString("%1%2").arg(p->mbeg ? "^" : "").arg(p->pattern) << "=";
  qDebug("#mark %s", command.toAscii().data() );
  parse_mark(command.toAscii().data());

  markHash.remove(item);
  markPattern->clear();
  markTable->setFocus();
  markBold->setChecked(false);
  markBlink->setChecked(false);
  markInverse->setChecked(false);
  markUnderline->setChecked(false);
  markForegroundColor->setCurrentIndex(markForegroundColor->findText("none"));
  markBackgroundColor->setCurrentIndex(markBackgroundColor->findText("none"));

  item->setHidden(true);  // HACK
  /*
  parent = item->parent();
  parent->removeChild(item);
  */
}


/*   BINDS TAB       */

void ObjectEditor::loadBindTab() {
  QString name, seq, call, command;
  QTreeWidgetItem *item;
  keynode *p;

  bindTable->clear();

  for (p = keydefs; p; p = p->next) {
    if (p->funct == key_run_command) {
      name = p->name;
      seq = seq_name(p->sequence, p->seqlen);
      call = p->call_data;
      command = internal_functions[lookup_edit_function(p->funct)].name;
      item = new QTreeWidgetItem(bindTable);
      item->setText(0, name);
      item->setText(1, seq);
      item->setText(2, call);
      item->setText(3, command);
      bindHash[item] = p;
    }
  }
}

void ObjectEditor::bindDetectClicked() {
  QString label("Blank"), seq;
  KeyBinder *dlg = new KeyBinder(label, seq, this);
  if (dlg->exec()) {
    qDebug("got result2 %s", seq.toAscii().constData());
  }
  delete dlg;
}

