// Estado global da aplicação
let currentData = {
    metas: [],
    tarefas: [],
    lembretes: []
};

// Inicialização da aplicação
document.addEventListener('DOMContentLoaded', function() {
    initializeApp();
    setupEventListeners();
    loadData();
});

// Configurar os event listeners
function setupEventListeners() {
    // Navegação pelas abas
    document.querySelectorAll('.tab-btn').forEach(btn => {
        btn.addEventListener('click', function() {
            switchTab(this.dataset.tab);
        });
    });

    // Formulários
    document.getElementById('add-meta-form').addEventListener('submit', handleAddMeta);
    document.getElementById('add-tarefa-form').addEventListener('submit', handleAddTarefa);
    document.getElementById('add-lembrete-form').addEventListener('submit', handleAddLembrete);
    document.getElementById('edit-form').addEventListener('submit', handleEditItem);

    // Controle do período da meta
    document.getElementById('meta-periodo').addEventListener('change', function() {
        const numeroGroup = document.getElementById('meta-numero-group');
        const numeroInput = document.getElementById('meta-numero');
        const label = numeroGroup.querySelector('label');
        
        if (this.value === '2') { // Anual
            numeroGroup.style.display = 'none';
            numeroInput.value = '';
        } else {
            numeroGroup.style.display = 'block';
            if (this.value === '0') { // Semanal
                label.textContent = 'Número da Semana (1-52):';
                numeroInput.max = 52;
            } else { // Mensal
                label.textContent = 'Número do Mês (1-12):';
                numeroInput.max = 12;
            }
        }
    });

    // Fechar os modais ao clicar fora deles
    window.addEventListener('click', function(event) {
        if (event.target.classList.contains('modal')) {
            closeModal(event.target.id);
        }
    });
}

// Inicializar a aplicação
function initializeApp() {
    // Definir o ano atual como padrão
    const currentYear = new Date().getFullYear();
    document.getElementById('meta-ano').value = currentYear;
    document.getElementById('ano-relatorio').value = currentYear;
}

// Carrega os dados da API
async function loadData() {
    showLoading('metas-list');
    showLoading('tarefas-list');
    showLoading('lembretes-list');
    
    try {
        // Carrega os dados de todas as APIs
        const [metasResponse, tarefasResponse, lembretesResponse] = await Promise.all([
            fetch('/api/metas'),
            fetch('/api/tarefas'),
            fetch('/api/lembretes')
        ]);
        
        const metasData = await metasResponse.json();
        const tarefasData = await tarefasResponse.json();
        const lembretesData = await lembretesResponse.json();
        
        if (metasData.success) {
            currentData.metas = metasData.data;
        }
        
        if (tarefasData.success) {
            currentData.tarefas = tarefasData.data;
        }
        
        if (lembretesData.success) {
            currentData.lembretes = lembretesData.data;
        }
        
        renderAllData();
    } catch (error) {
        console.error('Erro ao carregar dados:', error);
        showMessage('Erro ao carregar dados. Verifique se a API está rodando.', 'error');
        
        // plano B para se tiver algum dados vazio na API
        currentData = {
            metas: [],
            tarefas: [],
            lembretes: []
        };
        renderAllData();
    }
}

// Renderiza todos os dados
function renderAllData() {
    renderMetas();
    renderTarefas();
    renderLembretes();
}

// Navegação pelas abas
function switchTab(tabName) {
    // Remove classe active de todas as abas e conteúdos
    document.querySelectorAll('.tab-btn').forEach(btn => btn.classList.remove('active'));
    document.querySelectorAll('.tab-content').forEach(content => content.classList.remove('active'));
    
    // Ativa a aba e o conteúdo selecionados
    document.querySelector(`[data-tab="${tabName}"]`).classList.add('active');
    document.getElementById(tabName).classList.add('active');
}

// ========== FUNÇÕES DE META ==========

function renderMetas() {
    const container = document.getElementById('metas-list');
    
    if (currentData.metas.length === 0) {
        container.innerHTML = '<div class="message info">Nenhuma meta cadastrada. Clique em "Nova Meta" para começar!</div>';
        return;
    }
    
    container.innerHTML = currentData.metas.map((meta, index) => `
        <div class="item-card">
            <div class="item-header">
                <div>
                    <div class="item-title">${meta.descricao}</div>
                    <div class="item-category">${meta.categoria}</div>
                </div>
                <div class="status-badge status-${getStatusClass(meta.status)}">
                    ${getStatusText(meta.status)}
                </div>
            </div>
            <div class="item-body">
                <div class="item-details">
                    <div class="detail-item">
                        <div class="detail-label">Período</div>
                        <div class="detail-value">${getPeriodoText(meta.periodo)}</div>
                    </div>
                    <div class="detail-item">
                        <div class="detail-label">Ano</div>
                        <div class="detail-value">${meta.ano}</div>
                    </div>
                    ${meta.periodo !== 2 ? `
                    <div class="detail-item">
                        <div class="detail-label">${meta.periodo === 0 ? 'Semana' : 'Mês'}</div>
                        <div class="detail-value">${meta.numero}</div>
                    </div>
                    ` : ''}
                </div>
            </div>
            <div class="item-actions">
                <button class="btn btn-sm btn-success" onclick="updateStatus(${index}, 'meta', 'success')">
                    <i class="fas fa-check"></i> Sucesso
                </button>
                <button class="btn btn-sm btn-warning" onclick="updateStatus(${index}, 'meta', 'partial')">
                    <i class="fas fa-minus"></i> Parcial
                </button>
                <button class="btn btn-sm btn-danger" onclick="updateStatus(${index}, 'meta', 'fail')">
                    <i class="fas fa-times"></i> Falhou
                </button>
                <button class="btn btn-sm btn-secondary" onclick="editItem(${index}, 'meta')">
                    <i class="fas fa-edit"></i> Editar
                </button>
                <button class="btn btn-sm btn-danger" onclick="deleteItem(${index}, 'meta')">
                    <i class="fas fa-trash"></i> Remover
                </button>
            </div>
        </div>
    `).join('');
}

function showAddMetaModal() {
    document.getElementById('add-meta-modal').style.display = 'block';
    document.getElementById('meta-descricao').focus();
}

async function handleAddMeta(event) {
    event.preventDefault();
    
    const formData = {
        descricao: document.getElementById('meta-descricao').value,
        categoria: document.getElementById('meta-categoria').value,
        periodo: parseInt(document.getElementById('meta-periodo').value),
        ano: parseInt(document.getElementById('meta-ano').value),
        numero: document.getElementById('meta-numero').value ? parseInt(document.getElementById('meta-numero').value) : 0,
        status: 0 // Não é atingida por padrão
    };
    
    try {
        const response = await fetch('/api/metas', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(formData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            // Recarregar os dados da API
            await loadData();
            showMessage('Meta adicionada com sucesso!', 'success');
        } else {
            showMessage('Erro ao adicionar meta: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao adicionar meta:', error);
        showMessage('Erro ao adicionar meta. Verifique se a API está rodando.', 'error');
    }
    
    // Limpar o formulário e fechar o modal
    document.getElementById('add-meta-form').reset();
    closeModal('add-meta-modal');
}

// ========== FUNÇÕES DE TAREFA ==========

function renderTarefas() {
    const container = document.getElementById('tarefas-list');
    
    if (currentData.tarefas.length === 0) {
        container.innerHTML = '<div class="message info">Nenhuma tarefa cadastrada. Clique em "Nova Tarefa" para começar!</div>';
        return;
    }
    
    container.innerHTML = currentData.tarefas.map((tarefa, index) => `
        <div class="item-card">
            <div class="item-header">
                <div>
                    <div class="item-title">${tarefa.descricao}</div>
                    <div class="item-category">${tarefa.categoria}</div>
                </div>
                <div class="status-badge status-${getTarefaStatusClass(tarefa.status)}">
                    ${getTarefaStatusText(tarefa.status)}
                </div>
            </div>
            <div class="item-body">
                <div class="item-details">
                    <div class="detail-item">
                        <div class="detail-label">Data</div>
                        <div class="detail-value">${tarefa.data}</div>
                    </div>
                    <div class="detail-item">
                        <div class="detail-label">Horário</div>
                        <div class="detail-value">${tarefa.horario}</div>
                    </div>
                </div>
            </div>
            <div class="item-actions">
                <button class="btn btn-sm btn-success" onclick="updateStatus(${index}, 'tarefa', 'executada')">
                    <i class="fas fa-check"></i> Executada
                </button>
                <button class="btn btn-sm btn-warning" onclick="updateStatus(${index}, 'tarefa', 'parcial')">
                    <i class="fas fa-minus"></i> Parcial
                </button>
                <button class="btn btn-sm btn-danger" onclick="updateStatus(${index}, 'tarefa', 'adiada')">
                    <i class="fas fa-clock"></i> Adiada
                </button>
                <button class="btn btn-sm btn-secondary" onclick="updateStatus(${index}, 'tarefa', 'nao_executada')">
                    <i class="fas fa-times"></i> Não Executada
                </button>
                <button class="btn btn-sm btn-secondary" onclick="editItem(${index}, 'tarefa')">
                    <i class="fas fa-edit"></i> Editar
                </button>
                <button class="btn btn-sm btn-danger" onclick="deleteItem(${index}, 'tarefa')">
                    <i class="fas fa-trash"></i> Remover
                </button>
            </div>
        </div>
    `).join('');
}

function showAddTarefaModal() {
    document.getElementById('add-tarefa-modal').style.display = 'block';
    document.getElementById('tarefa-descricao').focus();
}

async function handleAddTarefa(event) {
    event.preventDefault();
    
    const formData = {
        descricao: document.getElementById('tarefa-descricao').value,
        categoria: document.getElementById('tarefa-categoria').value,
        data: document.getElementById('tarefa-data').value,
        horario: document.getElementById('tarefa-horario').value,
        status: 3 // Não é executada por padrão
    };
    
    try {
        const response = await fetch('/api/tarefas', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(formData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            // Recarregar os dados da API
            await loadData();
            showMessage('Tarefa adicionada com sucesso!', 'success');
        } else {
            showMessage('Erro ao adicionar tarefa: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao adicionar tarefa:', error);
        showMessage('Erro ao adicionar tarefa. Verifique se a API está rodando.', 'error');
    }
    
    // Limpa o  formulário e fecha o modal
    document.getElementById('add-tarefa-form').reset();
    closeModal('add-tarefa-modal');
}

// ========== FUNÇÕES DE LEMBRETE ==========

function renderLembretes() {
    const container = document.getElementById('lembretes-list');
    
    if (currentData.lembretes.length === 0) {
        container.innerHTML = '<div class="message info">Nenhum lembrete cadastrado. Clique em "Novo Lembrete" para começar!</div>';
        return;
    }
    
    container.innerHTML = currentData.lembretes.map((lembrete, index) => `
        <div class="item-card">
            <div class="item-header">
                <div>
                    <div class="item-title">${lembrete.descricao}</div>
                    <div class="item-category">${getTipoLembreteText(lembrete.tipo)}</div>
                </div>
            </div>
            <div class="item-body">
                <div class="item-details">
                    <div class="detail-item">
                        <div class="detail-label">Dia da Semana</div>
                        <div class="detail-value">${lembrete.diaDaSemana}</div>
                    </div>
                </div>
            </div>
            <div class="item-actions">
                <button class="btn btn-sm btn-danger" onclick="deleteItem(${index}, 'lembrete')">
                    <i class="fas fa-trash"></i> Remover
                </button>
            </div>
        </div>
    `).join('');
}

function showAddLembreteModal() {
    document.getElementById('add-lembrete-modal').style.display = 'block';
    document.getElementById('lembrete-descricao').focus();
}

async function handleAddLembrete(event) {
    event.preventDefault();
    
    const formData = {
        descricao: document.getElementById('lembrete-descricao').value,
        tipo: parseInt(document.getElementById('lembrete-tipo').value),
        diaDaSemana: document.getElementById('lembrete-dia').value
    };
    
    try {
        const response = await fetch('/api/lembretes', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(formData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            // Recarregar os dados da API
            await loadData();
            showMessage('Lembrete adicionado com sucesso!', 'success');
        } else {
            showMessage('Erro ao adicionar lembrete: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao adicionar lembrete:', error);
        showMessage('Erro ao adicionar lembrete. Verifique se a API está rodando.', 'error');
    }
    
    // Limpa o  formulário e fecha o modal
    document.getElementById('add-lembrete-form').reset();
    closeModal('add-lembrete-modal');
}

// ========== FUNÇÕES DE EDIÇÃO ==========

function editItem(index, type) {
    const item = currentData[type + 's'][index];
    const modal = document.getElementById('edit-modal');
    const form = document.getElementById('edit-form');
    const content = document.getElementById('edit-form-content');
    const title = document.getElementById('edit-modal-title');
    
    // Configurar título
    title.textContent = `Editar ${type === 'meta' ? 'Meta' : type === 'tarefa' ? 'Tarefa' : 'Lembrete'}`;
    
    // Configurar formulário baseado no tipo
    if (type === 'meta') {
        content.innerHTML = `
            <div class="form-group">
                <label for="edit-descricao">Descrição:</label>
                <input type="text" id="edit-descricao" value="${item.descricao}" required>
            </div>
            <div class="form-group">
                <label for="edit-categoria">Categoria:</label>
                <input type="text" id="edit-categoria" value="${item.categoria}" required>
            </div>
        `;
    } else if (type === 'tarefa') {
        content.innerHTML = `
            <div class="form-group">
                <label for="edit-descricao">Descrição:</label>
                <input type="text" id="edit-descricao" value="${item.descricao}" required>
            </div>
            <div class="form-group">
                <label for="edit-categoria">Categoria:</label>
                <input type="text" id="edit-categoria" value="${item.categoria}" required>
            </div>
            <div class="form-group">
                <label for="edit-data">Data (dd/mm/aaaa):</label>
                <input type="text" id="edit-data" value="${item.data}" required>
            </div>
            <div class="form-group">
                <label for="edit-horario">Horário:</label>
                <input type="text" id="edit-horario" value="${item.horario}" required>
            </div>
        `;
    } else if (type === 'lembrete') {
        content.innerHTML = `
            <div class="form-group">
                <label for="edit-descricao">Descrição:</label>
                <input type="text" id="edit-descricao" value="${item.descricao}" required>
            </div>
            <div class="form-group">
                <label for="edit-tipo">Tipo:</label>
                <select id="edit-tipo" required>
                    <option value="0" ${item.tipo === 0 ? 'selected' : ''}>Ligação</option>
                    <option value="1" ${item.tipo === 1 ? 'selected' : ''}>Reunião</option>
                    <option value="2" ${item.tipo === 2 ? 'selected' : ''}>Compra</option>
                </select>
            </div>
            <div class="form-group">
                <label for="edit-dia">Dia da Semana:</label>
                <input type="text" id="edit-dia" value="${item.diaDaSemana}" required>
            </div>
        `;
    }
    
    // Para armazenar informações para o submit
    form.dataset.editIndex = index;
    form.dataset.editType = type;
    
    modal.style.display = 'block';
    document.getElementById('edit-descricao').focus();
}

async function handleEditItem(event) {
    event.preventDefault();
    
    const index = parseInt(event.target.dataset.editIndex);
    const type = event.target.dataset.editType;
    
    try {
        let updateData = {};
        let endpoint = '';
        
        if (type === 'meta') {
            updateData = {
                descricao: document.getElementById('edit-descricao').value,
                categoria: document.getElementById('edit-categoria').value
            };
            endpoint = `/api/metas/${index}`;
        } else if (type === 'tarefa') {
            updateData = {
                descricao: document.getElementById('edit-descricao').value,
                categoria: document.getElementById('edit-categoria').value,
                data: document.getElementById('edit-data').value,
                horario: document.getElementById('edit-horario').value
            };
            endpoint = `/api/tarefas/${index}`;
        } else if (type === 'lembrete') {
            updateData = {
                descricao: document.getElementById('edit-descricao').value,
                tipo: parseInt(document.getElementById('edit-tipo').value),
                diaDaSemana: document.getElementById('edit-dia').value
            };
            endpoint = `/api/lembretes/${index}`;
        }
        
        const response = await fetch(endpoint, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify(updateData)
        });
        
        const result = await response.json();
        
        if (result.success) {
            // Recarregar os dados da API
            await loadData();
            showMessage('Item editado com sucesso!', 'success');
        } else {
            showMessage('Erro ao editar item: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao editar item:', error);
        showMessage('Erro ao editar item. Verifique se a API está rodando.', 'error');
    }
    
    closeModal('edit-modal');
}

// ========== FUNÇÕES DE STATUS ==========

async function updateStatus(index, type, status) {
    try {
        let statusValue;
        let endpoint = '';
        
        if (type === 'meta') {
            const statusMap = {
                'success': 0,    // SUCESSO
                'partial': 2,    // PARCIALMENTE_ATINGIDA
                'fail': 1        // SEM_SUCESSO
            };
            statusValue = statusMap[status];
            endpoint = `/api/metas/${index}`;
        } else if (type === 'tarefa') {
            const statusMap = {
                'executada': 0,      // EXECUTADA
                'parcial': 1,        // PARCIALMENTE_EXECUTADA
                'adiada': 2,         // ADIADA
                'nao_executada': 3   // NAO_EXECUTADA
            };
            statusValue = statusMap[status];
            endpoint = `/api/tarefas/${index}`;
        }
        
        const response = await fetch(endpoint, {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ status: statusValue })
        });
        
        const result = await response.json();
        
        if (result.success) {
            // Recarregar os dados da API
            await loadData();
            showMessage('Status atualizado com sucesso!', 'success');
        } else {
            showMessage('Erro ao atualizar status: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao atualizar status:', error);
        showMessage('Erro ao atualizar status. Verifique se a API está rodando.', 'error');
    }
}

// ========== FUNÇÕES DE EXCLUSÃO ==========

async function deleteItem(index, type) {
    if (confirm('Tem certeza que deseja remover este item?')) {
        try {
            let endpoint = '';
            
            if (type === 'meta') {
                endpoint = `/api/metas/${index}`;
            } else if (type === 'tarefa') {
                endpoint = `/api/tarefas/${index}`;
            } else if (type === 'lembrete') {
                endpoint = `/api/lembretes/${index}`;
            }
            
            const response = await fetch(endpoint, {
                method: 'DELETE'
            });
            
            const result = await response.json();
            
            if (result.success) {
                // Recarregar os dados da API
                await loadData();
                showMessage('Item removido com sucesso!', 'success');
            } else {
                showMessage('Erro ao remover item: ' + result.error, 'error');
            }
        } catch (error) {
            console.error('Erro ao remover item:', error);
            showMessage('Erro ao remover item. Verifique se a API está rodando.', 'error');
        }
    }
}

// ========== FUNÇÕES DE RELATÓRIO ==========

async function gerarRelatorio() {
    const ano = document.getElementById('ano-relatorio').value;
    const container = document.getElementById('relatorio-content');
    
    if (!ano) {
        showMessage('Por favor, informe o ano para o relatório.', 'error');
        return;
    }
    
    showLoading('relatorio-content');
    
    try {
        const response = await fetch(`/api/relatorio/${ano}`);
        const result = await response.json();
        
        if (result.success) {
            renderRelatorio(result.data);
        } else {
            showMessage('Erro ao gerar relatório: ' + result.error, 'error');
        }
    } catch (error) {
        console.error('Erro ao gerar relatório:', error);
        showMessage('Erro ao gerar relatório. Verifique se a API está rodando.', 'error');
    }
}

function generateRelatorioData(ano) {
    const metasAno = currentData.metas.filter(meta => meta.ano === ano);
    const tarefasAno = currentData.tarefas.filter(tarefa => {
        const dataAno = parseInt(tarefa.data.split('/')[2]);
        return dataAno === ano;
    });
    
    const metasSucesso = metasAno.filter(meta => meta.status === 0).length;
    const tarefasExecutadas = tarefasAno.filter(tarefa => tarefa.status === 0).length;
    
    const categoriasMetas = {};
    const categoriasTarefas = {};
    
    metasAno.forEach(meta => {
        categoriasMetas[meta.categoria] = (categoriasMetas[meta.categoria] || 0) + 1;
    });
    
    tarefasAno.forEach(tarefa => {
        categoriasTarefas[tarefa.categoria] = (categoriasTarefas[tarefa.categoria] || 0) + 1;
    });
    
    return {
        ano,
        metas: {
            total: metasAno.length,
            sucesso: metasSucesso,
            percentual: metasAno.length > 0 ? (metasSucesso / metasAno.length * 100).toFixed(2) : 0,
            categorias: categoriasMetas
        },
        tarefas: {
            total: tarefasAno.length,
            executadas: tarefasExecutadas,
            percentual: tarefasAno.length > 0 ? (tarefasExecutadas / tarefasAno.length * 100).toFixed(2) : 0,
            categorias: categoriasTarefas
        }
    };
}

function renderRelatorio(relatorio) {
    const container = document.getElementById('relatorio-content');
    
    container.innerHTML = `
        <div class="report-section">
            <h3>Relatório Anual de ${relatorio.ano}</h3>
            
            <div class="report-stats">
                <div class="stat-card">
                    <div class="stat-value">${relatorio.metas.total}</div>
                    <div class="stat-label">Total de Metas</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">${relatorio.metas.sucesso}</div>
                    <div class="stat-label">Metas Cumpridas</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">${relatorio.metas.percentual}%</div>
                    <div class="stat-label">Taxa de Sucesso</div>
                </div>
            </div>
            
            <h4>Metas por Categoria:</h4>
            <ul class="categories-list">
                ${Object.entries(relatorio.metas.categorias).map(([categoria, count]) => 
                    `<li><span>${categoria}</span><span>${count} vez(es)</span></li>`
                ).join('')}
            </ul>
        </div>
        
        <div class="report-section">
            <div class="report-stats">
                <div class="stat-card">
                    <div class="stat-value">${relatorio.tarefas.total}</div>
                    <div class="stat-label">Total de Tarefas</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">${relatorio.tarefas.executadas}</div>
                    <div class="stat-label">Tarefas Executadas</div>
                </div>
                <div class="stat-card">
                    <div class="stat-value">${relatorio.tarefas.percentual}%</div>
                    <div class="stat-label">Taxa de Execução</div>
                </div>
            </div>
            
            <h4>Tarefas por Categoria:</h4>
            <ul class="categories-list">
                ${Object.entries(relatorio.tarefas.categorias).map(([categoria, count]) => 
                    `<li><span>${categoria}</span><span>${count} vez(es)</span></li>`
                ).join('')}
            </ul>
        </div>
    `;
}

// ========== FUNÇÕES AUXILIARES ==========

function getStatusText(status) {
    const statusMap = {
        0: 'Sucesso',
        1: 'Sem Sucesso',
        2: 'Parcialmente Atingida'
    };
    return statusMap[status] || 'Desconhecido';
}

function getStatusClass(status) {
    const classMap = {
        0: 'success',
        1: 'danger',
        2: 'warning'
    };
    return classMap[status] || 'info';
}

function getTarefaStatusText(status) {
    const statusMap = {
        0: 'Executada',
        1: 'Parcialmente Executada',
        2: 'Adiada',
        3: 'Não Executada'
    };
    return statusMap[status] || 'Desconhecido';
}

function getTarefaStatusClass(status) {
    const classMap = {
        0: 'success',
        1: 'warning',
        2: 'info',
        3: 'danger'
    };
    return classMap[status] || 'info';
}

function getPeriodoText(periodo) {
    const periodoMap = {
        0: 'Semanal',
        1: 'Mensal',
        2: 'Anual'
    };
    return periodoMap[periodo] || 'Desconhecido';
}

function getTipoLembreteText(tipo) {
    const tipoMap = {
        0: 'Ligação',
        1: 'Reunião',
        2: 'Compra'
    };
    return tipoMap[tipo] || 'Desconhecido';
}

function closeModal(modalId) {
    document.getElementById(modalId).style.display = 'none';
}

function showLoading(containerId) {
    const container = document.getElementById(containerId);
    container.innerHTML = '<div class="loading"><i class="fas fa-spinner"></i><br>Carregando...</div>';
}

function showMessage(message, type) {
    // Remover as mensagens existentes
    document.querySelectorAll('.message').forEach(msg => msg.remove());
    
    // Criar uma nova mensagem
    const messageDiv = document.createElement('div');
    messageDiv.className = `message ${type}`;
    messageDiv.textContent = message;
    
    // Inserir ela e exibir no topo do conteúdo principal
    const mainContent = document.querySelector('.main-content');
    mainContent.insertBefore(messageDiv, mainContent.firstChild);
    
    // Remover após 5 segundos
    setTimeout(() => {
        messageDiv.remove();
    }, 5000);
}
