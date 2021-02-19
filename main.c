
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DIVISORIA ("--------------------------------------------------------------------------\n")

// Tipos

typedef struct {

    char logradouro[80];
    char bairro[20];
    char cep[10];
    char cidade[20];
    char estado[20];
    char telefone[15];
    char celular[15];
    char email[30];

} endereco;

typedef struct {

    int numero;
    char status;

} imovel_proprietario;

typedef struct {

    int id;
    char nome[80];
    char cpf[15];
    endereco endereco;
    int qtd_imoveis;
    imovel_proprietario imoveis[5];

} proprietario;

typedef struct {

    char logradouro[80];
    char bairro[20];
    char cep[10];
    char cidade[20];

} endereco_imovel;

typedef struct {

    char sigla;
    int id_locatario;

} status_locatario;

typedef union {

    char sigla;
    status_locatario locatario;

} status_imovel;

typedef struct {

    int id;
    endereco_imovel endereco;
    float area;
    int quartos;
    float valor;
    status_imovel status;

} imovel;

typedef union {

    float area;
    int quartos;
    char *bairro;

} imoveis_opcao;

typedef struct {

    int id;
    char nome[80];
    char cpf[15];
    endereco endereco;
    int id_imovel;
    int dia_vencimento;
    char inicio[9];
    char termino[9];

} locatario;

// Prototipos

// Utilitarias
void alocar(void **ptr, int tamAmostra, int tam);
int charParaInt(char c);
int digitoData(char *data, int i);
int dataEstaMenor(char data1[9], char data2[9]);
void getsString(char *campo, char *ptr);
void getsNumero(char *campo, int tam, char *ptr);
int formatar(char *ptr, char *formato);
int calcularDigitoCpf(char *cpf, int tam, int *pesos);
void getsCpf(char *campo, char *ptr);
void getsTelefone(char *campo, char *tel);
void getsCep(char *campo, char *cep);
void getsEmail(char *campo, char *email);
void getsData(char *campo, char *data);

// Operacao com dados
void cadastrarEndereco(endereco *end);
void cadastrarProprietario(proprietario **prop, int *qtd);
void cadastrarImovel(imovel **imoveis, int *qtd, proprietario *props, int qtdProps);
void cadastrarDadosImovel(imovel *imv);
void cadastrarImovelProprietario(imovel *imovel, proprietario *props, int qtdProps);
void alterarStatusImovel(proprietario *props, int qtdProps, imovel *imv, int locatarioId, char status);
void cadastrarAluguel(locatario **locs, int *qtd, imovel *imoveis, int qtdImoveis, proprietario *props, int qtdProps);
locatario *cadastrarLocatario(locatario **locs, int *qtd);
void terminarAluguel(locatario **locs, int *qtd, imovel *imoveis, int qtdImoveis, proprietario *props, int qtdProps);
void removerLocatario(locatario **ptr, int *tam, int indice);
void alterarProprietario(proprietario *prop, int qtd);
void alterarImovel(imovel *imoveis, int qtd);
void alterarLocatario(locatario *locs, int qtd);

// Visualizacao dos dados
void mostrarEndereco(endereco *end);
void mostrarProprietario(proprietario *prop);
void listarProprietarios(proprietario *props, int qtd);
void procurarProprietario(proprietario *props, int qtd);
void mostrarImovel(imovel *imv);
void listarImoveis(imovel *imoveis, int qtd);
int listarImoveisPorStatus(imovel *imoveis, int qtd, char status);
void procurarImoveis(imovel *imoveis, int qtd);
void mostrarLocatario(locatario *loc);
void listarLocatarios(locatario *locs, int qtd);
void addColunaString(FILE *arq, char exportar, char *valor, int tam);
void gerarRelatorio(proprietario *props, int qtdProps, imovel *imoveis, int qtdImoveis, locatario *locs, int qtdLocs);
int getProprietarioPorCpf(proprietario *props, int qtd, char *cpf);
int getLocatarioPorCpf(locatario *locs, int qtd, char *cpf);
int getImovel(imovel *imoveis, int qtd, int id);
int getProprietarioPorImovel(proprietario *props, int qtd, int imovel);

// Operacoes com arquivos
int carregarArquivo(char *nome, int tamAmostra, void **ptr);
void salvarArquivo(char *nome, int tamAmostra, void *ptr, int qtd);
int carregarProprietarios(proprietario **props);
void salvarProprietarios(proprietario *props, int qtd);
int carregarImoveis(imovel **props);
void salvarImoveis(imovel *props, int qtd);
int carregarLocatarios(locatario **locs);
void salvarLocatarios(locatario *locs, int qtd);

// Menus
void menuCadastrar(proprietario **props, imovel **imoveis, locatario **locatarios, int *qtdProps, int *qtdImoveis, int *qtdLocs);
void menuAlterar(proprietario *props, imovel *imoveis, locatario *locatarios, int qtdProps, int qtdImoveis, int qtdLocs);
void menuListar(proprietario *props, imovel *imoveis, locatario *locatarios, int qtdProps, int qtdImoveis, int qtdLocs);
void menuProcurar(proprietario *props, imovel *imoveis, int qtdProps, int qtdImoveis);

void alocar(void **ptr, int tamAmostra, int tam) {
    if (tam == 0) {
        if (*ptr != NULL) {
            free(*ptr);
            *ptr = NULL;
        }
        return;
    }

    *ptr = realloc(*ptr, tamAmostra * tam);

    if (*ptr == NULL) {
        printf("Falha ao alocar memoria! Erro fatal!");
        exit(1);
    }
}

int charParaInt(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    return 0;
}

int digitoData(char *data, int i) {
    return charParaInt(*(data + i)) * 10 + charParaInt(*(data + i + 1));
}

int dataEstaMenor(char data1[9], char data2[9]) {
    int n1, n2;
    // 00/00/00

    n1 = digitoData(data1, 6); // Ano da data 1
    n2 = digitoData(data2, 6); // Ano da data 2

    // Se o ano for diferente, retorna se o primeiro for menor
    if (n1 != n2) return n1 < n2;

    // Nesse momento significa que o ano e igual

    n1 = digitoData(data1, 3); // Mes da data 1
    n2 = digitoData(data2, 3); // Mes da data 2

    // Se o mes for diferente, retorna se o primeiro for menor
    if (n1 != n2) return n1 < n2;

    // Nesse momento significa que o ano e o mes sao iguais

    n1 = digitoData(data1, 0); // Dia da data 1
    n2 = digitoData(data2, 0); // Dia da data 2

    // Retorna se o dia for menor
    return n1 < n2;
}

void getsString(char *campo, char *ptr) {
    printf(campo);
    gets(ptr);
}

void getsNumero(char *campo, int tam, char *ptr) {
    char aux[50];
    int i = 0, o = 0;

    // Guarda o numero em um auxiliar de tamanho menor, permitindo formatacao customizada
    getsString(campo, aux);

    while (aux[i] != '\0') {
        // Loop para todos os caracteres no auxiliar

        if (aux[i] >= '0' && aux[i] <= '9') {
            // Se o caracter for numerico, aloca no ponteiro
            *(ptr + o) = aux[i];
            o++;

            if (o > tam) {
                // Se o tamanho do numero for maior do que o estipulado, pede para digitar novamente
                printf("O numero digitado esta maior que o esperado\n");
                getsNumero(campo, tam, ptr);
                return;
            }
        }
        i++;
    }

    *(ptr + o) = '\0';
}

int formatar(char *ptr, char *formato) {
    int i = 0, o = 0;
    char *formatado = malloc((strlen(formato) + 1) * sizeof(char));

    if (formatado == NULL) {
        exit(1);
    }

    while (*(ptr + i) != '\0') {

        while (*(formato + o) != '#') {

            if (*(formato + o) == '\0') {
                // Se o tamanho do numero for maior do que o estipulado, retorna erro
                free(formatado);
                return 0;
            }

            *(formatado + o) = *(formato + o);
            o++;
        }

        *(formatado + o) = *(ptr + i);
        o++;
        i++;
    }

    if (*(formato + o) != '\0') {
        // Se o formato nao tiver chegado ao fim, retorna erro
        free(formatado);
        return 0;
    }

    *(formatado + o) = '\0';
    strcpy(ptr, formatado);
    free(formatado);
    return 1;
}

int calcularDigitoCpf(char *cpf, int tam, int *pesos) {
    int soma = 0, digito, i;

    for (i = tam - 1; i >= 0; i--) {
        digito = charParaInt(*(cpf + i));
        soma += digito * pesos[10 - tam + i];
    }

    soma = 11 - soma % 11;
    return soma > 9 ? 0 : soma;
}

void getsCpf(char *campo, char *cpf) {
    // https://pt.wikipedia.org/wiki/Cadastro_de_pessoas_f%C3%ADsicas#Algoritmo
    int cpfPesos[10] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2};
    int i;

    getsNumero(campo, 11, cpf);

    int tam = strlen(cpf);

    if(tam != 11) {
        printf("CPF incompleto!\n");
        getsCpf(campo, cpf);
        return;
    }

    i = calcularDigitoCpf(cpf, 9, cpfPesos);

    if (charParaInt(*(cpf + 9)) != i) {
        // Se o primeiro digito calculado for diferente do digitado
        printf("CPF invalido! (Primeiro digito)\n");
        getsCpf(campo, cpf);
        return;
    }

    i = calcularDigitoCpf(cpf, 10, cpfPesos);

    if (charParaInt(*(cpf + 10)) != i) {
        // Se o segundo digito calculado for diferente do digitado
        printf("CPF invalido! (Segundo digito)\n");
        getsCpf(campo, cpf);
        return;
    }

    formatar(cpf, "###.###.###-##");
}

void getsTelefone(char *campo, char *tel) {
    int tam, sucesso;

    getsNumero(campo, 12, tel);
    tam = strlen(tel);

    if (tam < 10) {
        // Numero menor que 10 caracteres
        printf("Telefone incompleto!\n");
        getsTelefone(campo, tel);
        return;
    } else if (tam == 10) {
        sucesso = formatar(tel, "## ####-####");
    } else {
        sucesso = formatar(tel, "## #####-####");
    }

    if (!sucesso) {
        // Numero invalido!
        printf("Telefone invalido!\n");
        getsTelefone(campo, tel);
        return;
    }
}

void getsCep(char *campo, char *cep) {
    getsNumero(campo, 8, cep);

    if (!formatar(cep, "#####-###")) {
        printf("CEP invalido!\n");
        getsCep(campo, cep);
        return;
    }
}

void getsEmail(char *campo, char *email) {
    int i = 0;
    int status = 0;

    getsString(campo, email);

    while (*(email + i) != '\0') {
        if (status == 0 && *(email + i) == '@') {
            // Altera o status pra 1 quando achar o arroba
            status = 1;
        } else if (status == 1 && *(email + i) == '.') {
            // Altera o status pra 2 quando achar o ponto
            status = 2;
        }
        i++;
    }

    if (status != 2) {
        printf("Email invalido!\n");
        getsEmail(campo, email);
    }
}

void getsData(char *campo, char *data) {
    int tam, i, n;

    getsNumero(campo, 8, data);

    tam = strlen(data);

    if (tam == 8) {
        // Data com ano completo 00/00/0000
        for (i = 4; i < 7; i++) {
            *(data + i) = *(data + i + 2);
        }
    }

    if (!formatar(data, "##/##/##")) {
        printf("Data invalida!\n");
        getsData(campo, data);
        return;
    }

    n = digitoData(data, 0); // Dia
    if (n <= 0 || n > 31) {
        printf("Dia invalido!\n");
        getsData(campo, data);
        return;
    }

    n = digitoData(data, 3); // Mes
    if (n <= 0 || n > 12) {
        printf("Mes invalido!\n");
        getsData(campo, data);
        return;
    }
}

void cadastrarEndereco(endereco *end) {
    getsString("Logradouro:\t", end->logradouro);

    getsString("Bairro:\t\t", end->bairro);

    getsCep("CEP:\t\t", end->cep);

    getsString("Cidade:\t\t", end->cidade);

    getsString("Estado:\t\t", end->estado);

    getsTelefone("Telefone:\t", end->telefone);

    getsTelefone("Celular:\t", end->celular);

    getsEmail("Email:\t\t", end->email);
}

void cadastrarProprietario(proprietario **props, int *qtd) {
    proprietario *prop;

    alocar(props, sizeof(proprietario), *qtd + 1);
    prop = (*props + (*qtd)++);

    prop->id = *qtd;
    prop->qtd_imoveis = 0;

    getsString("Nome:\t\t", prop->nome);

    getsCpf("CPF:\t\t", prop->cpf);

    cadastrarEndereco(&prop->endereco);

    printf("Proprietario %i foi cadastrado com sucesso\n", prop->id);
    printf(DIVISORIA);
}

void cadastrarImovel(imovel **imoveis, int *qtd, proprietario *props, int qtdProps) {
    imovel *imv;

    alocar(imoveis, sizeof(imovel), *qtd + 1);
    imv = (*imoveis + (*qtd)++);

    imv->id = *qtd;
    imv->status.sigla = 'L';

    cadastrarDadosImovel(imv);
    cadastrarImovelProprietario(imv, props, qtdProps);
}

void cadastrarDadosImovel(imovel *imv) {
    getsString("Logradouro:\t\t", imv->endereco.logradouro);

    getsString("Bairro:\t\t\t", imv->endereco.bairro);

    getsCep("CEP:\t\t\t", imv->endereco.cep);

    getsString("Cidade:\t\t\t", imv->endereco.cidade);

    do {
        printf("Area (m2):\t\t");
        scanf("%f", &imv->area);
    } while (imv->area <= 0);

    do {
        printf("Quartos:\t\t");
        scanf("%i", &imv->quartos);
    } while (imv->quartos < 0);

    do {
        printf("Valor do Aluguel:\t");
        scanf("%f", &imv->valor);
    } while (imv->valor <= 0);

    fflush(stdin);
}

void cadastrarImovelProprietario(imovel *imovel, proprietario *props, int qtdProps) {
    proprietario *prop;
    imovel_proprietario *imv_prop;
    char cpf[15];
    int i;

    while(1) {
        getsCpf("CPF do Proprietario:\t", cpf);

        i = getProprietarioPorCpf(props, qtdProps, cpf);
        if (i == -1) continue;

        prop = props + i;

        if (prop->qtd_imoveis < 5) {
            break;
        } else {
            printf("O proprietario %s ja tem 5 imoveis\n", prop->nome);
            continue;
        }
    }

    imv_prop = &prop->imoveis[prop->qtd_imoveis];
    imv_prop->numero = imovel->id;
    imv_prop->status = imovel->status.sigla;
    prop->qtd_imoveis++;

    printf("Imovel %i cadastrado no perfil do proprietario!\n", imovel->id);
    printf(DIVISORIA);
}

void alterarStatusImovel(proprietario *props, int qtdProps, imovel *imv, int locatarioId, char status) {
    int i, o;
    proprietario *prop;

    // Altera o status com o respectivo locatario
    imv->status.sigla = status;
    imv->status.locatario.id_locatario = locatarioId;

    // Altera o status dos imoveis nos proprietarios
    for (i = 0; i < qtdProps; i++) {
        prop = props + i;

        for (o = 0; o < prop->qtd_imoveis; o++) {
            if (prop->imoveis[o].numero == imv->id) {
                prop->imoveis[o].status = status;
            }
        }
    }
}

void cadastrarAluguel(locatario **locs, int *qtd, imovel *imoveis, int qtdImoveis, proprietario *props, int qtdProps) {
    locatario *loc;
    imovel *imv = NULL;
    proprietario *prop;
    int id, i;

    i = listarImoveisPorStatus(imoveis, qtdImoveis, 'L');

    if (i == 0) {
        printf("Nenhum imovel livre!\n");
        printf(DIVISORIA);
        return;
    }

    system("PAUSE");

    printf("Num. do Imovel: ");
    scanf("%i", &id);
    fflush(stdin);

    i = getImovel(imoveis, qtdImoveis, id);

    if (i == -1) {
        printf("Imovel nao encontrado!\n");
        printf(DIVISORIA);
        return;
    }

    imv = imoveis + i;

    if (imv->status.sigla == 'A') {
        printf("Imovel ja alugado!\n\n");
        mostrarImovel(imv);
        printf(DIVISORIA);
        return;
    }

    loc = cadastrarLocatario(locs, qtd);

    // Associa o imovel com o locador
    loc->id_imovel = imv->id;

    // Altera o status nos proprietarios e no imovel para alugado
    alterarStatusImovel(props, qtdProps, imv, loc->id, 'A');

    printf("Aluguel cadastrado com sucesso!\n");
    printf(DIVISORIA);
}

locatario *cadastrarLocatario(locatario **locs, int *qtd) {
    locatario *loc;

    alocar(locs, sizeof(locatario), *qtd + 1);
    loc = (*locs + (*qtd)++);

    loc->id = *qtd;

    getsString("Nome:\t\t", loc->nome);

    getsCpf("CPF:\t\t", loc->cpf);

    cadastrarEndereco(&loc->endereco);

    do {
        printf("Dia do vencimento:\t");
        scanf("%i", &loc->dia_vencimento);
        fflush(stdin);
    } while(loc->dia_vencimento <= 0 || loc->dia_vencimento > 30);

    getsData("Data de inicio do contrato:\t", loc->inicio);

    getsData("Data de termino do contrato:\t", loc->termino);

    return loc;
}

void terminarAluguel(locatario **locs, int *qtd, imovel *imoveis, int qtdImoveis, proprietario *props, int qtdProps) {
    char cpf[15], data[9];
    int indiceLoc, i;
    locatario *loc;
    imovel *imv;
    proprietario *prop;

    getsCpf("CPF do Locatario:\t", cpf);

    indiceLoc = getLocatarioPorCpf(*locs, *qtd, cpf);

    if (indiceLoc == -1) {
        printf("Nenhum proprietario com o CPF digitado foi encontrado.\n");
        printf(DIVISORIA);
        return;
    }

    loc = *locs + indiceLoc;
    mostrarLocatario(loc);

    i = getImovel(imoveis, qtdImoveis, loc->id_imovel);

    if (i == -1) {
        printf("Erro fatal: Imovel associado nao foi encontrado!\n");
        return;
    }

    imv = imoveis + i;

    getsData("Data Atual:\t", data);

    // Verifica se a data esta menor que o termino e cobra multa
    if (dataEstaMenor(data, loc->termino)) {
        printf("\nMulta deve ser cobrada no valor de R$ %.2f\n", imv->valor);
        system("PAUSE");
    }

    // Altera o status nos proprietarios e no imovel para livre
    alterarStatusImovel(props, qtdProps, imv, -1, 'L');

    // Por fim, remove o locatario da memoria, levando o locatario para oblivion
    removerLocatario(locs, qtd, indiceLoc);
}

void removerLocatario(locatario **ptr, int *tam, int indice) {
    int i;

    // Move todos os itens sequentes do vetor para um indice anterior
    for (i = indice + 1; i < *tam; i++) {
        *(*ptr + i - 1) = *(*ptr + i);
    }

    // Remove a ultima posicao no vetor
    (*tam)--;
    alocar(ptr, sizeof(locatario), *tam);
}

void alterarProprietario(proprietario *props, int qtd) {
    int i;
    char cpf[15];
    proprietario *prop;

    getsCpf("CPF:\t\t", cpf);
    i = getProprietarioPorCpf(props, qtd, cpf);

    if (i == -1) {
        printf("Proprietario nao encontrado!\n");
        printf(DIVISORIA);
        return;
    }

    prop = props + i;
    printf("\nAlterar cadastro de %s (Num. %i)\n\n", prop->nome, prop->id);

    getsString("Nome:\t\t", prop->nome);

    cadastrarEndereco(&prop->endereco);

    printf("Proprietario alterado!\n");
    printf(DIVISORIA);
}

void alterarImovel(imovel *imoveis, int qtd) {
    int i;
    imovel *imv;

    printf("Num. do Imovel:\t");
    scanf("%i", &i);
    fflush(stdin);

    i = getImovel(imoveis, qtd, i);

    if (i == -1) {
        printf("Imovel nao encontrado!\n");
        printf(DIVISORIA);
        return;
    }

    imv = imoveis + i;
    printf("\nAlterar cadastro de imovel num. %i\n\n", imv->id);

    cadastrarDadosImovel(imv);

    printf("Imovel alterado!\n");
    printf(DIVISORIA);
}

void alterarLocatario(locatario *locs, int qtd) {
    int i;
    char cpf[15];
    locatario *loc;

    getsCpf("CPF:\t\t", cpf);

    i = getLocatarioPorCpf(locs, qtd, cpf);

    if (i == -1) {
        printf("Locatario nao encontrado!\n");
        printf(DIVISORIA);
        return;
    }

    loc = locs + i;

    printf("\nAlterar cadastro de %s (Num. %i)\n\n", loc->nome, loc->id);

    getsString("Nome:\t\t", loc->nome);

    cadastrarEndereco(&loc->endereco);

    do {
        printf("Dia do vencimento:\t");
        scanf("%i", &loc->dia_vencimento);
        fflush(stdin);
    } while(loc->dia_vencimento <= 0 || loc->dia_vencimento > 30);

    getsData("Data de inicio do contrato:\t", loc->inicio);

    getsData("Data de termino do contrato:\t", loc->termino);

    printf("Locatario alterado!\n");
    printf(DIVISORIA);
}

void mostrarEndereco(endereco *end) {
    printf("Logradouro:\t\t%s\n", end->logradouro);
    printf("Bairro:\t\t\t%s\n", end->bairro);
    printf("CEP:\t\t\t%s\n", end->cep);
    printf("Cidade:\t\t\t%s\n", end->cidade);
    printf("Estado:\t\t\t%s\n", end->estado);
    printf("Telefone:\t\t%s\n", end->telefone);
    printf("Celular:\t\t%s\n", end->celular);
    printf("Email:\t\t\t%s\n", end->email);
}

void mostrarProprietario(proprietario *prop) {
    int i;

    printf("Num:\t\t\t%i\n", prop->id);
    printf("Nome:\t\t\t%s\n", prop->nome);
    printf("CPF:\t\t\t%s\n", prop->cpf);

    mostrarEndereco(&prop->endereco);

    printf("Imoveis:\t\t%i\n", prop->qtd_imoveis);

    for (i = 0; i < prop->qtd_imoveis; i++) {
        printf("Imovel #%i:\t\tNum. %i (%s)\n", i + 1, prop->imoveis[i].numero, prop->imoveis[i].status == 'L' ? "Livre" : "Alugado");
    }

    printf(DIVISORIA);
}

void listarProprietarios(proprietario *props, int qtd) {
    int i;

    if (qtd == 0) {
        printf("Nenhum proprietario cadastrado!\n");
        printf(DIVISORIA);
    } else {
        for (i = 0; i < qtd; i++) {
            mostrarProprietario(props + i);
        }
    }

    system("PAUSE");
}

void procurarProprietario(proprietario *props, int qtd) {
    int prop;
    char cpf[15];

    printf("Procurar Proprietario por CPF\n\n");
    getsCpf("CPF:\t\t\t", cpf);

    prop = getProprietarioPorCpf(props, qtd, cpf);

    if (prop == -1) {
        printf("Nenhum proprietario com o CPF digitado foi encontrado.\n");
        printf(DIVISORIA);
    } else {
        printf(DIVISORIA);
        mostrarProprietario(props + prop);
    }

    system("PAUSE");
}

void mostrarImovel(imovel *imv) {
    printf("Num:\t\t\t%i\n", imv->id);
    printf("Area (m2):\t\t%.2f\n", imv->area);
    printf("Quartos:\t\t%i\n", imv->quartos);
    printf("Valor do Aluguel:\t%.2f\n", imv->valor);
    printf("Logradouro:\t\t%s\n", imv->endereco.logradouro);
    printf("Bairro:\t\t\t%s\n", imv->endereco.bairro);
    printf("CEP:\t\t\t%s\n", imv->endereco.cep);
    printf("Cidade:\t\t\t%s\n", imv->endereco.cidade);
    printf("Status:\t\t\t%s\n", imv->status.sigla == 'A' ? "Alugado" : "Livre");

    if (imv->status.sigla == 'A') {
        printf("Num. do Locatario:\t%i\n", imv->status.locatario.id_locatario);
    }

    printf(DIVISORIA);
}

void listarImoveis(imovel *imoveis, int qtd) {
    char status;
    int encontrado;

    printf("Listar imoveis por status - [A]lugado ou [L]ivre\n");

    do {
        printf("Status: ");
        scanf("%c", &status);
        status = toupper(status);
        fflush(stdin);
    } while (status != 'A' && status != 'L');

    encontrado = listarImoveisPorStatus(imoveis, qtd, status);

    if (encontrado == 0) {
        printf("Nenhum imovel %s!\n", status == 'A' ? "alugado" : "livre");
        printf(DIVISORIA);
    }

    system("PAUSE");
}

int listarImoveisPorStatus(imovel *imoveis, int qtd, char status) {
    int i, encontrado = 0;
    imovel *imv;

    for (i = 0; i < qtd; i++) {
        imv = imoveis + i;

        if (imv->status.sigla == status) {
            mostrarImovel(imv);
            encontrado++;
        }
    }

    return encontrado;
}

void procurarImoveis(imovel *imoveis, int qtd) {
    int i, encontrado = 0;
    char opcao;
    imoveis_opcao valor;
    imovel *imv;

    printf("Procurar imoveis por [A]rea, [Q]uartos ou [B]airro\n");

    do {
        printf("Tipo: ");
        scanf("%c", &opcao);
        opcao = toupper(opcao);
        fflush(stdin);
    } while (opcao != 'A' && opcao != 'Q' && opcao != 'B');

    switch (opcao) {
        case 'A':
            printf("Area (m2): ");
            scanf("%f", &valor.area);
            fflush(stdin);
            break;
        case 'Q':
            printf("Quantidade de Quartos: ");
            scanf("%i", &valor.quartos);
            fflush(stdin);
            break;
        case 'B':
            valor.bairro = malloc(sizeof(char) * 20);

            // Caso a alocacao de errado, finaliza o programa
            if (valor.bairro == NULL) {
                exit(2);
                return;
            }

            getsString("Bairro: ", valor.bairro);
            break;
    }

    for (i = 0; i < qtd; i++) {
        imv = imoveis + i;

        // Pula alugados
        if (imv->status.sigla != 'L') continue;

        // Pula com area diferente da digitada
        if (opcao == 'A' && imv->area != valor.area) continue;

        // Pula com num quartos diferente do digitado
        if (opcao == 'Q' && imv->quartos != valor.quartos) continue;

        // Pula com bairro diferente do digitado
        if (opcao == 'B' && strcmp(imv->endereco.bairro, valor.bairro) != 0) continue;

        mostrarImovel(imv);
        encontrado++;
    }

    if (encontrado == 0) {
        printf("Nenhum imovel encontrado!\n");
        printf(DIVISORIA);
    }

    // Caso a opcao seja bairro, vamos desalocar a string
    if (opcao == 'B') {
        free(valor.bairro);
    }

    system("PAUSE");
}

void mostrarLocatario(locatario *loc) {
    printf("Num:\t\t\t%i\n", loc->id);
    printf("Nome:\t\t\t%s\n", loc->nome);
    printf("CPF:\t\t\t%s\n", loc->cpf);

    mostrarEndereco(&loc->endereco);

    printf("Num. Imovel:\t\t%i\n", loc->id_imovel);
    printf("Dia do Vencimento:\t%i\n", loc->dia_vencimento);
    printf("Inicio do Contrato:\t%s\n", loc->inicio);
    printf("Termino de Contrato:\t%s\n", loc->termino);

    printf(DIVISORIA);
}

void listarLocatarios(locatario *locs, int qtd) {
    int i;

    if (qtd == 0) {
        printf("Nenhum locatario cadastrado!\n");
        printf(DIVISORIA);
    } else {
        for (i = 0; i < qtd; i++) {
            mostrarLocatario(locs + i);
        }
    }

    system("PAUSE");
}

void addColunaString(FILE *arq, char exportar, char *valor, int tam) {
    int i, t = strlen(valor);

    if (exportar == 'S') {
        // Escrever para CSV
        putc('"', arq);
        for (i = 0; i < t; i++) {
            if (*(valor + i) == '"') {
                // Se tiver uma aspa, coloca outra pra fazer escape
                putc('"', arq);
            }
            putc(*(valor + i), arq);
        }
        putc('"', arq);
        putc(',', arq);

    } else {
        // Exibir na tela
        for (i = 0; i < tam; i++) {
            if (i < t) {
                putc(*(valor + i), arq);
            } else {
                putc(' ', arq);
            }
        }
        putc(' ', arq);

    }
}

void gerarRelatorio(proprietario *props, int qtdProps, imovel *imoveis, int qtdImoveis, locatario *locs, int qtdLocs) {
    int dia, i;
    char exportar, aux[10];
    locatario *loc;
    imovel *imv;
    proprietario *prop;
    FILE *arq = NULL;

    do {
        printf("Dia Vencimento: ");
        scanf("%i", &dia);
        fflush(stdin);
    } while (dia <= 0 || dia > 30);

    do {
        printf("Exportar para CSV? (S/N) ");
        scanf("%c", &exportar);
        fflush(stdin);
        exportar = toupper(exportar);
    } while (exportar != 'S' && exportar != 'N');

    if (exportar == 'S') {
        arq = fopen("relatorio.csv", "w");

        if (arq == NULL) {
            printf("Nao foi possivel abrir o arquivo!\n");
            return;
        }

        fprintf(arq, "sep=,\n");
    } else {
        arq = stdout;
        system("cls");
    }

    addColunaString(arq, exportar, "Valor", 8);
    addColunaString(arq, exportar, "Locatario", 10);
    addColunaString(arq, exportar, "Nome Locatario", 20);
    addColunaString(arq, exportar, "CPF Locatario", 14);
    addColunaString(arq, exportar, "Proprietario", 12);
    addColunaString(arq, exportar, "Nome Proprietario", 20);
    addColunaString(arq, exportar, "CPF Proprietario", 16);
    putc('\n', arq);

    for (i = 0; i < qtdLocs; i++) {
        loc = locs + i;

        if (loc->dia_vencimento != dia) continue;

        imv = imoveis + getImovel(imoveis, qtdImoveis, loc->id_imovel);
        prop = props + getProprietarioPorImovel(props, qtdImoveis, loc->id_imovel);

        sprintf(aux, "%.2f", imv->valor);
        addColunaString(arq, exportar, aux, 8);

        sprintf(aux, "%i", loc->id);
        addColunaString(arq, exportar, aux, 10);

        addColunaString(arq, exportar, loc->nome, 20);
        addColunaString(arq, exportar, loc->cpf, 14);

        sprintf(aux, "%i", prop->id);
        addColunaString(arq, exportar, aux, 12);

        addColunaString(arq, exportar, prop->nome, 20);
        addColunaString(arq, exportar, prop->cpf, 16);

        putc('\n', arq);
    }

    if (exportar == 'S') {
        fclose(arq);
        printf("\nEscrito para o arquivo 'relatorio.csv' com sucesso!\n");
        printf(DIVISORIA);
    } else {
        printf("\n\n");
        system("PAUSE");
    }
}

int getProprietarioPorCpf(proprietario *props, int qtd, char *cpf) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp((props + i)->cpf, cpf) == 0) {
            return i;
        }
    }

    return -1;
}

int getLocatarioPorCpf(locatario *locs, int qtd, char *cpf) {
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp((locs + i)->cpf, cpf) == 0) {
            return i;
        }
    }

    return -1;
}

int getImovel(imovel *imoveis, int qtd, int id) {
    int i;

    for (i = 0; i < qtd; i++) {
        if ((imoveis + i)->id == id) {
            return i;
        }
    }

    return -1;
}

int getProprietarioPorImovel(proprietario *props, int qtd, int imovel) {
    int i, o;
    proprietario *prop;

    for (i = 0; i < qtd; i++) {
        prop = props + i;

        for (o = 0; o < prop->qtd_imoveis; o++) {
            if ((prop->imoveis + o)->numero == imovel) {
                return i;
            }
        }
    }

    return -1;
}

int carregarArquivo(char *nome, int tamAmostra, void **ptr) {
    int tam, qtd;
    FILE *arq = fopen(nome, "rb");

    // Caso o arquivo nao exista, retorna quantidade zerada
    if (arq == NULL) return 0;

    // Pula para o final do arquivo
    fseek(arq, 0, SEEK_END);

    // Obtem a posicao atual do arquivo (equivalente ao tamanho do arquivo)
    tam = ftell(arq);
    qtd = tam / tamAmostra;

    // Volta para o comeco do arquivo
    fseek(arq, 0, SEEK_SET);

    // Aloca memoria para os dados do arquivo
    alocar(ptr, tamAmostra, qtd);

    // Le todos os dados para o ponteiro
    fread(*ptr, tamAmostra, qtd, arq);

    // Fecha o arquivo
    fclose(arq);

    return qtd;
}

void salvarArquivo(char *nome, int tamAmostra, void *ptr, int qtd) {
    FILE *arq = fopen(nome, "wb");

    // Se o arquivo nao tiver sido aberto, seja por problema de permissoes, finaliza o programa
    if (arq == NULL) {
        exit(1);
        return;
    }

    // Escreve todos os dados do ponteiro para o arquivo
    fwrite(ptr, tamAmostra, qtd, arq);

    // Fecha o arquivo
    fclose(arq);
}

int carregarProprietarios(proprietario **props) {
    return carregarArquivo("proprietario.bin", sizeof(proprietario), (void **) props);
}

void salvarProprietarios(proprietario *props, int qtd) {
    salvarArquivo("proprietario.bin", sizeof(proprietario), (void *) props, qtd);
}

int carregarImoveis(imovel **imvs) {
    return carregarArquivo("imovel.bin", sizeof(imovel), (void **) imvs);
}

void salvarImoveis(imovel *imvs, int qtd) {
    salvarArquivo("imovel.bin", sizeof(imovel), (void *) imvs, qtd);
}

int carregarLocatarios(locatario **locs) {
    return carregarArquivo("locatario.bin", sizeof(locatario), (void **) locs);
}

void salvarLocatarios(locatario *locs, int qtd) {
    salvarArquivo("locatario.bin", sizeof(locatario), (void *) locs, qtd);
}

void menuCadastrar(proprietario **props, imovel **imoveis, locatario **locatarios, int *qtdProps, int *qtdImoveis, int *qtdLocs) {
    char opcao;

    printf("\t\t\t\tCADASTRAR\n");
    printf(DIVISORIA);

    do {
        printf("[1] - Cadastrar Proprietario\n");
        printf("[2] - Cadastrar Imovel\n");
        printf("[3] - Cadastrar Aluguel\n");
        printf("[V] - Voltar ao Menu\n\n");

        printf("Opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao) {
            case '1':
                cadastrarProprietario(props, qtdProps);
                salvarProprietarios(*props, *qtdProps);
                break;
            case '2':
                cadastrarImovel(imoveis, qtdImoveis, *props, *qtdProps);
                salvarImoveis(*imoveis, *qtdImoveis);
                salvarProprietarios(*props, *qtdProps);
                break;
            case '3':
                cadastrarAluguel(locatarios, qtdLocs, *imoveis, *qtdImoveis, *props, *qtdProps);
                salvarLocatarios(*locatarios, *qtdLocs);
                salvarImoveis(*imoveis, *qtdImoveis);
                salvarProprietarios(*props, *qtdProps);
                break;
        }
    } while (opcao != 'V' && opcao != 'v');

}

void menuAlterar(proprietario *props, imovel *imoveis, locatario *locatarios, int qtdProps, int qtdImoveis, int qtdLocs) {
    char opcao;

    printf("\t\t\t\tALTERAR\n");
    printf(DIVISORIA);

    do {
        printf("[1] - Alterar Proprietario\n");
        printf("[2] - Alterar Imovel\n");
        printf("[3] - Alterar Locatario\n");
        printf("[V] - Voltar ao Menu\n\n");

        printf("Opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao) {
            case '1':
                alterarProprietario(props, qtdProps);
                salvarProprietarios(props, qtdProps);
                break;
            case '2':
                alterarImovel(imoveis, qtdImoveis);
                salvarImoveis(imoveis, qtdImoveis);
                break;
            case '3':
                alterarLocatario(locatarios, qtdLocs);
                salvarLocatarios(locatarios, qtdLocs);
                break;
        }
    } while (opcao != 'V' && opcao != 'v');
}

void menuListar(proprietario *props, imovel *imoveis, locatario *locatarios, int qtdProps, int qtdImoveis, int qtdLocs) {
    char opcao;

    printf("\t\t\t\tLISTAR\n");
    printf(DIVISORIA);

    do {
        printf("[1] - Listar Proprietarios\n");
        printf("[2] - Listar Imoveis\n");
        printf("[3] - Listar Locatarios\n");
        printf("[V] - Voltar ao Menu\n\n");

        printf("Opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao) {
            case '1':
                listarProprietarios(props, qtdProps);
                break;
            case '2':
                listarImoveis(imoveis, qtdImoveis);
                break;
            case '3':
                listarLocatarios(locatarios, qtdLocs);
                break;
        }
    } while (opcao != 'V' && opcao != 'v');
}

void menuProcurar(proprietario *props, imovel *imoveis, int qtdProps, int qtdImoveis) {
    char opcao;

    printf("\t\t\t\tPROCURAR\n");
    printf(DIVISORIA);

    do {
        printf("[1] - Procurar Proprietarios\n");
        printf("[2] - Procurar Imoveis\n");
        printf("[V] - Voltar ao Menu\n\n");

        printf("Opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao) {
            case '1':
                procurarProprietario(props, qtdProps);
                break;
            case '2':
                procurarImoveis(imoveis, qtdImoveis);
                break;
        }
    } while (opcao != 'V' && opcao != 'v');
}

int main() {
    proprietario *props = NULL;
    imovel *imoveis = NULL;
    locatario *locatarios = NULL;
    int qtdProps = 0, qtdImoveis = 0, qtdLocs = 0;
    char opcao;

    qtdProps = carregarProprietarios(&props);
    qtdImoveis = carregarImoveis(&imoveis);
    qtdLocs = carregarLocatarios(&locatarios);

    printf("\t\t\tIMOBILIARIA BOA VISTA\n");
    printf(DIVISORIA);

    do {
        printf("[1] - Cadastrar\n");
        printf("[2] - Alterar\n");
        printf("[3] - Listar\n");
        printf("[4] - Procurar\n");
        printf("[5] - Terminar Aluguel\n");
        printf("[6] - Gerar Relatorio\n");
        printf("[S] - Sair\n\n");

        printf("Opcao: ");
        scanf("%c", &opcao);
        fflush(stdin);

        switch(opcao) {
            case '1':
                menuCadastrar(&props, &imoveis, &locatarios, &qtdProps, &qtdImoveis, &qtdLocs);
                break;
            case '2':
                menuAlterar(props, imoveis, locatarios, qtdProps, qtdImoveis, qtdLocs);
                break;
            case '3':
                menuListar(props, imoveis, locatarios, qtdProps, qtdImoveis, qtdLocs);
                break;
            case '4':
                menuProcurar(props, imoveis, qtdProps, qtdImoveis);
                break;
            case '5':
                terminarAluguel(&locatarios, &qtdLocs, imoveis, qtdImoveis, props, qtdProps);
                salvarLocatarios(locatarios, qtdLocs);
                salvarImoveis(imoveis, qtdImoveis);
                salvarProprietarios(props, qtdProps);
                break;
            case '6':
                gerarRelatorio(props, qtdProps, imoveis, qtdImoveis, locatarios, qtdLocs);
                break;
        }
    } while (opcao != 'S' && opcao != 's');

    return 0;
}
