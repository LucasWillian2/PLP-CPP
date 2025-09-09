#ifndef TAREFA_H
#define TAREFA_H

#include <string>

// Status específicos para Tarefas, conforme a especificação
enum StatusTarefa { EXECUTADA, PARCIALMENTE_EXECUTADA, ADIADA, NAO_EXECUTADA };

// Função auxiliar para exibir o status da tarefa
inline std::string statusTarefaParaString(StatusTarefa s) {
    if (s == EXECUTADA) return "Executada";
    if (s == PARCIALMENTE_EXECUTADA) return "Parcialmente Executada";
    if (s == ADIADA) return "Adiada";
    if (s == NAO_EXECUTADA) return "Nao Executada";
    return "N/A";
}

class Tarefa {
private:
    std::string descricao;
    std::string categoria;
    std::string horario;
    std::string data; // Novo atributo para a data
    StatusTarefa status;

public:
    // Construtor atualizado
    Tarefa(std::string desc, std::string cat, std::string hora, std::string dt);

    void atualizarStatus(StatusTarefa novo);
    void exibir();

    // Getters
    std::string getDescricao() const { return descricao; }
    std::string getCategoria() const { return categoria; }
    std::string getHorario() const { return horario; }
    std::string getData() const { return data; }
    StatusTarefa getStatus() const { return status; }

    //Setters
    void setDescricao(const std::string& novaDesc) { descricao = novaDesc; }
    void setCategoria(const std::string& novaCat) { categoria = novaCat; }
    void setHorario(const std::string& novoHorario) { horario = novoHorario; }
    void setData(const std::string& novaData) { data = novaData; }
};

#endif