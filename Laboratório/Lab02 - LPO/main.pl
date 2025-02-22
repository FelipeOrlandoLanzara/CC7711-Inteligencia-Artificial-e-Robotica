% CASAL Pietro e Antonita -> cinco filhos(João, Clara, Francisco, Valéria e Ana)

progenitor(pietro, joao).
progenitor(antonita, joao).
progenitor(pietro, clara).
progenitor(antonita, clara).
progenitor(pietro, francisco).
progenitor(antonita, francisco).
progenitor(pietro, valeria).
progenitor(antonita, valeria).
progenitor(pietro, ana).
progenitor(antonita, ana).

% ANA -> 2 filhas (Helena e Joana)

progenitor(ana, helena).
progenitor(ana, joana).

% Mario FILHo Joao

progenitor(joao , mario).

% Estranho também, foi constatar que o Carlos nasceu da relação entre a Helena, muito formosa, e o Mário

progenitor(helena, carlos).
progenitor(mario, carlos).

% Clara é mae de pietro e enzo

progenitor(clara, pietrojr).
progenitor(clara, enzo).

progenitor(jacynto, francisca).
progenitor(claudia, francisca).
progenitor(jacynto, antonia).
progenitor(claudia, antonia).

progenitor(luzia, jacynto).
progenitor(pablo, jacynto).

% Regras para determinar se alguém é uma pessoa
pessoa(X) :- progenitor(X, _).
pessoa(Y) :- progenitor(_, Y).

% Fatos sobre o sexo das pessoas
sexo(antonita, feminino).
sexo(pietro, masculino).
sexo(joao, masculino).
sexo(clara, feminino).
sexo(francisco, masculino).
sexo(valeria, feminino).
sexo(ana, feminino).
sexo(helena, feminino).
sexo(joana, feminino).
sexo(mario, masculino).
sexo(carlos, masculino).
sexo(fabiana, feminino).
sexo(pietrojr, masculino).
sexo(enzo, masculino).
sexo(jacynto, masculino).
sexo(francisca, feminino).
sexo(claudia, feminino).
sexo(antonia, feminino).
sexo(luzia, feminino).
sexo(pablo, masculino).

% Regras para definir descendência e ascendência
descendente(Y, X) :- progenitor(X, Y).
ascendente(X, Y) :- progenitor(X, Y).

% Regras para determinar pai e mae
pai(X, Y) :- progenitor(X, Y), sexo(X, masculino).
mae(X, Y) :- progenitor(X, Y), sexo(X, feminino).

% Regras para determinar irmãos e irmãs
irmao(Y, Z) :- progenitor(X, Y), progenitor(X, Z), sexo(Y, masculino), Y \= Z.
irma(Y, Z) :- progenitor(X, Y), progenitor(X, Z), sexo(Y, feminino), Y \= Z.

% Regras para determinar avô e avó
avof(X, Z) :- progenitor(X, Y), progenitor(Y, Z), sexo(X, feminino).
avom(X, Z) :- progenitor(X, Y), progenitor(Y, Z), sexo(X, masculino).

% Regras para determinar tios e tias
tio(X, Z) :- irmao(X, Y), progenitor(Y, Z).
tia(X, Z) :- irma(X, Y), progenitor(Y, Z).

% Regras para determinar primos e primas
primo(Z, B) :- 
    progenitor(X, Z), progenitor(A, B), 
    progenitor(C, X), progenitor(C, A), 
    sexo(Z, masculino).

prima(Z, B) :- 
    progenitor(X, Z), progenitor(A, B), 
    progenitor(C, X), progenitor(C, A), 
    sexo(Z, feminino).