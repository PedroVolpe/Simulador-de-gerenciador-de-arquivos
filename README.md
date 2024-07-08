# Simulador de Sistema de Arquivos

Este projeto é um simulador de sistema de arquivos em C++ que permite criar diretórios, navegar entre eles, remover arquivos e exibir a estrutura de diretórios em profundidade ou por nível.

## Funcionalidades

- **Criar Diretórios**: Permite criar novos diretórios dentro do diretório atual.
- **Mudar Diretório Corrente**: Permite navegar para um diretório filho ou voltar para o diretório pai.
- **Remover Arquivo**: Permite remover arquivos do diretório atual.
- **Exibir Estrutura em Profundidade**: Exibe a árvore de arquivos realizando um percurso em profundidade.
- **Exibir Estrutura por Nível**: Exibe a árvore de arquivos realizando um percurso em largura.

## Estrutura do Código

- `struct Node`: Representa um nó na árvore de diretórios/arquivos.
- `buildTree(Node* root, const std::string& path)`: Constrói a árvore de diretórios e arquivos a partir de um caminho raiz.
- `createDirectory(Node* currentDir)`: Cria um novo diretório no diretório atual.
- `changeDirectory(Node*& currentDir)`: Muda o diretório corrente para um diretório filho ou para o diretório pai.
- `removeFile(Node* currentDir)`: Remove um arquivo do diretório atual.
- `printTreePerLevel(Node* root)`: Exibe a árvore de diretórios e arquivos por nível.
- `printTreeInOrder(Node* node, int level = 0)`: Exibe a árvore de diretórios e arquivos em profundidade.

## Como Compilar e Executar

### Pré-requisitos

- Compilador C++ (g++ recomendado)

### Compilação

Para compilar o projeto, use o comando:

```sh
g++ -o simulador_sistema_de_arquivos main.cpp
