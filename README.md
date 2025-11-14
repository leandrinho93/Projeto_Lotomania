# Projeto_Lotomania
Gerar jogos da Lotomania  para fins de estudo e aprendizagem
Propósito Principal do Código: Gerador de Jogos de Loteria (LOTOMANIA)
O código em C é um analisador de frequência e gerador de jogos de loteria (simulando um tipo de loteria onde se escolhem 50 números de um total de 100), com base nos resultados de concursos anteriores.
Ele lê os resultados de 6 concursos passados, classifica os números em "quentes" e "frios", e usa essas estatísticas para gerar 3 jogos "inteligentes" e equilibrados.
Entrada de Dados (Análise)
O programa solicita ao usuário que digite os 20 números de cada um dos 6 concursos anteriores.

Ele usa a função extrairNumeros para garantir que apenas os números válidos (1 a 100) sejam armazenados, independentemente do formato de entrada (ex: "1-33 45. 78a").

2. Classificação Estatística
Cálculo da Frequência: Percorre todos os 6 concursos e conta quantas vezes cada número (de 1 a 100) foi sorteado.

Ordenação: Ordena todos os 100 números usando compararFrequencia.

Classificação em Grupos:

Quentes: Os QTD_QUENTES (20) números com a maior frequência de sorteio (excluindo aqueles com frequência zero).

Frios (Excluídos): São os QTD_FRIOS (10) números de menor frequência (preferencialmente com frequência zero), com um filtro adicional para excluir números que possuem um vizinho (n-1 ou n+1) que saiu em algum dos 6 concursos. A ideia é excluir números "azarentos" e isolados.

Variados: Todos os números que não são nem Quentes nem Frios.

3. Filtro de Repetição Consecutiva
O código refina a lista de Números Quentes, descartando qualquer número que tenha saído nos últimos 3 concursos consecutivos (repetiuTresVezes). Esta é uma tática comum em jogos de loteria para evitar repetições recentes.

4. Geração dos Jogos Equilibrados (gerarJogo)
O código gera NUM_JOGOS (3) jogos, cada um com TAM_JOGO (50) números, usando a lista de frios como números a serem excluídos e tentando equilibrar o resultado com base em:

Exclusão de Frios: Os números frios são explicitamente evitados.

Balanceamento Par/Ímpar: Tenta manter um número quase igual de números pares e ímpares (aproximadamente 25 pares e 25 ímpares em 50 números).

Balanceamento por Faixa (Padrões de Distribuição): O gerador usa percentuais para forçar a distribuição dos 50 números em faixas predefinidas (1-50, 51-100 ou 1-33, 34-66, 67-100).

Estilo A: 35% na faixa 1-50 e 65% na faixa 51-100.

Estilo B: 65% na faixa 1-50 e 35% na faixa 51-100.

Estilo C: Distribuição quase uniforme (33% em cada terço).

5. Saída e Salvamento
Os jogos gerados são ordenados e exibidos no console.

O programa pergunta se o usuário deseja salvar os resultados em um arquivo de texto (resultados.txt).




