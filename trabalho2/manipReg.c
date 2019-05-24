#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "manipulaReg.h"
#include "merge_sort.h"

//================ CODIGOS PARA REMOÇÃO =====================

void novo_no_removido(REG_REM* reg_vazio, long int pos, int tam, long int proxRem){
	reg_vazio->pos = pos;
	reg_vazio->tam = tam;
	reg_vazio->encadeamentoLista = proxRem;
}


//Função que percorre o arquivo e coloca na lista todos os registros com removido = '*', e retorna o valor do topoLista (posição do registro de menor tamanho)
REG_REM* lista_removidos(FILE* bin, REG_REM* lista, int* numero_registros_removidos){
//	printf("entrando em lista_removidos\n");
	int tam_bin = tamArquivo(bin); //tamanho do arquivo
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR); //fui para a pagina 2, onde começam os registro de dados
	char removido;
	int n_regs = 0;
	int tam_aux = 0;
	long int pos = 0;
	long int proxRem = 0;
	lista = calloc(1, sizeof(REG_REM));

	while(ftell(bin) < tam_bin){
//		printf("bin na posição: %ld\n", ftell(bin));
		fread(&removido, REM_TAM, 1, bin);
		fread(&tam_aux, TAM_TAM, 1, bin);
		fread(&proxRem, ENC_TAM, 1, bin);
	//	printf("LENDO : %c %d %ld\n", removido, tam_aux, proxRem);
		if(removido == '*'){
			n_regs++;
			pos = ftell(bin) - ENC_TAM - TAM_TAM - REM_TAM; //posição do inicio desse arquivo, ou seja, no encadeamentoLista
			//printf("\t\t +++ POS = %ld\n", pos);
			lista = realloc(lista, n_regs*sizeof(REG_REM));
			novo_no_removido(&lista[n_regs-1], pos, tam_aux, proxRem);
		}
		fseek(bin, tam_aux - ENC_TAM, SEEK_CUR);
	}
	/*
	printf("\tVETOR DE TAM ANTES DE ORDENAR\n");
	for (int i = 0; i < n_regs; ++i)
	{
		printf("[TAM = %d, POS = %ld, ENC = %ld]\n", lista[i].tam, lista[i].pos, lista[i].encadeamentoLista);
	}
	printf("\n");
	*/

	//ordenando a lista de registros de removidos
	//MS_sort(lista, n_regs, sizeof(REG_REM), int_compare_pos_reverse);

	//MS_sort(lista, n_regs, sizeof(REG_REM), int_compare_tam);

	/*
	printf("\tVETOR DE TAM DEPOIS\n");
	for (int i = 0; i < n_regs; ++i)
	{
		printf("%d, ", lista[i].tam);
	}
	printf("\n");
	*/

	*numero_registros_removidos = n_regs;

	if(n_regs > 0){
		return lista;
	}

	free(lista);
	return NULL;
}

int int_compare_tam(const void *A, const void *B) { // 1, 2, 2, 3, 4
	REG_REM **pA, **pB;
	pA = (REG_REM **) A;
	pB = (REG_REM **) B;
	return (*pB)->tam - (*pA)->tam;
}

int int_compare_pos_reverse(const void *A, const void *B) { // 4, 3, 2, 2, 1
	REG_REM **pA, **pB;
	pA = (REG_REM **) A;
	pB = (REG_REM **) B;
	return (*pA)->pos - (*pB)->pos;
}

int int_compare_tempo_reverse(const void *A, const void *B) { // 4, 3, 2, 2, 1
	REG_REM **pA, **pB;
	pA = (REG_REM **) A;
	pB = (REG_REM **) B;
	return (*pA)->tempo_descoberta - (*pB)->tempo_descoberta;
}


void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo, LISTA_REM *lista){
	int achei = 0;
	//=================================
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}

	char status;
	fread(&status, STATUS_TAM, 1, bin);
	//printf("status = %c\n", status);
	//long int topoLista = 0;
	//fread(&topoLista, TOPO_TAM, 1, bin);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		exit(0);
	}
	//===================================
	int tam_bin = tamArquivo(bin);
	//rewind(bin);
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);


	
	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		int id = atoi(valorCampo);
		remove_id(bin, tam_bin, id, &achei, lista);
		
		/*
		if(achei == 0){
			printf("Não achei o registro de ID |%d|\n", id);
		}
		else{
			printf("Achei o registro de ID |%d|\n", id);
		}*/
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
	//	printf("vou procurar o salario\n");
		double sal = atof(valorCampo);
		remove_salario(bin, tam_bin, sal, &achei, lista);
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
		remove_telefone(bin, tam_bin, tel, &achei, lista);
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
		remove_nome(bin, tam_bin, nome, &achei, lista);
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
		remove_cargo(bin, tam_bin, cargo, &achei, lista);
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

	//int n_rem = 0;
	//REG_REM* listaRem = NULL;
	//long int topo = -1;
//	printf("achei = %d\n", achei);

		//listando os removidos
	//listaRem = lista_removidos(bin, listaRem, &n_rem);
		//printf("tamanho da lista = %d\n", n_rem);
	//long int topo = listaRem[0].pos;


	fclose(bin);

	ordenaLista_atualizaEncAndTopoLista(nomeBin, lista, lista->n_rem);
	
	/*if(listaRem != NULL){
		//atualizando topoLista
		atualiza_topoLista(nomeBin, topo);

		//atualizando o encadeamentoLista
		atualiza_encadLista(nomeBin, listaRem, n_rem);

		free(listaRem);	
	}*/
	//free(listaRem);

	//ordenando a lista de registros de removidos
	//MS_sort(lista, n_regs, sizeof(REG_REM), int_compare_pos_reverse);

	//MS_sort(lista, n_regs, sizeof(REG_REM), int_compare_tam);

}

void ordenaLista_atualizaEncAndTopoLista(char* nomeBin, LISTA_REM* lista, int n_rem){
//	printf("Entrando em ordenaLista_atualizaEncAndTopoLista, n_rem: %d\n", n_rem);

	//ordenando a lista de registros de removidos
	MS_sort(lista->vetor_removidos, lista->n_rem, sizeof(REG_REM*), int_compare_tempo_reverse);

	MS_sort(lista->vetor_removidos, lista->n_rem, sizeof(REG_REM*), int_compare_pos_reverse);

	MS_sort(lista->vetor_removidos, lista->n_rem, sizeof(REG_REM*), int_compare_tam);

	long int topo = lista->vetor_removidos[0]->pos;

	if(lista != NULL){
		//atualizando topoLista
		atualiza_topoLista(nomeBin, topo);

		//atualizando o encadeamentoLista
		atualiza_encadLista(nomeBin, lista->vetor_removidos, lista->n_rem);

		//free(lista);	
	}
}

void atualiza_topoLista(char* nomeBin, long int topo){
	FILE* bin = fopen(nomeBin, "rb+");

	//printf("-----novo topoooo = %ld\n", topo);
	fseek(bin, sizeof(char), SEEK_CUR);
	fwrite(&topo, TOPO_TAM, 1, bin);

	fclose(bin);
}

void atualiza_encadLista(char* nomeBin, REG_REM** lista, int n_rem){
	FILE* bin = fopen(nomeBin, "rb+");
	
	long int encadLista = -1;

	for (int i = 1; i < n_rem; ++i)
	{
		fseek(bin, lista[i-1]->pos + REM_TAM + TAM_TAM, SEEK_CUR); //pulando até o registro removido que quero atualizar o encadeamento
	
		encadLista = lista[i]->pos;// + REM_TAM + TAM_TAM; //posição do proximo menor registro removido
		//printf("-----novo encadeamento = %ld\n", encadLista);
		fwrite(&encadLista, ENC_TAM, 1, bin);

		rewind(bin);
	}

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
 				funcionalidade: indica se a busca é para 4 - remoção ou 6 - atualização
*/
void remove_id (FILE* bin, int tam_bin, int id, int *achei, LISTA_REM* lista/*, int funcionalidade*/){
	int lixo = 0;
	REGDADOS* rd;
	REG_REM* registro_rem = NULL;
	//printf("Quero remover o id |%d|\n", id);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){
			(*achei) = 1;
			registro_rem = calloc(1, sizeof(REG_REM));
			registro_rem->pos = ftell(bin);
			registro_rem->encadeamentoLista = -1;
			registro_rem->tam = rd->tamanhoRegistro;
			remove_registro_generico(bin, rd->tamanhoRegistro, lista);

			free(rd);
			return;
		}
		free(rd);
	}
	return;
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
void remove_salario (FILE* bin, int tam_bin, double sal, int *achei, LISTA_REM* lista){
	int lixo = 0;
	//char removido = '*';
	//long int encadeamento = -1;
	//char arroba = '@';
	REGDADOS* rd;

	//printf("Quero remover o salario |%lf|\n", sal);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->salarioServidor == sal){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O SAL Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
		*/	//==========================
			remove_registro_generico(bin, rd->tamanhoRegistro, lista);
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
void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei, LISTA_REM* lista){
	int lixo = 0;
//	char removido = '*';
//	long int encadeamento = -1;
//	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o telefone |%s|\n", tel);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->telefoneServidor,tel) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O TEL Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
			*///==========================
			remove_registro_generico(bin, rd->tamanhoRegistro, lista);
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
void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei, LISTA_REM* lista){
	int lixo = 0;
	//char removido = '*';
	//long int encadeamento = -1;
	//char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o nome |%s|\n", nome);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->nomeServidor, nome) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O NOME Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
			*///==========================
			remove_registro_generico(bin, rd->tamanhoRegistro, lista);
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
void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei, LISTA_REM* lista){
	int lixo = 0;
	/*char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	*/REGDADOS* rd;
	//printf("Quero remover o cargo |%s|\n", cargo);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->cargoServidor, cargo) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O CARGO Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
		/*	fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			}
		*/	//==========================
			remove_registro_generico(bin, rd->tamanhoRegistro, lista);
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
		}
		free(rd);
	}
}

void remove_registro_generico(FILE *bin, int tamanhoRegistro, LISTA_REM* lista){

	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';

	REG_REM* reg_removido = calloc(1, sizeof(REG_REM));
	reg_removido->tam = tamanhoRegistro;
	reg_removido->encadeamentoLista = -1;
	reg_removido->tempo_descoberta = lista->conta_descobertos;
	
	fseek(bin, -(tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
	reg_removido->pos = ftell(bin);

	fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
	fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
	fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
	for(int i = 0; i < tamanhoRegistro - ENC_TAM; i++){
		fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
	}

	lista->n_rem++;
	lista->vetor_removidos = realloc(lista->vetor_removidos, lista->n_rem*sizeof(REG_REM*));
	lista->vetor_removidos[lista->n_rem - 1] = reg_removido;
	lista->conta_descobertos++;

//	free(reg_removido);
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


//======================== CODIGOS PARA INSERÇÃO =================================

void firstFit_insere(char* nomeBin, REGDADOS* rd, long int* ultimo_reg){
	//=====================
//	printf("====================Entrando em firstFit_insere\n");
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		//fclose(bin);
		exit(0);
	}

	char status = 0;
	fread(&status, STATUS_TAM, 1, bin);
	//printf("status = %c\n", status);

	//checando status do arquivo
	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		exit(0);
	}



	achaFirstFit_eInsereReg(bin, rd, ultimo_reg);
	fclose(bin);

}

void achaFirstFit_eInsereReg(FILE* bin, REGDADOS* rd, long int* ultimo_reg){
//	printf("entrando em achaFirstFit_eInsereReg\n");
	int tam_bin = tamArquivo(bin); //tamanho do arquivo
	long int topoLista = 0;

	fseek(bin, STATUS_TAM, SEEK_CUR); //pula status
	fread(&topoLista, TOPO_TAM, 1, bin); //lendo o topoLista

	rewind(bin);
	//fseek(bin, topoLista, SEEK_CUR); //fui para o primeiro removido

	char removido;
	int tam_aux = 0;
	long int encadLista_atual = topoLista;

	//int tamRegAnterior = 0;
	//int tamPagina = 0;

	//REGCAB* rc = calloc(1, sizeof(REGCAB));
	//rc->tags[3] = 'n';
	//rc->tags[4] = 'c';

	long int encadLista_anterior = -1;
	int indice_lista = 0;

	long int pos = 0;

	while(ftell(bin) < tam_bin/* && encadLista_atual != -1*/){			//os reg removidos estão encadeados por tamanho, do menor pro menor
		if(encadLista_atual == -1){ //então não há espaços de registros removidos. Inserir no FIM
			insere_umReg_noFim(bin, rd, ultimo_reg);
			atualizaTopoEncadLista_aposInserir(bin, encadLista_anterior, encadLista_atual, indice_lista);
			return;
		}
		fseek(bin, encadLista_atual, SEEK_SET);
		pos = ftell(bin);
//		printf("bin na posição: %ld\n", ftell(bin));		// então o primeiro q couber o registro, eu insiro
		fread(&removido, REM_TAM, 1, bin);
		fread(&tam_aux, TAM_TAM, 1, bin);
		fread(&encadLista_atual, ENC_TAM, 1, bin);
	
//		printf("LENDO : %c %d %ld\n", removido, tam_aux, encadLista_atual);
//		printf("[Tamanho disponivel = %d\n\tTamanho do reg = %d\n]", tam_aux, rd->tamanhoRegistro);
		//if(removido == '*'){
//			printf("removido [%d]\n", indice_lista);
			indice_lista++;
			if(tam_aux >= rd->tamanhoRegistro){
//				printf("\n====Achei um best fit! \n\tTamanho disponivel = %d\n\tTamanho do reg = %d\n\t\tTam do lixo = %d\n", tam_aux, rd->tamanhoRegistro, tam_aux - rd->tamanhoRegistro);
				fseek(bin, -(REM_TAM + TAM_TAM + ENC_TAM), SEEK_CUR); //volta pro inicio do registro pra poder escrever nele
				//tamPagina = tam_aux;
				//structToBin(&tamPagina, &tamRegAnterior, rd, rc, bin); //insere o novo registro
				insere_umReg_naPos(bin, rd, pos);
				//atualizar os encadeamentoLista e topoLista no Arquivo
				atualizaTopoEncadLista_aposInserir(bin, encadLista_anterior, encadLista_atual, indice_lista);

				return;			
			}
		//}
		encadLista_anterior = pos;
		//se o registro não couber no espaço do registro vazio acima, ir para o próximo espaço disponível
		//rewind(bin);
	}




}

void atualizaTopoEncadLista_aposInserir(FILE* bin, long int encadLista_anterior, long int encadLista_atual, int indice_lista){
//	printf("Atualizando topo e encadeamentoLista\n");
	//atualizando topoLista
	rewind(bin);		// então preciso atualizar o topoLista, se não for o primeiro/unico da lista, não precisa atualizar
	if(indice_lista == 1){ //se era o primeiro (ou unico) registro removido da lista
		fseek(bin, STATUS_TAM, SEEK_CUR); //pula status
		fwrite(&encadLista_atual, TOPO_TAM, 1, bin);
		return;
	}

	//atualizando encadeamentoLista
	//rewind(bin);
	fseek(bin, encadLista_anterior + REM_TAM + TAM_TAM, SEEK_CUR);
	fwrite(&encadLista_atual, TOPO_TAM, 1, bin);
}

void insere_umReg_naPos(FILE* bin, REGDADOS* rd, long int pos/*, int tam_lixo*/){
//	printf("insere_umReg_naPos\n");
	rewind(bin);

	char tagN = 'n';
	char tagC = 'c';
	//char lixo = '@';

	fseek(bin, pos, SEEK_CUR);

	fwrite(&rd->removido, REM_TAM, 1, bin); //reescrevendo o removido de '*' para '-'
	fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanho, pois este permanece o mesmo
	fwrite(&rd->encadeamentoLista, ENC_TAM, 1, bin);
	
	fwrite(&rd->idServidor, ID_TAM, 1, bin);
	fwrite(&rd->salarioServidor, SAL_TAM, 1, bin);
	if(sizeof(rd->telefoneServidor) == 0){
		strcpy(rd->telefoneServidor, "\0@@@@@@@@@");
	}
	fwrite(&rd->telefoneServidor, TEL_TAM * sizeof(char), 1, bin);

	if(rd->nomeServidor[0] != '\0'){
		int tamanho2 = strlen(rd->nomeServidor) + 2;
		fwrite(&tamanho2, TAM_TAM, 1, bin);
		fwrite(&tagN, TAG_TAM, 1, bin);
		fwrite(&rd->nomeServidor, strlen(rd->nomeServidor) * sizeof(char) + 1, 1, bin);
	}

	if(rd->cargoServidor[0] != '\0'){
		int tamanho = strlen(rd->cargoServidor) + 2;
		fwrite(&tamanho, TAM_TAM, 1, bin);
		fwrite(&tagC, TAG_TAM, 1, bin);
		fwrite(&rd->cargoServidor, strlen(rd->cargoServidor)  * sizeof(char) + 1, 1, bin);
	}

}

void insere_umReg_noFim(FILE* bin, REGDADOS* rd, long int* ultimo_reg){
//	printf("entrando em insere_umReg_noFim\n");
	char tagN = 'n';
	char tagC = 'c';

	char removido;
	fseek(bin, (*ultimo_reg), SEEK_SET);
	fread(&removido, TAM_TAM, 1, bin);
	//printf("REmovido do ultimo registro antes de inserir = |%c|\n", removido);
	if(removido == '*'){ //caso o ultimo registro seja um registro removido com pouco espaço.
		insere_umReg_naPos(bin, rd, (*ultimo_reg));
		return;
	}

	int tamRegAnterior = 0;
	fread(&tamRegAnterior, TAM_TAM, 1, bin); //tamanho do registro anterior

	//rewind(bin);
	//fseek(bin, pos, SEEK_SET);

	fseek(bin, 0, SEEK_END); //indo pro final do arquivo
	int pos = ftell(bin);

	//calculando o espaço restante na pagina de disco atual pra saber se o registro cabe nela
	int tam_restante_pagina = TAM_PAG_DISCO - (pos % TAM_PAG_DISCO); 

	if(tam_restante_pagina < rd->tamanhoRegistro + REM_TAM + TAM_TAM){ // se o tamanho que ainda resta na pagina for menor que o tamanho do registro que quero inserir,
		char arroba = '@';												// preencho ela com arroba, incremendo o tamanho do registro anterior e vou pra proxima pagina
		
		for(int i = 0; i < tam_restante_pagina; i++){
			fwrite(&arroba, sizeof(char), 1, bin);
			tamRegAnterior = tamRegAnterior + 1;
		}

		//atualizar o tamanho do ultimo registro da pagina (ou seja, com o tamanho desses arrobas adicionados)
		fseek(bin, -(tamRegAnterior + TAM_TAM), SEEK_CUR);
		fwrite(&tamRegAnterior, TAM_TAM, 1, bin);

		fseek(bin, 0, SEEK_END);
	}

	(*ultimo_reg) = ftell(bin); //posição inicial do ultimo registro

	fwrite(&rd->removido, REM_TAM, 1, bin); //reescrevendo o removido de '*' para '-'
	fwrite(&rd->tamanhoRegistro, TAM_TAM, 1, bin);
	fwrite(&rd->encadeamentoLista, ENC_TAM, 1, bin);
	
	fwrite(&rd->idServidor, ID_TAM, 1, bin);
	fwrite(&rd->salarioServidor, SAL_TAM, 1, bin);
	fwrite(&rd->telefoneServidor, TEL_TAM, 1, bin);

	if(rd->nomeServidor[0] != '\0'){
		int tamanho2 = strlen(rd->nomeServidor) + 2;
		fwrite(&tamanho2, TAM_TAM, 1, bin);
		fwrite(&tagN, TAG_TAM, 1, bin);
		fwrite(&rd->nomeServidor, strlen(rd->nomeServidor) * sizeof(char) + 1, 1, bin);
	}

	if(rd->cargoServidor[0] != '\0'){
		int tamanho = strlen(rd->cargoServidor) + 2;
		fwrite(&tamanho, TAM_TAM, 1, bin);
		fwrite(&tagC, TAG_TAM, 1, bin);
		fwrite(&rd->cargoServidor, strlen(rd->cargoServidor)  * sizeof(char) + 1, 1, bin);
	}

}


//=========================== CÓDIGOS PARA ATUALIZAÇÃO ===============================
//busca um campo com o valor dado, e retorna sua posição caso encontre, ou -1 caso contrário.
long int busca_campo(char* nomeBin, char nomeCampo[MAX], char valor[MAX]){
	FILE* bin = fopen(nomeBin, "rb+")	;

	int achei = 0;
	//int tam_bin = tamArquivo(bin);
	int tam_pag = 0;
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);
	int numPaginasAcessadas = 1; //ja acessei o Registro de cabeçalho na primeira pagina


	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		//int id = atoi(valor);
		//busca_e_atualiza_id();
		
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
		//double sal = atof(valor);
		//atualiza_sal();
		
	}
	else if(strcmp(nomeCampo, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valor);
		
	}
	else if(strcmp(nomeCampo,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valor)+1, sizeof(char));
		strcpy(nome, valor);
		
		free(nome);

	}
	else if(strcmp(nomeCampo, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valor)+1, sizeof(char));
		strcpy(cargo, valor);
		
		free(cargo);
	}
	
	if(tam_pag > 0 && tam_pag < 32000){
		numPaginasAcessadas++;
	}
	if(achei > 0){
		//printf("Número de páginas de disco acessadas: %d\n", numPaginasAcessadas);
		return numPaginasAcessadas;
	}
	else{
		//printf("Registro inexistente.\n");
		return -1;
	}
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
void busca_id_e_atualiza (FILE* bin, int tam_bin, int id, int *achei){
	int lixo = 0;
	//char removido = '*';
	//long int encadeamento = -1;
	//char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o id |%d|\n", id);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O ID Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
		/*	fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
		*/	//==========================
			//remove_registro_generico(bin, rd->tamanhoRegistro);
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
void atualiza_salario (FILE* bin, int tam_bin, double sal, int *achei){
	int lixo = 0;
	//char removido = '*';
	//long int encadeamento = -1;
	//char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o salario |%lf|\n", sal);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->salarioServidor == sal){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O SAL Q QUERO REMOVER!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
		*/	//==========================
			//remove_registro_generico(bin, rd->tamanhoRegistro);
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
void atualiza_telefone (FILE* bin, int tam_bin, char* tel, int *achei){
	int lixo = 0;
//	char removido = '*';
//	long int encadeamento = -1;
//	char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o telefone |%s|\n", tel);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->telefoneServidor,tel) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O TEL Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
			*///==========================
			//remove_registro_generico(bin, rd->tamanhoRegistro);
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
void atualiza_nome (FILE* bin, int tam_bin, char* nome, int *achei){
	int lixo = 0;
	//char removido = '*';
	//long int encadeamento = -1;
	//char arroba = '@';
	REGDADOS* rd;
	//printf("Quero remover o nome |%s|\n", nome);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->nomeServidor, nome) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O NOME Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			/*fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
			*///==========================
			//remove_registro_generico(bin, rd->tamanhoRegistro);
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
void atualiza_cargo (FILE* bin, int tam_bin, char* cargo, int *achei){
	int lixo = 0;
	/*char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	*/REGDADOS* rd;
	//printf("Quero remover o cargo |%s|\n", cargo);

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(strcmp(rd->cargoServidor, cargo) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
		//	printf("ACHEI O CARGO Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
		/*	fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			}
		*/	//==========================
			//remove_registro_generico(bin, rd->tamanhoRegistro, lista);
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
		}
		free(rd);
	}
}