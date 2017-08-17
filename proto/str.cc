#include <string>
#include <iostream>
using namespace std;
int main() {
    std::string str = "sfda";
    str.append("sdf");
    cout << str << endl;
    str.at(1) = '\0';
    cout << str << endl;
    cout << str.length() << endl;
    cout << str.size() << endl;
}
