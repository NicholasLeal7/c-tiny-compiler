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
