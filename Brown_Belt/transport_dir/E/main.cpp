#include "DataBase.h"

using namespace std;

int main() {
    try {
        cout.precision(6);
        DataBase db;
        db.Read(cin);
        db.Perform();
        db.Answer(cout);
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
