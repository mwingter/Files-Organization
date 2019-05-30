/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					xxxxxxxx

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
	leCabecalho(bin_in, rc);
	rc->topoLista = -1; //se tiver algum removido, ele não será passado para o arquivo ordenado, então topoLista será -1


	fseek(bin_in, TAM_PAG_DISCO, SEEK_SET);

	REGDADOS* lista = NULL;
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

int int_compare_id(const void *A, const void *B) { // para ordenar de forma crescente
	REGDADOS *pA, *pB;
	pA = (REGDADOS *) A;
	pB = (REGDADOS *) B;
	return pB->idServidor - pA->idServidor;
}

int recalcula_tam_registro(REGDADOS *rd){
	//printf("\nRecalcula tam registro\n");
	//printf("nome = %s\n", rd->nomeServidor);
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

//========================== MERGING DE DOIS ARQUIVOS ======================
void read_andMerge(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
	//========= checando arquivo 1 ============
	FILE* bin_in1 = fopen(nomeBin_in1, "rb");
	if(bin_in1 == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status1;
	fread(&status1, STATUS_TAM, 1, bin_in1);
	//printf("status = %c\n", status);

	if (status1 == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin_in1);
		exit(0);
	}
	//========= checando arquivo 2 ============
	FILE* bin_in2 = fopen(nomeBin_in2, "rb");
	if(bin_in2 == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status2;
	fread(&status2, STATUS_TAM, 1, bin_in2);
	//printf("status = %c\n", status);

	if (status2 == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin_in2);
		exit(0);
	}
	//========================================

	//merge dos dois arquivos de entrada para o arquivo de saida


}



//========================== MATCHING DE DOIS ARQUIVOS ======================
void read_andMatch(char *nomeBin_in1, char *nomeBin_in2, char *nomeBin_out){
	//========= checando arquivo 1 ============
	FILE* bin_in1 = fopen(nomeBin_in1, "rb");
	if(bin_in1 == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status1;
	fread(&status1, STATUS_TAM, 1, bin_in1);
	//printf("status = %c\n", status);

	if (status1 == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin_in1);
		exit(0);
	}
	//========= checando arquivo 2 ============
	FILE* bin_in2 = fopen(nomeBin_in2, "rb");
	if(bin_in2 == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status2;
	fread(&status2, STATUS_TAM, 1, bin_in2);
	//printf("status = %c\n", status);

	if (status2 == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin_in2);
		exit(0);
	}
	//======== abrindo arquivo de saída ===========
	FILE* bin_out = fopen(nomeBin_out, "wb");

	int tam_bin_in1 = tamArquivo(bin_in1);
	int tam_bin_in2 = tamArquivo(bin_in2);

	//lendo o registro de cabeçalho e salvando numa struct
	REGCAB* rc = calloc(1, sizeof(REGCAB));
	REGCAB* rc2 = calloc(1, sizeof(REGCAB));
	leCabecalho(bin_in1, rc);
	leCabecalho(bin_in2, rc2);
	
	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo


	//lendo os registros de dados e passando para o arquivo de saida os registros que existem nos dois arquivos de entrada (match)
	fseek(bin_in1, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina
	fseek(bin_in2, TAM_PAG_DISCO, SEEK_SET); //pulando pra segunda pagina

	REGDADOS* rd1 = NULL;
	REGDADOS* rd2 = NULL;
	//int n_reg = 0; //numero de registros na rd
	int tam_pagina1 = 0;
	int tam_pagina2 = 0;
	//int tam_ult_registro1 = 0;
	//int tam_ult_registro2 = 0;
	//char removido = '@';
	//int tam_reg_aux = 0;

	int is_rd1_val;
	int is_rd2_val;

	int	tam_pagina_out = 32000;
	int tamRegAnt = 0;

	rd1 = calloc(1, sizeof(REGDADOS));
	leUmRegistroBin(bin_in1, rd1, &tam_pagina1);

	rd2 = calloc(1, sizeof(REGDADOS));
	leUmRegistroBin(bin_in2, rd2, &tam_pagina2);
	
	//percorrendo o arquivo e montando a rd de structs de registros de dados
	while((ftell(bin_in1) < tam_bin_in1) || (ftell(bin_in2) < tam_bin_in2)){
		//printf("comparando %d com %d\n", rd1->idServidor, rd2->idServidor);

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
			//printf("deu match!\n");
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