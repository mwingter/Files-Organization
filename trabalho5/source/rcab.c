/*
	TRABALHO 5 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcab.h"

/*
 * criaRegCabecalho
 * Função: Lê o cabeçalho de um arquivo csv, e salva na struct de cabeçalho.
 * Parâmetros: 	
 				f: Arquivo csv
 				rc: Registro de cabeçalho
*/
void criaRegCabecalho(FILE* f, REGCAB* rc){
	char ch;
	int contaVirgula = 0;
	int count = 0;
	char* c1 = malloc(sizeof(char));
	char* c2 = malloc(sizeof(char));
	char* c3 = malloc(sizeof(char));
	char* c4 = malloc(sizeof(char));
	char* c5 = malloc(sizeof(char));

	while(contaVirgula < 5){
		ch = fgetc(f);
		if(ch == '\n'){
			break;
		}
		if(ch != ','){
			if(contaVirgula == 0){
				c1 = realloc(c1, (count+2)*sizeof(char));
				c1[count] = ch;
				c1[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 1){
				c2 = realloc(c2, (count+2)*sizeof(char));
				c2[count] = ch;
				c2[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 2){
				c3 = realloc(c3, (count+2)*sizeof(char));
				c3[count] = ch;
				c3[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 3){
				c4 = realloc(c4, (count+2)*sizeof(char));
				c4[count] = ch;
				c4[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 4){
				c5 = realloc(c5, (count+2)*sizeof(char));
				c5[count] = ch;
				c5[count+1] = '\0';
				count++;
			}
		}
		else if(ch == ','){
			contaVirgula++;
			count = 0;
		}
	}

	strcpy(rc->campos[0], c1);
	strcpy(rc->campos[1], c2);
	strcpy(rc->campos[2], c3);
	strcpy(rc->campos[3], c4);
	strcpy(rc->campos[4], c5);

	//completando o primeiro byte com \0
	for(int i = 0; i < 5; i++){
		int j = strlen(rc->campos[i]);
		rc->campos[i][j+1] = '\0';
	}

	//completando os vazios com '@'
	for(int j = 0; j < 5; j++){
		for(int i = strlen(rc->campos[j]) + 1; i < 40; i++){
			rc->campos[j][i] = '@';
		}
	}
	
	rc->tags[0] = 'i';
	rc->tags[1] = 's';
	rc->tags[2] = 't';
	rc->tags[3] = 'n';
	rc->tags[4] = 'c';

	rc->status = '1'; //Ao se abrir um arquivo para escrita, seu status deve ser 0 
					//e, ao finalizar o uso desse arquivo, seu status deve ser 1

	rc->topoLista = -1;

	free(c1);free(c2);free(c3);free(c4);free(c5);
}

/*
 * regCabToArqBin
 * Função: Escreve um registro de cabeçalho em um arquivo binário.
 * Parâmetros: 	
 				bin: Arquivo binário
 				c: Registro de cabeçalho
*/
void regCabToArqBin(REGCAB* c, FILE* bin){

	int tamRegistroCab = 214;//1+8+1+40+1+40+1+40+1+40+1+40
	int restoPagina = TAM_PAG_DISCO - tamRegistroCab;

	fwrite(&c->status, STATUS_TAM, 1, bin);

	fwrite(&c->topoLista, TOPO_TAM, 1, bin);

	fwrite(&c->tags[0], TAG_TAM, 1, bin);

	fwrite(&c->campos[0], CAMPO_TAM*sizeof(char), 1, bin);

	fwrite(&c->tags[1], TAG_TAM, 1, bin);

	fwrite(&c->campos[1], CAMPO_TAM*sizeof(char), 1, bin);

	fwrite(&c->tags[2], TAG_TAM, 1, bin);

	fwrite(&c->campos[2], CAMPO_TAM*sizeof(char), 1, bin);

	fwrite(&c->tags[3], TAG_TAM, 1, bin);

	fwrite(&c->campos[3], CAMPO_TAM*sizeof(char), 1, bin);

	fwrite(&c->tags[4], TAG_TAM, 1, bin);

	fwrite(&c->campos[4], CAMPO_TAM*sizeof(char), 1, bin);

	
	char arroba = '@';
	for(int i = 0; i < restoPagina; i++){ //preenchendo com @ o resto da pagina
		fwrite(&arroba, sizeof(char), 1, bin);
	}
}

/*
 * atualizaStatus
 * Função: Atualiza o status do cabeçalho de um arquivo binário para 1.
 * Parâmetros: 	
 				bin: Arquivo binário
 				rc: Registro de cabeçalho
*/
void atualizaStatus(REGCAB* rc, FILE* bin){
	strcpy(&rc->status, "1");
	fseek(bin, 0, SEEK_SET);
	fwrite(&rc->status, STATUS_TAM, 1, bin);
}

/*
 * leCabecalho
 * Função: Lê o cabeçalho de um arquivo binário e salva os dados na struct de cabeçalho.
 * Parâmetros: 	
 				bin: Arquivo binário
 				rc: Registro de cabeçalho
*/
void leCabecalho(FILE* bin, REGCAB *rc) {	
	fread(&rc->status, STATUS_TAM, 1, bin);

	if (rc->status == 0) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	fread(&rc->topoLista, TOPO_TAM, 1, bin);
	for (int i = 0; i < 5; i++) {
		fread(&rc->tags[i], TAG_TAM, 1, bin);
		fread(rc->campos[i], CAMPO_TAM, 1, bin);
	}
}