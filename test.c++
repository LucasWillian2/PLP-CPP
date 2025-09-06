#include <iostream>
using namespace std;

int main() {
    int x = 42;
    int* ptr = &x;
    
    cout << "Valor de x: " << x << endl;
    cout << "Endereco de x: " << ptr << endl;
    cout << "Valor apontado pelo ponteiro: " << *ptr << endl;

    return 0;
}