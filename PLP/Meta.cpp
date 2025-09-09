#include "Meta.h"
#include <iostream>
#include <string> // Incluir para to_string

using namespace std;

// Implementação do construtor atualizado
Meta::Meta(string desc, string cat, PeriodoMeta p, int a, int num) {
    descricao = desc;
    categoria = cat;
    periodo = p;
    ano = a;
    numero = num;
    status = PARCIALMENTE_ATINGIDA; // Um status inicial padrão
}

void Meta::atualizarStatus(StatusMeta novo) {
    status = novo;
}

void Meta::exibir() {
    cout << "Meta: " << descricao
         << " | Categoria: " << categoria
         << " | Periodo: " << periodoParaString(periodo)
         << " " << (periodo != ANUAL ? to_string(numero) + "/" : "") << ano
         << " | Status: " << statusMetaParaString(status) << endl;
}