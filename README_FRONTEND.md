# Frontend do Planner Virtual

Este é o frontend web para o sistema Planner Virtual, desenvolvido em HTML, CSS e JavaScript, com uma API REST em Python Flask para comunicação com o backend C++.

## 🚀 Funcionalidades

### Interface Web Moderna
- **Design Responsivo**: Funciona em desktop, tablet e mobile
- **Interface Intuitiva**: Navegação por abas para diferentes seções
- **Feedback Visual**: Mensagens de sucesso/erro e animações
- **Tema Moderno**: Gradientes e sombras para uma aparência profissional

### Gestão de Metas
- ✅ Adicionar metas com período (Semanal/Mensal/Anual)
- ✅ Listar e visualizar todas as metas
- ✅ Editar descrição e categoria das metas
- ✅ Atualizar status (Sucesso/Sem Sucesso/Parcialmente Atingida)
- ✅ Remover metas

### Gestão de Tarefas
- ✅ Adicionar tarefas com data e horário
- ✅ Listar e visualizar todas as tarefas
- ✅ Editar descrição, categoria, data e horário
- ✅ Atualizar status (Executada/Parcialmente Executada/Adiada/Não Executada)
- ✅ Remover tarefas

### Gestão de Lembretes
- ✅ Adicionar lembretes semanais por tipo (Ligação/Reunião/Compra)
- ✅ Listar e visualizar todos os lembretes
- ✅ Remover lembretes

### Relatórios
- ✅ Geração de relatórios anuais
- ✅ Estatísticas de metas e tarefas
- ✅ Análise por categorias
- ✅ Percentuais de sucesso/execução

## 🛠️ Tecnologias Utilizadas

### Frontend
- **HTML5**: Estrutura semântica
- **CSS3**: Estilos modernos com Flexbox e Grid
- **JavaScript ES6+**: Lógica da aplicação
- **Font Awesome**: Ícones
- **Responsive Design**: Mobile-first approach

### Backend API
- **Python 3.8+**: Linguagem da API
- **Flask**: Framework web
- **Flask-CORS**: Cross-origin resource sharing
- **CSV**: Persistência de dados

### Integração
- **REST API**: Comunicação entre frontend e backend
- **JSON**: Formato de dados
- **Fetch API**: Requisições HTTP

## 📁 Estrutura do Projeto

```
frontend/
├── index.html          # Página principal
├── styles.css          # Estilos CSS
└── script.js           # Lógica JavaScript

api/
└── server.py           # Servidor Flask da API

requirements.txt        # Dependências Python
README_FRONTEND.md      # Esta documentação
```

## 🚀 Como Executar

### Pré-requisitos
- Python 3.8 ou superior
- Navegador web moderno
- Backend C++ compilado (meu_programa.exe)

### Instalação

1. **Instalar dependências Python:**
   ```bash
   pip install -r requirements.txt
   ```

2. **Compilar o backend C++ (se ainda não foi feito):**
   ```bash
   cd PLP
   g++ main.cpp Planner.cpp Meta.cpp Tarefa.cpp Lembrete.cpp -o meu_programa
   ```

3. **Executar a API:**
   ```bash
   python api/server.py
   ```

4. **Acessar o frontend:**
   - Abra seu navegador
   - Acesse: `http://localhost:5000`

## 🔧 Configuração

### Porta do Servidor
Por padrão, a API roda na porta 5000. Para alterar, edite o arquivo `api/server.py`:

```python
app.run(debug=True, host='0.0.0.0', port=5000)  # Altere a porta aqui
```

### Caminhos dos Arquivos
Os caminhos dos arquivos CSV e executável podem ser ajustados no início do arquivo `api/server.py`:

```python
PLANNER_EXECUTABLE = "../PLP/meu_programa.exe"
DATA_DIR = "../PLP"
METAS_FILE = os.path.join(DATA_DIR, "metas.csv")
TAREFAS_FILE = os.path.join(DATA_DIR, "tarefas.csv")
LEMBRETES_FILE = os.path.join(DATA_DIR, "lembretes.csv")
```

## 📱 Interface do Usuário

### Navegação
- **Aba Metas**: Gerencia metas por período
- **Aba Tarefas**: Gerencia tarefas com data/horário
- **Aba Lembretes**: Gerencia lembretes semanais
- **Aba Relatórios**: Gera relatórios anuais

### Ações Disponíveis
- **Adicionar**: Botão "+" em cada seção
- **Editar**: Ícone de lápis em cada item
- **Atualizar Status**: Botões coloridos para cada status
- **Remover**: Ícone de lixeira em cada item

### Feedback Visual
- **Mensagens**: Sucesso (verde), erro (vermelho), info (azul)
- **Status**: Cores diferentes para cada status
- **Loading**: Indicadores de carregamento
- **Animações**: Transições suaves

## 🔄 Fluxo de Dados

1. **Frontend** → Requisição HTTP → **API Flask**
2. **API Flask** → Leitura/Escrita → **Arquivos CSV**
3. **API Flask** → Resposta JSON → **Frontend**
4. **Frontend** → Atualização da Interface

## 🐛 Solução de Problemas

### Erro de Conexão
- Verifique se a API está rodando na porta 5000
- Confirme se o backend C++ está compilado
- Verifique os caminhos dos arquivos

### Dados Não Carregam
- Verifique se os arquivos CSV existem
- Confirme as permissões de leitura/escrita
- Verifique o console do navegador para erros

### Interface Não Responsiva
- Verifique se o JavaScript está habilitado
- Confirme se os arquivos CSS estão carregando
- Teste em diferentes navegadores

## 🔮 Melhorias Futuras

- [ ] Autenticação de usuários
- [ ] Sincronização em tempo real
- [ ] Notificações push
- [ ] Exportação de dados (PDF, Excel)
- [ ] Temas personalizáveis
- [ ] Modo offline
- [ ] PWA (Progressive Web App)

## 📄 Licença

Este projeto é parte do sistema Planner Virtual desenvolvido para fins educacionais.

## 👥 Contribuição

Para contribuir com melhorias:
1. Faça um fork do projeto
2. Crie uma branch para sua feature
3. Implemente as mudanças
4. Teste thoroughly
5. Submeta um pull request

---

**Desenvolvido com ❤️ para o Planner Virtual**
