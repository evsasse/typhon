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
  - Suporta qualquer tipo de `for` dentro de `while` dentro de `if` dentro de `while` dentro de `else`...
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

- Os tipos suportados são boolean, integer, float, list, e string
  - Outros tipos são usados internamente e podem aparecer, como function ou None
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

## Arquitetura
1. Flex trata dos tokens
  - Utiliza contextos para trazer tokens de indentação que estão no início de uma linha
  - Utiliza contextos para ignorar comentários simples e multilinha
  - Utiliza contextos para trazer cada caractere de uma string separadamente
  - Alguns desses contextos poderiam ser desnecessários e substituídos por uma expressão regular mais complexa, mas por simplicidade preferi utilizá-los
2. Bison trata de cada statement, composto por apenas uma linha
  - Não cuida de blocos, pois não encontrei uma forma de tratar os blocos definidos pela indentação. Necessitaria de mais informação de contexto, pois só sabe que um bloco terminou quando já está tratando de um statement, que possui menos indentações que o esperado
3. AST
  - Uma statement/linha é recebido, com a informação do número de indentações. É tratado para identificar a qual bloco pertence.
  - Caso pertença ao bloco atual, é adicionado.
  - Caso seja a abertura de um novo bloco, é adicionado ao atual, e o atual se torna esse novo.
  - Caso pertença a um bloco "pai", fecha os blocos filhos, que podem requerer execução de seu corpo, e é adicionado ao "pai"
  - Caso seja um statement simples (expressão, assignment, etc) e pertença ao bloco principal do programa ele é executado assim que adicionado.
  - Caso seja um statement composto (como `if`, `for`, etc) o corpo só será executado quando for fechado por um statement de indentação menor.
  - Caso seja um statement composto que pode ter mais de um bloco (`if` seguido de `elif` seguido de `else`), o corpo só será executado quando o for fechado por um statement de menor indentação que não seja um dos blocos que pode o seguir que esteja na mesma indentação da abertura.
  - Caso seja uma função, seu corpo não é executado quando definido, apenas quando chamado posteriormente.
4. Execução/Interpretação
  - Existem dois lados que precisam ser observados: Expressão literal, Objeto instânciado
  - Cada tipo de statement tem um comportamento diferente, vou explicar o seguinte `a[2] = 1+2-4`. Consideramos que a variável `a` foi definida anteirormente como `a = [0,1,2]`
  - A indireção de funções como `__add__` ou `__getitem__`, que no caso built-in mas poderiam facilmente apontar para uma função interpretada, abre espaço para que poderia facilmente ser implementada a orientação à objetos definidos pelo usuário.
  
  1. Começamos interpretando o assignment literal, que é definido por um lado esquerdo `a[2]` e direito `1+2-4`
  2. O lado esquerdo é testado para saber se é um nome simples, como no caso `abc = 123`, e se é um literal como `123 = 456`. Neste caso não é, o lado esquerdo é uma operação binária.
  3. É testado se a operação é do tipo `KEY`, que aponta para uma posição dentro de um objeto, e se o objeto indicado por `a` implementa a operação `__setitem__` que é chamada para setar uma valor em uma posição de uma lista.
  4. O lado direito é interpretado, se trata de duas operações binárias, `((1+2)-4)`. Começando pela mais interior, executamos cada literal, o `1` nos retorna um objeto `<int 1>`. Verificamos a existência da operação `__add__` no objeto `<int 1>`, e passamos à ela o objeto `<int 2>`. Essa operação nos retorna um objeto `<int 3>`, onde executamos `__sub__` passando `<int 4>`. Isso no retorna um objeto final `<int -1>`.
  5. Chamamos `__setitem__` passando o valor interpretado `<int 2>` como índice/posição e `<int -1>` como novo valor. O índice é testado para verificar se está dentro do tamanho da lista, e o valor é setado.

## Testes

Os testes não foram realizados de maneira muito formalizada, servem apenas de exemplos de funcionalidades e durante a implementação foram utilizados como forma de debuggar alguns problemas mais complexos que necessitavam digitar grandes trechos de código a cada teste.
