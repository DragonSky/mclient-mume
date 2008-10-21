#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <string>
using std::string;


int main(int argc, char** argv) {
    cout << "Hello, bitches!" << endl;

    string response;
    cin >> response;

    if(response == "list") {
        cout << "You told me to 'list', bitch." << endl;
    }

    cout << response << endl;
    return 0;
}
