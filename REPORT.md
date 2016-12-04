# Relatório final

## Pontos a serem implementados:
- Proposta de nova linguagem [+1]
- Interpretador [+2]

## Outros pontos possíveis:
- Orientação a objetos [+1] ~
- Tratamento de strings [+1] ~
- ~~Geração de código intermediário [+2]~~
- ~~Tradutor entre linguagens [+?]~~

## Especificação
- Subset da sintaxe do Python
- Comentários multilinha

<br/>

- Blocos definidos por indentação
  - Erros de indentação devem ser retornados nos seguintes cenários:
    - Mais indentações do que o bloco atual esperava
    - Esperava um acréscimo de indetações por estar abrindo um novo bloco
    - Há menos indentações que o bloco atual, terminando ele, mas não há um bloco "pai" com o mesmo número de indentações
- Indentação por tabs ou espaços
  - Reclama se misturar. Mas ainda cada tab e espaço é considerado uma única indentação.
  - Opção híbrida, passando argumento. ex: `./typhon -t 3`, nesse caso cada tab é considerado como 3 espaços.
- Apenas linhas com um statement
  - Não suporta o seguinte ex: `a=1; b=2; c=3`
- `if x:`, `elif y:`, `else:`
- `for x in y:`, `else:`
- `while x:`, `else:`
  - Os blocos `if`, `elif`, `else`, `for` e `while` tem acesso e alteram o namespace do bloco "pai"
  - O bloco `else` de um `for` ou `while` é executado se o loop termina sem chamar `break`
  - *Infelizmente não cheguei a implementar o `break` e `continue`, então o bloco `else` dos loops sempre é executado*
- `pass`
  - O statement `pass` não faz nada, pode ser usado para definir blocos que esperam um corpo, mas você não quer realizar nada dentro deles.
  - Também é usado para sempre haver um statement com nenhuma indentação no fim do arquivo, de modo a fechar qualquer bloco anterior.
  - Similarmente existe um statement interno `SyntaxError`, que não realiza nada, além de resetar o parser para esperar nenhuma indentação.
- Operações matemáticas
  - `+ - * /`
  - `%` mod
  - `**` exponenciação
  - `//` divisão inteira
- Operações de comparação
  - `< <= == != >= >`
  - Não suporta a concatenação de comparações como o seguinte ex: `123 < 456 < 789`
- Operações lógicas
  - `and && or || not !`
- Assignments simples e especiais.
  - Simples: `x = 123`
  - Especial: `x += 123`, `y -= 456`, `*= /= %= **= //=`
- Comentários
  - Simples: `# a simple comment`
  - Multilinha: `### a multiline comment, \n other line \n some other line ###`
- Funções, com parâmetros, argumentos e retornos:
  - `def f(x, y):`
  - `return z`
  - `f(foo, bar)`
  - Uma função não vê o escopo exterior, não podendo usar variavéis externas.
  - Mas suporta passagem de qualquer tipo por paramêtro, inclusive outras funções e ela mesma.
  - Poderiamons definir uma função recursiva infinita como o seguinte:
  
    ```
    def foo(bar): # recebe um parâmetro chamado bar
      bar(bar) # chama bar, passando bar
    foo(foo) # chama a função foo passando ela mesma por parâmetro
    ```
    
- Qualquer operação pode ser chamada sobre qualquer objeto, nome, ou resultado de outra operação
  - Se aquela operação específica não é definida para o objeto, um erro correspondente é retornado
  - Se o nome não está definido, um erro correspondente é retornado
  
  ```
  ./typhon $ Typhon 0.0.1
  >>> 1()   
  TypeError: <int 1> object is not callable
  >>> a
  NameError: name 'a' is not defined
  >>> ([2]+[3])**3 
  TypeError: unsupported operand type(s) for **: '<list [<int 2><int 3>]>' and '<int 3>'
  ```
  
- Modo de debug, passando argumento. ex:`./typhon -d`
