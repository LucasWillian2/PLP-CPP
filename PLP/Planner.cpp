#include "Planner.h"
#include <iostream>
using namespace std;

void Planner::adicionarMeta(Meta m) {
    metas.push_back(m);
}

void Planner::adicionarTarefa(Tarefa t) {
    tarefas.push_back(t);
}

void Planner::listarMetas() {
    for (auto &m : metas) m.exibir();
}

void Planner::listarTarefas() {
    for (auto &t : tarefas) t.exibir();
}
void Planner::atualizarStatusMeta(int indice, Status novo) {
    if (indice >= 0 && indice < metas.size())
        metas[indice].atualizarStatus(novo);
    else
        cout << "Meta invalida!\n";
}

void Planner::atualizarStatusTarefa(int indice, Status novo) {
    if (indice >= 0 && indice < tarefas.size())
        tarefas[indice].atualizarStatus(novo);
    else
        cout << "Tarefa invalida!\n";
}