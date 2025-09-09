#ifndef META_H
#define META_H

#include <string>

// Status específicos para Metas, conforme a especificação
enum StatusMeta { SUCESSO, SEM_SUCESSO, PARCIALMENTE_ATINGIDA };

// Período de tempo para a Meta
enum PeriodoMeta { SEMANAL, MENSAL, ANUAL };

// Função auxiliar para exibir o período
inline std::string periodoParaString(PeriodoMeta p) {
    if (p == SEMANAL) return "Semanal";
    if (p == MENSAL) return "Mensal";
    if (p == ANUAL) return "Anual";
    return "N/A";
}

// Função auxiliar para exibir o status da meta
inline std::string statusMetaParaString(StatusMeta s) {
    if (s == SUCESSO) return "Sucesso";
    if (s == SEM_SUCESSO) return "Sem Sucesso";
    if (s == PARCIALMENTE_ATINGIDA) return "Parcialmente Atingida";
    return "N/A";
}

class Meta {
private:
    std::string descricao;
    std::string categoria;
    StatusMeta status;
    PeriodoMeta periodo;
    int ano;
    int numero; // Guarda o nº da semana ou do mês

public:
    // Construtor atualizado
    Meta(std::string desc, std::string cat, PeriodoMeta p, int a, int num);

    void atualizarStatus(StatusMeta novo);
    void exibir();

    // Getters para os novos atributos
    std::string getDescricao() const { return descricao; }
    std::string getCategoria() const { return categoria; }
    StatusMeta getStatus() const { return status; }
    PeriodoMeta getPeriodo() const { return periodo; }
    int getAno() const { return ano; }
    int getNumero() const { return numero; }

    //Setters
    void setDescricao(const std::string& novaDesc) { descricao = novaDesc; }
    void setCategoria(const std::string& novaCat) { categoria = novaCat; }
};

#endif