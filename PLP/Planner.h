#ifndef PLANNER_H
#define PLANNER_H

#include <vector>
#include <string>
#include "Meta.h"
#include "Tarefa.h"
#include "Lembrete.h"

class Planner {
private:
    std::vector<Meta> metas;
    std::vector<Tarefa> tarefas;
    std::vector<Lembrete> lembretes;

public:
    // Métodos para Metas e Tarefas
    void adicionarMeta(const Meta& m);
    void adicionarTarefa(const Tarefa& t);
    void listarMetas();
    void listarTarefas();
    void atualizarStatusMeta(int indice, StatusMeta novo);
    void atualizarStatusTarefa(int indice, StatusTarefa novo);
    void removerMeta(int indice);
    void removerTarefa(int indice);
    void editarMeta(int indice, const std::string& novaDesc, const std::string& novaCat);
    void editarTarefa(int indice, const std::string& novaDesc, const std::string& novaCat, const std::string& novaData, const std::string& novoHorario);
    bool temMetas() const;
    bool temTarefas() const;
    void adicionarLembrete(const Lembrete& l);
    void listarLembretes();
    bool temLembretes() const;

    // Persistência de dados
    void salvarDados(const std::string& fMetas, const std::string& fTarefas, const std::string& fLembretes);
    void carregarDados(const std::string& fMetas, const std::string& fTarefas, const std::string& fLembretes);

    void gerarRelatorio() const;
};

#endif