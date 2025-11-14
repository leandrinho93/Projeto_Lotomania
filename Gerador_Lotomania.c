#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define TOTAL_NUMEROS 100
#define CONCURSOS 6
#define NUM_POR_CONCURSO 20
#define QTD_QUENTES 20
#define QTD_FRIOS 10
#define NUM_JOGOS 3   // jogos 2,3,4
#define TAM_JOGO 50

typedef struct {
    int numero;
    int frequencia;
} Numero;

// Extrai números de qualquer entrada (letras, espaços, etc.)
int extrairNumeros(char *entrada, int numeros[]) {
    int count = 0;
    char *p = entrada;
    while (*p) {
        if (isdigit((unsigned char)*p)) {
            int n = (int)strtol(p, &p, 10);
            if (n >= 1 && n <= TOTAL_NUMEROS) {
                if (count < NUM_POR_CONCURSO) numeros[count++] = n;
            }
        } else p++;
    }
    return count;
}

// Comparação decrescente por frequência
int compararFrequencia(const void *a, const void *b) {
    Numero *na = (Numero *)a;
    Numero *nb = (Numero *)b;
    // Critério secundário: se a frequência for igual, ordena pelo número crescente
    if (nb->frequencia == na->frequencia) {
        return na->numero - nb->numero;
    }
    return nb->frequencia - na->frequencia;
}

// Verifica se número saiu 3 vezes seguidas nos últimos 3 concursos
int repetiuTresVezes(int numero, int concursos[CONCURSOS][NUM_POR_CONCURSO]) {
    if (CONCURSOS < 3) return 0;
    int count = 0;
    for (int i = CONCURSOS - 3; i < CONCURSOS; i++) {
        if (i < 0) continue;
        int achou = 0;
        for (int j = 0; j < NUM_POR_CONCURSO; j++)
            if (concursos[i][j] == numero) { achou = 1; break; }
        if (achou) count++;
    }
    return count == 3;
}

int ehPar(int n) { return n % 2 == 0; }

// comparação crescente
int cmpInt(const void *a, const void *b) {
    return (*(int*)a) - (*(int*)b);
}

// Gera um jogo equilibrado considerando frios (excluídos), par/ímpar e faixas
void gerarJogo(int jogo[], int tamanho, int quentes[], int qtdQuentes, int frios[], int qtdFrios,
               double faixa1, double faixa2, double faixa3) {
    int i = 0;

    int lim1 = 0, lim2 = 0;
    // Cálculo dos limites de faixa (0.0001 para correção de ponto flutuante)
    if (faixa1 > 0.0) lim1 = (int)(TOTAL_NUMEROS * faixa1 + 0.0001);
    if (faixa2 > 0.0) lim2 = lim1 + (int)(TOTAL_NUMEROS * faixa2 + 0.0001);
    
    // Garantir que os limites estejam dentro do intervalo [0, TOTAL_NUMEROS]
    if (lim1 < 0) lim1 = 0; if (lim2 < 0) lim2 = 0;
    if (lim1 > TOTAL_NUMEROS) lim1 = TOTAL_NUMEROS;
    if (lim2 > TOTAL_NUMEROS) lim2 = TOTAL_NUMEROS;
    if (lim2 < lim1) lim2 = lim1;

    while (i < tamanho) {
        int n = 1 + rand() % TOTAL_NUMEROS;

        // Não incluir frios (excluídos)
        int ehFrio = 0;
        for (int f = 0; f < qtdFrios; f++)
            if (n == frios[f]) { ehFrio = 1; break; }
        if (ehFrio) continue;

        // Verificar faixa
        int faixaValida = 0;
        // Faixa 1: 1 a lim1
        if (faixa1 > 0.0 && n >= 1 && n <= lim1) faixaValida = 1;
        // Faixa 2: lim1+1 a lim2
        else if (faixa2 > 0.0 && n > lim1 && n <= lim2) faixaValida = 1;
        // Faixa 3: lim2+1 a TOTAL_NUMEROS
        else if (faixa3 > 0.0 && n > lim2 && n <= TOTAL_NUMEROS) faixaValida = 1;
        
        if (!faixaValida) continue;

        // Evitar repetição dentro do jogo
        int repetido = 0;
        for (int j = 0; j < i; j++) if (jogo[j] == n) { repetido = 1; break; }
        if (repetido) continue;

        // Balanceamento par/ímpar
        int totalPar = tamanho / 2;
        int totalImpar = tamanho - totalPar;
        int pares = 0, impares = 0;
        for (int j = 0; j < i; j++) { if (ehPar(jogo[j])) pares++; else impares++; }
        
        // Permite 1 número a mais para garantir a geração
        if (ehPar(n) && pares >= totalPar + 1) continue; 
        if (!ehPar(n) && impares >= totalImpar + 1) continue;

        jogo[i++] = n;
    }
}

void imprimirArray(const char *titulo, int arr[], int qtd) {
    printf("%s", titulo);
    if (qtd == 0) { printf(" <vazio>"); }
    for (int i = 0; i < qtd; i++) {
        printf("%d", arr[i]);
        if (i < qtd - 1) printf(" ");
    }
    printf("\n");
}

// Salva os jogos em um arquivo .txt
void salvarResultados(const char *nomeArquivo, int jogos[NUM_JOGOS][TAM_JOGO]) {
    FILE *arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo '%s'.\n", nomeArquivo);
        return;
    }

    fprintf(arquivo, "=== RESULTADOS GERADOS ===\n\n");
    for (int j = 0; j < NUM_JOGOS; j++) {
        fprintf(arquivo, "Jogo %d:\n", j + 1);
        for (int k = 0; k < TAM_JOGO; k++) {
            fprintf(arquivo, "%02d ", jogos[j][k]);
        }
        fprintf(arquivo, "\n\n");
    }

    fprintf(arquivo, "=== Fim dos resultados ===\n");
    fclose(arquivo);
    printf("\nResultados salvos com sucesso em '%s'.\n", nomeArquivo);
}

// Placeholder para salvar como imagem
void salvarComoImagem(const char *nomeArquivo, int jogos[NUM_JOGOS][TAM_JOGO]) {
    printf("Função salvarComoImagem ainda não implementada.\n");
}

int main() {
    int concursos[CONCURSOS][NUM_POR_CONCURSO];
    char entrada[4096];
    int numerosExtraidos[NUM_POR_CONCURSO];

    printf("=== Entrada dos concursos ===\n");
    for (int c = 0; c < CONCURSOS; c++) {
        printf("Digite os %d numeros do concurso %d (qualquer formato):\n", NUM_POR_CONCURSO, c + 1);
        if (!fgets(entrada, sizeof(entrada), stdin)) {
            printf("Erro de leitura. Tente novamente.\n");
            c--;
            continue;
        }
        int qtd = extrairNumeros(entrada, numerosExtraidos);
        if (qtd != NUM_POR_CONCURSO) {
            printf("Erro: voce deve informar exatamente %d numeros (extraidos = %d).\n", NUM_POR_CONCURSO, qtd);
            c--;
            continue;
        }
        for (int i = 0; i < NUM_POR_CONCURSO; i++)
            concursos[c][i] = numerosExtraidos[i];
    }

    srand((unsigned int)time(NULL));

    // Contar frequência
    Numero numeros[TOTAL_NUMEROS];
    for (int i = 0; i < TOTAL_NUMEROS; i++) { numeros[i].numero = i + 1; numeros[i].frequencia = 0; }
    for (int c = 0; c < CONCURSOS; c++)
        for (int i = 0; i < NUM_POR_CONCURSO; i++)
            numeros[concursos[c][i] - 1].frequencia++;

    qsort(numeros, TOTAL_NUMEROS, sizeof(Numero), compararFrequencia);

    printf("\n=== Frequência dos números (agrupada) ===\n");
    int i = 0;
    while (i < TOTAL_NUMEROS) {
        int freq = numeros[i].frequencia;
        printf("Números ");
        int j = i;
        while (j < TOTAL_NUMEROS && numeros[j].frequencia == freq) {
            printf("%d", numeros[j].numero);
            if (j < TOTAL_NUMEROS - 1 && numeros[j + 1].frequencia == freq) printf("-");
            j++;
        }
        printf(" = saíram %d vezes\n", freq);
        if (freq == 0) break; // Para a exibição se a frequência chegar a zero
        i = j;
    }

    // Quentes (os de maior frequência)
    int quentes[QTD_QUENTES], qtdQuentes = 0;
    for (i = 0; i < TOTAL_NUMEROS && qtdQuentes < QTD_QUENTES; i++) {
        if (numeros[i].frequencia == 0) break; // Apenas números que saíram
        quentes[qtdQuentes++] = numeros[i].numero;
    }

    // Variados (todos que não são quentes)
    int variados[TOTAL_NUMEROS];
    int idx_var = 0;
    for (i = 0; i < TOTAL_NUMEROS; i++) {
        int n = numeros[i].numero;
        int isQuente = 0;
        for (int j = 0; j < qtdQuentes; j++) if (quentes[j] == n) { isQuente = 1; break; }
        if (!isQuente) variados[idx_var++] = n;
    }

    // Frios (Preferencialmente frequência zero, com filtro de vizinho)
    int frios[QTD_FRIOS], qtdFrios = 0;
    int frios_zero_freq[TOTAL_NUMEROS], idx_zero = 0;
    int k;

    // 1. Coleta números de frequência zero
    for (k = 0; k < idx_var; k++) {
        // Como 'numeros' está ordenado por frequência decrescente, 
        // os números com freq=0 estarão no final do array original 'numeros'.
        // Em 'variados' eles estão misturados, então precisamos checar
        // no array original 'numeros' (ou recomputar).
        // Vamos checar a frequência no array ordenado 'numeros':
        int freq = 0;
        for (int l = 0; l < TOTAL_NUMEROS; l++) {
            if (numeros[l].numero == variados[k]) {
                freq = numeros[l].frequencia;
                break;
            }
        }

        if (freq == 0) {
            frios_zero_freq[idx_zero++] = variados[k];
            // Remove de 'variados' para não ser processado depois
            variados[k] = -1; // Marca como removido temporariamente
        }
    }

    // Reorganiza 'variados' para remover os marcados
    int novo_idx_var = 0;
    for (k = 0; k < idx_var; k++) {
        if (variados[k] != -1) {
            variados[novo_idx_var++] = variados[k];
        }
    }
    idx_var = novo_idx_var;


    // 2. Preenche 'frios' com números de frequência zero (com filtro de vizinho)
    while (qtdFrios < QTD_FRIOS && idx_zero > 0) {
        int pos = rand() % idx_zero;
        int candidato = frios_zero_freq[pos];

        // Aplica o filtro de vizinho (não ter saído nos concursos)
        int temVizinho = 0;
        for (int c = 0; c < CONCURSOS && !temVizinho; c++) {
            for (int j = 0; j < NUM_POR_CONCURSO; j++) {
                int sorteado = concursos[c][j];
                if (sorteado == candidato - 1 || sorteado == candidato + 1) {
                    temVizinho = 1;
                    break;
                }
            }
        }

        if (temVizinho) { 
            // Se tiver vizinho, descarta e tenta outro da lista de zeros
            frios_zero_freq[pos] = frios_zero_freq[--idx_zero];
            continue; 
        }

        // Adiciona aos frios
        frios[qtdFrios++] = candidato;
        // Remove da lista de zeros
        frios_zero_freq[pos] = frios_zero_freq[--idx_zero];
    }


    // 3. Se ainda faltam, usa os 'variados' restantes (frequência baixa > 0)
    //    mantendo o filtro de vizinho e aleatoriedade
    while (qtdFrios < QTD_FRIOS && idx_var > 0) {
        int pos = rand() % idx_var;
        int candidato = variados[pos];

        int temVizinho = 0;
        for (int c = 0; c < CONCURSOS && !temVizinho; c++) {
            for (int j = 0; j < NUM_POR_CONCURSO; j++) {
                int sorteado = concursos[c][j];
                if (sorteado == candidato - 1 || sorteado == candidato + 1) {
                    temVizinho = 1;
                    break;
                }
            }
        }
        
        if (temVizinho) { 
            variados[pos] = variados[--idx_var]; // Descarta com vizinho
            continue; 
        }

        // Adiciona aos frios
        frios[qtdFrios++] = candidato;
        // Remove da lista de variados
        variados[pos] = variados[--idx_var];
    }

    printf("\n=== Classificação dos números ===\n");
    imprimirArray("Números quentes: ", quentes, qtdQuentes);
    imprimirArray("Números variados (não-quentes e não-frios): ", variados, idx_var);
    imprimirArray("Números frios (pref. freq. zero, sem vizinhos): ", frios, qtdFrios);
    
    // Seu código original não imprimia os variados (idx=0), a correção acima exibe.

    int quentesFinal[QTD_QUENTES], qtdFinal = 0;
    printf("\nVerificando quentes que saíram 3 vezes consecutivas...\n");
    for (i = 0; i < qtdQuentes; i++) {
        if (!repetiuTresVezes(quentes[i], concursos)) {
            quentesFinal[qtdFinal++] = quentes[i];
        } else {
            printf("Número %d descartado por repetição consecutiva\n", quentes[i]);
        }
    }
    
    // ... (restante do main() sem alterações)

    int jogos[NUM_JOGOS][TAM_JOGO];
    printf("\n=== Gerando %d jogos de %d numeros ===\n", NUM_JOGOS, TAM_JOGO);

    printf("Estilo A: 35%% 1-50, 65%% 51-100\n");
    gerarJogo(jogos[0], TAM_JOGO, quentesFinal, qtdFinal, frios, qtdFrios, 0.35, 0.65, 0.0);

    printf("Estilo B: 65%% 1-50, 35%% 51-100\n");
    gerarJogo(jogos[1], TAM_JOGO, quentesFinal, qtdFinal, frios, qtdFrios, 0.65, 0.35, 0.0);

    printf("Estilo C: ~33%% 1-33, ~33%% 34-66, ~34%% 67-100\n");
    gerarJogo(jogos[2], TAM_JOGO, quentesFinal, qtdFinal, frios, qtdFrios, 0.33, 0.33, 0.34);

    // Exibir jogos + frios excluídos
    for (int j = 0; j < NUM_JOGOS; j++) {
        qsort(jogos[j], TAM_JOGO, sizeof(int), cmpInt);
        printf("\nJogo %d (ordenado):\n", j + 1);
        for (int k = 0; k < TAM_JOGO; k++) printf("%d ", jogos[j][k]);
        printf("\n");

        printf("Frios excluídos neste jogo: ");
        int countExcluidos = 0;
        for (int f = 0; f < qtdFrios; f++) {
            int presente = 0;
            for (int k = 0; k < TAM_JOGO; k++) {
                if (jogos[j][k] == frios[f]) { presente = 1; break; }
            }
            if (!presente) {
                printf("%d ", frios[f]);
                countExcluidos++;
            }
        }
        if (countExcluidos == 0) printf("<nenhum>");
        printf("\n");
    }

    printf("\nDeseja salvar os resultados?\n1 - Sim (arquivo .txt)\n2 - Sim (imagem .ppm)\n3 - Não\nEscolha: ");
    int op; scanf("%d", &op); getchar();
    if (op == 1) salvarResultados("resultados.txt", jogos);
    else if (op == 2) salvarComoImagem("resultados.ppm", jogos);

    printf("\n=== Fim do programa ===\n");
    printf("Pressione ENTER para sair...");
    getchar();

    return 0;
}
