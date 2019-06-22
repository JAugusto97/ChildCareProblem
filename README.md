# ChildCareProblem
Primeiro trabalho da disciplina de Sistemas Operacionais ministrada pelo prof. Hélio Crestana Guardia e pela profa. Kelen Vivaldini. 

  Sistemas Operacionais<br>
  Child Care Problem<br>
<br>
Esse projeto tem o intuito de implementar um programa com várias threads que
se comunicam. Para isso foram utilizados mecanismos como semáforos e um mutex.<br>
<br>
O problema ilustra o funcionamento de um centro de cuidados infantis (creche),
na qual há a movimentação, entrada e saída, de crianças que frequentam e adultos que trabalham na creche.<br>
<br>
As crianças podem entrar somente se houver pelo menos um adulto que possa vigiá-la, porém,
se não houver uma vaga, elas devem aguardar em uma fila.<br>
<br>
Os adultos podem entrar sem nenhuma restrição, entretanto, para que possam sair,
é necessário que não haja crianças na creche ou que a quantidade de crianças
seja de: no máximo 3 vezes a quantidade de adultos restantes após sua saída,
pois cada adulto supervisiona até 3 crianças que estão dentro da creche.<br>
<br>
Caso um adulto queira sair, mas não pode devido às condições impostas, ele deve
entrar na fila de espera para sair.<br>
<br>
O usuário deve informar 2 valores de entrada ao executar o programa, primeiro o
número de crianças que deseja, depois o número de adultos. Se não for passado
nenhum valor dos dois valores, então serão tomados como 8 e 2, respectivamente.
Se o número de argumentos forem incompatíveis, maior que 2, então o programa
não é executado e uma mensagem será mostrada na tela.<br>
<br>
Para compilar o programa é necessário executar o Makefile presente na pasta.
Para sua execução, apenas digitar em linha de comando ./creche arg1 arg2.
