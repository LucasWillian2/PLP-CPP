#include "Tarefa.h"
#include <iostream>

using namespace std;

// Implementação do construtor atualizado
Tarefa::Tarefa(string desc, string cat, string hora, string dt) {
    descricao = desc;
    categoria = cat;
    horario = hora;
    data = dt;
    status = NAO_EXECUTADA; // Status inicial padrão
}

void Tarefa::atualizarStatus(StatusTarefa novo) {
    status = novo;
}

void Tarefa::exibir() {
    cout << "Tarefa: " << descricao
         << " | Data: " << data // Exibindo a nova informação
         << " | Horario: " << horario
         << " | Categoria: " << categoria
         << " | Status: " << statusTarefaParaString(status) << endl;
}