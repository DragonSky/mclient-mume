#include <QRegExp>

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


int main(int argc, char** argv) {

    ifstream fin;
    fin.open("../output/gildor_list.txt");

    /*
    QRegExp rx;
    QString index, item, cost;
    rx.setPattern("^(\\d+)\.(.+?) up to (.+?)$");
    if (rx.indexIn(str) != -1) {
        index = rx.cap(1);
        item = rx.cap(2);
        cost = rx.cap(3);
    }
    */

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
        if(ssize == 1 && sv[0] != "---") continue;
        if(sv[0] == "***") cout << "got a 'more' line!" << endl;
        else {
            for(unsigned int i=0; i<ssize; ++i) {
                if(i==0) {
                    stringstream token;
                    for(unsigned int j=0; j<sv[i].size(); ++j) {
                        if(sv[j] != ".") {
                            token << sv[j];
                        }
                    }
                    int number = atoi(token.str().c_str());
                    cout << number << endl;
                
                } else if(i != 0) {
                
                }
            }
        }
        cout << endl;
    }


    return 0;
}
