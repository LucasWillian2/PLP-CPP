#ifndef META_H
#define META_H

#include <iostream>
#include <string>
using namespace std;

enum Status { NAO_CONCLUIDO, PARCIAL, CONCLUIDO, ADIADO };

class Meta {
public:
    string descricao;
    string categoria;
    Status status;

    Meta(string desc, string cat);
    void atualizarStatus(Status novo);
    void exibir();
};

#endif
