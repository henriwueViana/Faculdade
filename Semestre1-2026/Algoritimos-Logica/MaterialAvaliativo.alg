//Disciplina: Algoritmos e Lógica de Programação
//Atividade: MAPA
//Autor: henriwue (eu)

algoritmo "MonitoramentoServidoresDataNorte"
var
   idServidor, idPiorServidor, qtdAmostras : inteiro
   tempoAmostra, somaTempos, mediaTempo, maiorMedia : real
   continuarServidores, continuarAmostras : caractere

inicio
   maiorMedia <- -1
   idPiorServidor <- 0
   
   escreval("========================================")
   escreval("    SISTEMA DE MONITORAMENTO - DATANORTE")
   escreval("========================================")
   
   continuarServidores <- "S"
   
   enquanto (continuarServidores = "S") faca
      escreval()
      escreva("Digite o identificador (ID) do servidor: ")
      leia(idServidor)
      
      somaTempos <- 0
      qtdAmostras <- 0
      
      escreval()
      escreval("--- Leituras de amostras para o Servidor ", idServidor, " ---")
      escreval("Digite o tempo em ms (digite um valor negativo para encerrar):")
      
      continuarAmostras <- "S"
      
      enquanto (continuarAmostras = "S") faca
         escreva("Digite o tempo da amostra (ms): ")
         leia(tempoAmostra)
         
         se (tempoAmostra >= 0) entao
            somaTempos <- somaTempos + tempoAmostra
            qtdAmostras <- qtdAmostras + 1
         senao
            continuarAmostras <- "N"
         fimse
      fimenquanto
      
      se (qtdAmostras > 0) entao
         mediaTempo <- somaTempos / qtdAmostras

         //Aqui estava dando um erro repetindo o pedido varias vezes, não consegui tesolver
         
         escreval()
         escreval("----------------------------------------")
         escreval("Identificador do Servidor: ", idServidor)
         escreval("Quantidade de amostras: ", qtdAmostras)
         escreval("Tempo medio de resposta: ", mediaTempo)
         escreval("----------------------------------------")
         
         
         se (maiorMedia = -1) entao
            maiorMedia <- mediaTempo
            idPiorServidor <- idServidor
         senao
            se (mediaTempo > maiorMedia) entao
               maiorMedia <- mediaTempo
               idPiorServidor <- idServidor
            fimse
         fimse
      senao
         escreval("Aviso: Nenhuma amostra valida registrada para este servidor.")
      fimse
      
      escreval()
      escreva("Deseja cadastrar outro servidor? (S/N): ")
      leia(continuarServidores)
   fimenquanto
   
   escreval()
   escreval("========================================")
   escreval("            RELATORIO FINAL             ")
   escreval("========================================")
   se (maiorMedia <> -1) entao
      escreval("Servidor com o PIOR desempenho:")
      escreval("-> ID do Servidor: ", idPiorServidor)
      escreval("-> Maior tempo medio: ", maiorMedia)
   senao
      escreval("Nenhum dado de servidor foi processado.")
   fi
   escreval("========================================")
   fimse

fimalgoritmo