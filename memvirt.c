#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memvirt.h"

int MAX_ITENS;
 
typedef struct pagina{
	int bitReferencia;
	int valor;
} pagina;

typedef struct fila{
	int primeiro;
	int ultimo;
	pagina * elementos;
} fila;

int tam_fila(fila *q);
int adicionar(fila *q, int valor, struct result * resultado, int processo);
int remover(fila *q);
void destruir(fila *q);
void exibirFila(fila *q);

fila* criar(){
	fila *q = (fila*) malloc(sizeof (fila));
	q->elementos = (pagina *) malloc (MAX_ITENS * sizeof(pagina));
	q->primeiro = 0;
	q->ultimo = 0;
	for(int i=0; i<MAX_ITENS; i++){
		q->elementos[i].valor = -1;
		q->elementos[i].bitReferencia=0;
	}
	return q;
}

int tam_fila(fila *q){
	int tmp = q->ultimo - q->primeiro;
	if(tmp < 0){
		tmp = MAX_ITENS + tmp + 1;
	}
	return tmp;
}

//verifica se o elemento ja se encontra no array
int paginaCarregada(fila *q, int valor){
	for(int i=0; i < MAX_ITENS; i++){
		if(q->elementos[i].valor == valor){
			q->elementos[i].bitReferencia = 1; //garante que o bit de referencia sera 1
			return 1;
		}
	};
	return 0;
}

int adicionar(fila *q, int valor, struct result * resultado, int processo){
	int tmp = tam_fila(q);
	
	
	if(!paginaCarregada(q, valor)){ //se a pagina nao foi carregada 
		resultado->pfs[processo]++;	
		while(1){
			if(tmp == MAX_ITENS){
				if(q->primeiro == MAX_ITENS){
					q->primeiro = 0; //retorna ao inicio da fila circular
				}else{
					q->primeiro++;
				}
			}
			if((q->ultimo >= MAX_ITENS) && (q->primeiro > 0)){
				q->ultimo = 0; //retorno fila circular
			}
			if(q->elementos[q->ultimo].bitReferencia == 0){ 
				q->elementos[q->ultimo].valor = valor;
				q->elementos[q->ultimo].bitReferencia = 1;
				q->ultimo++;
				break;
			}
			
			q->elementos[q->ultimo].bitReferencia = 0;
			q->ultimo++; 
			if(q->ultimo >= MAX_ITENS){
				q->ultimo=0;
			}
		}
	}
	//exibirFila(q);
	return 0;
}

int remover(fila *q){
	int tmp = q->elementos[q->primeiro].valor;
	if(q->primeiro == MAX_ITENS){
		q->primeiro = 0; //retorna ao inicio da fila circular
	}else{
		q->primeiro++;
	}
	return tmp;
}

void destruir(fila *q){
	free(q);
}

/**
void exibirFila(fila *q){
	for(int i=0; i<MAX_ITENS; i++)
		printf("%d \t||\t%d\n", q->elementos[i].valor, q->elementos[i].bitReferencia);
	printf("\n");
}**/

struct result * inicializarResultado(struct result * resultado, int num_procs){
	resultado = (struct result *) malloc(sizeof(struct result));
	
	resultado -> refs = (uint32_t *) malloc (sizeof(uint32_t) * num_procs);
	resultado -> pfs = (uint32_t *) malloc (sizeof(uint32_t) * num_procs);
	resultado -> pf_rate = (float *) malloc (sizeof(float) * num_procs);
	
	for(int i=0; i<num_procs; i++){
			resultado->refs[i] = 0;
			resultado->pfs[i] = 0;
			resultado->pf_rate = 0;
	}
	return resultado;
}


struct result * memvirt(int num_procs, uint32_t num_frames, char * filename){
	if((!filename) || (num_procs == 0) || (num_frames == 0)){
		return NULL;
	}  
	
	if(num_procs > num_frames){
		return NULL;
	}
	
	MAX_ITENS = 0;
	struct result * resultado = NULL;
	
	MAX_ITENS = num_frames / num_procs;
	resultado = inicializarResultado(resultado, num_procs);
	
	int processo;
	int page;
	int unused __attribute__ ((unused));
	
	fila **array = (fila **) malloc(sizeof(fila *) * num_procs);
	for(int i=0; i < num_procs; i++){
		array[i] = (fila *) malloc(sizeof(fila) * MAX_ITENS);
		array[i] = criar();
	}
	FILE *arq = fopen(filename, "r");
	if (!arq) exit(1);
	while(!feof(arq)) {
		unused = fscanf(arq, "%d %d\n", &processo, &page);
		adicionar(array[processo], page, resultado, processo); 
		resultado->refs[processo]++; //incrementa a referencia a página por processo
	}
	
	for(int i=0; i<num_procs; i++){
		free(array[i]);
	}
	free(array);
	
	return resultado;
}
