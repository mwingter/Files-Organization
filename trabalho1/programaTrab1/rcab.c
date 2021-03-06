#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcab.h"


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
	//printf("c5 = %s, e no rc fica = %s\n", c5, rc->campos[4]);

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

	//printf("\tREG CABEÇALHO: \n");
	//printf("\t%.40s\n\t%.40s\n\t%.40s\n\t%.40s\n\t%.40s\n\n", rc->campos[0], rc->campos[1], rc->campos[2], rc->campos[3], rc->campos[4]);
	
	rc->tags[0] = 'i';
	rc->tags[1] = 's';
	rc->tags[2] = 't';
	rc->tags[3] = 'n';
	rc->tags[4] = 'c';

	rc->status = '1'; //Ao se abrir um arquivo para escrita, seu status deve ser 0 
					//e, ao finalizar o uso desse arquivo, seu status deve ser 1

	rc->topoLista = -1;

	//status(1), topoLista(8), tag1(1), campo1(40), tag2(1), campo2(40),......., tag5(1), campo5(40) => totalBytes= 10+5+200 = 215
	//printf("|%c|%lf|%c|%s|%c|%s|%c|%s|%c|%s|%c|%s|\n", rc->status, rc->topoLista, rc->tags[0], rc->campos[0], rc->tags[1], rc->campos[1], rc->tags[2], rc->campos[2], rc->tags[3], rc->campos[3], rc->tags[4], rc->campos[4]);

	free(c1);free(c2);free(c3);free(c4);free(c5);

}

void regCabToArqBin(REGCAB* c, FILE* bin){

	int tamRegistroCab = 214;//1+8+1+40+1+40+1+40+1+40+1+40
	int restoPagina = 32000 - tamRegistroCab;

	fwrite(&c->status, sizeof(char), 1, bin);

	fwrite(&c->topoLista, sizeof(long int), 1, bin);

	fwrite(&c->tags[0], sizeof(char), 1, bin);

	fwrite(&c->campos[0], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[1], sizeof(char), 1, bin);

	fwrite(&c->campos[1], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[2], sizeof(char), 1, bin);

	fwrite(&c->campos[2], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[3], sizeof(char), 1, bin);

	fwrite(&c->campos[3], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[4], sizeof(char), 1, bin);

	fwrite(&c->campos[4], 40*sizeof(char), 1, bin);

	
	char arroba = '@';
	for(int i = 0; i < restoPagina; i++){ //preenchendo com @ o resto da pagina
		fwrite(&arroba, sizeof(char), 1, bin);
	}
}

void atualizaStatus(REGCAB* rc, FILE* bin){

}