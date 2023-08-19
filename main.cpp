//------------------------------------------------------------
//Nome: Eric Akio Uchiyamada                TIA: 42107865
//Nome: Lucas Goulart de Farias Meres       TIA: 42127459
//Nome: Oliver Kieran Galvão McCormack      TIA: 42122058
//Nome: Pedro Loureiro Morone Branco Volpe  TIA: 42131936
//Nome: Renan Tagliaferro                   TIA: 42105846
//------------------------------------------------------------

#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <queue>
#include <sys/stat.h>
#include <unistd.h>

struct Node
{
  std::string name; // Nome do diretório ou arquivo
  std::string path; // Caminho a partir da raiz até o ID
  char tipo; // Se é Arquivo ‘a’ ou Diretório ‘d’
  Node* dir_ant; // Diretório anterior
  std::vector<Node*> children; // Lista de diretórios ou arquivos a partir do corrente

  Node(const std::string& name, const std::string& path, char tipo, Node* dir_ant)
      : name(name), path(path), tipo(tipo), dir_ant(dir_ant) {}

  ~Node()
  {
    for (auto child : children)
    {
      delete child;
    }
  }
};

void buildTree(Node* root, const std::string& path)
{
  DIR* dir; // Variável resposável pela abertura dos diretórios.
  struct dirent* entry;

  dir = opendir(path.c_str()); // Abre os diretórios e retorna para dir
  if (dir == nullptr)
  {
    std::cerr << "Erro ao abrir o diretório: " << path << std::endl;
    return;
  }

  // Para o loop caso a sequencia de diretórios tenha acabado, ou seja, entry == nullptr
  while ((entry = readdir(dir)) != nullptr){
    if (entry->d_type == DT_DIR)
    {
      // Cria uma varíavel que será o nome do diretório
      std::string name = entry->d_name;  
      if (name != "." && name != "..")   // Checa se 
      {
        std::string childPath = path + "/" + name;
        Node* child = new Node(name, childPath, 'd', root);
        root->children.push_back(child);
        buildTree(child, childPath);
      }
    }
      // Verifica se a entrada atual é um arquivo regular.
    else if (entry->d_type == DT_REG) 
    {
      std::string name = entry->d_name;
      std::string filePath = path + "/" + name;
      Node* child = new Node(name, filePath, 'a', root);
      root->children.push_back(child);
    }
  }

  closedir(dir);
}

void createDirectory(Node* currentDir)
{
  std::string dirName;
  std::cout << "Digite o nome do diretório a ser criado no diretório atual: ";
  std::cin >> dirName;

  std::string dirPath = currentDir->path + "/" + dirName;

  // "0777" indica que o diretório terá permissões de leitura, gravação e execução para o usuário, grupo e outros
  if (mkdir(dirPath.c_str(), 0777) == 0) 
  {
    // Cria um novo nó que armazenará informações do diretório
    Node* newDir = new Node(dirName, dirPath, 'd', currentDir); 
    currentDir->children.push_back(newDir);
    std::cout << "Diretório criado com sucesso!" << std::endl;
  }
  else
  {
    std::cout << "Erro ao criar diretório." << std::endl;
  }
}

void changeDirectory(Node*& currentDir)
{
  std::string dirName;
  std::cout << "Digite o nome do diretório para onde deseja ir: ";
  std::cin >> dirName;

  if (dirName == "..") // Checa se o diretório se chama ".." para evitar erros de criação
  {
    if (currentDir->dir_ant != nullptr)
    {
      currentDir = currentDir->dir_ant; // Transforma o diretório atual ao dir anterior
      std::cout << "Diretório corrente alterado para: " << currentDir->path << std::endl;
    }
    else
    {
      std::cout << "Não é possível voltar mais um nível." << std::endl;
    }
  }
  else
  {
    bool found = false;

    for (auto child : currentDir->children)
    {
      if (child->name == dirName && child->tipo == 'd')
      {
        currentDir = child;
        found = true;
        break;
      }
    }

    if (found)
    {
      std::cout << "Diretório corrente alterado para: " << currentDir->path << std::endl;
    }
    else
    {
      std::cout << "Diretório não encontrado." << std::endl;
    }
  }
}


void removeFile(Node* currentDir)
{
  std::string fileName;
  std::cout << "Digite o nome do arquivo a ser removido (o arquivo deve estár na pasta atual): ";
  std::cin >> fileName;

  std::string filePath = currentDir->path + "/" + fileName;

  // Verifica se o arquivo existe
  if (access(filePath.c_str(), F_OK) == -1)
  {
    std::cout << "O arquivo não existe." << std::endl;
    return;
  }

  // Verifica as permissões de escrita no diretório
  if (access(currentDir->path.c_str(), W_OK) == -1)
  {
    std::cout << "Sem permissão para remover arquivos neste diretório." << std::endl;
    return;
  }

  // Tenta remover o arquivo
  if (remove(filePath.c_str()) == 0)
  {
    // Remove o nó correspondente ao arquivo da lista de filhos do diretório corrente
    for (auto it = currentDir->children.begin(); it != currentDir->children.end(); ++it)
    {
      if ((*it)->name == fileName && (*it)->tipo == 'a')
      {
        delete *it;
        currentDir->children.erase(it);
        std::cout << "Arquivo removido com sucesso!" << std::endl;
        return;
      }
    }
  }

  // Imprime uma mensagem de erro com base no motivo da falha na remoção do arquivo
  if (errno == EACCES)
  {
    std::cout << "Sem permissão para remover o arquivo." << std::endl;
  }
  else if (errno == ENOENT)
  {
    std::cout << "O arquivo não existe." << std::endl;
  }
  else
  {
    std::cout << "Erro ao remover o arquivo." << std::endl;
  }
}

void printTreePerLevel(Node* root)
{
  std::queue<Node*> nodeQueue;
  nodeQueue.push(root);

  while (!nodeQueue.empty())
  {
    int levelSize = nodeQueue.size();

    for (int i = 0; i < levelSize; i++)
    {
      Node* node = nodeQueue.front();
      nodeQueue.pop();

      std::cout << node->path << "\n";

      for (auto child : node->children)
      {
        nodeQueue.push(child);
      }
    }

    std::cout << std::endl;
  }
}

void printTreeInOrder(Node* node, int level = 0)
{
  if (node->tipo == 'a')
  {
    std::cout << std::string(level * 2, ' ') << node->path << std::endl;
  }
  else if (node->tipo == 'd')
  {
    std::cout << std::string(level * 2, ' ') <<  node->path << std::endl;
    for (auto child : node->children)
    {
      printTreeInOrder(child, level + 1);
    }
  }
}

void printMenu()
{
  std::cout << "--------------------------------------" << std::endl;
  std::cout << "Simulador de Sistemas de Arquvios:" << std::endl;
  std::cout << "m - Criar diretório" << std::endl;
  std::cout << "c - Mudar diretório corrente" << std::endl;
  std::cout << "d - Remover arquivo" << std::endl;
  std::cout << "p - Apresentar a árvore realizando um percurso em profundidade" << std::endl;
  std::cout << "l - Apresentar a árvore realizando um percurso em largura" << std::endl;
  std::cout << "s - Sair" << std::endl;
  std::cout << "--------------------------------------" << std::endl;
}

int main()
{
  
  std::string directoryPath;
  std::cout << "Digite o caminho do diretório raiz: ";
  std::getline(std::cin, directoryPath);

  Node* root = new Node(directoryPath, directoryPath, 'd', nullptr);
  buildTree(root, directoryPath);

  Node* currentDir = root; 

  std::string option;

  while (true)
  {
    printMenu();
    std::cout << "Digite a opção desejada: ";
    std::cin >> option;

    if (option == "m")
    {
      createDirectory(currentDir);
    }
    else if (option == "c")
    {
      changeDirectory(currentDir);
    }
    else if (option == "d")
    {
      removeFile(currentDir);
    }
    else if (option == "p")
    {
      std::cout << "Árvore de arquivos em Profundidade (Em ordem):" << std::endl;
      printTreeInOrder(currentDir);
      std::cout << std::endl;
    }
    else if (option == "l")
    {
      std::cout << "Árvore de arquivos em Nível:" << std::endl;
      printTreePerLevel(currentDir);
      std::cout << std::endl;
    }
    else if (option == "s")
    {
      break;
    }
    else
    {
      std::cout << "Opção inválida." << std::endl;
    }
  }
    
  delete root;
  
  return 0;
}
