# Gerenciador de Perguntas para Quiz de TI

**Integrantes:**

1. João Victor Guimarães Gomes
2. Matheus Augusto Antunes Pentogennis
3. Gustavo Gabriel Naves Ferreira
4. Lucas Siqueira Teles
5. Lucas da Graça Leandro
6. Gabriel Gonçalves Lima

**Apresentaçao em video:** [video do Youtube]

## Descrição

Sistema em linguagem C, executado no terminal, para cadastrar e organizar as perguntas que futuramente serão usadas em um quiz de orientação para estudantes do Ensino Médio interessados nos cursos de Ciência da Computação (CC), Engenharia de Software (ES) e Análise e Desenvolvimento de Sistemas (ADS).

Nesta etapa o programa somente gerencia o banco de perguntas. Não aplica o quiz nem calcula resultados.

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

## Validações

- Opção do menu e código precisam ser numeros inteiros validos (o código deve ser positivo).
- Não é permitido cadastrar código repetido.
- Textos vazios (ou só com espaços) são rejeitados.
- O caractere `;` e rejeitado nos textos, pois separa os campos do CSV.
- Curso e resposta aceitam maiusculas ou minusculas (`cc`, `sim`), mas só são gravados como `CC/ES/ADS` e `SIM/NAO`.
- A consulta por categoria ignora diferença entre maiúsculas e minúsculas.
- Textos maiores que o limite do campo são rejeitados, sem deixar lixo na entrada.
- Erros ao abrir arquivos e ao substituir o CSV sao informados ao usuário.
- Linhas invalidas encontradas no CSV (por exemplo, editadas a mão) são ignoradas nas listagens e consultas, e preservadas ao atualizar ou excluir, para não haver perda de dados.

## Observação

As mensagens do programa não usam acentos para evitar problemas de codificacao no terminal do Windows/Linux.
