#include "big_integer.h"

using namespace std;

int main() {
    big_integer a("-1591563309890326054125627839548891585559049824963");
    big_integer b("-3417856182746231874623148723164812376512852437523846123876");
    big_integer c = (b >> 31);

    cout << a;
    cout << b;
    cout << c;

    return 0;
}
