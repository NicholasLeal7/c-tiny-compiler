# Inicialmente, decidi que que o arquivo fonte de entrada, deve obter no máximo até 10000 caractéres
para evitar problema de overflow de buffer. Acredito que essa quantidade é suficiente para testar o programa.

# Para suprir a necessidade de no máximo 15 caracteres em um IDENTIFICADOR, optei
por atribuir uma variavel maximo_char e retornar erro instantaneamente na máquina de estados
caso a quantidade ultrapasse 15.

# Para implementar a máquina de estados dos identificadores, usei a mesma base do miniléxico feito em video-aula
e criei AFD's (segue as imagens) para a construir as máquinas de estados de charconst e intconst.

# Quando estava implementando o reconhecimento dos operadores como + - > <...  e sinais como () , ; {}, resolvi
criar uma função para reconhcer operadores e sinais, pois o código estava virando
uma torre gigante de else if. Tomei essa decisão pois percebi que teria que criar uma função
para reconhecer palavras reservadas, então acabou sendo apropriado.

# Na parte de atribuir tamanho ao vetor que será responsável por armazenar o intconst, decidi deixar o tamanho de
8 caracteres, pois 0xFFFFFFFF é o maior número que é possível de se fazer na notação hexa com um tipo inteiro de 32 bits.
Então, a minha função que reconhece números, retorna ERRO caso tenha mais de 8 caractéres + 2(0x).

# O analisador léxico foi finalizado com a criação das funções que reconhece as palavras reservadas e pela a que
reconhece os comentários e adapta suas linhas, sem muitas complicações.

# A análise sintática foi concluída sem muitas ressalvas, porém eu me baseei no enunciado desatualizado erroneamente e 
perdi um bom tempo tentando encontrar lógica na gramática. Quando abri o moodle para enviar uma mensagem questionando, vi que
havia uma mensagem informando que a gramática recebeu ajustes, aí sim funcionou de verdade,

# Na hora de contar as linhas, em obter_atomo(), a função reconhecer_comentarios() possui uma individualidade. Todos os átomos
reconhecidos podem ter sua respectiva linha atribuída exatamente antes de retornarmos o TInfoAtomo, pois nenhuma função, com exceção
do reconhece_comentarios(), modifica a variável global de linhas. Portanto, a atribuição da linha em que o comentário se iniciou é feito
dentro da função de comentários e caso o estejamos analisando um comentário, a parte de atualizar antes do retorno deve ser ignorada.
