#!/usr/bin/env python3
"""
API Server para o Planner Virtual
Conecta o frontend web com o backend C++
"""

import json
import subprocess
import os
import sys
from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import csv
from datetime import datetime

app = Flask(__name__)
CORS(app)  # Permite as requisições do frontend

# Configurações
PLANNER_EXECUTABLE = "../PLP/meu_programa.exe"
DATA_DIR = "../PLP"
METAS_FILE = os.path.join(DATA_DIR, "metas.csv")
TAREFAS_FILE = os.path.join(DATA_DIR, "tarefas.csv")
LEMBRETES_FILE = os.path.join(DATA_DIR, "lembretes.csv")

# Mapeamento de status
STATUS_META_MAP = {
    'SUCESSO': 0,
    'SEM_SUCESSO': 1,
    'PARCIALMENTE_ATINGIDA': 2
}

STATUS_TAREFA_MAP = {
    'EXECUTADA': 0,
    'PARCIALMENTE_EXECUTADA': 1,
    'ADIADA': 2,
    'NAO_EXECUTADA': 3
}

PERIODO_META_MAP = {
    'SEMANAL': 0,
    'MENSAL': 1,
    'ANUAL': 2
}

TIPO_LEMBRETE_MAP = {
    'LIGACAO': 0,
    'REUNIAO': 1,
    'COMPRA': 2
}

def reverse_map(mapping):
    """Inverte um mapeamento"""
    return {v: k for k, v in mapping.items()}

STATUS_META_REVERSE = reverse_map(STATUS_META_MAP)
STATUS_TAREFA_REVERSE = reverse_map(STATUS_TAREFA_MAP)
PERIODO_META_REVERSE = reverse_map(PERIODO_META_MAP)
TIPO_LEMBRETE_REVERSE = reverse_map(TIPO_LEMBRETE_MAP)

def load_csv_data(filename):
    """Carrega os dados de um arquivo CSV"""
    data = []
    if os.path.exists(filename):
        try:
            with open(filename, 'r', encoding='utf-8') as file:
                reader = csv.DictReader(file)
                for row in reader:
                    data.append(row)
        except Exception as e:
            print(f"Erro ao carregar {filename}: {e}")
    return data

def save_csv_data(filename, data, headers):
    """Salva os dados em um arquivo CSV"""
    try:
        with open(filename, 'w', newline='', encoding='utf-8') as file:
            writer = csv.DictWriter(file, fieldnames=headers)
            writer.writeheader()
            writer.writerows(data)
        return True
    except Exception as e:
        print(f"Erro ao salvar {filename}: {e}")
        return False

def run_planner_command(command_data):
    """Executa comando no planner C++ e retorna o resultado"""
    try:
        # Cria arquivo temporário com dados de entrada
        temp_input = "temp_input.txt"
        with open(temp_input, 'w') as f:
            f.write(json.dumps(command_data))
        
        # Executa o planner
        result = subprocess.run(
            [PLANNER_EXECUTABLE],
            input=json.dumps(command_data),
            text=True,
            capture_output=True,
            timeout=30
        )
        
        # Remove arquivo temporário
        if os.path.exists(temp_input):
            os.remove(temp_input)
        
        return {
            'success': result.returncode == 0,
            'stdout': result.stdout,
            'stderr': result.stderr
        }
    except subprocess.TimeoutExpired:
        return {'success': False, 'error': 'Timeout na execução'}
    except Exception as e:
        return {'success': False, 'error': str(e)}

@app.route('/')
def index():
    """Serve o arquivo index.html"""
    return send_from_directory('../frontend', 'index.html')

@app.route('/<path:filename>')
def serve_static(filename):
    """Serve arquivos estáticos do frontend"""
    return send_from_directory('../frontend', filename)

# ========== ROTAS DE META ==========

@app.route('/api/metas', methods=['GET'])
def get_metas():
    """Retorna todas as metas"""
    try:
        metas = load_csv_data(METAS_FILE)
        # Converte os dados para o formato do frontend
        formatted_metas = []
        for meta in metas:
            formatted_metas.append({
                'descricao': meta.get('descricao', ''),
                'categoria': meta.get('categoria', ''),
                'status': int(meta.get('status', 0)),
                'periodo': int(meta.get('periodo', 0)),
                'ano': int(meta.get('ano', 2024)),
                'numero': int(meta.get('numero', 0))
            })
        return jsonify({'success': True, 'data': formatted_metas})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/metas', methods=['POST'])
def add_meta():
    """Adiciona uma nova meta"""
    try:
        data = request.json
        metas = load_csv_data(METAS_FILE)
        
        # Adiciona uma nova meta
        new_meta = {
            'descricao': data['descricao'],
            'categoria': data['categoria'],
            'status': str(data.get('status', 0)),
            'periodo': str(data['periodo']),
            'ano': str(data['ano']),
            'numero': str(data.get('numero', 0))
        }
        metas.append(new_meta)
        
        # Salva os dados
        headers = ['descricao', 'categoria', 'status', 'periodo', 'ano', 'numero']
        if save_csv_data(METAS_FILE, metas, headers):
            return jsonify({'success': True, 'message': 'Meta adicionada com sucesso'})
        else:
            return jsonify({'success': False, 'error': 'Erro ao salvar meta'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/metas/<int:index>', methods=['PUT'])
def update_meta(index):
    """Atualiza uma meta existente"""
    try:
        data = request.json
        metas = load_csv_data(METAS_FILE)
        
        if 0 <= index < len(metas):
            # Atualiza oscampos fornecidos
            if 'descricao' in data:
                metas[index]['descricao'] = data['descricao']
            if 'categoria' in data:
                metas[index]['categoria'] = data['categoria']
            if 'status' in data:
                metas[index]['status'] = str(data['status'])
            
            # Salva os dados
            headers = ['descricao', 'categoria', 'status', 'periodo', 'ano', 'numero']
            if save_csv_data(METAS_FILE, metas, headers):
                return jsonify({'success': True, 'message': 'Meta atualizada com sucesso'})
            else:
                return jsonify({'success': False, 'error': 'Erro ao salvar meta'})
        else:
            return jsonify({'success': False, 'error': 'Índice inválido'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/metas/<int:index>', methods=['DELETE'])
def delete_meta(index):
    """Remove uma meta"""
    try:
        metas = load_csv_data(METAS_FILE)
        
        if 0 <= index < len(metas):
            metas.pop(index)
            
            # Salva os dados
            headers = ['descricao', 'categoria', 'status', 'periodo', 'ano', 'numero']
            if save_csv_data(METAS_FILE, metas, headers):
                return jsonify({'success': True, 'message': 'Meta removida com sucesso'})
            else:
                return jsonify({'success': False, 'error': 'Erro ao salvar dados'})
        else:
            return jsonify({'success': False, 'error': 'Índice inválido'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

# ========== ROTAS DE TAREFA ==========

@app.route('/api/tarefas', methods=['GET'])
def get_tarefas():
    """Retorna todas as tarefas"""
    try:
        tarefas = load_csv_data(TAREFAS_FILE)
        # Converte os dados para o formato do frontend
        formatted_tarefas = []
        for tarefa in tarefas:
            formatted_tarefas.append({
                'descricao': tarefa.get('descricao', ''),
                'categoria': tarefa.get('categoria', ''),
                'data': tarefa.get('data', ''),
                'horario': tarefa.get('horario', ''),
                'status': int(tarefa.get('status', 3))
            })
        return jsonify({'success': True, 'data': formatted_tarefas})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/tarefas', methods=['POST'])
def add_tarefa():
    """Adiciona uma nova tarefa"""
    try:
        data = request.json
        tarefas = load_csv_data(TAREFAS_FILE)
        
        # Adiciona uma nova tarefa
        new_tarefa = {
            'descricao': data['descricao'],
            'categoria': data['categoria'],
            'data': data['data'],
            'horario': data['horario'],
            'status': str(data.get('status', 3))
        }
        tarefas.append(new_tarefa)
        
        # Salva os dados
        headers = ['descricao', 'categoria', 'data', 'horario', 'status']
        if save_csv_data(TAREFAS_FILE, tarefas, headers):
            return jsonify({'success': True, 'message': 'Tarefa adicionada com sucesso'})
        else:
            return jsonify({'success': False, 'error': 'Erro ao salvar tarefa'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/tarefas/<int:index>', methods=['PUT'])
def update_tarefa(index):
    """Atualiza uma tarefa existente"""
    try:
        data = request.json
        tarefas = load_csv_data(TAREFAS_FILE)
        
        if 0 <= index < len(tarefas):
            # Atualiza os campos fornecidos
            if 'descricao' in data:
                tarefas[index]['descricao'] = data['descricao']
            if 'categoria' in data:
                tarefas[index]['categoria'] = data['categoria']
            if 'data' in data:
                tarefas[index]['data'] = data['data']
            if 'horario' in data:
                tarefas[index]['horario'] = data['horario']
            if 'status' in data:
                tarefas[index]['status'] = str(data['status'])
            
            # Salva os dados
            headers = ['descricao', 'categoria', 'data', 'horario', 'status']
            if save_csv_data(TAREFAS_FILE, tarefas, headers):
                return jsonify({'success': True, 'message': 'Tarefa atualizada com sucesso'})
            else:
                return jsonify({'success': False, 'error': 'Erro ao salvar tarefa'})
        else:
            return jsonify({'success': False, 'error': 'Índice inválido'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/tarefas/<int:index>', methods=['DELETE'])
def delete_tarefa(index):
    """Remove uma tarefa"""
    try:
        tarefas = load_csv_data(TAREFAS_FILE)
        
        if 0 <= index < len(tarefas):
            tarefas.pop(index)
            
            # Salva os dados
            headers = ['descricao', 'categoria', 'data', 'horario', 'status']
            if save_csv_data(TAREFAS_FILE, tarefas, headers):
                return jsonify({'success': True, 'message': 'Tarefa removida com sucesso'})
            else:
                return jsonify({'success': False, 'error': 'Erro ao salvar dados'})
        else:
            return jsonify({'success': False, 'error': 'Índice inválido'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

# ========== ROTAS DE LEMBRETE ==========

@app.route('/api/lembretes', methods=['GET'])
def get_lembretes():
    """Retorna todos os lembretes"""
    try:
        lembretes = load_csv_data(LEMBRETES_FILE)
        # Converte os dados para o formato do front
        formatted_lembretes = []
        for lembrete in lembretes:
            formatted_lembretes.append({
                'descricao': lembrete.get('descricao', ''),
                'tipo': int(lembrete.get('tipo', 0)),
                'diaDaSemana': lembrete.get('diaDaSemana', '')
            })
        return jsonify({'success': True, 'data': formatted_lembretes})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/lembretes', methods=['POST'])
def add_lembrete():
    """Adiciona um novo lembrete"""
    try:
        data = request.json
        lembretes = load_csv_data(LEMBRETES_FILE)
        
        # Adiciona um novo lembrete
        new_lembrete = {
            'descricao': data['descricao'],
            'tipo': str(data['tipo']),
            'diaDaSemana': data['diaDaSemana']
        }
        lembretes.append(new_lembrete)
        
        # Salva os dados
        headers = ['descricao', 'tipo', 'diaDaSemana']
        if save_csv_data(LEMBRETES_FILE, lembretes, headers):
            return jsonify({'success': True, 'message': 'Lembrete adicionado com sucesso'})
        else:
            return jsonify({'success': False, 'error': 'Erro ao salvar lembrete'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

@app.route('/api/lembretes/<int:index>', methods=['DELETE'])
def delete_lembrete(index):
    """Remove um lembrete"""
    try:
        lembretes = load_csv_data(LEMBRETES_FILE)
        
        if 0 <= index < len(lembretes):
            lembretes.pop(index)
            
            # Salva os dados
            headers = ['descricao', 'tipo', 'diaDaSemana']
            if save_csv_data(LEMBRETES_FILE, lembretes, headers):
                return jsonify({'success': True, 'message': 'Lembrete removido com sucesso'})
            else:
                return jsonify({'success': False, 'error': 'Erro ao salvar dados'})
        else:
            return jsonify({'success': False, 'error': 'Índice inválido'})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

# ========== ROTAS DE RELATÓRIO ==========

@app.route('/api/relatorio/<int:ano>', methods=['GET'])
def get_relatorio(ano):
    """Gera relatório anual"""
    try:
        metas = load_csv_data(METAS_FILE)
        tarefas = load_csv_data(TAREFAS_FILE)
        
        # Filtra os dados do ano
        metas_ano = [meta for meta in metas if int(meta.get('ano', 0)) == ano]
        tarefas_ano = [tarefa for tarefa in tarefas if 
                      len(tarefa.get('data', '')) == 10 and 
                      int(tarefa['data'].split('/')[2]) == ano]
        
        # Calcula as estatísticas das metas
        metas_sucesso = len([m for m in metas_ano if int(m.get('status', 0)) == 0])
        metas_total = len(metas_ano)
        metas_percentual = (metas_sucesso / metas_total * 100) if metas_total > 0 else 0
        
        # Calcula as estatísticas das tarefas
        tarefas_executadas = len([t for t in tarefas_ano if int(t.get('status', 3)) == 0])
        tarefas_total = len(tarefas_ano)
        tarefas_percentual = (tarefas_executadas / tarefas_total * 100) if tarefas_total > 0 else 0
        
        # Categorias das metas
        categorias_metas = {}
        for meta in metas_ano:
            cat = meta.get('categoria', 'Sem categoria')
            categorias_metas[cat] = categorias_metas.get(cat, 0) + 1
        
        # Categorias das tarefas
        categorias_tarefas = {}
        for tarefa in tarefas_ano:
            cat = tarefa.get('categoria', 'Sem categoria')
            categorias_tarefas[cat] = categorias_tarefas.get(cat, 0) + 1
        
        relatorio = {
            'ano': ano,
            'metas': {
                'total': metas_total,
                'sucesso': metas_sucesso,
                'percentual': round(metas_percentual, 2),
                'categorias': categorias_metas
            },
            'tarefas': {
                'total': tarefas_total,
                'executadas': tarefas_executadas,
                'percentual': round(tarefas_percentual, 2),
                'categorias': categorias_tarefas
            }
        }
        
        return jsonify({'success': True, 'data': relatorio})
    except Exception as e:
        return jsonify({'success': False, 'error': str(e)})

if __name__ == '__main__':
    # Cria diretórios se eles não existirem
    os.makedirs(DATA_DIR, exist_ok=True)
    
    # Cria arquivos CSV se eles não existirem
    if not os.path.exists(METAS_FILE):
        with open(METAS_FILE, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(['descricao', 'categoria', 'status', 'periodo', 'ano', 'numero'])
    
    if not os.path.exists(TAREFAS_FILE):
        with open(TAREFAS_FILE, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(['descricao', 'categoria', 'data', 'horario', 'status'])
    
    if not os.path.exists(LEMBRETES_FILE):
        with open(LEMBRETES_FILE, 'w', newline='', encoding='utf-8') as f:
            writer = csv.writer(f)
            writer.writerow(['descricao', 'tipo', 'diaDaSemana'])
    
    print("Iniciando servidor da API...")
    print("Frontend disponível em: http://localhost:5000")
    print("API disponível em: http://localhost:5000/api/")
    
    app.run(debug=True, host='0.0.0.0', port=5000)
