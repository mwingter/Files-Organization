/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "organizaArq.h"



//============================= ORDENAÇÃO INTERNA ===========================
void ordena_por_id(char *nomeBin_in, char *nomeBin_out){
	//=====================
	FILE* bin_in = fopen(nomeBin_in, "rb");
	if(bin_in == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status;
	fread(&status, STATUS_TAM, 1, bin_in);
	//printf("status = %c\n", status);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin_in);
		exit(0);
	}
	//=======================
	int tam_bin_in = tamArquivo(bin_in);
	//lendo o registro de cabeçalho e salvando numa struct
	REGCAB* rc = calloc(1, sizeof(REGCAB));
	criaRegCabecalho(bin_in, rc);
	rc->topoLista = -1; //se tiver algum removido, ele não será passado para o arquivo ordenado, então topoLista será -1


	fseek(bin_in, TAM_PAG_DISCO, SEEK_SET);

	REGDADOS* lista = NULL;
	int n_reg = 0; //numero de registros na lista
	int tam_pagina = 0; //a função q le um registro do arquivo e salva na struct precisa desse parâmetro, mas aqui não será necessário

	//percorrendo o arquivo e montando a lista de structs de registros
	while(ftell(bin_in) != tam_bin_in){
		n_reg++;
		lista = realloc(lista, n_reg * sizeof(REGDADOS));
		leUmRegistroBin(bin_in, &lista[n_reg - 1], &tam_pagina);
	}

	//ordenando pelo id a lista de structs de registros
	MS_sort(lista, n_reg, sizeof(REGDADOS), int_compare_id);

	//passando os registros ordenados pro novo arquivo
	FILE* bin_out = fopen(nomeBin_out, "wb");
	tam_pagina = 32000;
	int tamRegAnt = 0;

	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo

	for(int i = 0; i < n_reg; i++){ //passando os registros de dados para o novo arquivo
		structToBin(&tam_pagina, &tamRegAnt, &lista[i], rc, bin_out);
	}



	fclose(bin_in);
	fclose(bin_out);
}

//========================== MERGING DE DOIS ARQUIVOS ======================
void read_andMerge(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
	//========= checando arquivo 1 ============
	FILE* bin_in1 = fopen(nomeBin_in1, "rb");
	fileCheck(bin_in1);

	FILE* bin_in2 = fopen(nomeBin_in2, "rb");
	fileCheck(bin_in2);
	
	FILE* bin_out = fopen(nomeBin_out, "wb");
	fileCheck(bin_out);

	REGCAB* rc = calloc(1, sizeof(REGCAB));
	REGDADOS *rd1 = NULL;
	REGDADOS *rd2 = NULL;


	leCabecalho(nomeBin_in1, rc);
	regCabToArqBin(rc, bin_out);

	do{
		rd1 = pegarProximoRegistro(bin_in1);
		rd2 = pegarProximoRegistro(bin_in2);
		pegaRegistroNoOffset

		if(rd1.idServidor > rd2.idServidor && rd1 && rd2){
			REGDADOS *aux = rd2;
			rd2 = rd1;
			rd1 = aux;
		}

		escreveRegDados(bin_out, rd1);
		if(rd1.idServidor != rd2.idServidor) escreveRegDados(bin_out, rd2);

		if (rd1) free(rd1);
		if (rd2) free(rd2);
	}
	while(rd1 || rd2);

	free(rc);
	fclose(bin_in1);
	fclose(bin_in2);
	fclose(bin_out);
}

//========================== MATCHING DE DOIS ARQUIVOS ======================
void read_andMatch(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
}

//========================== FUNCOES AUXILIARES =============================

int int_compare_id(const void *A, const void *B) { // para ordenar de forma crescente
	REGDADOS *pA, *pB;
	pA = (REGDADOS *) A;
	pB = (REGDADOS *) B;
	return pB->idServidor - pA->idServidor;
}

void fileCheck(FILE *fp){
	if(fp == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status;
	fread(&status, STATUS_TAM, 1, fp);
	//printf("status = %c\n", status);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(fp);
		exit(0);
	}
}

REGDADOS *pegarProximoRegistro(FILE *fp){
	if(feof(fp)) return NULL;

}

void escreveRegDados(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin){
	if (r->idServidor == -1){
		return;
	}

	int aux = r->tamanhoRegistro +  5;

	if((*tamPagina - aux) < 0){
		char arroba = '@';
		
		for(int i = 0; i < *tamPagina; i++){
			fwrite(&arroba, sizeof(char), 1, bin);
			(*tamRegAnterior) = (*tamRegAnterior) + 1;
		}

		//adicionar o tamanho desses arrobas no ultimo registro da pagina
		fseek(bin, -(*tamRegAnterior + 4), SEEK_CUR);
		fwrite(&(*tamRegAnterior), sizeof(int), 1, bin);

		fseek(bin, 0, SEEK_END);

		*tamPagina = 32000;
	}

	*tamPagina -= aux;

	fwrite(&r->removido, REM_TAM, 1, bin);
	fwrite(&r->tamanhoRegistro, TAM_TAM, 1, bin);
	fwrite(&r->encadeamentoLista, ENC_TAM, 1, bin);
	fwrite(&r->idServidor, ID_TAM, 1, bin);
	fwrite(&r->salarioServidor, SAL_TAM, 1, bin);
	fwrite(&r->telefoneServidor, TEL_TAM * sizeof(char), 1, bin);

	if(r->nomeServidor[0] != '\0'){
		int tamanho2 = strlen(r->nomeServidor) + 2;
		fwrite(&tamanho2, TAM_TAM, 1, bin);
		fwrite(&c->tags[3], TAG_TAM, 1, bin);
		fwrite(&r->nomeServidor, strlen(r->nomeServidor) * sizeof(char) + 1, 1, bin);
	}

	if(r->cargoServidor[0] != '\0'){
		int tamanho = strlen(r->cargoServidor) + 2;
		fwrite(&tamanho, TAM_TAM, 1, bin);
		fwrite(&c->tags[4], TAG_TAM, 1, bin);
		fwrite(&r->cargoServidor, strlen(r->cargoServidor)  * sizeof(char) + 1, 1, bin);
	}

	if(*tamPagina == 0){
		*tamPagina = 32000;
	}
	
	(*tamRegAnterior) = r->tamanhoRegistro;
}
}