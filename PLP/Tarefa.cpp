#include "Tarefa.h"

Tarefa::Tarefa(string desc, string cat, string hora) {
    descricao = desc;
    categoria = cat;
    horario = hora;
    status = NAO_CONCLUIDO;
}

void Tarefa::atualizarStatus(Status novo) {
    status = novo;
}

void Tarefa::exibir() {
    cout << "Tarefa: " << descricao 
         << " | Horario: " << horario 
         << " | Categoria: " << categoria 
         << " | Status: " << status << endl;
}
