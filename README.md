# Simulador de Sistemas Operacionais

Este projeto é da disciplina de SO e mostra, na prática, como o sistema operacional pode cuidar de processos e memória.
A ideia foi fazer algo simples de entender, testável no terminal e que também tenha versão com GUI.

## 1) O que este projeto faz

### Escalonamento de processos
- Round Robin com quantum
- SJF preemptivo
- Prioridade preemptiva

### Gerenciamento de memória com paginação
- FIFO
- LRU
- Ótimo (implementado como simplificação didática)

### Resultados que o programa gera
- Timeline de execução dos processos
- Tabela com métricas por processo
- Média de tempo de espera
- Média de tempo de resposta
- Total de page faults

## 2) Estrutura do projeto

```text
Projeto_SO/
  examples/
    processes.csv
  gui/
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    main.cpp
    moc_MainWindow.cpp
    ui_MainWindow.h
  src/
    CSVParser.cpp
    CSVParser.h
    Memory.cpp
    Memory.h
    Process.h
    Scheduler.cpp
    Scheduler.h
    Simulator.cpp
    Simulator.h
    main.cpp
  Makefile
  README.md
```

## 3) Como é o CSV de entrada

O arquivo deve começar com o cabeçalho:

```csv
chegada,burst,prioridade,memoria
```

Depois, cada linha representa um processo:

```csv
0,5,3,64
1,3,2,128
2,8,1,32
3,2,4,256
4,4,2,64
```

- `chegada`: momento em que o processo entra no sistema
- `burst`: tempo total que ele precisa da CPU
- `prioridade`: número menor = mais prioridade
- `memoria`: memória necessária em MB

## 4) Como compilar e executar

> Execute os comandos dentro da pasta do projeto.

### 4.1 Modo terminal

Compilar:

```bash
make terminal
```

Executar com o CSV de exemplo:

```bash
./Projeto_SO.exe examples/processes.csv
```

Ou apenas:

```bash
make run
```

### 4.2 Modo GUI (Qt)

Compilar a interface:

```bash
make gui
```

Executar:

```bash
make run-gui
```

## 5) O que cada arquivo faz

- `src/CSVParser.*`: lê o CSV e cria os objetos de processo
- `src/Process.h`: define a estrutura de dados de um processo
- `src/Scheduler.*`: faz o escalonamento e monta a timeline
- `src/Memory.*`: simula page faults e substituição de páginas
- `src/Simulator.*`: junta escalonador e memória, calcula métricas
- `src/main.cpp`: versão de terminal que roda os testes
- `gui/*`: versão com interface gráfica usando Qt

## 6) Configurações importantes

No terminal, os valores padrão estão em `src/main.cpp`:
- `quantum`
- `memoria_fisica`
- `tamanho_pagina`

Na GUI, esses valores são escolhidos pela interface.

## 7) Observações

- O projeto é focado em didática, então algumas partes são simplificadas.
- A simulação de memória trata cada processo como se tivesse uma página fixa.
- A política ótima é uma implementação teórica só para comparação.

## 8) Sugestão para apresentação

Mostre nesta ordem:
1. como o CSV é lido;
2. como se escolhe o algoritmo/política;
3. a timeline de execução;
4. as métricas e as diferenças entre políticas.

Assim você explica bem o que o código faz e por que cada algoritmo se comporta diferente.
