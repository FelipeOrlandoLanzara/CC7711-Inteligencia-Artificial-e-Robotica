import numpy as np
import skfuzzy as fuzz
import matplotlib.pyplot as plt
from skfuzzy import control as ctrl

#Variaveis de Entrada (Antecedent)

# exercicio 01
comer = ctrl.Antecedent(np.arange(0, 11, 2), 'comer')

preco = ctrl.Antecedent(np.arange(0, 11, 2), 'preco')
qualidade_servico = ctrl.Antecedent(np.arange(0, 11, 2), 'qualidade_servico')


#Variaveis de saída (Consequent)
saida = ctrl.Consequent(np.arange(0, 11, 2), 'pesado')

nota = ctrl.Consequent(np.arange(0, 11, 1), 'nota')

# automf -> Atribuição de categorias automaticamente

comer.automf(names=['pouco','razoavel','bastante'],)

preco.automf(names=['barato','medio','caro'])
qualidade_servico.automf(names=['ruim','medio','bom'])

# atribuicao sem o automf

saida['leve'] = fuzz.trapmf(saida.universe, [0, 0, 4, 6])
saida['medio'] = fuzz.trapmf(saida.universe, [4, 6, 8, 10])
saida['pesado'] = fuzz.trapmf(saida.universe, [8, 10, 10, 10])

nota['ruim'] = fuzz.trapmf(nota.universe, [0, 0, 3, 4])
nota['medio'] = fuzz.trapmf(nota.universe, [4, 5, 6, 7])
nota['bom'] = fuzz.trapmf(nota.universe, [7, 8, 10, 10])

#Visualizando as variáveis

comer.view()
saida.view()

preco.view()
qualidade_servico.view()
nota.view()


#Criando as regras
regra_1 = ctrl.Rule(comer['bastante'], saida['pesado'])
regra_2 = ctrl.Rule(comer['razoavel'], saida['medio'])
regra_3 = ctrl.Rule(comer['pouco'], saida['leve'])
controlador = ctrl.ControlSystem([regra_1, regra_2, regra_3])

regra_4 = ctrl.Rule(preco['caro'] & qualidade_servico['bom'], nota['medio'])
regra_5 = ctrl.Rule(preco['caro'] & qualidade_servico['medio'], nota['medio'])
regra_6 = ctrl.Rule(preco['caro'] & qualidade_servico['ruim'], nota['ruim'])
regra_7 = ctrl.Rule(preco['medio'] & qualidade_servico['bom'], nota['bom'])
regra_8 = ctrl.Rule(preco['medio'] & qualidade_servico['medio'], nota['medio'])
regra_9 = ctrl.Rule(preco['medio'] & qualidade_servico['ruim'], nota['ruim'])
regra_10 = ctrl.Rule(preco['barato'] & qualidade_servico['bom'], nota['bom'])
regra_11 = ctrl.Rule(preco['barato'] & qualidade_servico['medio'], nota['bom'])
regra_12 = ctrl.Rule(preco['barato'] & qualidade_servico['ruim'], nota['medio'])
controlador2 = ctrl.ControlSystem([regra_4, regra_5, regra_6, regra_7, regra_8, regra_9, regra_10, regra_11, regra_12])

#Simulando
CalculoSaida = ctrl.ControlSystemSimulation(controlador)

CalculoNota = ctrl.ControlSystemSimulation(controlador2)


notaComer = int(input('Comer: '))
CalculoSaida.input['comer'] = notaComer
CalculoSaida.compute()

notaPreco = int(input('Preco: '))
notaServico = int(input('Serviço: '))
CalculoNota.input['preco'] = notaPreco
CalculoNota.input['qualidade_servico'] = notaServico
CalculoNota.compute()


valorSaida = CalculoSaida.output['pesado']
valorNota = CalculoNota.output['nota']


print("\nComer %d \nSaida de %5.2f" %(
        notaComer,
        valorSaida))

print("\nPreco %d \nServiço %d \nNota %5.2f" %(
        notaPreco,
        notaServico,
        valorNota))

comer.view(sim=CalculoSaida)
saida.view(sim=CalculoSaida)

preco.view(sim=CalculoNota)
qualidade_servico.view(sim=CalculoNota)
nota.view(sim=CalculoNota)

plt.show()