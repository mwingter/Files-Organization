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
	//criaRegCabecalho(bin_in, rc);
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
	//printf("ANTES DE ORDENAR\n");
	while(ftell(bin_in) < tam_bin_in){
		n_reg++;
		lista = realloc(lista, n_reg * sizeof(REGDADOS));
		lista[n_reg - 1].nomeServidor[0] = '\0';
		lista[n_reg - 1].cargoServidor[0] = '\0';
		lista[n_reg - 1].idServidor = -1;
		fread(&removido,REM_TAM,1,bin_in);
		//printf("removido = %c\n", removido);
		fread(&tam_reg_aux,TAM_TAM,1,bin_in);
		fseek(bin_in, - (REM_TAM + TAM_TAM), SEEK_CUR);
		//if(removido == '-'){
			leUmRegistroBin(bin_in, &lista[n_reg - 1], &tam_pagina);
			//printaUmReg(&lista[n_reg - 1]);
			tam_ult_registro = recalcula_tam_registro(&lista[n_reg - 1]);
			//printf("\tantes tam = %d\n\t depois tam_ult_registro = %d\n", lista[n_reg - 1].tamanhoRegistro, tam_ult_registro);
			lista[n_reg - 1].tamanhoRegistro = tam_ult_registro;
			if(lista[n_reg - 1].idServidor == -1){
				//lista[n_reg - 1] = NULL;
				n_reg--;
			}

			if(tam_pagina >= TAM_PAG_DISCO){ //se for o ultimo registro da pagina
				//printf("tam pagina (%d) >= TAM_PAG_DISCO\n", tam_pagina);
				tam_pagina = tam_ult_registro;
			}
		//}
		/*else{
			if(removido == '*'){
				fseek(bin_in, tam_reg_aux, SEEK_CUR);
				tam_pagina += tam_reg_aux;
				if(tam_pagina >= TAM_PAG_DISCO){
					tam_pagina = 0;
				}			
			}
			else{
				while(!feof(bin_in) && fgetc(bin_in) != '-'){
					continue;
				}
				if (!feof(bin_in)) {
					fseek(bin_in, -1, SEEK_CUR);
				}
			}
		}*/

	}

	//ordenando pelo id a lista de structs de registros
	MS_sort(lista, n_reg, sizeof(REGDADOS), int_compare_id);

	//passando os registros ordenados pro novo arquivo
	FILE* bin_out = fopen(nomeBin_out, "wb");
	tam_pagina = 32000;
	int tamRegAnt = 0;

	regCabToArqBin(rc, bin_out); //passando o registro de cabeçalho para o novo arquivo
	

	//printf("\nDEPOIS DE ORDENAR\n");
	for (int i = 0; i < n_reg; ++i)
	{
		//printaUmReg(&lista[i]);
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
	//========================================

}
