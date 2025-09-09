#include "Lembrete.h"
#include <iostream>

using namespace std;

// Implementação da função auxiliar
string tipoLembreteParaString(TipoLembrete tipo) {
    if (tipo == LIGACAO) return "Ligacao";
    if (tipo == REUNIAO) return "Reuniao";
    if (tipo == COMPRA) return "Compra";
    return "N/A";
}

Lembrete::Lembrete(const string& desc, TipoLembrete t, const string& dia) {
    descricao = desc;
    tipo = t;
    diaDaSemana = dia;
}

void Lembrete::exibir() const {
    cout << "Lembrete: " << descricao
         << " | Tipo: " << tipoLembreteParaString(tipo)
         << " | Dia: " << diaDaSemana << endl;
}