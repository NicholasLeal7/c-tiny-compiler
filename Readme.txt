# Inicialmente, decidi que que o arquivo fonte de entrada, deve obter no máximo até 10000 caractéres
para evitar problema de overflow de buffer. Acredito que essa quantidade é suficiente para testar o programa.

# Para suprir a necessidade de no máximo 15 caracteres em um IDENTIFICADOR, optei
por atribuir uma variavel maximo_char e retornar erro instantaneamente na máquina de estados
caso a quantidade ultrapasse 15.

# Para implementar a máquina de estados dos identificadores, usei a mesma base do miniléxico feito em video-aula
e criei AFD's (segue as imagens) para a construir as máquinas de estados de charconst e intconst.