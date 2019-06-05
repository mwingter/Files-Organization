/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "organizaArq.h"

//========================== FUNCOES AUXILIARES ======================
/*
 * int_compare_id
 * Função: Compara dois valores de id.
 * Parâmetros: 	
 				A: valor 1
 				B: valor 2

 * Retorno: Resultado de B - A.
*/
int int_compare_id(const void *A, const void *B) { // para ordenar de forma crescente
	REGDADOS *pA, *pB;
	pA = (REGDADOS *) A;
	pB = (REGDADOS *) B;
	return pB->idServidor - pA->idServidor;
}

/*
 * recalcula_tam_registro
 * Função: Recalcula o tamanho de um dado registro de dados.
 * Parâmetros: 	
 				rd: Registro de dados

 * Retorno: Novo tamanho do registro de dados.
*/
int recalcula_tam_registro(REGDADOS *rd){
	int tam = ENC_TAM + ID_TAM + SAL_TAM + TEL_TAM;
	if(rd->nomeServidor[0] != '\0'){
		tam += rd->tamNomeServidor - 1;
		tam += TAG_TAM;
		tam += TAM_TAM;
	}
	if(rd->cargoServidor[0] != '\0'){
		tam += rd->tamCargoServidor - 1;
		tam += TAG_TAM;
		tam += TAM_TAM;
	}
	return tam;
}

/*
 * avancaProximo
 * Função: Lê um registro de um dos dois arquivos de dados recebidos (o registro que for menor será 
 			substituido pela nova leitura no arquivo correspondente, ou seja, se o registro 1 for menor, 
 			ele será lido do arquivo 1, se o 2 for menor será lido do arquivo 2).

 * Parâmetros: 	
 				rd1: Registro de dados 1
 				rd2: Registro de dados 2
 				bin_in1: Arquivo binário de dados 1
 				bin_in2: Arquivo binário de dados 2
 				tam_pagina1: Ponteiro para controlar o tamanho da pagina do arquivo 1
 				tam_pagina2: Ponteiro para controlar o tamanho da pagina do arquivo 2
*/
void avancaProximo(REGDADOS *rd1, REGDADOS *rd2, FILE *bin_in1, FILE *bin_in2, int *tam_pagina1, int *tam_pagina2){
	if(rd1->idServidor < rd2->idServidor){
		free(rd1);
		rd1 = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin_in1, rd1, tam_pagina1);
	}
	else if(rd1->idServidor > rd2->idServidor){
		free(rd2);
		rd2 = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin_in2, rd2, tam_pagina2);
	}
}

/*
 * check_file_status
 * Função: Checa se um dado arquivo binário de dados possui falhas de processamento. Se houverem falhas, imprime "Falha no processamento do arquivo" e fecha o programa.

 * Parâmetros: 	
 				bin: Arquivo binário de dados.
*/
void check_file_status(FILE *bin){
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}
	char status;
	fread(&status, STATUS_TAM, 1, bin);
	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		exit(0);
	}
}

//============================= ORDENAÇÃO INTERNA ===========================
/*
 * ordena_por_id
 * Função: Ordena um arquivo binário, reescrevendo-o ordenado em um novo arquivo.

 * Parâmetros: 	
 				nomeBin_in: Nome do arquivo binário desordenado dado.
 				nomeBin_out: Nome do arquivo binário ordenado que será criado.
*/
void ordena_por_id(char *nomeBin_in, char *nomeBin_out){
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);

	int tam_bin_in = tamArquivo(bin_in);
	REGCAB* rc = calloc(1, sizeof(REGCAB));
	REGDADOS* lista = NULL;

	//lendo o registro de cabeçalho e salvando numa struct
	leCabecalho(bin_in, rc);
	rc->topoLista = -1; //se tiver algum removido, ele não será passado para o arquivo ordenado, então topoLista será -1


	fseek(bin_in, TAM_PAG_DISCO, SEEK_SET);

	int n_reg = 0; //numero de registros na lista
	int tam_pagina = 0; //a função q le um registro do arquivo e salva na struct precisa desse parâmetro, mas aqui não será necessário
	int tam_ult_registro = 0;
	char removido = '@';
	int tam_reg_aux = 0;

	//percorrendo o arquivo e montando a lista de structs de registros de dados
	while(ftell(bin_in) < tam_bin_in){
		n_reg++;
		lista = realloc(lista, n_reg * sizeof(REGDADOS));
		lista[n_reg - 1].nomeServidor[0] = '\0';
		lista[n_reg - 1].cargoServidor[0] = '\0';
		lista[n_reg - 1].idServidor = -1;

		fread(&removido,REM_TAM,1,bin_in);
		fread(&tam_reg_aux,TAM_TAM,1,bin_in);
		fseek(bin_in, - (REM_TAM + TAM_TAM), SEEK_CUR);

		//lendo um registro do arquivo dado
		leUmRegistroBin(bin_in, &lista[n_reg - 1], &tam_pagina);
		tam_ult_registro = recalcula_tam_registro(&lista[n_reg - 1]);
		lista[n_reg - 1].tamanhoRegistro = tam_ult_registro;

		if(lista[n_reg - 1].idServidor == -1){
			n_reg--;
		}

		if(tam_pagina >= TAM_PAG_DISCO){ //se for o ultimo registro da pagina
			tam_pagina = tam_ult_registro;
		}
	}

	//ordenando pelo id a lista de structs de registros
	MS_sort(lista, n_reg, sizeof(REGDADOS), int_compare_id);

	//passando os registros ordenados pro novo arquivo
	FILE* bin_out = fopen(nomeBin_out, "wb");
	tam_pagina = 32000;
	int tamRegAnt = 0;

	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo
	
	//passando os registros de dados para o novo arquivo
	for (int i = 0; i < n_reg; ++i){
		structToBin(&tam_pagina, &tamRegAnt, &lista[i], rc, bin_out);
	}

	fclose(bin_in);
	fclose(bin_out);
	free(rc); free(lista);
}

//========================== MERGING DE DOIS ARQUIVOS ======================
/*
 * read_andMerge
 * Função: Realiza a união de dois arquivos binários dados, considerando o campo idServidor, e cria um novo arquivo com essa união.

 * Parâmetros: 	
 				nomeBin_in1: Nome do arquivo binário 1 dado.
 				nomeBin_in2: Nome do arquivo binário 2 dado.
 				nomeBin_out: Nome do arquivo binário criado com a união dos arquivos 1 e 2.
*/
void read_andMerge(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
	FILE* bin_in1 = fopen(nomeBin_in1, "rb");
	FILE* bin_in2 = fopen(nomeBin_in2, "rb");
	FILE* bin_out = fopen(nomeBin_out, "wb");
	check_file_status(bin_in1);
	check_file_status(bin_in2);

	int tam_bin_in1 = tamArquivo(bin_in1);
	int tam_bin_in2 = tamArquivo(bin_in2);
	REGCAB* rc = calloc(1, sizeof(REGCAB));
	REGCAB* rc2 = calloc(1, sizeof(REGCAB));

	//lendo o registro de cabeçalho e salvando numa struct
	leCabecalho(bin_in1, rc);
	leCabecalho(bin_in2, rc2);
	
	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo

	//lendo os registros de dados e passando para o arquivo de saida os registros que existem nos dois arquivos de entrada (match)
	fseek(bin_in1, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina
	fseek(bin_in2, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina

	REGDADOS* rd1 = NULL;
	REGDADOS* rd2 = NULL;

	int is_rd1_val;
	int is_rd2_val;
	int tamRegAnt = 0;
	int tam_pagina1 = 0;
	int tam_pagina2 = 0;
	int	tam_pagina_out = 32000;

	rd1 = calloc(1, sizeof(REGDADOS));
	is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);

	rd2 = calloc(1, sizeof(REGDADOS));
	is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
	
	//percorrendo o arquivo e montando a rd de structs de registros de dados
	while((ftell(bin_in1) < tam_bin_in1) || (ftell(bin_in2) < tam_bin_in2)){

		if(rd1->idServidor < rd2->idServidor){
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
			free(rd1);
			rd1 = calloc(1, sizeof(REGDADOS));
			is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);
		}
		else if(rd1->idServidor > rd2->idServidor){
			rd2->tamanhoRegistro = recalcula_tam_registro(rd2);
			structToBin(&tam_pagina_out, &tamRegAnt, rd2, rc, bin_out);
			free(rd2);
			rd2 = calloc(1, sizeof(REGDADOS));
			is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
		}
		else{ //if rd1 == rd2
			if(rd1->removido == '*'){
				continue;
			}
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
			
			free(rd2);
			rd2 = calloc(1, sizeof(REGDADOS));
			is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);

			free(rd1);
			rd1 = calloc(1, sizeof(REGDADOS));
			is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);
		}
	}

	if(!is_rd1_val && !is_rd2_val){
		
		if(rd1->idServidor == rd2->idServidor){
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
		}
		else if (rd1->idServidor < rd2->idServidor){
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
			rd2->tamanhoRegistro = recalcula_tam_registro(rd2);
			structToBin(&tam_pagina_out, &tamRegAnt, rd2, rc, bin_out);	
		}
		else{
			rd2->tamanhoRegistro = recalcula_tam_registro(rd2);
			structToBin(&tam_pagina_out, &tamRegAnt, rd2, rc, bin_out);	
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
		}
	}
 
	fclose(bin_in1);fclose(bin_in2);fclose(bin_out);
	free(rc); free(rc2);free(rd1); free(rd2);
}

//========================== MATCHING DE DOIS ARQUIVOS ======================
/*
 * read_andMatch
 * Função: Realiza a intersecção de dois arquivos binários dados, considerando o campo idServidor, e cria um novo arquivo com essa intersecção.

 * Parâmetros: 	
 				nomeBin_in1: Nome do arquivo binário 1 dado.
 				nomeBin_in2: Nome do arquivo binário 2 dado.
 				nomeBin_out: Nome do arquivo binário criado com a intersecção dos arquivos 1 e 2.
*/
void read_andMatch(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
	FILE* bin_in1 = fopen(nomeBin_in1, "rb");
	FILE* bin_in2 = fopen(nomeBin_in2, "rb");
	FILE* bin_out = fopen(nomeBin_out, "wb");
	check_file_status(bin_in1);
	check_file_status(bin_in2);

	int tam_bin_in1 = tamArquivo(bin_in1);
	int tam_bin_in2 = tamArquivo(bin_in2);
	REGCAB* rc = calloc(1, sizeof(REGCAB));
	REGCAB* rc2 = calloc(1, sizeof(REGCAB));

	//lendo o registro de cabeçalho e salvando numa struct
	leCabecalho(bin_in1, rc);
	leCabecalho(bin_in2, rc2);
	
	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo

	//lendo os registros de dados e passando para o arquivo de saida os registros que existem nos dois arquivos de entrada (match)
	fseek(bin_in1, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina
	fseek(bin_in2, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina

	REGDADOS* rd1 = NULL;
	REGDADOS* rd2 = NULL;

	int is_rd1_val;
	int is_rd2_val;
	int tamRegAnt = 0;
	int tam_pagina1 = 0;
	int tam_pagina2 = 0;
	int	tam_pagina_out = 32000;

	rd1 = calloc(1, sizeof(REGDADOS));
	is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);

	rd2 = calloc(1, sizeof(REGDADOS));
	is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
	
	//percorrendo o arquivo e montando a rd de structs de registros de dados
	while((ftell(bin_in1) < tam_bin_in1) || (ftell(bin_in2) < tam_bin_in2)){

		if(rd1->idServidor < rd2->idServidor){

			free(rd1);
			rd1 = calloc(1, sizeof(REGDADOS));
			is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);
		}
		else if(rd1->idServidor > rd2->idServidor){
			free(rd2);
			rd2 = calloc(1, sizeof(REGDADOS));
			is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
		}
		else{ //if rd1 == rd2
			if(rd1->removido == '*'){
				continue;
			}
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
			
			free(rd2);
			rd2 = calloc(1, sizeof(REGDADOS));
			is_rd2_val = leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
			free(rd1);
			rd1 = calloc(1, sizeof(REGDADOS));
			is_rd1_val = leUmRegistroBin(bin_in1, rd1, &tam_pagina1);
		}
			
	}
	if(!is_rd1_val && !is_rd2_val){
		if(rd1->idServidor == rd2->idServidor){
			rd1->tamanhoRegistro = recalcula_tam_registro(rd1);
			structToBin(&tam_pagina_out, &tamRegAnt, rd1, rc, bin_out);
		}
	}
 
	fclose(bin_in1);fclose(bin_in2);fclose(bin_out);
	free(rc); free(rc2);free(rd1); free(rd2);
}