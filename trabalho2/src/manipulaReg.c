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
	//printf("status = %c\n", status);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		return;
	}
	//=======================
	int tam_bin = tamArquivo(bin);
	//rewind(bin);
	//fseek(bin, TAM_PAG_DISCO, SEEK_CUR);
	
	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		int id = atoi(valorCampo);
		remove_id(bin, tam_bin, id, &achei);
		/*
		if(achei == 0){
			printf("Não achei o registro de ID |%d|\n", id);
		}
		else{
			printf("Achei o registro de ID |%d|\n", id);
		}*/
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
		printf("vou procurar o salario\n");
		double sal = atof(valorCampo);
		remove_salario(bin, tam_bin, sal, &achei);
		/*
		if(achei == 0){
			printf("Não achei o registro de Salario |%lf|\n", sal);
		}
		else{
			printf("Achei o registro de Salario |%lf|\n", sal);
		}*/
	}
	else if(strcmp(nomeCampo, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valorCampo);
		remove_telefone(bin, tam_bin, tel, &achei);
		/*
		if(achei == 0){
			printf("Não achei o registro de Salario |%lf|\n", sal);
		}
		else{
			printf("Achei o registro de Salario |%lf|\n", sal);
		}*/
	}
	else if(strcmp(nomeCampo,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valorCampo)+1, sizeof(char));
		strcpy(nome, valorCampo);
		remove_nome(bin, tam_bin, nome, &achei);
		/*
		if(achei == 0){
			printf("Não achei o registro de Salario |%lf|\n", sal);
		}
		else{
			printf("Achei o registro de Salario |%lf|\n", sal);
		}*/
		free(nome);

	}
	else if(strcmp(nomeCampo, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valorCampo)+1, sizeof(char));
		strcpy(cargo, valorCampo);
		remove_cargo(bin, tam_bin, cargo, &achei);
		/*
		if(achei == 0){
			printf("Não achei o registro de Salario |%lf|\n", sal);
		}
		else{
			printf("Achei o registro de Salario |%lf|\n", sal);
		}*/
		free(cargo);
	}
	/*
	if(achei > 0){
		printf("Listar o arquivo binário %s.", nomeBin);
	}*/
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
	//printf("Quero remover o id |%d|\n", id);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O ID Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro - ENC_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;
			
			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);

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
void remove_salario (FILE* bin, int tam_bin, double sal, int *achei){
	int lixo = 0;
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o salario |%lf|\n", sal);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->salarioServidor == sal){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O SAL Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro - ENC_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
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
void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei){
	int lixo = 0;
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o telefone |%s|\n", tel);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->telefoneServidor == tel){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O TEL Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro - ENC_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
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
void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei){
	int lixo = 0;
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o nome |%s|\n", nome);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->nomeServidor == nome){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O NOME Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro - ENC_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
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
void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei){
	int lixo = 0;
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o cargo |%s|\n", cargo);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->cargoServidor == cargo){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O CARGO Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			fwrite(&arroba, sizeof(char), rd->tamanhoRegistro - ENC_TAM, bin); //completando o registro com lixo
			//==========================
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
		}
		free(rd);
	}
}


//função que volta pro inicio do registro anterior (tem q estar no inicio de algum registro pra funcionar) e printa ele
void volta_le_Eprinta_umRegistro(FILE*bin, int tamReg){
	fseek(bin, -(tamReg + TAM_TAM + REM_TAM), SEEK_CUR);
	int lixo = 0;
	REGDADOS* rd = calloc(1, sizeof(REGDADOS));
	rd->nomeServidor[0] = '\0';
	rd->cargoServidor[0] = '\0';
	leUmRegistroBin(bin, rd, &lixo);
	printaUmReg(rd);
	free(rd);
}

//função que printa um registro
void printaUmReg(REGDADOS* rd){
	if(rd->removido == '-'){
		printf("==== PRINTANDO REGISTRO '-' =====\n");
		printf("REM: %c\nTamReg: %d, Encadeam: %ld, ID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", 
				rd->removido,rd->tamanhoRegistro ,rd->encadeamentoLista, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
		printf("=================================\n");	
	}
	else{
		printf("==== PRINTANDO REGISTRO '*' =====\n");
		printf("REM: %c\nTamReg: %d, Encadeam: %ld, ID: %d\nSalario: %lf\nTel: %s\n", 
				rd->removido,rd->tamanhoRegistro ,rd->encadeamentoLista, rd->idServidor, rd->salarioServidor, rd->telefoneServidor);
		printf("=================================\n");	
	}
}


//========== CODIGOS PARA INSERÇÃO ==============



void bestFit_insere(char* nomeBin, REGDADOS* rd){
	//=====================
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	char status = 0;
	fread(&status, STATUS_TAM, 1, bin);
	//printf("status = %c\n", status);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		return;
	}
	//=======================


}