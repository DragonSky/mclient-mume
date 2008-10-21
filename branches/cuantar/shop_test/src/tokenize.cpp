#include <QApplication>
#include <QGroupBox>
#include <QList>
#include <QMainWindow>
#include <QPushButton>
#include <QStandardItemModel>
#include <QTreeView>
#include <QVBoxLayout>

#include <cstdlib>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <sstream>
using std::stringstream;
#include <vector>
using std::vector;


void tokenize(const string& fname, QStandardItem* parentItem);
    

int main(int argc, char** argv) {

    string fname;
    if(argc < 2) return 1;
    else fname = argv[1];

    QApplication app(argc, argv);

    QStandardItemModel* model = new QStandardItemModel();
    QStandardItem* parentItem = model->invisibleRootItem();
    tokenize(fname, parentItem);
    QStringList labels;
    labels << "Index" << "Item name" << "Price";
    model->setHorizontalHeaderLabels(labels);
    
    QTreeView* tree = new QTreeView();
    tree->setModel(model);
    tree->setColumnWidth(1,400);
    tree->setRootIsDecorated(false);
    tree->setSelectionMode(QAbstractItemView::MultiSelection);

    QVBoxLayout* layout = new QVBoxLayout();
    QGroupBox* group = new QGroupBox();
    group->setLayout(layout);

    layout->addWidget(tree);

    QPushButton* button = new QPushButton();
    button->setText("Buy these things");

    layout->addWidget(button);

    QMainWindow mw;
    mw.setCentralWidget(group);
    mw.setMinimumSize(800,600);
    mw.setWindowTitle("MUME Shopper");

    mw.show();

    return app.exec();
}


void tokenize(const string& fname, QStandardItem* parentItem) {
    ifstream fin;
    fin.open(fname.c_str());

    char* buf = new char[128];
    while(fin.good()) {
        fin.getline(buf, 128);
        stringstream stream;
        stream << buf;
        if(stream.str() == "You can buy:") continue;
        vector<string> sv;
        while(stream.good()) {
            string str;
            stream >> str;
            sv.push_back(str);
        }
        //cout << "vector size: " << sv.size() << endl;
        unsigned int ssize = sv.size();
        if(ssize == 1) {
            if(sv[0] == "-----") {
                QList<QStandardItem*> itemList;
                itemList.append(new QStandardItem("-----"));
                itemList.append(new QStandardItem("-----"));
                itemList.append(new QStandardItem("-----"));
                parentItem->appendRow(itemList);

            } else if(sv[0] == "") {
            }
            continue;
        }
        if(sv[0] == "***") continue;//cout << "got a 'more' line!" << endl;
        else {
            stringstream item;
            stringstream price;
            stringstream token;
            bool num_end = false;
            bool found_price = false;
            int index = -1;
            for(unsigned int i=0; i<ssize; ++i) {
                if(i==0) {
                    item.str("");
                    price.str("");
                    token.str("");
                    for(unsigned int j=0; j<sv[i].size(); ++j) {
                        //cout << "j is " << j << endl;
                        if(sv[i][j] != '.') {
                        //cout << "adding " << sv[i][j] << " to token" << endl;
                            token << sv[i][j];
                        } else {
                            num_end = true;
                        }
                    }
                    if(num_end == true) {
                        index = atoi(token.str().c_str());
                    } //else cout << "this line has no number" << endl;
                
                } else { // not the first token
                    if(sv[i] != "up" && sv[i] != "for" 
                            && found_price == false) {
                        item << sv[i];
                        if(i != ssize) item << " ";
                        
                    } else if(found_price == true) {
                        // already found a price
                        if(sv[i] == "to") continue;
                        price << sv[i];
                        if(i != ssize) price << " ";

                    } else {
                        found_price = true;
                    }

                    /*
                    if(nmap.find(sv[i]) != nmap.end()) {

                    
                    }
                    */

                } // if
            }
            QList<QStandardItem*> itemList;
            itemList.append(new QStandardItem(QString::number(index)));
            itemList.append(new QStandardItem(item.str().c_str()));
            itemList.append(new QStandardItem(price.str().c_str()));
            parentItem->appendRow(itemList);
            /*
            cout << "index " << index << ":" << endl; 
            cout << "\tthe item is: " << item.str() << endl;
            cout << "\tits price is: " << price.str() << endl;
            */
        }
    }
}
