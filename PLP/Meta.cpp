#include "Meta.h"

Meta::Meta(string desc, string cat) {
    descricao = desc;
    categoria = cat;
    status = NAO_CONCLUIDO;
}

void Meta::atualizarStatus(Status novo) {
    status = novo;
}

void Meta::exibir() {
    cout << "Meta: " << descricao 
         << " | Categoria: " << categoria 
         << " | Status: " << status << endl;
}
