/*
Nomes dos Integrantes do grupo:
Giulia Tafuri;
Igor Alves;
Isa Beatriz;
Nikolas de Azevedo;
Vitor Luis.

Descrição:
Programa que administra um campeonato de pontos corridos. 
Funções implementadas:
- criaTabela - Cria tabela do campeonato, pedindo informações como o nome do campeonato e times ao usuário.
- atualizaTabela - Atualiza a tabela do campeonato a partir de alguns dados atuais de cada time.
- retiraTime - Retira time do campeonato a partir do nome do time.
- registraJogo - Atualiza a tabela do campeonato dada partida entre dois times e o placar.
- campeaoAtual - Mostra ao usuário o melhor time atualmente no camponato, seus pontos e saldo de gols.
- trocaDivisões - Rebaixa certo número de times de divisão e promove outros.
- visualizaTabela - Visualiza toda tabela do campeonato.
- reiniciaTabela - Reinicia os dados dos times de campeonato a partir do nome do campeonato.
- apagarCampeonato - Apaga o arquivo que armazena informações do campeonato.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 35
#define scanfCampeonato(campeonato) scanf(" %30[A-Za-z0-9_ ]", campeonato);
#define scanfTime(time) scanf(" %30[^\n]", time);

// Definindo Linked List que será utilizada para organização dos times e algumas funções úteis.
typedef struct {
    char nome[N];
    int gols_marcados, gols_contra, vitorias, derrotas, empates;
} TIME;

typedef struct aux{
    TIME reg;
    struct aux *prox;
} ELEMENTO;

typedef struct {
    ELEMENTO *inicio;
    int tamanho;
} LKL;

void inicializarLista(LKL *l) {
    l->inicio = NULL;
    l->tamanho = 0;
}

void inserirTime(LKL *l, TIME reg) {
    ELEMENTO *end=l->inicio, *novo;
    if (!(novo = malloc(sizeof(ELEMENTO)))) {
        puts("Erro alocacao de memoria;");
        return;
    }

    novo->reg = reg;

    if (end==NULL || strcmp(end->reg.nome, reg.nome)>0) {
        novo->prox = end;
        l->inicio = novo;
    
    } else if (strcmp(end->reg.nome, reg.nome)==0) {
        free(novo);
        puts("Time ja no campeonato.");
        return;

    } else {
        while (end->prox!=NULL && strcmp(end->prox->reg.nome, reg.nome)<0) {
            end = end->prox;
        }

        if (end->prox!=NULL && strcmp(end->prox->reg.nome, reg.nome)==0) {
            free(novo);
            puts("Time ja no campeonato.");
            return;
        }


        novo->prox = end->prox;
        end->prox = novo;
    }
    l->tamanho++;
}

void tirarTime(LKL* l, TIME *reg) {
    if (l->inicio==NULL) return;

    *reg = l->inicio->reg;
    ELEMENTO *apagar = l->inicio;
    l->inicio = l->inicio->prox;
    free(apagar);
    l->tamanho--;
}

// Definindo funções de MergeSort com base em 5 critérios:
// alfabético, saldo de gols, número de vitórias, número de pontos e padrão (critérios de desempate seguem ordem: pontos, vitórias, saldo de gols, alfabética).
void mergeSort(TIME *times, int ini, int meio, int fim) {
    int tamanho1=meio-ini+1, tamanho2=fim-(meio+1)+1, contador1=0, contador2=0, aux=ini;
    TIME seq1[tamanho1], seq2[tamanho2];

    while (contador1 < tamanho1) {
        seq1[contador1] = times[ini + contador1];
        contador1++;
    }
    while (contador2 < tamanho2) {
        seq2[contador2] = times[meio + 1 + contador2];
        contador2++;
    }
    contador1=0;
    contador2=0;

    while (contador1<tamanho1 && contador2<tamanho2) {
        int pontos1=seq1[contador1].vitorias*3+seq1[contador1].empates, pontos2=seq2[contador2].vitorias*3+seq2[contador2].empates;
        if (pontos1>pontos2) {
            times[aux] = seq1[contador1++];

        } else if (pontos1<pontos2){
            times[aux] = seq2[contador2++];   

        } else {
            if (seq1[contador1].vitorias>seq2[contador2].vitorias) {
                times[aux] = seq1[contador1++];

            } else if (seq1[contador1].vitorias<seq2[contador2].vitorias) {
                times[aux] = seq2[contador2++];       

            } else {
                int saldo1=seq1[contador1].gols_marcados-seq1[contador1].gols_contra, saldo2=seq2[contador2].gols_marcados-seq2[contador2].gols_contra;
                if (saldo1>saldo2) {
                    times[aux] = seq1[contador1++];
                
                } else if (saldo1<saldo2) {
                    times[aux] = seq2[contador2++];       
                
                } else {
                    if (strcmp(seq1[contador1].nome, seq2[contador2].nome)<0) {
                        times[aux] = seq1[contador1++];

                    } else {
                        times[aux] = seq2[contador2++];     

                    }
                }
            }
        }
        aux++;
    }

    while (contador1<tamanho1) {
        times[aux++] = seq1[contador1++];
    }
    while (contador2<tamanho2) {
        times[aux++] = seq2[contador2++];       
    }
}
void mergeSortGeral(TIME *times, int ini, int fim) {    
    if (ini < fim) {
        int meio = (ini+fim)/2;
        
        mergeSortGeral(times, ini, meio);
        mergeSortGeral(times, meio+1, fim);

        mergeSort(times, ini, meio, fim);
    }
}


void mergeNome(TIME *times, int ini, int meio, int fim) {
    int tamanho1=meio-ini+1, tamanho2=fim-(meio+1)+1, contador1=0, contador2=0, aux=ini;
    TIME seq1[tamanho1], seq2[tamanho2];

    while (contador1 < tamanho1) {
        seq1[contador1] = times[ini + contador1];
        contador1++;
    }
    while (contador2 < tamanho2) {
        seq2[contador2] = times[meio + 1 + contador2];
        contador2++;
    }
    contador1=0;
    contador2=0;

    while (contador1<tamanho1 && contador2<tamanho2) {
        if (strcmp(seq1[contador1].nome, seq2[contador2].nome)<0) {
            times[aux++] = seq1[contador1++];
        } else {
            times[aux++] = seq2[contador2++];       
        }
    }
    while (contador1<tamanho1) {
        times[aux++] = seq1[contador1++];
    }
    while (contador2<tamanho2) {
        times[aux++] = seq2[contador2++];       
    }
}
void mergeSortNome(TIME *times, int ini, int fim) {    
    if (ini < fim) {
        int meio = (ini+fim)/2;
        
        mergeSortNome(times, ini, meio);
        mergeSortNome(times, meio+1, fim);

        mergeNome(times, ini, meio, fim);
    }
}


void mergeSaldo(TIME *times, int ini, int meio, int fim) {
    int tamanho1=meio-ini+1, tamanho2=fim-(meio+1)+1, contador1=0, contador2=0, aux=ini;
    TIME seq1[tamanho1], seq2[tamanho2];

    while (contador1 < tamanho1) {
        seq1[contador1] = times[ini + contador1];
        contador1++;
    }
    while (contador2 < tamanho2) {
        seq2[contador2] = times[meio + 1 + contador2];
        contador2++;
    }
    contador1=0;
    contador2=0;

    while (contador1<tamanho1 && contador2<tamanho2) {
        int saldo1=seq1[contador1].gols_marcados-seq1[contador1].gols_contra, saldo2=seq2[contador2].gols_marcados-seq2[contador2].gols_contra;
        if (saldo1>saldo2) {
            times[aux++] = seq1[contador1++];
        } else {
            times[aux++] = seq2[contador2++];       
        }
    }
    while (contador1<tamanho1) {
        times[aux++] = seq1[contador1++];
    }
    while (contador2<tamanho2) {
        times[aux++] = seq2[contador2++];       
    }
}
void mergeSortSaldo(TIME *times, int ini, int fim) {    
    if (ini < fim) {
        int meio = (ini+fim)/2;
        
        mergeSortSaldo(times, ini, meio);
        mergeSortSaldo(times, meio+1, fim);

        mergeSaldo(times, ini, meio, fim);
    }
}


void mergeVitorias(TIME *times, int ini, int meio, int fim) {
    int tamanho1=meio-ini+1, tamanho2=fim-(meio+1)+1, contador1=0, contador2=0, aux=ini;
    TIME seq1[tamanho1], seq2[tamanho2];

    while (contador1 < tamanho1) {
        seq1[contador1] = times[ini + contador1];
        contador1++;
    }
    while (contador2 < tamanho2) {
        seq2[contador2] = times[meio + 1 + contador2];
        contador2++;
    }
    contador1=0;
    contador2=0;

    while (contador1<tamanho1 && contador2<tamanho2) {
        if (seq1[contador1].vitorias>seq2[contador2].vitorias) {
            times[aux++] = seq1[contador1++];
        } else {
            times[aux++] = seq2[contador2++];       
        }
    }
    while (contador1<tamanho1) {
        times[aux++] = seq1[contador1++];
    }
    while (contador2<tamanho2) {
        times[aux++] = seq2[contador2++];       
    }
}
void mergeSortVitorias(TIME *times, int ini, int fim) {    
    if (ini < fim) {
        int meio = (ini+fim)/2;
        
        mergeSortVitorias(times, ini, meio);
        mergeSortVitorias(times, meio+1, fim);

        mergeVitorias(times, ini, meio, fim);
    }
}


void mergePontos(TIME *times, int ini, int meio, int fim) {
    int tamanho1=meio-ini+1, tamanho2=fim-(meio+1)+1, contador1=0, contador2=0, aux=ini;
    TIME seq1[tamanho1], seq2[tamanho2];

    while (contador1 < tamanho1) {
        seq1[contador1] = times[ini + contador1];
        contador1++;
    }
    while (contador2 < tamanho2) {
        seq2[contador2] = times[meio + 1 + contador2];
        contador2++;
    }
    contador1=0;
    contador2=0;

    while (contador1<tamanho1 && contador2<tamanho2) {
        int pontos1=seq1[contador1].vitorias*3+seq1[contador1].empates, pontos2=seq2[contador2].vitorias*3+seq2[contador2].empates;
        if (pontos1>pontos2) {
            times[aux++] = seq1[contador1++];
        } else {
            times[aux++] = seq2[contador2++];       
        }
    }
    while (contador1<tamanho1) {
        times[aux++] = seq1[contador1++];
    }
    while (contador2<tamanho2) {
        times[aux++] = seq2[contador2++];       
    }
}
void mergeSortPontos(TIME *times, int ini, int fim) {    
    if (ini < fim) {
        int meio = (ini+fim)/2;
        
        mergeSortPontos(times, ini, meio);
        mergeSortPontos(times, meio+1, fim);

        mergePontos(times, ini, meio, fim);
    }
}


// Assinatura de funções que usuário pode utilizar.
int criaTabela();
int atualizaTabela();
int retiraTime();
int registraJogo();
int campeaoAtual();
int trocaDivisoes();
int visualizaTabela();
int reiniciaTabela();
int apagarCampeonato();


// Função principal que carrega leque de opções para o usuário
int main() {
    int opcao, parada=0, resultado;

    do {
        // puts("Opcoes do programa:\n"
        // "1 - Inicia tabela de campeonato novo.\n"
        // "2 - Atualizar a tabela com base nos dados do time.\n"
        // "3 - Retira time do campeonato.\n"
        // "4 - Registar partida nova jogado entre dois times.\n"
        // "5 - Visualizar dados do campeao atual.\n"
        // "6 - Trocar times de divisao.\n"
        // "7 - Mostrar tabela do campeonato.\n"
        // "8 - Reiniciar campeonato.\n"
        // "9 - Apagar o campeonato.\n"
        // "0 - Sair do programa.");
        // printf("Forneca o numero da funcao que deseja usar:\t");
        scanf("%d", &opcao);
        while(getchar() != '\n');

        switch (opcao) {
            case 0:
                resultado = 0;
                parada = 1;
                break;

            case 1:
                resultado = criaTabela();
                break;

            case 2:
                resultado = atualizaTabela();
                break;

            case 3:
                resultado = retiraTime();
                break;

            case 4:
                resultado = registraJogo();
                break;

            case 5:
                resultado = campeaoAtual();
                break;

            case 6:
                resultado = trocaDivisoes();
                break;
            
            case 7:
                resultado = visualizaTabela();
                break;
            
            case 8:
                resultado = reiniciaTabela();
                break;
            
            case 9:
                resultado = apagarCampeonato();
                break;

            default:
                puts("Opcao invalida fornecida.\n");
        }

        switch (resultado) {
            case 0:
                break;

            case 1:
                puts("Funcao terminada corretamente.\n");
                break;

            case -1:
                puts("Erro na abertura do arquivo.\n");
                break;

            case -2:
                puts("Campeonato nao encontrado.\n");
                break;

            case -3:
                puts("Erro de alocacao de memoria.\n");
                break;
                
            case -4:
                printf("Erro de leitura no arquivo.\n");
                break;

            case -5:
                puts("Time nao encontrado no campeonato.\n");
                break;

        }
    
    } while(!parada);
    puts("Fim do programa.");
}


// Função que cria campeonato com base no nome do campeonato e dos times
int criaTabela() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato (Atencao: este sera o nome do arquivo):");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (arq = fopen(nome, "r")) {
        fclose(arq);
        puts("O campeonato ja existe.\n");
        return 0;
    }
    
    if (!(arq = fopen(nome, "w"))) return -1;
    
    LKL times;
    TIME novo;
    inicializarLista(&times);

    // puts("\nForneca o nome do time (para parar forneca 'fim'):");
    scanfTime(novo.nome);
    
    while (strcmp("fim", novo.nome)!=0) {
        novo.vitorias = 0;
        novo.empates = 0;
        novo.derrotas = 0;
        novo.gols_marcados = 0;
        novo.gols_contra = 0;
        inserirTime(&times, novo);

        // puts("\nForneca o nome do time (para parar forneca 'fim'):");
        scanfTime(novo.nome);
    }

    while (times.tamanho!=0) {
        tirarTime(&times, &novo);
        fwrite(&novo, sizeof(TIME), 1, arq);
    }
    fclose(arq);

    return 1;
}


// Função que atualiza campeonato com base no nome do campeonato, nome do time e seus valores de vitória, empate, derrota, gols feitos, gols recebidos
int atualizaTabela() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times;
    char nome_time[N];
    int num_times;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            free(times);
            fclose(arq);
            return -4;
        }    
    }
    fclose(arq);

    // puts("Forneca o nome do time.");
    scanfTime(nome_time);

    int ini=0, fim=num_times-1, meio, encontrado=0;
    while (ini<=fim) {
        meio = (fim+ini)/2;
        if (strcmp(times[meio].nome, nome_time)==0) {
            encontrado = 1;
            break;
        } else if (strcmp(times[meio].nome, nome_time)<0) {
            ini = meio+1;
        } else {
            fim = meio-1;
        }
    }

    if (!encontrado) {
        int parada=0, verificador;
        while (!parada) {
            // puts("Time nao encontrado, deseja inserir na tabela? (1 ou 0)");
            scanf(" %d", &verificador);
            while(getchar() != '\n');

            if (!verificador) {
                puts("Time nao adicionado.");
                parada=1;

            } else if (verificador==1) {
                TIME novo;
                strcpy(novo.nome, nome_time);
                // puts("Forneca num de vitorias, empates, derrotas, gols marcados e gols contra: (nesta ordem)");
                scanf(" %d %d %d %d %d", &novo.vitorias, &novo.empates, &novo.derrotas, &novo.gols_marcados, &novo.gols_contra);
                while(getchar() != '\n');
                
                num_times++;
                if (!(times = realloc(times, sizeof(TIME)*num_times))) return -3;

                for (int i=num_times-1; ini<i; i--) {
                    times[i] = times[i-1];
                }
                times[ini] = novo;

                
                if (!(arq = fopen(nome, "w"))) {
                    free(times);
                    return -1;
                }

                for (int i=0; i<num_times; i++) {
                    fwrite(&times[i], sizeof(TIME), 1, arq);
                }
                fclose(arq);

                parada=1;

            } else {
                puts("Valor invalido fornecido.");
            }
        }
    
    } else {
        // puts("Time encontrado. Forneca num de vitorias, empates, derrotas, gols marcados e gols contra: (nesta ordem)");
        scanf(" %d %d %d %d %d", &times[meio].vitorias, &times[meio].empates, &times[meio].derrotas, &times[meio].gols_marcados, &times[meio].gols_contra);
        while(getchar() != '\n');

        if (!(arq = fopen(nome, "w"))) return -1;

        for (int i=0; i<num_times; i++) {
            fwrite(&times[i], sizeof(TIME), 1, arq);
        }
        fclose(arq);
    }

    free(times);

    return 1;
}


// Função que retira time de campeonato com base no nome do campeonato e nome do time.
int retiraTime() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times;
    char nome_deletar[N];
    int num_times;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            fclose(arq);
            return -4;
        }    
    }
    fclose(arq);

    // puts("Forneca o nome do time:");
    scanfTime(nome_deletar);

    int ini=0, fim=num_times-1, meio, encontrado=0;
    while (ini<=fim) {
        meio = (fim+ini)/2;
        if (strcmp(times[meio].nome, nome_deletar)==0) {
            encontrado = 1;
            break;
        } else if (strcmp(times[meio].nome, nome_deletar)<0) {
            ini = meio+1;
        } else {
            fim = meio-1;
        }
    }

    if (!encontrado) return -5;

    if (!(arq = fopen(nome, "w"))) {
        free(times);
        return -1;
    }

    for (int i=0; i<num_times; i++) {
        if (i!=meio) {
            fwrite(&times[i], sizeof(TIME), 1, arq);
        }
    }

    free(times);
    fclose(arq);

    return 1;
}


// Função que atualiza tabela do campeonato com base no nome do campeonato, número de gols e nome de dois times.
int registraJogo() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times;
    char nome1[N], nome2[N];
    int num_times, gols1, gols2;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            fclose(arq);
            return -4;
        }    
    }
    rewind(arq);
    fclose(arq);

    // puts("Forneca o num de gols e o nome do primeiro time: (gols nome)");
    scanf(" %d", &gols1);
    scanfTime(nome1);

    // puts("Forneca o num de gols e o nome do segundo time: (gols nome)");
    scanf(" %d", &gols2);
    scanfTime(nome2);

    int ini=0, fim=num_times-1, encontrado=0, meio1, meio2;
    while (ini<=fim) {
        meio1 = (fim+ini)/2;
        if (strcmp(times[meio1].nome, nome1)==0) {
            encontrado = 1;
            break;
        } else if (strcmp(times[meio1].nome, nome1)<0) {
            ini = meio1+1;
        } else {
            fim = meio1-1;
        }
    }

    if (!encontrado) {
        free(times);
        return -5;
    }

    ini=0, fim=num_times-1, encontrado=0;
    while (ini<=fim) {
        meio2 = (fim+ini)/2;
        if (strcmp(times[meio2].nome, nome2)==0) {
            encontrado = 1;
            break;
        } else if (strcmp(times[meio2].nome, nome2)<0) {
            ini = meio2+1;
        } else {
            fim = meio2-1;
        }
    }

    if (!encontrado) {
        free(times);
        return -5;
    }

    times[meio1].gols_marcados += gols1;
    times[meio1].gols_contra += gols2;

    times[meio2].gols_marcados += gols2;
    times[meio2].gols_contra += gols1;

    if (gols1>gols2) {
        times[meio1].vitorias ++;
        times[meio2].derrotas ++;

    } else if (gols1<gols2) {
        times[meio2].vitorias ++;
        times[meio1].derrotas ++;
        
    } else {
        times[meio1].empates ++;
        times[meio2].empates ++;
    }

    if (!(arq = fopen(nome, "w"))) {
        free(times);
        return -1;
    }

    for (int i=0; i<num_times; i++) {
        fwrite(&times[i], sizeof(TIME), 1, arq);
    }

    free(times);
    fclose(arq);

    return 1;
}


// Função que mostra o atual campeão do campeonato com base no nome do campeonato.
int campeaoAtual() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times;
    int num_times;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            fclose(arq);
            return -4;
        }    
    }
    fclose(arq);
    
    int fim=num_times-1;
    mergeSortGeral(times, 0, fim);
    
    puts("Campeao atual:");
    printf("%s\t\tP=%d\tPJ=%d\tV=%d\tE=%d\tD=%d\tGM=%d\tGC=%d\tSG=%d\n",
    times[0].nome, times[0].vitorias*3+times[0].empates, times[0].vitorias+times[0].empates+times[0].derrotas,
    times[0].vitorias, times[0].empates, times[0].derrotas, times[0].gols_marcados, times[0].gols_contra,
    times[0].gols_marcados-times[0].gols_contra);

    return 1;
}


// Função que troca times de campeonatos com base no nome de dois campeonatos e número de times trocados.
int trocaDivisoes() {
    FILE *arq1, *arq2;
    char nome1[N], nome2[N];

    // puts("\nForneca o nome do campeonato de maior divisao:");
    scanfCampeonato(nome1);
    while(getchar() != '\n');
    strcat(nome1, ".bin");

    if (!(arq1 = fopen(nome1, "r"))) return -2;

    TIME *times1;
    int num_times1;

    fseek(arq1, 0, SEEK_END);
    num_times1 = ftell(arq1)/sizeof(TIME);
    rewind(arq1);
    
    if (!(times1 = malloc(sizeof(TIME)*num_times1))) {
        fclose(arq1);
        return -3;
    }

    for (int i=0; i<num_times1; i++) {
        if (!fread(times1+i, sizeof(TIME), 1, arq1)) {
            free(times1);
            fclose(arq1);
            return -4;
        }    
    }
    fclose(arq1);

    // puts("\nForneca o nome do campeonato de menor divisao:");
    scanfCampeonato(nome2);
    while(getchar() != '\n');
    strcat(nome2, ".bin");

    if (!(arq2 = fopen(nome2, "r"))) {
        free(times1);
        return -2;
    }

    TIME *times2;
    int num_times2;

    fseek(arq2, 0, SEEK_END);
    num_times2 = ftell(arq2)/sizeof(TIME);
    rewind(arq2);
    
    if (!(times2 = malloc(sizeof(TIME)*num_times2))) {
        free(times1);
        fclose(arq2);
        return -3;
    }

    for (int i=0; i<num_times2; i++) {
        if (!fread(times2+i, sizeof(TIME), 1, arq2)) {
            free(times1);
            free(times2);
            fclose(arq2);
            return -4;
        }    
    }
    fclose(arq2);

    int num_troca;

    // puts("\nForneca o numero de times mudando de divisao:");
    scanf(" %d", &num_troca);
    while(getchar() != '\n');

    if (num_troca>num_times1 || num_troca>num_times2 || num_troca<=0) {
        free(times1);
        free(times2);
        puts("Numero invalido de times a serem trocados.\n");
        return 0;
    }

    int fim=num_times1-1;
    mergeSortGeral(times1, 0, fim);

    fim=num_times2-1;
    mergeSortGeral(times2, 0, fim);

    TIME aux;

    for (int i=0; i<num_troca; i++) {
        aux = times1[(num_times1-1)-i];
        times1[(num_times1-1)-i] = times2[i];
        times2[i] = aux;
    }

    if (!(arq1 = fopen(nome1, "w"))) {
        free(times1);
        free(times2);
        return -1;
    }

    if (!(arq2 = fopen(nome2, "w"))) {
        free(times1);
        free(times2);
        fclose(arq1);
        return -1;
    }

    for (int i=0; i<num_times1; i++) {
        fwrite(&times1[i], sizeof(TIME), 1, arq1);
    }
    
    for (int i=0; i<num_times2; i++) {
        fwrite(&times2[i], sizeof(TIME), 1, arq2);
    }

    free(times1);
    free(times2);
    fclose(arq1);
    fclose(arq2);

    return 1;
}


// Função que mostra toda tabela de campeonato com base no nome do campeonato e o modo desejado.
int visualizaTabela() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times, novo;
    char nome_novo[N];
    int num_times;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            fclose(arq);
            return -4;
        }    
    }
    fclose(arq);

    int opcao;
    // puts("\nOpcoes de ordenacao da tabela:\n"
    // "1-Ordem alfabetica\n"
    // "2-Numero de pontos\n"
    // "3-Numero de vitorias\n"
    // "4-Saldo de gols\n"
    // "5-Padrao");
    // printf("Forneca o numero da opcao:\t");
    scanf(" %d", &opcao);
        while(getchar() != '\n');

        switch (opcao) {
            case 1:
                mergeSortNome(times, 0, num_times-1);
                break;

            case 2:
                mergeSortPontos(times, 0, num_times-1);
                break;

            case 3:
                mergeSortVitorias(times, 0, num_times-1);
                break;

            case 4:
                mergeSortSaldo(times, 0, num_times-1);
                break;

            case 5:
                mergeSortGeral(times, 0, num_times-1);
                break;

            default:
                puts("Valor errado fornecido, visualizacao padrao.");
                mergeSortGeral(times, 0, num_times-1);
        }

    puts("Times do campeonato:");
    int tamanho_margem = N;
    for (int i=0; i<num_times; i++) {
        printf("%-*s\tP=%d\tPJ=%d\tV=%d\tE=%d\tD=%d\tGM=%d\tGC=%d\tSG=%d\n", tamanho_margem,
        times[i].nome, times[i].vitorias*3+times[i].empates, times[i].vitorias+times[i].empates+times[i].derrotas,
        times[i].vitorias, times[i].empates, times[i].derrotas, times[i].gols_marcados, times[i].gols_contra,
        times[i].gols_marcados-times[i].gols_contra);
    }

    free(times);

    return 1;
}


// Função que reinicia dados de campeonato com base no nome do campeonato.
int reiniciaTabela() {
    FILE *arq;
    char nome[N];

    // puts("\nForneca o nome do campeonato:");
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (!(arq = fopen(nome, "r"))) return -2;

    TIME *times;
    int num_times;

    fseek(arq, 0, SEEK_END);
    num_times = ftell(arq)/sizeof(TIME);
    rewind(arq);
    
    if (!(times = malloc(sizeof(TIME)*num_times))) return -3;

    for (int i=0; i<num_times; i++) {
        if (!fread(times+i, sizeof(TIME), 1, arq)) {
            fclose(arq);
            return -4;
        }    
    }
    fclose(arq);

    for (int i=0; i<num_times; i++) {
        times[i].vitorias = times[i].derrotas = times[i].empates = times[i].gols_marcados =  times[i].gols_contra = 0;  
    }

    if (!(arq = fopen(nome, "w"))) {
        free(times);
        return -1;
    }

    for (int i=0; i<num_times; i++) {
        fwrite(&times[i], sizeof(TIME), 1, arq);
    }

    free(times);
    fclose(arq);

    return 1;
}


// Função que apaga o campeonato com base no nome do campeonato.
int apagarCampeonato() {
    char nome[N];

    // puts("\nForneca o nome do campeonato:"); 
    scanfCampeonato(nome);
    while(getchar() != '\n');
    strcat(nome, ".bin");

    if (remove(nome)) return -2;

    return 1;
}

