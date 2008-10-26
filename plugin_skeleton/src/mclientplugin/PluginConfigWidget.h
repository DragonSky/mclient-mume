#ifndef PLUGINCONFIGWIDGET_H
#define PLUGINCONFIGWIDGET_H

#include <QWidget>

#include <QHash>

class QItemSelection;
class QModelIndex;
class QPluginLoader;
class QPushButton;
class QString;


class PluginConfigWidget : public QWidget {
    Q_OBJECT
    
    public:
        PluginConfigWidget(QHash<QString, QPluginLoader*> plugins,
                QWidget* parent=0);
        ~PluginConfigWidget();

    public slots:
        void on_configure(const QModelIndex& index);
        void updateSelection(const QItemSelection &selected,
            const QItemSelection &deselected);

    private:
        QHash<QString, QPluginLoader*> _plugins;
        QPushButton* _configButton;

};


#endif /* PLUGINCONFIGWIDGET_H */
