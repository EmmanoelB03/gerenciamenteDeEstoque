# Sistema de Gerenciamento Simples com GTK (Projeto Hemir)

Este é um programa de desktop simples desenvolvido em C com a biblioteca GTK+ 3. Ele oferece funcionalidades básicas de login de usuário e gerenciamento de registros (CRUD - Criar, Ler, Atualizar, Deletar), com os dados sendo armazenados em arquivos de texto locais. A interface gráfica é carregada a partir de um arquivo Glade (`projeto_hemir.glade`).

## Funcionalidades Principais

* **Autenticação de Usuário:** Sistema de login para acessar as funcionalidades do programa.
* **Cadastro de Dados:** Permite registrar novas entradas (ex: informações de equipamentos, usuários, etc.).
* **Consulta de Dados:** Exibe os registros armazenados.
* **Alteração de Dados:** Permite modificar registros existentes.
* **Remoção de Dados:** Permite excluir registros.
* **Persistência de Dados:** As informações são salvas em arquivos de texto (`dados.txt` para login, `bancoDeDados.txt` para os registros principais).
* **Interface Gráfica:** Construída com GTK+ 3 e definida em um arquivo `.glade`.
* **Reinicialização da Aplicação:** Função para reiniciar o programa (específica para Windows).

## Pré-requisitos

Para compilar e executar este programa, você precisará de:

* Um compilador C (como GCC, preferencialmente via MinGW no Windows).
* As bibliotecas de desenvolvimento do GTK+ 3.
* Sistema Operacional Windows (a função de reinicialização `restartApplication` utiliza `ShellExecute` da API do Windows).
* O arquivo de interface `projeto_hemir.glade` deve estar presente no mesmo diretório do executável.
* Arquivos de dados:
    * `dados.txt`: Para as credenciais de login.
    * `bancoDeDados.txt`: Para os dados da aplicação.

## Como Compilar

Você precisará ter o ambiente de desenvolvimento GTK+ 3 configurado. Se você estiver usando MinGW no Windows, certifique-se de que o compilador consiga encontrar os cabeçalhos e bibliotecas do GTK.

Um comando de compilação comum, assumindo que `pkg-config` está configurado e seu arquivo de código se chama `main.c`:

```bash
gcc main.c -o projeto_hemir.exe $(pkg-config --cflags --libs gtk+-3.0)
