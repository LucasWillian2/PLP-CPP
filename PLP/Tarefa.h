#ifndef TAREFA_H
#define TAREFA_H

#include <iostream>
#include <string>
using namespace std;

#include "Meta.h" // usa o enum Status

class Tarefa {
public:
    string descricao;
    string categoria;
    string horario;
    Status status;

    Tarefa(string desc, string cat, string hora);
    void atualizarStatus(Status novo);
    void exibir();
};

#endif
