#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "manipulaReg.h"

void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo){
	int achei = 0;
	//=====================
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	char status = 0;
	fread(&status, STATUS_TAM, 1, bin);
	printf("status = %c\n", status);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		return;
	}
	//=======================
	int tam_bin = tamArquivo(bin);
	rewind(bin);
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);
	
	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		int id = atoi(valorCampo);
		//busca_id(bin, tam_bin, id, &numPaginasAcessadas, &tam_pag, rc, &achei);
		remove_id(bin, tam_bin, id, &achei);
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
		//double sal = atof(valorCampo);
		//busca_salario(bin, tam_bin, sal, &numPaginasAcessadas, &tam_pag, rc, &achei);
		//remove_sal();
	}
	else if(strcmp(nomeCampo, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valorCampo);
		//busca_telefone(bin, tam_bin, tel, &numPaginasAcessadas, &tam_pag, rc, &achei);
	}
	else if(strcmp(nomeCampo,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valorCampo)+1, sizeof(char));
		strcpy(nome, valorCampo);
		//busca_nome(bin, tam_bin, nome, &numPaginasAcessadas, &tam_pag, rc, &achei);
		free(nome);

	}
	else if(strcmp(nomeCampo, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valorCampo)+1, sizeof(char));
		strcpy(cargo, valorCampo);
		//busca_cargo(bin, tam_bin, cargo, &numPaginasAcessadas, &tam_pag, rc, &achei);
		free(cargo);
	}
	
	if(achei > 0){
		printf("Listar o arquivo binário arquivo.bin.");
	}
	/*else{
		printf("Registro inexistente.\n");
	}*/

	fclose(bin);
}

/*
 * remove_id
 * Função: Busca dados do tipo idServidor em um dado arquivo binário e o remove
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				id: Valor do id que quero buscar
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_id (FILE* bin, int tam_bin, int id, int *achei){
	int lixo = 0;
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	REGDADOS* rd;
	printf("Quero remover o id |%d|\n", id);
	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){
			//====removendo registro====
			printf("tam reg eh %d\n", rd->tamanhoRegistro);
			printf("ACHEI O ID Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + TAG_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro + TAM_TAM + TAG_TAM - REM_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;
			free(rd);
			return;
		}
		free(rd);

	}
}

/*
 * remove_salario
 * Função: Busca dados do tipo salarioServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				sal: Valor do salario que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_salario (FILE* bin, int tam_bin, double sal, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(rd->salarioServidor == sal){
			//printf("cargoooo = %s, tam = %d\n", rd->cargoServidor, rd->tamCargoServidor);
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * remove_telefone
 * Função: Busca dados do tipo telefoneServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				tel: Numero do telefone que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_telefone (FILE* bin, int tam_bin, char tel[TEL_TAM], int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->telefoneServidor, tel) == 0){
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * remove_nome
 * Função: Busca dados do tipo nomeServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				nome: Nome que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_nome (FILE* bin, int tam_bin, char* nome, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->nomeServidor, nome) == 0){
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * remove_cargo
 * Função: Busca dados do tipo cargoServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				cargo: Cargo que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	//printf("CARGO A PESQUISARRRR : %s\n", cargo);
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->cargoServidor, cargo) == 0){
			//printa o registro encontrado
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
			break;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}