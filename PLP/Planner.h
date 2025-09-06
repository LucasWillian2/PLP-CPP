#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include "Meta.h"
#include "Tarefa.h"
using namespace std;

class Planner {
public:
    vector<Meta> metas;
    vector<Tarefa> tarefas;

    void adicionarMeta(Meta m);
    void adicionarTarefa(Tarefa t);
    void listarMetas();
    void listarTarefas();
    void atualizarStatusMeta(int indice, Status novo);
    void atualizarStatusTarefa(int indice, Status novo);
};

#endif
