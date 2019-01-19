#include "simplegrade.h"
#include "memvirt.h"
#include <stdio.h>

//imprime o resultado recebido
void imprimeResultado(struct result * resultado, int num_procs){
	int somaRef = 0;
	int somaPfs = 0;
	for(int i=0; i<num_procs; i++){
		somaRef += resultado->refs[i];
		somaPfs += resultado->pfs[i];
	}
	if(somaPfs > 0)
		resultado->total_pf_rate = 100*(somaPfs/(float)somaRef); 
	else
		resultado->total_pf_rate = 0;
	printf("[Taxa de falta de pagina de toda da simulacao: %.2f%%]\n\n", resultado->total_pf_rate);
}

//free no resultado
void limpar(struct result *a){
	free(a);
}

void testeApenasUmProcesso(){
	int num_procs = 1;
	uint32_t num_frames = 3;
	struct result * resultado = memvirt(num_procs, num_frames, "alunos/t01.txt");
	WHEN("Tenho apenas um processo");
    IF("Tenho um número de quadros igual a 3 e 300 referencias a paginas");
    THEN("Espero que tenha 174 faltas de pagina ");
    isEqual(resultado->refs[0], 300, 1);
    isEqual(resultado->pfs[0], 174, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
    
    num_procs = 1;
	num_frames = 10;
	resultado = memvirt(num_procs, num_frames, "alunos/t01.txt");
	WHEN("Tenho apenas um processo");
    IF("Tenho um número de quadros igual a 10 e 300 referencias a paginas");
    THEN("Espero que tenha 8 faltas de pagina ");
    isEqual(resultado->refs[0], 300, 1);
    isEqual(resultado->pfs[0], 8, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
    
    num_procs = 1;
	num_frames = 30;
	resultado = memvirt(num_procs, num_frames, "alunos/t01.txt");
	WHEN("Tenho apenas um processo");
    IF("Tenho um número de quadros igual a 30 e 300 referencias a paginas");
    THEN("Espero que tenha 8 faltas de pagina ");
    isEqual(resultado->refs[0], 300, 1);
    isEqual(resultado->pfs[0], 8, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
}

void testeQuatroProcessos(){
	int num_procs = 4;
	uint32_t num_frames = 12;
	struct result * resultado = memvirt(num_procs, num_frames, "alunos/t02.txt");
	WHEN("Tenho quatro processos, cada um com 100 referencias a paginas");
    IF("Tenho um número de quadros 12 ou seja, 3 por processo ( 12/4 )");
    THEN("Espero que o processo 0 tenha 74 faltas, processo 1: 56, processo 2: 58 e processo 3: 87");
    isEqual(resultado->pfs[0], 74, 1);
    isEqual(resultado->pfs[1], 56, 1);
    isEqual(resultado->pfs[2], 58, 1);
    isEqual(resultado->pfs[3], 87, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
    
    num_procs = 4;
	num_frames = 24;
	resultado = memvirt(num_procs, num_frames, "alunos/t02.txt");
	WHEN("Tenho quatro processos, cada um com 100 referencias a paginas");
    IF("Tenho um número de quadros 24 ou seja, 6 por processo ( 24/4 )");
    THEN("Espero que o processo 0 tenha 28 faltas, processo 1: 27, processo 2: 38 e processo 3: 44");
    isEqual(resultado->pfs[0], 28, 1);
    isEqual(resultado->pfs[1], 27, 1);
    isEqual(resultado->pfs[2], 38, 1);
    isEqual(resultado->pfs[3], 44, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
    
    num_procs = 4;
	num_frames = 48;
	resultado = memvirt(num_procs, num_frames, "alunos/t02.txt");
	WHEN("Tenho quatro processos, cada um com 100 referencias a paginas");
    IF("Tenho um número de quadros 48 ou seja, 12 por processo ( 48/4 )");
    THEN("Espero que tenha apenas as primeiras faltas ao carregar para memória");
    isEqual(resultado->pfs[0], 9, 1);
    isEqual(resultado->pfs[1], 8, 1);
    isEqual(resultado->pfs[2], 8, 1);
    isEqual(resultado->pfs[3], 9, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
}

void testeDezProcessos(){
	int num_procs = 10;
	uint32_t num_frames = 50;
	struct result * resultado = memvirt(num_procs, num_frames, "alunos/t03.txt");
	WHEN("Tenho dez processos, cada um com 200 referencias a paginas");
    IF("Tenho um número de quadros 50 ou seja, 5 por processo ( 50/10 )");
    THEN("Espero que o numero de faltas seja 74, 67, 71, 111, 77, 79, 75, 114, 86, 94");
    isEqual(resultado->pfs[0], 74, 1);
    isEqual(resultado->pfs[1], 67, 1);
    isEqual(resultado->pfs[2], 71, 1);
    isEqual(resultado->pfs[3], 111, 1);
    isEqual(resultado->pfs[4], 77, 1);
    isEqual(resultado->pfs[5], 79, 1);
    isEqual(resultado->pfs[6], 75, 1);
    isEqual(resultado->pfs[7], 114, 1);
    isEqual(resultado->pfs[8], 86, 1);
    isEqual(resultado->pfs[9], 94, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
    
    num_procs = 10;
	num_frames = 1000;
	resultado = memvirt(num_procs, num_frames, "alunos/t03.txt");
	WHEN("Tenho dez processos, cada um com 200 referencias a paginas");
    IF("Tenho um número de quadros 1000 ou seja, 100 por processo ( 1000/10 )");
    THEN("Espero que tenha apenas as primeiras faltas ao carregar para memória");
    isEqual(resultado->pfs[0], 9, 1);
    isEqual(resultado->pfs[1], 9, 1);
    isEqual(resultado->pfs[2], 9, 1);
    isEqual(resultado->pfs[3], 9, 1);
    isEqual(resultado->pfs[4], 9, 1);
    isEqual(resultado->pfs[5], 9, 1);
    isEqual(resultado->pfs[6], 9, 1);
    isEqual(resultado->pfs[7], 9, 1);
    isEqual(resultado->pfs[8], 9, 1);
    isEqual(resultado->pfs[9], 9, 1);
    imprimeResultado(resultado, num_procs);
    free(resultado);
}

int main(){
    DESCRIBE("Teste com apenas um processo");
    testeApenasUmProcesso();
    DESCRIBE("Teste com quatro processos");
    testeQuatroProcessos();
    DESCRIBE("Teste com dez processos");
    testeDezProcessos();
    
    GRADEME();
   	if (grade==maxgrade)
		return 0;
	else return grade;
}
