//Disciplina: ALgoritomos e Lógica de Programação
//Atividade: 01
//Autor: henriwue (eu)

algoritmo "VerificacaoStorageBackup"
var
   volumeDiario, espacoNecessario : real
   diasRetencao : inteiro
   capacidadeTB, capacidadeGB : real

inicio
   // Avaliação de dados e entrada
   escreva("Digite o volume de dados gerado por dia (em GB): ")
   leia(volumeDiario)
   
   escreva("Digite o número de dias de retenção exigido: ")
   leia(diasRetencao)
   
   escreva("Digite a capacidade total do storage (em TB): ")
   leia(capacidadeTB)
   
   // Calcular o espaço necessario para o periodo de retenção
   espacoNecessario <- volumeDiario * diasRetencao
   
   // Converter a capacidade do armazenamento. Formula (1 TB = 1024 GB)
   capacidadeGB <- capacidadeTB * 1024
   
   // Exibir o espaço necessario 
   escreval()
   escreval("----------------------------------------")
   escreval("Espaço necessário para o backup: ", espacoNecessario, " GB")
   
   // Exibir o resultado
   se (espacoNecessario <= capacidadeGB) entao
      escreval("CAPACIDADE SUFICIENTE")
   senao
      escreval("CAPACIDADE INSUFICIENTE")
   fimse
   escreval("----------------------------------------")

fimalgoritmo