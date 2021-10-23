#include "DataBase.h"

using namespace std;

int main() {
    cout.precision(6);
    DataBase db;
    db.Read(cin);
    db.Perform();
    db.Answer(cout);
    return 0;
}
