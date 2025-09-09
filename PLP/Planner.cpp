#include "Planner.h"
#include "Lembrete.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;

// Métodos de Adicionar e Listar (já devem estar corretos)
void Planner::adicionarMeta(const Meta& m) { metas.push_back(m); }
void Planner::adicionarTarefa(const Tarefa& t) { tarefas.push_back(t); }

void Planner::listarMetas() {
    cout << "\n--- LISTA DE METAS ---\n";
    if (metas.empty()) { cout << "Nenhuma meta cadastrada.\n"; return; }
    for (int i = 0; i < metas.size(); ++i) { cout << "[" << i << "] "; metas[i].exibir(); }
}

void Planner::listarTarefas() {
    cout << "\n--- LISTA DE TAREFAS ---\n";
    if (tarefas.empty()) { cout << "Nenhuma tarefa cadastrada.\n"; return; }
    for (int i = 0; i < tarefas.size(); ++i) { cout << "[" << i << "] "; tarefas[i].exibir(); }
}

// Métodos de Atualizar Status (já devem estar corretos)
void Planner::atualizarStatusMeta(int indice, StatusMeta novo) {
    if (indice >= 0 && indice < metas.size()) { metas[indice].atualizarStatus(novo); }
    else { cout << "Erro: Indice de meta invalido!\n"; }
}

void Planner::atualizarStatusTarefa(int indice, StatusTarefa novo) {
    if (indice >= 0 && indice < tarefas.size()) { tarefas[indice].atualizarStatus(novo); }
    else { cout << "Erro: Indice de tarefa invalido!\n"; }
}

// ++ IMPLEMENTAÇÃO DE REMOVER E EDITAR ++
void Planner::removerMeta(int indice) {
    if (indice >= 0 && indice < metas.size()) {
        metas.erase(metas.begin() + indice);
        cout << "Meta removida com sucesso!\n";
    } else {
        cout << "Erro: Indice invalido!\n";
    }
}

void Planner::removerTarefa(int indice) {
    if (indice >= 0 && indice < tarefas.size()) {
        tarefas.erase(tarefas.begin() + indice);
        cout << "Tarefa removida com sucesso!\n";
    } else {
        cout << "Erro: Indice invalido!\n";
    }
}

void Planner::editarMeta(int indice, const string& novaDesc, const string& novaCat) {
    if (indice >= 0 && indice < metas.size()) {
        metas[indice].setDescricao(novaDesc);
        metas[indice].setCategoria(novaCat);
        cout << "Meta editada com sucesso!\n";
    } else {
        cout << "Erro: Indice invalido!\n";
    }
}

void Planner::editarTarefa(int indice, const string& novaDesc, const string& novaCat, const string& novaData, const string& novoHorario) {
    if (indice >= 0 && indice < tarefas.size()) {
        tarefas[indice].setDescricao(novaDesc);
        tarefas[indice].setCategoria(novaCat);
        tarefas[indice].setData(novaData);
        tarefas[indice].setHorario(novoHorario);
        cout << "Tarefa editada com sucesso!\n";
    } else {
        cout << "Erro: Indice invalido!\n";
    }
}

void Planner::adicionarLembrete(const Lembrete& l) {
    lembretes.push_back(l);
}

void Planner::listarLembretes() {
    cout << "\n--- LISTA DE LEMBRETES SEMANAIS ---\n";
    if (lembretes.empty()) {
        cout << "Nenhum lembrete cadastrado.\n";
        return;
    }
    for (int i = 0; i < lembretes.size(); ++i) {
        cout << "[" << i << "] ";
        lembretes[i].exibir();
    }
}

void Planner::gerarRelatorio() const {
    if (metas.empty() && tarefas.empty()) {
        cout << "\nNao ha dados suficientes para gerar um relatorio.\n";
        return;
    }

    int ano;
    cout << "\n--- GERACAO DE RELATORIO ---\n";
    cout << "Digite o ano para o relatorio: ";
    cin >> ano;

    // ----- CÁLCULO PARA METAS ANUAIS -----
    int metasAnuaisTotal = 0;
    int metasAnuaisSucesso = 0;
    map<string, int> categoriasMetas;

    for (const auto& meta : metas) {
        if (meta.getAno() == ano) {
            metasAnuaisTotal++;
            if (meta.getStatus() == SUCESSO) {
                metasAnuaisSucesso++;
            }
            categoriasMetas[meta.getCategoria()]++;
        }
    }

    // ----- CÁLCULO PARA TAREFAS DURANTE O ANO -----
    int tarefasAnuaisTotal = 0;
    int tarefasAnuaisExecutadas = 0;
    map<string, int> categoriasTarefas;

    for (const auto& tarefa : tarefas) {
        // Extrai o ano da string de data "dd/mm/aaaa"
        if (tarefa.getData().length() == 10 && stoi(tarefa.getData().substr(6)) == ano) {
            tarefasAnuaisTotal++;
            if (tarefa.getStatus() == EXECUTADA) {
                tarefasAnuaisExecutadas++;
            }
            categoriasTarefas[tarefa.getCategoria()]++;
        }
    }

    // ----- EXIBIÇÃO DO RELATÓRIO -----
    cout << "\n===== RELATORIO ANUAL DE " << ano << " =====\n";
    cout << fixed << setprecision(2); // Configura para 2 casas decimais

    // Estatísticas de Metas
    cout << "\n--- METAS ---\n";
    if (metasAnuaisTotal > 0) {
        double percMetas = (double)metasAnuaisSucesso / metasAnuaisTotal * 100.0;
        cout << "Total de Metas: " << metasAnuaisTotal << endl;
        cout << "Metas cumpridas com Sucesso: " << metasAnuaisSucesso << " (" << percMetas << "%)\n";
        
        cout << "Categorias de Metas mais realizadas:\n";
        for (const auto& par : categoriasMetas) {
            cout << "  - " << par.first << ": " << par.second << " vez(es)\n";
        }
    } else {
        cout << "Nenhuma meta encontrada para este ano.\n";
    }

    // Estatísticas de Tarefas
    cout << "\n--- TAREFAS ---\n";
    if (tarefasAnuaisTotal > 0) {
        double percTarefas = (double)tarefasAnuaisExecutadas / tarefasAnuaisTotal * 100.0;
        cout << "Total de Tarefas: " << tarefasAnuaisTotal << endl;
        cout << "Tarefas Executadas: " << tarefasAnuaisExecutadas << " (" << percTarefas << "%)\n";

        cout << "Categorias de Tarefas mais realizadas:\n";
        for (const auto& par : categoriasTarefas) {
            cout << "  - " << par.first << ": " << par.second << " vez(es)\n";
        }
    } else {
        cout << "Nenhuma tarefa encontrada para este ano.\n";
    }

    cout << "\n=====================================\n";
}

void Planner::salvarDados(const string& fMetas, const string& fTarefas, const string& fLembretes) {
    ofstream outFileLembretes(fLembretes);
    if (outFileLembretes.is_open()) {
        for (const auto& lembrete : lembretes) {
            outFileLembretes << lembrete.getDescricao() << ","
                             << lembrete.getTipo() << ","
                             << lembrete.getDiaDaSemana() << "\n";
        }
        outFileLembretes.close();
    }
}

void Planner::carregarDados(const string& fMetas, const string& fTarefas, const string& fLembretes) {
    ifstream inFileLembretes(fLembretes);
    string linha;
    if (inFileLembretes.is_open()) {
        while (getline(inFileLembretes, linha)) {
            stringstream ss(linha);
            string desc, dia;
            int tipoInt;

            getline(ss, desc, ',');
            ss >> tipoInt; ss.ignore();
            getline(ss, dia, ',');

            adicionarLembrete(Lembrete(desc, (TipoLembrete)tipoInt, dia));
        }
        inFileLembretes.close();
    }
}

bool Planner::temLembretes() const { return !lembretes.empty(); }
bool Planner::temMetas() const { return !metas.empty(); }
bool Planner::temTarefas() const { return !tarefas.empty(); }