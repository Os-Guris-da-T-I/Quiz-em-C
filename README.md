# Gerenciador de Perguntas para Quiz de TI

**Integrantes:**

1. Joao Victor Guimaraes Gomes
2. Matheus Augusto Antunes Pentogennis
3. Gustavo Gabriel Naves Ferreira
4. Lucas Siqueira Teles
5. Lucas da Graca Leandro
6. Gabriel Goncalves Lima

**Apresentacao em video:** [COLAR O LINK DO YOUTUBE AQUI]

## Descricao

Sistema em linguagem C, executado no terminal, para cadastrar e organizar as perguntas que futuramente serao usadas em um quiz de orientacao para estudantes do Ensino Medio interessados nos cursos de Ciencia da Computacao (CC), Engenharia de Software (ES) e Analise e Desenvolvimento de Sistemas (ADS).

Nesta etapa o programa somente gerencia o banco de perguntas. Nao aplica o quiz nem calcula resultados.

## Arquivos

- `quiz_ti.c` - codigo-fonte
- `perguntas.csv` - banco de perguntas usado nos testes

## Como compilar

Com GCC:

```bash
gcc quiz_ti.c -o quiz_ti
```

## Como executar

No Windows:

```bash
quiz_ti.exe
```

No Linux/macOS:

```bash
./quiz_ti
```

O programa deve ser executado na mesma pasta do `perguntas.csv`.

## Funcionalidades

1. Cadastrar pergunta
2. Listar todas as perguntas
3. Consultar perguntas por categoria
4. Consultar perguntas por curso
5. Atualizar pergunta
6. Excluir pergunta
0. Sair

## Estrutura do CSV

Cada linha usa:

`id;texto;categoria;curso;resposta`

Exemplo:

`1;Voce gosta de resolver problemas de logica?;Raciocinio;CC;SIM`

- Cursos permitidos: `CC`, `ES`, `ADS`
- Respostas permitidas: `SIM`, `NAO`

## Validacoes

- Opcao do menu e codigo precisam ser numeros inteiros validos (o codigo deve ser positivo).
- Nao e permitido cadastrar codigo repetido.
- Textos vazios (ou so com espacos) sao rejeitados.
- O caractere `;` e rejeitado nos textos, pois separa os campos do CSV.
- Curso e resposta aceitam maiusculas ou minusculas (`cc`, `sim`), mas so sao gravados como `CC/ES/ADS` e `SIM/NAO`.
- A consulta por categoria ignora diferenca entre maiusculas e minusculas.
- Textos maiores que o limite do campo sao rejeitados, sem deixar lixo na entrada.
- Erros ao abrir arquivos e ao substituir o CSV sao informados ao usuario.
- Linhas invalidas encontradas no CSV (por exemplo, editadas a mao) sao ignoradas nas listagens e consultas, e preservadas ao atualizar ou excluir, para nao haver perda de dados.

## Observacao

As mensagens do programa nao usam acentos para evitar problemas de codificacao no terminal do Windows.
