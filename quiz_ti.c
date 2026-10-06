/*
 * Gerenciador de Perguntas para Quiz de TI
 *
 * Cadastra, lista, consulta, atualiza e exclui perguntas
 * guardadas no arquivo perguntas.csv (campos separados por ';').
 *
 * Formato de cada linha:  id;texto;categoria;curso;resposta
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARQUIVO "perguntas.csv"
#define TEMPORARIO "temporario.csv"
#define TAM_LINHA 1024

typedef struct {
    int id;
    char texto[250];
    char categoria[50];
    char curso[10];
    char resposta[4];
} Pergunta;

/* ===================== FUNCOES AUXILIARES ===================== */

/* Descarta o que sobrou no teclado depois de uma linha muito grande */
void limparBuffer() {
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

/* Troca o '\n' (e o '\r' do Windows) do final da linha por '\0' */
void tirarQuebra(char *texto) {
    texto[strcspn(texto, "\r\n")] = '\0';
}

/* Remove os espacos do comeco e do fim do texto */
void tirarEspacos(char *texto) {
    int inicio = 0;
    int fim = strlen(texto) - 1;
    int i;

    while (texto[inicio] == ' ' || texto[inicio] == '\t') {
        inicio++;
    }
    while (fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t')) {
        fim--;
    }

    for (i = 0; i <= fim - inicio; i++) {
        texto[i] = texto[inicio + i];
    }
    texto[fim - inicio + 1] = '\0';
}

void paraMaiusculo(char *texto) {
    int i;
    for (i = 0; texto[i] != '\0'; i++) {
        texto[i] = toupper((unsigned char) texto[i]);
    }
}

/* Compara dois textos sem diferenciar maiuscula de minuscula.
   Retorna 1 se forem iguais e 0 se forem diferentes. */
int iguaisSemCaso(const char *a, const char *b) {
    char copiaA[TAM_LINHA];
    char copiaB[TAM_LINHA];

    strcpy(copiaA, a);
    strcpy(copiaB, b);
    paraMaiusculo(copiaA);
    paraMaiusculo(copiaB);

    return strcmp(copiaA, copiaB) == 0;
}

/* Retorna 1 se o texto tiver so digitos (1 a 9 digitos) */
int soNumeros(const char *texto) {
    int i;

    if (texto[0] == '\0' || strlen(texto) > 9) {
        return 0;
    }
    for (i = 0; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char) texto[i])) {
            return 0;
        }
    }
    return 1;
}

/* ===================== LEITURA DO TECLADO ===================== */

/*
 * Le um texto do teclado e repete ate ser valido:
 * nao pode ser vazio, nem ter ';' e precisa caber no tamanho.
 * Retorna 0 se a entrada do teclado acabou (Ctrl+D / Ctrl+Z).
 */
int lerTexto(char *mensagem, char *destino, int tamanho) {
    char entrada[TAM_LINHA];

    while (1) {
        printf("%s", mensagem);

        if (fgets(entrada, TAM_LINHA, stdin) == NULL) {
            return 0;
        }

        /* se nao tem '\n', a linha era maior que o buffer */
        if (strchr(entrada, '\n') == NULL && !feof(stdin)) {
            limparBuffer();
            printf("Erro: texto muito longo.\n");
            continue;
        }

        tirarQuebra(entrada);
        tirarEspacos(entrada);

        if (strlen(entrada) == 0) {
            printf("Erro: este campo nao pode ficar vazio.\n");
        } else if (strchr(entrada, ';') != NULL) {
            printf("Erro: nao use o caractere ';' (ele separa os campos do CSV).\n");
        } else if (strlen(entrada) >= tamanho) {
            printf("Erro: o limite e de %d caracteres.\n", tamanho - 1);
        } else {
            strcpy(destino, entrada);
            return 1;
        }
    }
}

/* Le um numero inteiro (so digitos). Retorna 0 se a entrada acabou. */
int lerNumero(char *mensagem, int *numero) {
    char entrada[20];

    while (1) {
        if (!lerTexto(mensagem, entrada, sizeof(entrada))) {
            return 0;
        }
        if (soNumeros(entrada)) {
            *numero = atoi(entrada);
            return 1;
        }
        printf("Erro: digite somente numeros inteiros.\n");
    }
}

/* Le um codigo de pergunta (inteiro maior que zero) */
int lerCodigo(char *mensagem, int *codigo) {
    while (1) {
        if (!lerNumero(mensagem, codigo)) {
            return 0;
        }
        if (*codigo > 0) {
            return 1;
        }
        printf("Erro: o codigo deve ser maior que zero.\n");
    }
}

/* ===================== VALIDACOES ===================== */

int cursoValido(const char *curso) {
    return strcmp(curso, "CC") == 0 ||
           strcmp(curso, "ES") == 0 ||
           strcmp(curso, "ADS") == 0;
}

int respostaValida(const char *resposta) {
    return strcmp(resposta, "SIM") == 0 ||
           strcmp(resposta, "NAO") == 0;
}

/* Le o curso e repete ate o usuario digitar CC, ES ou ADS */
int lerCurso(char *mensagem, char *curso) {
    char entrada[50];

    while (1) {
        if (!lerTexto(mensagem, entrada, sizeof(entrada))) {
            return 0;
        }
        paraMaiusculo(entrada);

        if (cursoValido(entrada)) {
            strcpy(curso, entrada);
            return 1;
        }
        printf("Erro: o curso deve ser somente CC, ES ou ADS.\n");
    }
}

/* Le a resposta e repete ate o usuario digitar SIM ou NAO */
int lerResposta(char *mensagem, char *resposta) {
    char entrada[50];

    while (1) {
        if (!lerTexto(mensagem, entrada, sizeof(entrada))) {
            return 0;
        }
        paraMaiusculo(entrada);

        if (respostaValida(entrada)) {
            strcpy(resposta, entrada);
            return 1;
        }
        printf("Erro: a resposta deve ser somente SIM ou NAO.\n");
    }
}

const char *nomeDoCurso(const char *curso) {
    if (strcmp(curso, "CC") == 0) {
        return "Ciencia da Computacao";
    }
    if (strcmp(curso, "ES") == 0) {
        return "Engenharia de Software";
    }
    return "Analise e Desenvolvimento de Sistemas";
}

/* ===================== ARQUIVO CSV ===================== */

/*
 * Transforma uma linha do CSV em uma Pergunta usando strtok().
 * Retorna 1 se a linha estiver correta e 0 se estiver invalida.
 */
int lerLinhaCSV(const char *linha, Pergunta *p) {
    char copia[TAM_LINHA];
    char *campos[5];
    char *token;
    int i;
    int pontoEVirgulas = 0;

    /* o strtok pula campos vazios (;;), entao contamos os ';' antes */
    for (i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == ';') {
            pontoEVirgulas++;
        }
    }
    if (pontoEVirgulas != 4) {
        return 0;
    }

    /* o strtok altera o texto, por isso usamos uma copia */
    strcpy(copia, linha);

    i = 0;
    token = strtok(copia, ";");
    while (token != NULL && i < 5) {
        campos[i] = token;
        i++;
        token = strtok(NULL, ";");
    }
    if (i != 5) {
        return 0;
    }

    if (!soNumeros(campos[0]) || atoi(campos[0]) <= 0) {
        return 0;
    }
    if (strlen(campos[1]) >= sizeof(p->texto) ||
        strlen(campos[2]) >= sizeof(p->categoria)) {
        return 0;
    }
    if (!cursoValido(campos[3]) || !respostaValida(campos[4])) {
        return 0;
    }

    p->id = atoi(campos[0]);
    strcpy(p->texto, campos[1]);
    strcpy(p->categoria, campos[2]);
    strcpy(p->curso, campos[3]);
    strcpy(p->resposta, campos[4]);
    return 1;
}

void gravarPergunta(FILE *arquivo, const Pergunta *p) {
    fprintf(arquivo, "%d;%s;%s;%s;%s\n",
            p->id, p->texto, p->categoria, p->curso, p->resposta);
}

/* Se o CSV foi editado a mao e nao termina com '\n', acrescenta um */
void garantirQuebraFinal() {
    FILE *arquivo = fopen(ARQUIVO, "rb");
    int ultimo = '\n';

    if (arquivo == NULL) {
        return;
    }
    if (fseek(arquivo, -1, SEEK_END) == 0) {
        ultimo = fgetc(arquivo);
    }
    fclose(arquivo);

    if (ultimo != '\n' && ultimo != EOF) {
        arquivo = fopen(ARQUIVO, "a");
        if (arquivo != NULL) {
            fprintf(arquivo, "\n");
            fclose(arquivo);
        }
    }
}

/* Retorna 1 se ja existe uma pergunta com esse codigo */
int codigoExiste(int codigo) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    Pergunta p;
    char linha[TAM_LINHA];
    int existe = 0;

    if (arquivo == NULL) {
        return 0;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (lerLinhaCSV(linha, &p) && p.id == codigo) {
            existe = 1;
            break;
        }
    }

    fclose(arquivo);
    return existe;
}

/*
 * Troca o arquivo original pelo temporario.
 * Retorna 1 se deu certo e 0 se deu erro.
 */
int trocarArquivos() {
    if (remove(ARQUIVO) != 0) {
        printf("Erro: nao foi possivel remover o arquivo original.\n");
        remove(TEMPORARIO);
        return 0;
    }
    if (rename(TEMPORARIO, ARQUIVO) != 0) {
        printf("Erro: nao foi possivel renomear o arquivo temporario.\n");
        printf("Seus dados continuam salvos em %s.\n", TEMPORARIO);
        return 0;
    }
    return 1;
}

void mostrarPergunta(const Pergunta *p) {
    printf("\nCodigo: %d\n", p->id);
    printf("Pergunta: %s\n", p->texto);
    printf("Categoria: %s\n", p->categoria);
    printf("Curso: %s\n", p->curso);
    printf("Resposta: %s\n", p->resposta);
    printf("-----------------------------------------\n");
}

/* ===================== FUNCIONALIDADES ===================== */

void cadastrarPergunta() {
    FILE *arquivo;
    Pergunta p;

    printf("\n========== CADASTRAR PERGUNTA ==========\n");

    if (!lerCodigo("Codigo: ", &p.id)) return;

    if (codigoExiste(p.id)) {
        printf("Erro: ja existe uma pergunta com o codigo %d.\n", p.id);
        return;
    }

    if (!lerTexto("Pergunta: ", p.texto, sizeof(p.texto))) return;
    if (!lerTexto("Categoria: ", p.categoria, sizeof(p.categoria))) return;
    if (!lerCurso("Curso (CC/ES/ADS): ", p.curso)) return;
    if (!lerResposta("Resposta (SIM/NAO): ", p.resposta)) return;

    garantirQuebraFinal();

    /* "a" = acrescenta no final, sem apagar o que ja existe.
       Se o arquivo nao existir, ele e criado. */
    arquivo = fopen(ARQUIVO, "a");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQUIVO);
        return;
    }

    gravarPergunta(arquivo, &p);
    fclose(arquivo);

    printf("Pergunta cadastrada com sucesso!\n");
}

void listarPerguntas() {
    FILE *arquivo;
    Pergunta p;
    char linha[TAM_LINHA];
    int encontrou = 0;
    int invalidas = 0;

    printf("\n========== TODAS AS PERGUNTAS ==========\n");

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Nenhuma pergunta cadastrada (arquivo %s nao existe).\n", ARQUIVO);
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (linha[0] == '\0') {
            continue;
        }
        if (lerLinhaCSV(linha, &p)) {
            mostrarPergunta(&p);
            encontrou = 1;
        } else {
            invalidas++;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhuma pergunta encontrada.\n");
    }
    if (invalidas > 0) {
        printf("Aviso: %d linha(s) invalida(s) do arquivo foram ignoradas.\n", invalidas);
    }
}

void consultarPorCategoria() {
    FILE *arquivo;
    Pergunta p;
    char linha[TAM_LINHA];
    char categoria[50];
    int encontrou = 0;

    printf("\n========== CONSULTAR POR CATEGORIA ==========\n");

    if (!lerTexto("Categoria desejada: ", categoria, sizeof(categoria))) return;

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQUIVO);
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (lerLinhaCSV(linha, &p) && iguaisSemCaso(p.categoria, categoria)) {
            if (!encontrou) {
                printf("\nPerguntas encontradas:\n");
            }
            printf("[%d] %s - %s - %s\n", p.id, p.texto, p.curso, p.resposta);
            encontrou = 1;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhuma pergunta encontrada para essa categoria.\n");
    }
}

void consultarPorCurso() {
    FILE *arquivo;
    Pergunta p;
    char linha[TAM_LINHA];
    char curso[10];
    int encontrou = 0;

    printf("\n========== CONSULTAR POR CURSO ==========\n");

    if (!lerCurso("Curso (CC/ES/ADS): ", curso)) return;

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQUIVO);
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (lerLinhaCSV(linha, &p) && strcmp(p.curso, curso) == 0) {
            if (!encontrou) {
                printf("\nPerguntas relacionadas a %s:\n", nomeDoCurso(curso));
            }
            printf("[%d] %s - %s - %s\n", p.id, p.texto, p.categoria, p.resposta);
            encontrou = 1;
        }
    }

    fclose(arquivo);

    if (!encontrou) {
        printf("Nenhuma pergunta encontrada para esse curso.\n");
    }
}

/*
 * Para atualizar, copiamos o arquivo para um temporario. Quando a
 * linha do codigo aparece, gravamos os dados novos no lugar. No fim,
 * o temporario substitui o original. Assim, as outras perguntas
 * nao sao perdidas.
 */
void atualizarPergunta() {
    FILE *arquivo;
    FILE *temporario;
    Pergunta p;
    char linha[TAM_LINHA];
    int codigo;
    int encontrou = 0;
    int cancelou = 0;

    printf("\n========== ATUALIZAR PERGUNTA ==========\n");

    if (!lerCodigo("Codigo da pergunta: ", &codigo)) return;

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQUIVO);
        return;
    }

    temporario = fopen(TEMPORARIO, "w");
    if (temporario == NULL) {
        printf("Erro: nao foi possivel criar o arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (linha[0] == '\0') {
            continue;
        }

        /* linha invalida: copia do jeito que esta, para nao perder nada */
        if (!lerLinhaCSV(linha, &p)) {
            fprintf(temporario, "%s\n", linha);
            continue;
        }

        if (p.id == codigo && !encontrou) {
            encontrou = 1;
            printf("\nPergunta encontrada. Digite os novos dados.\n");

            if (!lerTexto("Novo texto: ", p.texto, sizeof(p.texto)) ||
                !lerTexto("Nova categoria: ", p.categoria, sizeof(p.categoria)) ||
                !lerCurso("Novo curso (CC/ES/ADS): ", p.curso) ||
                !lerResposta("Nova resposta (SIM/NAO): ", p.resposta)) {
                cancelou = 1;
                break;
            }
        }

        gravarPergunta(temporario, &p);
    }

    fclose(arquivo);
    fclose(temporario);

    if (cancelou) {
        remove(TEMPORARIO);
        printf("Atualizacao cancelada. Nada foi alterado.\n");
    } else if (encontrou) {
        if (trocarArquivos()) {
            printf("Pergunta atualizada com sucesso!\n");
        }
    } else {
        remove(TEMPORARIO);
        printf("Nenhuma pergunta encontrada com esse codigo.\n");
    }
}

/* Mesma ideia da atualizacao, mas a linha do codigo nao e copiada */
void excluirPergunta() {
    FILE *arquivo;
    FILE *temporario;
    Pergunta p;
    char linha[TAM_LINHA];
    int codigo;
    int encontrou = 0;

    printf("\n========== EXCLUIR PERGUNTA ==========\n");

    if (!lerCodigo("Codigo da pergunta: ", &codigo)) return;

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo %s.\n", ARQUIVO);
        return;
    }

    temporario = fopen(TEMPORARIO, "w");
    if (temporario == NULL) {
        printf("Erro: nao foi possivel criar o arquivo temporario.\n");
        fclose(arquivo);
        return;
    }

    while (fgets(linha, TAM_LINHA, arquivo) != NULL) {
        tirarQuebra(linha);
        if (linha[0] == '\0') {
            continue;
        }

        if (!lerLinhaCSV(linha, &p)) {
            fprintf(temporario, "%s\n", linha);
            continue;
        }

        if (p.id == codigo && !encontrou) {
            encontrou = 1;      /* nao grava: a pergunta e removida */
        } else {
            gravarPergunta(temporario, &p);
        }
    }

    fclose(arquivo);
    fclose(temporario);

    if (encontrou) {
        if (trocarArquivos()) {
            printf("Pergunta excluida com sucesso!\n");
        }
    } else {
        remove(TEMPORARIO);
        printf("Nenhuma pergunta encontrada com esse codigo.\n");
    }
}

/* ===================== PROGRAMA PRINCIPAL ===================== */

int main() {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=========================================\n");
        printf("   GERENCIADOR DE PERGUNTAS - QUIZ DE TI\n");
        printf("=========================================\n");
        printf("1 - Cadastrar pergunta\n");
        printf("2 - Listar todas as perguntas\n");
        printf("3 - Consultar perguntas por categoria\n");
        printf("4 - Consultar perguntas por curso\n");
        printf("5 - Atualizar pergunta\n");
        printf("6 - Excluir pergunta\n");
        printf("0 - Sair\n");
        printf("-----------------------------------------\n");

        if (!lerNumero("Escolha uma opcao: ", &opcao)) {
            printf("\nEntrada encerrada. Programa finalizado.\n");
            break;
        }

        switch (opcao) {
            case 1:
                cadastrarPergunta();
                break;
            case 2:
                listarPerguntas();
                break;
            case 3:
                consultarPorCategoria();
                break;
            case 4:
                consultarPorCurso();
                break;
            case 5:
                atualizarPergunta();
                break;
            case 6:
                excluirPergunta();
                break;
            case 0:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Erro: opcao invalida. Escolha uma opcao do menu.\n");
        }
    }

    return 0;
}
