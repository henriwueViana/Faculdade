/* ============================================================================
   DISCIPLINA: LINGUAGEM E TÉCNICAS DE PROGRAMAÇÃO / Linguagem C
   ATIVIDADE: MAPA - Sistema de Gerenciamento de Biblioteca
   AUTOR: Henrique (eu)
   ============================================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVROS 100
#define MAX_USUARIOS 100
#define MAX_EMPRESTIMOS 100

/* Primeira Parte = Estruturas de Dados/Structs*/

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    int codigo;
    char titulo[100];
    char autor[80];
    char editora[60];
    int anoPublicacao;
    int exemplaresDisponiveis;
    int totalEmprestimos; 
    char status[20];      
} Livro;

typedef struct {
    int matricula;
    char nome[100];
    char curso[50];
    char telefone[15];
    Data dataCadastro;
} Usuario;

typedef struct {
    int codigoEmprestimo;
    int matriculaUsuario;
    int codigoLivro;
    Data dataEmprestimo;
    Data dataDevolucaoPrevista;
    char status[20]; 
} Emprestimo;

/* Variaveiss */

Livro livros[MAX_LIVROS];
Usuario usuarios[MAX_USUARIOS];
Emprestimo emprestimos[MAX_EMPRESTIMOS];

int qtdLivros = 0;
int qtdUsuarios = 0;
int qtdEmprestimos = 0;

/* Funções */

void inicializarEstruturas();
void limparBuffer();
void removerQuebraLinha(char *str);
void salvarDados();
void carregarDados();
void realizarBackup();
void cadastrarLivro();
void cadastrarUsuario();
void realizarEmprestimo();
void realizarDevolucao();
void renovarEmprestimo();
void pesquisarLivros();
void pesquisarUsuarios();
void listarEmprestimosAtivos();
void relatorioLivrosMaisEmprestados();
void relatorioUsuariosAtrasados();
void buscaAvancada();
void menuLivros();
void menuUsuarios();
void menuEmprestimos();
void menuRelatorios();

int main() {
    inicializarEstruturas();
    carregarDados();

    int opcao = -1;

    do {
        printf("\n========================================\n");
        printf("    BIBLIOTECA     \n");
        printf("========================================\n");
        printf("1. Gestao de Livros\n");
        printf("2. Gestao de Usuarios\n");
        printf("3. Gestao de Emprestimos e Devolucoes\n");
        printf("4. Relatorios e Buscas Avançadas\n");
        printf("5. Salvar Dados e Fazer Backup Manual\n");
        printf("0. Sair do Sistema\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                menuLivros();
                break;
            case 2:
                menuUsuarios();
                break;
            case 3:
                menuEmprestimos();
                break;
            case 4:
                menuRelatorios();
                break;
            case 5:
                salvarDados();
                realizarBackup();
                printf("\nDados salvos e backup realizado com sucesso!\n");
                break;
            case 0:
                salvarDados();
                realizarBackup();
                printf("\nEncerrando o sistema. Salvos automaticamente.\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novammente.\n");
        }
    } while (opcao != 0);

    return 0;
}

/* Outras funções*/

void inicializarEstruturas() {
    qtdLivros = 0;
    qtdUsuarios = 0;
    qtdEmprestimos = 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void salvarDados() {
    FILE *fLivros = fopen("livros.txt", "w");
    if (fLivros != NULL) {
        fprintf(fLivros, "%d\n", qtdLivros);
        for (int i = 0; i < qtdLivros; i++) {
            fprintf(fLivros, "%d;%s;%s;%s;%d;%d;%d;%s\n",
                    livros[i].codigo, livros[i].titulo, livros[i].autor,
                    livros[i].editora, livros[i].anoPublicacao,
                    livros[i].exemplaresDisponiveis, livros[i].totalEmprestimos,
                    livros[i].status);
        }
        fclose(fLivros);
    }

    FILE *fUsuarios = fopen("usuarios.txt", "w");
    if (fUsuarios != NULL) {
        fprintf(fUsuarios, "%d\n", qtdUsuarios);
        for (int i = 0; i < qtdUsuarios; i++) {
            fprintf(fUsuarios, "%d;%s;%s;%s;%d;%d;%d\n",
                    usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso,
                    usuarios[i].telefone, usuarios[i].dataCadastro.dia,
                    usuarios[i].dataCadastro.mes, usuarios[i].dataCadastro.ano);
        }
        fclose(fUsuarios);
    }

    FILE *fEmprestimos = fopen("emprestimos.txt", "w");
    if (fEmprestimos != NULL) {
        fprintf(fEmprestimos, "%d\n", qtdEmprestimos);
        for (int i = 0; i < qtdEmprestimos; i++) {
            fprintf(fEmprestimos, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%s\n",
                    emprestimos[i].codigoEmprestimo, emprestimos[i].matriculaUsuario,
                    emprestimos[i].codigoLivro, emprestimos[i].dataEmprestimo.dia,
                    emprestimos[i].dataEmprestimo.mes, emprestimos[i].dataEmprestimo.ano,
                    emprestimos[i].dataDevolucaoPrevista.dia, emprestimos[i].dataDevolucaoPrevista.mes,
                    emprestimos[i].dataDevolucaoPrevista.ano, emprestimos[i].status);
        }
        fclose(fEmprestimos);
    }
}

void carregarDados() {
    FILE *fLivros = fopen("livros.txt", "r");
    if (fLivros != NULL) {
        if (fscanf(fLivros, "%d\n", &qtdLivros) == 1) {
            for (int i = 0; i < qtdLivros; i++) {
                char linha[300];
                if (fgets(linha, sizeof(linha), fLivros)) {
                    sscanf(linha, "%d;%99[^;];%79[^;];%59[^;];%d;%d;%d;%19[^\n]",
                           &livros[i].codigo, livros[i].titulo, livros[i].autor,
                           livros[i].editora, &livros[i].anoPublicacao,
                           &livros[i].exemplaresDisponiveis, &livros[i].totalEmprestimos,
                           livros[i].status);
                }
            }
        }
        fclose(fLivros);
    }

    FILE *fUsuarios = fopen("usuarios.txt", "r");
    if (fUsuarios != NULL) {
        if (fscanf(fUsuarios, "%d\n", &qtdUsuarios) == 1) {
            for (int i = 0; i < qtdUsuarios; i++) {
                char linha[300];
                if (fgets(linha, sizeof(linha), fUsuarios)) {
                    sscanf(linha, "%d;%99[^;];%49[^;];%14[^;];%d;%d;%d",
                           &usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso,
                           usuarios[i].telefone, &usuarios[i].dataCadastro.dia,
                           &usuarios[i].dataCadastro.mes, &usuarios[i].dataCadastro.ano);
                }
            }
        }
        fclose(fUsuarios);
    }

    FILE *fEmprestimos = fopen("emprestimos.txt", "r");
    if (fEmprestimos != NULL) {
        if (fscanf(fEmprestimos, "%d\n", &qtdEmprestimos) == 1) {
            for (int i = 0; i < qtdEmprestimos; i++) {
                char linha[300];
                if (fgets(linha, sizeof(linha), fEmprestimos)) {
                    sscanf(linha, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%19[^\n]",
                           &emprestimos[i].codigoEmprestimo, &emprestimos[i].matriculaUsuario,
                           &emprestimos[i].codigoLivro, &emprestimos[i].dataEmprestimo.dia,
                           &emprestimos[i].dataEmprestimo.mes, &emprestimos[i].dataEmprestimo.ano,
                           &emprestimos[i].dataDevolucaoPrevista.dia, &emprestimos[i].dataDevolucaoPrevista.mes,
                           &emprestimos[i].dataDevolucaoPrevista.ano, emprestimos[i].status);
                }
            }
        }
        fclose(fEmprestimos);
    }
}

void realizarBackup() {
    salvarDados();

    FILE *src, *dst;
    char ch;

    src = fopen("livros.txt", "r");
    dst = fopen("livros_bkp.txt", "w");
    if (src && dst) {
        while ((ch = fgetc(src)) != EOF) fputc(ch, dst);
        fclose(src); fclose(dst);
    }

    src = fopen("usuarios.txt", "r");
    dst = fopen("usuarios_bkp.txt", "w");
    if (src && dst) {
        while ((ch = fgetc(src)) != EOF) fputc(ch, dst);
        fclose(src); fclose(dst);
    }

    src = fopen("emprestimos.txt", "r");
    dst = fopen("emprestimos_bkp.txt", "w");
    if (src && dst) {
        while ((ch = fgetc(src)) != EOF) fputc(ch, dst);
        fclose(src); fclose(dst);
    }
}

void cadastrarLivro() {
    if (qtdLivros >= MAX_LIVROS) {
        printf("\nErro: Limite maximo de livros atingido!\n");
        return;
    }

    Livro l;
    printf("\n--- CADASTRO DE LIVRO ---\n");
    printf("Codigo do livro (inteiro): ");
    scanf("%d", &l.codigo);
    limparBuffer();

    // Validação de código duplicado
    for (int i = 0; i < qtdLivros; i++) {
        if (livros[i].codigo == l.codigo) {
            printf("\nErro: Ja existe um livro com este codigo!\n");
            return;
        }
    }

    printf("Titulo: ");
    fgets(l.titulo, sizeof(l.titulo), stdin);
    removerQuebraLinha(l.titulo);

    printf("Autor: ");
    fgets(l.autor, sizeof(l.autor), stdin);
    removerQuebraLinha(l.autor);

    printf("Editora: ");
    fgets(l.editora, sizeof(l.editora), stdin);
    removerQuebraLinha(l.editora);

    printf("Ano de Publicacao: ");
    scanf("%d", &l.anoPublicacao);

    printf("Numero de Exemplares Disponiveis: ");
    scanf("%d", &l.exemplaresDisponiveis);
    limparBuffer();

    l.totalEmprestimos = 0;
    if (l.exemplaresDisponiveis > 0) {
        strcpy(l.status, "Disponivel");
    } else {
        strcpy(l.status, "Indisponivel");
    }

    livros[qtdLivros++] = l;
    salvarDados();
    printf("\nLivro cadastrado com sucesso!\n");
}

void cadastrarUsuario() {
    if (qtdUsuarios >= MAX_USUARIOS) {
        printf("\nErro: Limite maximo de usuarios atingido!\n");
        return;
    }

    Usuario u;
    printf("\n--- CADASTRO DE USUARIO ---\n");
    printf("Matricula (inteiro): ");
    scanf("%d", &u.matricula);
    limparBuffer();

    for (int i = 0; i < qtdUsuarios; i++) {
        if (usuarios[i].matricula == u.matricula) {
            printf("\nErro: Ja existe um usuario com esta matricula!\n");
            return;
        }
    }

    printf("Nome Completo: ");
    fgets(u.nome, sizeof(u.nome), stdin);
    removerQuebraLinha(u.nome);

    printf("Curso: ");
    fgets(u.curso, sizeof(u.curso), stdin);
    removerQuebraLinha(u.curso);

    printf("Telefone: ");
    fgets(u.telefone, sizeof(u.telefone), stdin);
    removerQuebraLinha(u.telefone);

    printf("Data de Cadastro (DD MM AAAA): ");
    scanf("%d %d %d", &u.dataCadastro.dia, &u.dataCadastro.mes, &u.dataCadastro.ano);
    limparBuffer();

    usuarios[qtdUsuarios++] = u;
    salvarDados();
    printf("\nUsuario cadastrado com sucesso!\n");
}

void realizarEmprestimo() {
    if (qtdEmprestimos >= MAX_EMPRESTIMOS) {
        printf("\nErro: Limite de emprestimos atingido!\n");
        return;
    }

    int codLivro, matUsuario;
    printf("\n--- REALIZAR EMPRESTIMO ---\n");
    printf("Matricula do Usuario: ");
    scanf("%d", &matUsuario);

    int idxUsuario = -1;
    for (int i = 0; i < qtdUsuarios; i++) {
        if (usuarios[i].matricula == matUsuario) {
            idxUsuario = i;
            break;
        }
    }
    if (idxUsuario == -1) {
        printf("\nErro: Usuario nao encontrado!\n");
        return;
    }

    printf("Codigo do Livro: ");
    scanf("%d", &codLivro);

    int idxLivro = -1;
    for (int i = 0; i < qtdLivros; i++) {
        if (livros[i].codigo == codLivro) {
            idxLivro = i;
            break;
        }
    }

    if (idxLivro == -1) {
        printf("\nErro: Livro nao encontrado!\n");
        return;
    }

    if (livros[idxLivro].exemplaresDisponiveis <= 0) {
        printf("\nErro: Nao ha exemplares disponiveis deste livro!\n");
        return;
    }

    Emprestimo emp;
    emp.codigoEmprestimo = qtdEmprestimos + 1;
    emp.matriculaUsuario = matUsuario;
    emp.codigoLivro = codLivro;

    printf("Data do Emprestimo (DD MM AAAA): ");
    scanf("%d %d %d", &emp.dataEmprestimo.dia, &emp.dataEmprestimo.mes, &emp.dataEmprestimo.ano);
    limparBuffer();

    emp.dataDevolucaoPrevista = emp.dataEmprestimo;
    emp.dataDevolucaoPrevista.dia += 7;
    if (emp.dataDevolucaoPrevista.dia > 30) {
        emp.dataDevolucaoPrevista.dia -= 30;
        emp.dataDevolucaoPrevista.mes += 1;
        if (emp.dataDevolucaoPrevista.mes > 12) {
            emp.dataDevolucaoPrevista.mes = 1;
            emp.dataDevolucaoPrevista.ano += 1;
        }
    }

    strcpy(emp.status, "Ativo");

    livros[idxLivro].exemplaresDisponiveis--;
    livros[idxLivro].totalEmprestimos++;
    if (livros[idxLivro].exemplaresDisponiveis == 0) {
        strcpy(livros[idxLivro].status, "Indisponivel");
    }

    emprestimos[qtdEmprestimos++] = emp;
    salvarDados();

    printf("\nEmprestimo realizado com sucesso!\n");
    printf("Codigo do Emprestimo: %d\n", emp.codigoEmprestimo);
    printf("Data Prevista de Devolucao: %02d/%02d/%04d\n",
           emp.dataDevolucaoPrevista.dia, emp.dataDevolucaoPrevista.mes, emp.dataDevolucaoPrevista.ano);
}

void realizarDevolucao() {
    int codEmp;
    printf("\n--- REALIZAR DEVOLUCAO ---\n");
    printf("Codigo do Emprestimo: ");
    scanf("%d", &codEmp);
    limparBuffer();

    int idxEmp = -1;
    for (int i = 0; i < qtdEmprestimos; i++) {
        if (emprestimos[i].codigoEmprestimo == codEmp && strcmp(emprestimos[i].status, "Ativo") == 0) {
            idxEmp = i;
            break;
        }
    }

    if (idxEmp == -1) {
        printf("\nErro: Emprestimp ativo nao encontrado!\n");
        return;
    }

    strcpy(emprestimos[idxEmp].status, "Devolvido");

    for (int i = 0; i < qtdLivros; i++) {
        if (livros[i].codigo == emprestimos[idxEmp].codigoLivro) {
            livros[i].exemplaresDisponiveis++;
            strcpy(livros[i].status, "Disponivel");
            break;
        }
    }

    salvarDados();
    printf("\nDevolucao concluida com sucesso!\n");
}

void renovarEmprestimo() {
    int codEmp;
    printf("\n--- RENOVAR EMPRESTIMO ---\n");
    printf("Codigo do Emprestimo: ");
    scanf("%d", &codEmp);
    limparBuffer();

    for (int i = 0; i < qtdEmprestimos; i++) {
        if (emprestimos[i].codigoEmprestimo == codEmp && strcmp(emprestimos[i].status, "Ativo") == 0) {
            emprestimos[i].dataDevolucaoPrevista.dia += 7;
            if (emprestimos[i].dataDevolucaoPrevista.dia > 30) {
                emprestimos[i].dataDevolucaoPrevista.dia -= 30;
                emprestimos[i].dataDevolucaoPrevista.mes += 1;
                if (emprestimos[i].dataDevolucaoPrevista.mes > 12) {
                    emprestimos[i].dataDevolucaoPrevista.mes = 1;
                    emprestimos[i].dataDevolucaoPrevista.ano += 1;
                }
            }
            salvarDados();
            printf("\nEmprestimo renovado com sucesso por mais 7 dias!\n");
            printf("Nova Data de Devolucao: %02d/%02d/%04d\n",
                   emprestimos[i].dataDevolucaoPrevista.dia,
                   emprestimos[i].dataDevolucaoPrevista.mes,
                   emprestimos[i].dataDevolucaoPrevista.ano);
            return;
        }
    }
    printf("\nErro: Emprestimo ativo nao encontrado!\n");
}


void pesquisarLivros() {
    int opcao;
    printf("\n--- PESQUISA DE LIVROS ---\n");
    printf("1. Buscar por Codigo\n");
    printf("2. Buscar por Titulo\n");
    printf("3. Buscar por Autor\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        int cod;
        printf("Digite o codigo: ");
        scanf("%d", &cod);
        for (int i = 0; i < qtdLivros; i++) {
            if (livros[i].codigo == cod) {
                printf("\nEncontrado: [%d] %s | Autor: %s | Editora: %s | Disponiveis: %d | Status: %s\n",
                       livros[i].codigo, livros[i].titulo, livros[i].autor,
                       livros[i].editora, livros[i].exemplaresDisponiveis, livros[i].status);
                return;
            }
        }
        printf("\nLivro nao encontrado.\n");
    } else if (opcao == 2) {
        char termo[100];
        printf("Digite o Titulo (ou parte dele): ");
        fgets(termo, sizeof(termo), stdin);
        removerQuebraLinha(termo);
        int achou = 0;
        for (int i = 0; i < qtdLivros; i++) {
            if (strstr(livros[i].titulo, termo) != NULL) {
                printf("[%d] %s | Autor: %s | Disponiveis: %d\n",
                       livros[i].codigo, livros[i].titulo, livros[i].autor, livros[i].exemplaresDisponiveis);
                achou = 1;
            }
        }
        if (!achou) printf("\nNenhum livro encontrado.\n");
    } else if (opcao == 3) {
        char termo[80];
        printf("Digite o Autor (ou parte dele): ");
        fgets(termo, sizeof(termo), stdin);
        removerQuebraLinha(termo);
        int achou = 0;
        for (int i = 0; i < qtdLivros; i++) {
            if (strstr(livros[i].autor, termo) != NULL) {
                printf("[%d] %s | Autor: %s | Disponiveis: %d\n",
                       livros[i].codigo, livros[i].titulo, livros[i].autor, livros[i].exemplaresDisponiveis);
                achou = 1;
            }
        }
        if (!achou) printf("\nNenhum autor encontrado.\n");
    }
}

void pesquisarUsuarios() {
    int opcao;
    printf("\n--- PESQUISA DE USUARIOS ---\n");
    printf("1. Buscar por Matricula\n");
    printf("2. Buscar por Nome\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    if (opcao == 1) {
        int mat;
        printf("Digite a Matricula: ");
        scanf("%d", &mat);
        for (int i = 0; i < qtdUsuarios; i++) {
            if (usuarios[i].matricula == mat) {
                printf("\nEncontrado: Mat: %d | Nome: %s | Curso: %s | Tel: %s\n",
                       usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso, usuarios[i].telefone);
                return;
            }
        }
        printf("\nUsuario nao encontrado.\n");
    } else if (opcao == 2) {
        char termo[100];
        printf("Digite o Nome (ou parte dele): ");
        fgets(termo, sizeof(termo), stdin);
        removerQuebraLinha(termo);
        int achou = 0;
        for (int i = 0; i < qtdUsuarios; i++) {
            if (strstr(usuarios[i].nome, termo) != NULL) {
                printf("Mat: %d | Nome: %s | Curso: %s\n",
                       usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso);
                achou = 1;
            }
        }
        if (!achou) printf("\nNenhum usuario encontrado.\n");
    }
}

void listarEmprestimosAtivos() {
    printf("\n--- EMPRESTIMOS ATIVOS ---\n");
    int encontrou = 0;
    for (int i = 0; i < qtdEmprestimos; i++) {
        if (strcmp(emprestimos[i].status, "Ativo") == 0) {
            printf("Cod. Emp: %d | Mat. Usuario: %d | Cod. Livro: %d | Data: %02d/%02d/%04d | Devolucao Prevista: %02d/%02d/%04d\n",
                   emprestimos[i].codigoEmprestimo, emprestimos[i].matriculaUsuario,
                   emprestimos[i].codigoLivro, emprestimos[i].dataEmprestimo.dia,
                   emprestimos[i].dataEmprestimo.mes, emprestimos[i].dataEmprestimo.ano,
                   emprestimos[i].dataDevolucaoPrevista.dia, emprestimos[i].dataDevolucaoPrevista.mes,
                   emprestimos[i].dataDevolucaoPrevista.ano);
            encontrou = 1;
        }
    }
    if (!encontrou) {
        printf("Nenhum emprestimo ativo no momento.\n");
    }
}


void relatorioLivrosMaisEmprestados() {
    printf("\n--- RELATORIO: LIVROS MAIS EMPRESTADOS ---\n");
    if (qtdLivros == 0) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    Livro temp[MAX_LIVROS];
    for (int i = 0; i < qtdLivros; i++) temp[i] = livros[i];

    for (int i = 0; i < qtdLivros - 1; i++) {
        for (int j = 0; j < qtdLivros - i - 1; j++) {
            if (temp[j].totalEmprestimos < temp[j+1].totalEmprestimos) {
                Livro aux = temp[j];
                temp[j] = temp[j+1];
                temp[j+1] = aux;
            }
        }
    }

    for (int i = 0; i < qtdLivros; i++) {
        printf("%d. [%d] %s - Total de Emprestimos: %d\n",
               i + 1, temp[i].codigo, temp[i].titulo, temp[i].totalEmprestimos);
    }
}

void relatorioUsuariosAtrasados() {
    Data dataAtual;
    printf("\n--- RELATORIO: EMPRESTIMOS EM ATRASO ---\n");
    printf("Informe a Data de Hoje (DD MM AAAA): ");
    scanf("%d %d %d", &dataAtual.dia, &dataAtual.mes, &dataAtual.ano);
    limparBuffer();

    int atrasados = 0;
    for (int i = 0; i < qtdEmprestimos; i++) {
        if (strcmp(emprestimos[i].status, "Ativo") == 0) {
            Data prev = emprestimos[i].dataDevolucaoPrevista;
            int taAtrasado = 0;

            if (dataAtual.ano > prev.ano) taAtrasado = 1;
            else if (dataAtual.ano == prev.ano && dataAtual.mes > prev.mes) taAtrasado = 1;
            else if (dataAtual.ano == prev.ano && dataAtual.mes == prev.mes && dataAtual.dia > prev.dia) taAtrasado = 1;

            if (taAtrasado) {
                printf("Cod Emp: %d | Matricula Usuario: %d | Cod Livro: %d | Previsto para: %02d/%02d/%04d [EM ATRASO]\n",
                       emprestimos[i].codigoEmprestimo, emprestimos[i].matriculaUsuario,
                       emprestimos[i].codigoLivro, prev.dia, prev.mes, prev.ano);
                atrasados = 1;
            }
        }
    }

    if (!atrasados) {
        printf("Nenhum empréstimo em atraso para a data informada.\n");
    }
}

void buscaAvancada() {
    char editora[60];
    int ano;
    printf("\n--- BUSCA AVANCADA (EDITORA E ANO) ---\n");
    printf("Digite a Editora: ");
    fgets(editora, sizeof(editora), stdin);
    removerQuebraLinha(editora);

    printf("Digite o Ano de Publicacao minimo: ");
    scanf("%d", &ano);
    limparBuffer();

    int encontrou = 0;
    for (int i = 0; i < qtdLivros; i++) {
        if (strstr(livros[i].editora, editora) != NULL && livros[i].anoPublicacao >= ano) {
            printf("[%d] %s | Editora: %s | Ano: %d | Status: %s\n",
                   livros[i].codigo, livros[i].titulo, livros[i].editora, livros[i].anoPublicacao, livros[i].status);
            encontrou = 1;
        }
    }
    if (!encontrou) printf("Nenhum livro atende aos criterios da busca.\n");
}


void menuLivros() {
    int op;
    do {
        printf("\n--- SUBMENU: GESTAO DE LIVROS ---\n");
        printf("1. Cadastrar Novo Livro\n");
        printf("2. Pesquisar Livro\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: cadastrarLivro(); break;
            case 2: pesquisarLivros(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);
}

void menuUsuarios() {
    int op;
    do {
        printf("\n--- SUBMENU: GESTAO DE USUARIOS ---\n");
        printf("1. Cadastrar Novo Usuario\n");
        printf("2. Pesquisar Usuario\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: cadastrarUsuario(); break;
            case 2: pesquisarUsuarios(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);
}

void menuEmprestimos() {
    int op;
    do {
        printf("\n--- SUBMENU: EMPRESTIMOS E DEVOLUCOES ---\n");
        printf("1. Realizar Emprestimo\n");
        printf("2. Realizar Devolucao\n");
        printf("3. Renovar Emprestimo\n");
        printf("4. Listar Emprestimos Ativos\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: realizarEmprestimo(); break;
            case 2: realizarDevolucao(); break;
            case 3: renovarEmprestimo(); break;
            case 4: listarEmprestimosAtivos(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);
}

void menuRelatorios() {
    int op;
    do {
        printf("\n--- SUBMENU: RELATORIOS E CONSULTAS ---\n");
        printf("1. Relatorio de Livros Mais Emprestados\n");
        printf("2. Relatorio de Emprestimos em Atraso\n");
        printf("3. Busca Avançada de Livros\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Opcao: ");
        scanf("%d", &op);
        limparBuffer();

        switch (op) {
            case 1: relatorioLivrosMaisEmprestados(); break;
            case 2: relatorioUsuariosAtrasados(); break;
            case 3: buscaAvancada(); break;
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);
}