#include <iostream>
#include "Planner.h"
#include "Lembrete.h"
using namespace std;

void mostrarMenu() {
    cout << "\n===== PLANNER VIRTUAL =====\n";
    cout << "1. Adicionar Meta\n";
    cout << "2. Adicionar Tarefa\n";
    cout << "3. Listar Metas\n";
    cout << "4. Listar Tarefas\n";
    cout << "5. Atualizar Status de Meta\n";
    cout << "6. Atualizar Status de Tarefa\n";
    cout << "7. Editar Meta\n";     
    cout << "8. Remover Meta\n";    
    cout << "9. Editar Tarefa\n";   
    cout << "10. Remover Tarefa\n"; 
    cout << "11. Adicionar Lembrete Semanal\n";
    cout << "12. Listar Lembretes\n";
    cout << "13. Gerar Relatorio Anual\n";
    cout << "0. Sair\n";
    cout << "Escolha uma opcao: ";
}

int main() {
    Planner p;
    int opcao;

    const string arquivoMetas = "metas.csv";
    const string arquivoTarefas = "tarefas.csv";
    const string arquivoLembretes = "lembretes.csv";

    p.carregarDados(arquivoMetas, arquivoTarefas, arquivoLembretes);
    cout << "Dados carregados com sucesso!" << endl;

    do {
        mostrarMenu();
        cin >> opcao;
        cin.ignore(); // limpa o buffer do cin

        if (opcao == 1) {
            string desc, cat;
            int p_opcao, ano, num;
            cout << "Descricao da meta: ";
            getline(cin, desc);
            cout << "Categoria: ";
            getline(cin, cat);
            cout << "Periodo (0=Semanal, 1=Mensal, 2=Anual): ";
            cin >> p_opcao;
            PeriodoMeta periodo = (PeriodoMeta)p_opcao;
            cout << "Ano: ";
            cin >> ano;
            if (periodo != ANUAL) {
                cout << (periodo == SEMANAL ? "Numero da Semana: " : "Numero do Mes: ");
                cin >> num;
            } else {
                num = 0;
            }
            cin.ignore();
            p.adicionarMeta(Meta(desc, cat, periodo, ano, num));
        }
        else if (opcao == 2) {
            string desc, cat, hora, data;
            cout << "Descricao da tarefa: ";
            getline(cin, desc);
            cout << "Categoria: ";
            getline(cin, cat);
            cout << "Data (dd/mm/aaaa): ";
            getline(cin, data);
            cout << "Horario (ex: 08:00-09:00): ";
            getline(cin, hora);
            p.adicionarTarefa(Tarefa(desc, cat, hora, data));
        }
        else if (opcao == 3) {
            p.listarMetas();
        }
        else if (opcao == 4) {
            p.listarTarefas();
        }
        else if (opcao == 5) {
            p.listarMetas();
            if (p.temMetas()) {
                int i, s;
                cout << "Escolha o indice da meta: ";
                cin >> i;
                cout << "Novo status (0=Sucesso, 1=Sem Sucesso, 2=Parcialmente Atingida): ";
                cin >> s;
                p.atualizarStatusMeta(i, (StatusMeta)s);
            }
        }
        else if (opcao == 6) {
            p.listarTarefas();
            int i, s;
            cout << "Escolha o indice da tarefa: ";
            cin >> i;
            cout << "Novo status (0=NAO_CONCLUIDO, 1=PARCIAL, 2=CONCLUIDO, 3=ADIADO): ";
            cin >> s;
            p.atualizarStatusTarefa(i, (StatusTarefa)s);
        }
        else if (opcao == 7) { // Editar Meta
            p.listarMetas();
            if (p.temMetas()) {
                int i;
                cout << "Escolha o indice da meta para editar: ";
                cin >> i;
                cin.ignore();

                string novaDesc, novaCat;
                cout << "Nova descricao: ";
                getline(cin, novaDesc);
                cout << "Nova categoria: ";
                getline(cin, novaCat);
                p.editarMeta(i, novaDesc, novaCat);
            } else {
                cout << "Nenhuma meta para editar." << endl;
            }
        }
        else if (opcao == 8) { // Remover Meta
            p.listarMetas();
            if (p.temMetas()) {
                int i;
                cout << "Escolha o indice da meta para remover: ";
                cin >> i;
                cin.ignore();
                p.removerMeta(i);
            } else {
                cout << "Nenhuma meta para remover." << endl;
            }
        }
        else if (opcao == 9) { // Editar Tarefa
            p.listarTarefas();
            if (p.temTarefas()) {
                int i;
                cout << "Escolha o indice da tarefa para editar: ";
                cin >> i;
                cin.ignore();

                string novaDesc, novaCat, novaData, novoHorario;
                cout << "Nova descricao: ";
                getline(cin, novaDesc);
                cout << "Nova categoria: ";
                getline(cin, novaCat);
                cout << "Nova data (dd/mm/aaaa): ";
                getline(cin, novaData);
                cout << "Novo horario: ";
                getline(cin, novoHorario);
                p.editarTarefa(i, novaDesc, novaCat, novaData, novoHorario);
            } else {
                cout << "Nenhuma tarefa para editar." << endl;
            }
        }
        else if (opcao == 10) { // Remover Tarefa
            p.listarTarefas();
            if (p.temTarefas()) {
                int i;
                cout << "Escolha o indice da tarefa para remover: ";
                cin >> i;
                cin.ignore();
                p.removerTarefa(i);
            } else {
                cout << "Nenhuma tarefa para remover." << endl;
            }
        }
        else if (opcao == 11) { // Adicionar Lembrete
                string desc, dia;
                int tipoInt;

                cout << "Descricao do lembrete: ";
                getline(cin, desc);
                cout << "Tipo (0=Ligacao, 1=Reuniao, 2=Compra): ";
                cin >> tipoInt;
                cin.ignore();
                cout << "Dia da Semana: ";
                getline(cin, dia);

                p.adicionarLembrete(Lembrete(desc, (TipoLembrete)tipoInt, dia));
                cout << "Lembrete adicionado com sucesso!\n";
        }
        else if (opcao == 12) {
                p.listarLembretes();
        }
        else if (opcao == 13) {
            p.gerarRelatorio();
        }

    } while (opcao != 0);

    p.salvarDados(arquivoMetas, arquivoTarefas, arquivoLembretes);
    cout << "Dados salvos com sucesso!" << endl;

    cout << "Saindo do planner..." << endl;
    return 0;
}
