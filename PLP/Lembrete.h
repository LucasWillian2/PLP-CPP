#ifndef LEMBRETE_H
#define LEMBRETE_H

#include <string>

// Enum para os tipos de lembrete, conforme a especificação
enum TipoLembrete { LIGACAO, REUNIAO, COMPRA };

// Função auxiliar para converter o tipo para string
std::string tipoLembreteParaString(TipoLembrete tipo);

class Lembrete {
private:
    std::string descricao;
    TipoLembrete tipo;
    std::string diaDaSemana;

public:
    Lembrete(const std::string& desc, TipoLembrete t, const std::string& dia);

    void exibir() const;

    // Getters necessários para salvar os dados
    std::string getDescricao() const { return descricao; }
    TipoLembrete getTipo() const { return tipo; }
    std::string getDiaDaSemana() const { return diaDaSemana; }
};

#endif