# Frontend do Planner Virtual

Este é o frontend web para o sistema Planner Virtual, desenvolvido em HTML, CSS e JavaScript, com uma API REST em Python Flask para comunicação com o backend C++.

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