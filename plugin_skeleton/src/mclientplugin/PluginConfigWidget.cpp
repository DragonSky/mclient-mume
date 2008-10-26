#include "PluginConfigWidget.h"

#include "MClientPluginInterface.h"

#include <QDebug>
#include <QPluginLoader>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QTreeView>
#include <QVBoxLayout>


PluginConfigWidget::PluginConfigWidget(QHash<QString, QPluginLoader*> plugins,
        QWidget* parent) : QWidget(parent) {

    _plugins = plugins;

    QWidget* window = new QWidget();
    QStandardItemModel* model = new QStandardItemModel(window);
    QStandardItem* parentItem = model->invisibleRootItem();
        
    QStringList labels;
    labels << "Plugin" << "Description";
    model->setHorizontalHeaderLabels(labels);
    
    QPluginLoader* pl;
    foreach(pl, _plugins) {
        MClientPluginInterface* ip = 
                qobject_cast<MClientPluginInterface*>(pl->instance());

        if(!ip) continue;
        QList<QStandardItem*> itemList;
        QStandardItem* first = new QStandardItem(ip->longName());
        first->setData(ip->shortName(), Qt::UserRole);
        itemList.append(first);
        itemList.append(new QStandardItem(ip->description()));
        QStandardItem* item;
        foreach(item, itemList) {
            item->setEditable(false);
        }
        parentItem->appendRow(itemList);

    }
    
    QTreeView* tree = new QTreeView(this);
    tree->setModel(model);
    tree->setColumnWidth(1,400);
    tree->setRootIsDecorated(false);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->addWidget(tree);
    this->setLayout(vlayout);
    this->setMinimumSize(500,500);

    connect(tree->selectionModel(), 
            SIGNAL(selectionChanged(const QItemSelection&, 
                    const QItemSelection&)), 
            this, 
            SLOT(updateSelection(const QItemSelection&, 
                    const QItemSelection&)));
}


PluginConfigWidget::~PluginConfigWidget() {
}


void PluginConfigWidget::updateSelection(const QItemSelection &selected,
        const QItemSelection &deselected) {

    QModelIndex index;
    QModelIndexList items = selected.indexes();
    
    foreach(index, items) {
        if(index.column() == 0) {
            QString sn = index.data(Qt::UserRole).toString();
            qDebug() << sn;
            QHash<QString, QPluginLoader*>::iterator it = _plugins.find(sn);
            if(it != _plugins.end()) {
                MClientPluginInterface* pi = 
                    qobject_cast<MClientPluginInterface*>(it.value()->instance());
                if(!pi) qDebug() << "suck";
                if(pi->configurable()) qDebug() << "it is configurable";
            }
        }
        // Check whether the item represented by index is configurable, and if
        // so, enable the Configure..." button.
        
    }
}


void PluginConfigWidget::on_configure(const QModelIndex& index) {
    // The Configure... button has been pressed, so we want to get the plugin
    // at the current index and configure() it.

    /*
    if(ip->configurable()) {
        ip->configure();
    }
    */
    qDebug() << "Checking to see if something is configurable";
}
