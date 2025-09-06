#include <iostream>
#include "Planner.h"
using namespace std;

void mostrarMenu() {
    cout << "\n===== PLANNER VIRTUAL =====" << endl;
    cout << "1. Adicionar Meta" << endl;
    cout << "2. Adicionar Tarefa" << endl;
    cout << "3. Listar Metas" << endl;
    cout << "4. Listar Tarefas" << endl;
    cout << "0. Sair" << endl;
    cout << "Escolha uma opcao: ";
}

int main() {
    Planner p;
    int opcao;

    do {
        mostrarMenu();
        cin >> opcao;
        cin.ignore(); // limpa o buffer do cin

        if (opcao == 1) {
            string desc, cat;
            cout << "Descricao da meta: ";
            getline(cin, desc);
            cout << "Categoria: ";
            getline(cin, cat);
            p.adicionarMeta(Meta(desc, cat));
        }
        else if (opcao == 2) {
            string desc, cat, hora;
            cout << "Descricao da tarefa: ";
            getline(cin, desc);
            cout << "Categoria: ";
            getline(cin, cat);
            cout << "Horario (ex: 08:00-09:00): ";
            getline(cin, hora);
            p.adicionarTarefa(Tarefa(desc, cat, hora));
        }
        else if (opcao == 3) {
            p.listarMetas();
        }
        else if (opcao == 4) {
            p.listarTarefas();
        } 
        else if (opcao == 5) {
            p.listarMetas();
            int i, s;
            cout << "Escolha o indice da meta: ";
            cin >> i;
            cout << "Novo status (0=NAO_CONCLUIDO, 1=PARCIAL, 2=CONCLUIDO, 3=ADIADO): ";
            cin >> s;
            p.atualizarStatusMeta(i, (Status)s);
        }
        else if (opcao == 6) {
            p.listarTarefas();
            int i, s;
            cout << "Escolha o indice da tarefa: ";
            cin >> i;
            cout << "Novo status (0=NAO_CONCLUIDO, 1=PARCIAL, 2=CONCLUIDO, 3=ADIADO): ";
            cin >> s;
            p.atualizarStatusTarefa(i, (Status)s);
        }

    } while (opcao != 0);

    cout << "Saindo do planner..." << endl;
    return 0;
}
