#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "manipulaReg.h"
#include "merge_sort.h"

//================ CODIGOS PARA REMOÇÃO =====================

/*
 * novo_no_removido
 * Função: Recebe dados de um registro removido e salva em uma struct de registro removido.
 * Parâmetros: 	
 				reg_vazio: struct onde serão salvos os dados do registro removido
 				pos: posição do registro removido
 				tam: tamanho do registro removido
 				proxRem: encadeamentoLista do registro removido
*/
void novo_no_removido(REG_REM* reg_vazio, long int pos, int tam, long int proxRem){
	reg_vazio->pos = pos;
	reg_vazio->tam = tam;
	reg_vazio->encadeamentoLista = proxRem;
}


/*
 * lista_removidos
 * Função: Percorre o arquivo binário e coloca na lista todos os registros com removido = '*', e retorna a lista.
 * Parâmetros: 	
 				bin: Arquivo binário
 				lista: lista de registros removidos
 				numero_registros_removidos: ponteiro para controlar o tamanho da lista de registros removidos
*/
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

/*
 * int_compare_tam
 * Função: Compara dois valores, e retorna a diferença entre B - A.
 * Parâmetros: 	
 				A: valor 1
 				B: valor 2
*/
int int_compare_tam(const void *A, const void *B) { // 1, 2, 2, 3, 4
	REG_REM *pA, *pB;
	pA = (REG_REM *) A;
	pB = (REG_REM *) B;
	return pB->tam - pA->tam;
}

/*
 * int_compare_tam
 * Função: Compara dois valores, e retorna a diferença entre A - B.
 * Parâmetros: 	
 				A: valor 1
 				B: valor 2
*/
int int_compare_pos_reverse(const void *A, const void *B) { // 4, 3, 2, 2, 1
	REG_REM *pA, *pB;
	pA = (REG_REM *) A;
	pB = (REG_REM *) B;
	return pA->pos - pB->pos;
}

/*
 * busca_RemoveReg
 * Função: Percorre um arquivo binário procurando pelo campo e valor dado.
 * Parâmetros: 	
 				nomeBin: nome do arquivo binário
 				nomeCampo: nome do campo que quero encontrar
 				valorCampo: valor do campo que quero encontrar
*/
void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo){
	int achei = 0;
	//=====================
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
	//=======================
	int tam_bin = tamArquivo(bin);
	//rewind(bin);
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);


	
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
	//	printf("vou procurar o salario\n");
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

	int n_rem = 0;
	REG_REM* listaRem = NULL;
	//long int topo = -1;
//	printf("achei = %d\n", achei);

		//listando os removidos
	listaRem = lista_removidos(bin, listaRem, &n_rem);
		//printf("tamanho da lista = %d\n", n_rem);
	//long int topo = listaRem[0].pos;

	fclose(bin);

	ordenaLista_atualizaEncAndTopoLista(nomeBin, listaRem, n_rem);
	
	/*if(listaRem != NULL){
		//atualizando topoLista
		atualiza_topoLista(nomeBin, topo);

		//atualizando o encadeamentoLista
		atualiza_encadLista(nomeBin, listaRem, n_rem);

		free(listaRem);	
	}*/
	free(listaRem);
}

/*
 * ordenaLista_atualizaEncAndTopoLista
 * Função: Ordena uma lista de registros removidos, primeiro por posição reversa, depois por tamanho
 * Parâmetros: 	
 				nomeBin: nome do arquivo binário
 				lista: lista de registros removidos
 				n_rem: tamanho da lista
*/
void ordenaLista_atualizaEncAndTopoLista(char* nomeBin, REG_REM* lista, int n_rem){
//	printf("Entrando em ordenaLista_atualizaEncAndTopoLista, n_rem: %d\n", n_rem);

	//ordenando a lista de registros de removidos
	MS_sort(lista, n_rem, sizeof(REG_REM), int_compare_pos_reverse);

	MS_sort(lista, n_rem, sizeof(REG_REM), int_compare_tam);

	long int topo = lista[0].pos;

	if(lista != NULL){
		//atualizando topoLista
		atualiza_topoLista(nomeBin, topo);

		//atualizando o encadeamentoLista
		atualiza_encadLista(nomeBin, lista, n_rem);

		//free(lista);	
	}
}

/*
 * atualiza_topoLista
 * Função: Atualiza o topoLista de um arquivo
 * Parâmetros: 	
 				nomeBin: nome do arquivo binário
 				topo: valor do topoLista que quero atualizar
*/
void atualiza_topoLista(char* nomeBin, long int topo){
	FILE* bin = fopen(nomeBin, "rb+");

	//printf("-----novo topoooo = %ld\n", topo);
	fseek(bin, sizeof(char), SEEK_CUR);
	fwrite(&topo, TOPO_TAM, 1, bin);

	fclose(bin);
}

/*
 * atualiza_encadLista
 * Função: Atualiza os valores de encadeamentoLista de um arquivo
 * Parâmetros: 	
 				nomeBin: nome do arquivo binário
 				lista: lista de registros removidos
 				n_rem: tamanho da lista de registros removidos
*/
void atualiza_encadLista(char* nomeBin, REG_REM* lista, int n_rem){
	FILE* bin = fopen(nomeBin, "rb+");
	
	long int encadLista = -1;

	for (int i = 1; i < n_rem; ++i)
	{
		fseek(bin, lista[i-1].pos + REM_TAM + TAM_TAM, SEEK_CUR); //pulando até o registro removido que quero atualizar o encadeamento
	
		encadLista = lista[i].pos;// + REM_TAM + TAM_TAM; //posição do proximo menor registro removido
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
 				id: Valor do id que quero buscar
 				achei: variavel que indica se encontrei registros ou não
 				funcionalidade: indica se a busca é para 4 - remoção ou 6 - atualização
*/
void remove_id (FILE* bin, int tam_bin, int id, int *achei/*, int funcionalidade*/){
	int lixo = 0;
	REGDADOS* rd;
	//printf("Quero remover o id |%d|\n", id);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){
			(*achei) = 1;
			//if()
			remove_registro_generico(bin, rd->tamanhoRegistro);

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
 				sal: Valor do salario que quero buscar
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_salario (FILE* bin, int tam_bin, double sal, int *achei){
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
			remove_registro_generico(bin, rd->tamanhoRegistro);
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
 				tel: Numero do telefone que quero buscar
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_telefone (FILE* bin, int tam_bin, char* tel, int *achei){
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
			remove_registro_generico(bin, rd->tamanhoRegistro);
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
 				nome: Nome que quero buscar
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_nome (FILE* bin, int tam_bin, char* nome, int *achei){
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
			remove_registro_generico(bin, rd->tamanhoRegistro);
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
 				cargo: Cargo que quero buscar
 				achei: variavel que indica se encontrei registros ou não
*/
void remove_cargo (FILE* bin, int tam_bin, char* cargo, int *achei){
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
			remove_registro_generico(bin, rd->tamanhoRegistro);
			(*achei) = 1;

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
		}
		free(rd);
	}
}

/*
 * remove_registro_generico
 * Função: Remove um registro no arquivo
 * Parâmetros: 	
 				bin: Arquivo binário
 				tamanhoRegistro: tamanho do registro que quero remover
*/
void remove_registro_generico(FILE *bin, int tamanhoRegistro){
	char removido = '*';
	long int encadeamento = -1;
	char arroba = '@';
	
	fseek(bin, -(tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
	fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
	fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
	fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
	for(int i = 0; i < tamanhoRegistro - ENC_TAM; i++){
		fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
	}
}

/*
 * volta_le_Eprinta_umRegistro
 * Função: volta pro inicio do registro anterior (tem q estar no inicio de algum registro pra funcionar) e printa ele
 * Parâmetros: 	
 				bin: Arquivo binário
 				tamReg: tamanho do registro
*/
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


/*
 * printaUmReg
 * Função: Recebe uma struct de registro de dados e printa na tela os dados
 * Parâmetros: 	
 				rd: registro de dados
*/
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

/*
 * firstFit_insere
 * Função: Abre um arquivo binário e chama a função que procura o first fit de um registro e o insere no arquivo
 * Parâmetros: 	
 				nomeBin: nome do arquivo binário
 				rd: registro de dados
 				ultimo_reg: ponteiro para controlar a posição do ultimo registro do arquivo
*/
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

/*
 * achaFirstFit_eInsereReg
 * Função: Percorre um arquivo binário até encontrar o first fit de um registro, e o insere no arquivo
 * Parâmetros: 	
 				bin: arquivo binário
 				rd: registro de dados
 				ultimo_reg: ponteiro para controlar a posição do ultimo registro do arquivo
*/
void achaFirstFit_eInsereReg(FILE* bin, REGDADOS* rd, long int* ultimo_reg){
	//printf("entrando em achaFirstFit_eInsereReg\n");
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
			//atualizaTopoEncadLista_aposInserir(bin, encadLista_anterior, encadLista_atual, indice_lista);
			return;
		}
		fseek(bin, encadLista_atual, SEEK_SET);
		pos = ftell(bin);
//		printf("bin na posição: %ld\n", ftell(bin));		// então o primeiro q couber o registro, eu insiro
		fread(&removido, REM_TAM, 1, bin);
		fread(&tam_aux, TAM_TAM, 1, bin);
		fread(&encadLista_atual, ENC_TAM, 1, bin);
	
		//printf("LENDO : %c %d %ld\n", removido, tam_aux, encadLista_atual);
	//	printf("[Tamanho disponivel = %d\n\tTamanho do reg = %d\n]", tam_aux, rd->tamanhoRegistro);
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

/*
 * atualizaTopoEncadLista_aposInserir
 * Função: Atualiza tanto o topoLista quanto os encadeamentoLista de um arquivo após inserir um registro
 * Parâmetros: 	
 				bin: arquivo binário
 				encadLista_anterior: encadeamentoLista do registro anterior
 				encadLista_atual: encadeamentoLista do registro atual
 				indice_lista: indice da lista para controlar quantos registros removidos já percorri
*/
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

/*
 * insere_umReg_naPos
 * Função: Insere um registro no arquivo binário na posição dada
 * Parâmetros: 	
 				bin: arquivo binário
 				rd: registro de dados
 				pos: posição que quero inserir o registro
*/
void insere_umReg_naPos(FILE* bin, REGDADOS* rd, long int pos/*, int tam_lixo*/){
	//printf("\n\t====== insere_umReg_naPos\n");
	rewind(bin);

	char tagN = 'n';
	char tagC = 'c';
	//char lixo = '@';

	fseek(bin, pos, SEEK_CUR);
	//printf("removido = |%c|\n", rd->removido);
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

/*
 * insere_umReg_noFim
 * Função: Insere um registro no arquivo binário no fim do arquivo
 * Parâmetros: 	
 				bin: arquivo binário
 				rd: registro de dados
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void insere_umReg_noFim(FILE* bin, REGDADOS* rd, long int* ultimo_reg){
//	printf("\n\t=====entrando em insere_umReg_noFim\n");
	char tagN = 'n';
	char tagC = 'c';

	char removido;
	fseek(bin, (*ultimo_reg), SEEK_SET);
	fread(&removido, TAM_TAM, 1, bin);
//	printf("removido = |%c|\n", rd->removido);
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

/*
 * busca_atualiza_campo
 * Função: busca um campo com o valor dado, e retorna sua posição caso encontre, ou -1 caso contrário.
 * Parâmetros: 	
 				nomeBin: arquivo binário
 				nomeCampo: nome do campo que quero buscar para atualizar
 				valor: valor do campo que quero buscar para atualizar
 				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)

*/
void busca_atualiza_campo(char* nomeBin, char nomeCampo[MAX], char valor[MAX], char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		exit(0);
	}
	char status = 0;
	fread(&status, STATUS_TAM, 1, bin);
	//checando status do arquivo
	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		exit(0);
	}

	int achei = 0;
	int tam_bin = tamArquivo(bin);
	//int tam_pag = 0;
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);


	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		int id = atoi(valor);
		atualiza_id(bin, tam_bin, id, &achei, nomeCampoAtualiza, valorCampoAtualiza, ultimo_reg);
		
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
		double sal = atof(valor);
		atualiza_salario(bin, tam_bin, sal, &achei, nomeCampoAtualiza, valorCampoAtualiza, ultimo_reg);
		
	}
	else if(strcmp(nomeCampo, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valor);
		atualiza_telefone(bin, tam_bin, tel, &achei, nomeCampoAtualiza, valorCampoAtualiza, ultimo_reg);
		
	}
	else if(strcmp(nomeCampo,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valor)+1, sizeof(char));
		strcpy(nome, valor);
		atualiza_nome(bin, tam_bin, nome, &achei, nomeCampoAtualiza, valorCampoAtualiza, ultimo_reg);
		
		free(nome);

	}
	else if(strcmp(nomeCampo, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valor)+1, sizeof(char));
		strcpy(cargo, valor);
		atualiza_cargo(bin, tam_bin, cargo, &achei, nomeCampoAtualiza, valorCampoAtualiza, ultimo_reg);
		
		free(cargo);
	}


	if(achei == 0){
	//	printf("Não achei nada pra atualizar, campo = %s, valor = %s\n", nomeCampo, valor);
	}

	int n_rem = 0;
	REG_REM* listaRem = NULL;
	listaRem = lista_removidos(bin, listaRem, &n_rem);
	ordenaLista_atualizaEncAndTopoLista(nomeBin, listaRem, n_rem);
	
	free(listaRem);
	fclose(bin);
	
}

/*
 * atualiza_UmRegistro
 * Função: Atualiza uma struct registro de dados com o novo valor.
 * Parâmetros: 	
				rd: struct de registro de dados que quero atualizar
 				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
*/
void atualiza_UmRegistro(REGDADOS* rd, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX]){
	//printf("\nAtualizando registro de ID = %d\n\t====== TAM do Reg ANTES = %d\n", rd->idServidor, rd->tamanhoRegistro);
	int tam_reg_aux = ENC_TAM + ID_TAM + SAL_TAM + TEL_TAM;
	//printf("tam_reg_aux mais os fixos = %d\n", tam_reg_aux);

	int id = 0;
	double sal = 0;
	if(strcmp(nomeCampoAtualiza,"idServidor") == 0){
		if(strcmp(valorCampoAtualiza, "NULO") == 0){
			id = -1;
		}
		else{
			id = atoi(nomeCampoAtualiza);
		}		
		rd->idServidor = id;
	}

	else if(strcmp(nomeCampoAtualiza, "salarioServidor") == 0){
		if(strcmp(valorCampoAtualiza, "NULO") == 0){
			sal = -1;
		}
		else{
			sal = atof(valorCampoAtualiza);
		}
		rd->salarioServidor = sal;
		
	}
	else if(strcmp(nomeCampoAtualiza, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valorCampoAtualiza);
		strcpy(rd->telefoneServidor, tel);

		if(tel[0] != '('){
			rd->telefoneServidor[0] = '\0';
			for(int i = 1; i < 14; i++){
				rd->telefoneServidor[i] = '@';
			}
		}
		
	}
	else if(strcmp(nomeCampoAtualiza,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valorCampoAtualiza)+1, sizeof(char));
		strcpy(nome, valorCampoAtualiza);
		strcpy(rd->nomeServidor, nome);
		
		if(nome[0] != '\0'){
			rd->nomeServidor[strlen(nome)] = '\0';
		}

		if(strlen(nome) != 0){
			int tam_nome_antes = rd->tamNomeServidor;
			rd->tamNomeServidor = strlen(rd->nomeServidor);
			if(tam_nome_antes == 0){
				//rd->tamNomeServidor += 1;
			}
		//	tam_reg_aux += TAG_TAM + TAM_TAM;
			//printf("tam_reg_aux antes = %d\n", tam_reg_aux);

		}
		free(nome);

	}
	else if(strcmp(nomeCampoAtualiza, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valorCampoAtualiza)+1, sizeof(char));
		strcpy(cargo, valorCampoAtualiza);
		//printf("\t\t\tCargo antes de atualizar = %s, tam = %ld\n", rd->cargoServidor, strlen(rd->cargoServidor));
		strcpy(rd->cargoServidor, cargo);
		//printf("\t\t\tCargo depois de atualizar = %s, tam = %ld\n", rd->cargoServidor, strlen(rd->cargoServidor));

		
		if(cargo[0] != '\0'){
		//	printf("**botei barra zero no cargo\n");
			rd->cargoServidor[strlen(cargo)] = '\0';
		}
		
		if(strlen(rd->cargoServidor) != 0){
		//	printf("incrementei o tam do cargo no registro\n");
			int tam_antes = rd->tamCargoServidor;
			rd->tamCargoServidor = strlen(rd->cargoServidor);
			if(tam_antes == 0){
				//rd->tamCargoServidor += 1;
			}
			//printf("tam_reg_aux antes = %d\n", tam_reg_aux);
			//tam_reg_aux += TAG_TAM + TAM_TAM;
			//printf("tam_reg_aux depois = %d\n", tam_reg_aux);

		}
		free(cargo);
	}
	if(strlen(rd->nomeServidor) > 0){
			tam_reg_aux += TAG_TAM + TAM_TAM;
	}
	if(strlen(rd->cargoServidor) > 0){
			tam_reg_aux += TAG_TAM + TAM_TAM;
	}

	tam_reg_aux += rd->tamNomeServidor + rd->tamCargoServidor;
	//printf("tam nome = %d, tam cargo = %d\n", rd->tamNomeServidor, rd->tamCargoServidor);


	if(tam_reg_aux > rd->tamanhoRegistro){
		rd->tamanhoRegistro = tam_reg_aux; //só vou mudar no tamanho do registro no arquivo se o atualizado for maior que o antigo
	}

	//printf("\t====== TAM do Reg ATUALIZADO = %d\n\t====== TAM FINAL SALVO = %d\n\n", tam_reg_aux, rd->tamanhoRegistro);
}


/*
 * atualiza_id
 * Função: Busca dados do tipo idServidor em um dado arquivo binário e atualiza os registros encontrados
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				id: Valor do id que quero buscar
 				achei: variavel que indica se encontrei registros ou não
  				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void atualiza_id (FILE* bin, int tam_bin, int id, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
//	printf("Buscando o id |%d|\n", id);
	int lixo = 0;
	REGDADOS* rd;
	long int pos = 0;

	//rewind(bin);

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		pos = ftell(bin);
		leUmRegistroBin(bin, rd, &lixo);
		if(rd->idServidor == id){ //encontrei registro no arquivo
			(*achei) = 1;
			//====atualizando registro====
			int tam_reg_aux = rd->tamanhoRegistro;
			//printf("|ANTES| tam do reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O ID Q QUERO ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			atualiza_UmRegistro(rd, nomeCampoAtualiza, valorCampoAtualiza);
			//printf("DEPOIS DE ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			//printf("|DEPOIS| tam do reg eh %d\n", rd->tamanhoRegistro);
			
			if(tam_reg_aux >= rd->tamanhoRegistro){ //se o tamanho antigo do registro for maior ou igual, então o registro atualizado permanece ali
				//printf("Atualizando sem remover e inserir\n");
				//fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
				insere_umReg_naPos(bin, rd, pos);
			}
			//long int ultimo_reg = 0;
			else{
				//senão o registro é removido e inserido em outro lugar
			//	printf("Atualizando COM inserção e remoção!\n");
				remove_registro_generico(bin, tam_reg_aux);
				achaFirstFit_eInsereReg(bin, rd, ultimo_reg);
			}
			
			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, tam_reg_aux);

			free(rd);
			return;
		}
		free(rd);

	}
}

/*
 * atualiza_salario
 * Função: Busca dados do tipo salarioServidor em um dado arquivo binário e atualiza os registros encontrados
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				sal: Valor do salario que quero buscar
 				achei: variavel que indica se encontrei registros ou não
 				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void atualiza_salario (FILE* bin, int tam_bin, double sal, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
//	printf("Quero atualizar o salario |%lf|\n", sal);
	int lixo = 0;
	REGDADOS* rd;
	long int pos = 0;
	long int pos_prox = 0;

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		pos = ftell(bin);
		leUmRegistroBin(bin, rd, &lixo);
		pos_prox = ftell(bin);
		if(rd->salarioServidor == sal){
			(*achei) = 1;
			//====atualizando registro====
			int tam_reg_aux = rd->tamanhoRegistro;
			//printf("|ANTES| tam do reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O SAL Q QUERO ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			atualiza_UmRegistro(rd, nomeCampoAtualiza, valorCampoAtualiza);
			//printf("DEPOIS DE ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			//printf("|DEPOIS| tam do reg eh %d\n", rd->tamanhoRegistro);
			//fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			
			if(tam_reg_aux >= rd->tamanhoRegistro){ //se o tamanho antigo do registro for maior ou igual, então o registro atualizado permanece ali
			//	printf("Atualizando sem remover e inserir\n");
				insere_umReg_naPos(bin, rd, pos);
			}
			//long int ultimo_reg = 0;
			else{
			//	printf("Atualizando COM inserção e remoção!\n");
				//senão o registro é removido e inserido em outro lugar
				remove_registro_generico(bin, tam_reg_aux);
				achaFirstFit_eInsereReg(bin, rd, ultimo_reg);				
			}

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
			
			fseek(bin, pos_prox, SEEK_SET);
		}
		free(rd);
	}
}

/*
 * atualiza_telefone
 * Função: Busca dados do tipo telefoneServidor em um dado arquivo binário e atualiza os registros encontrados
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				tel: Numero do telefone que quero buscar
 				achei: variavel que indica se encontrei registros ou não
  				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void atualiza_telefone (FILE* bin, int tam_bin, char* tel, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
//	printf("Quero atualizar o telefone |%s|\n", tel);
	int lixo = 0;
	REGDADOS* rd;
	long int pos = 0;
	long int pos_prox = 0;


	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		pos = ftell(bin);
		leUmRegistroBin(bin, rd, &lixo);
		pos_prox = ftell(bin);
		if(strcmp(rd->telefoneServidor,tel) == 0){
			(*achei) = 1;
			//====atualizando registro====
			int tam_reg_aux = rd->tamanhoRegistro;
			//printf("|ANTES| tam do reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O TEL Q QUERO ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			atualiza_UmRegistro(rd, nomeCampoAtualiza, valorCampoAtualiza);
			//printf("DEPOIS DE ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			//printf("|DEPOIS| tam do reg eh %d\n", rd->tamanhoRegistro);
		//	fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			
			if(tam_reg_aux >= rd->tamanhoRegistro){ //se o tamanho antigo do registro for maior ou igual, então o registro atualizado permanece ali
				//printf("Atualizando sem remover e inserir\n");
				insere_umReg_naPos(bin, rd, pos);
			}
			//long int ultimo_reg = 0;
			else{
				//printf("Atualizando COM inserção e remoção!\n");
				//senão o registro é removido e inserido em outro lugar
				remove_registro_generico(bin, tam_reg_aux);
				achaFirstFit_eInsereReg(bin, rd, ultimo_reg);
			}

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);

			fseek(bin, pos_prox, SEEK_SET);
		}
		free(rd);
	}
}

/*
 * atualiza_nome
 * Função: Busca dados do tipo nomeServidor em um dado arquivo binário e atualiza os registros encontrados
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				nome: Nome que quero buscar
 				achei: variavel que indica se encontrei registros ou não
 				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void atualiza_nome (FILE* bin, int tam_bin, char* nome, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
//	printf("Quero atualizar o nome |%s|\n", nome);
	int lixo = 0;
	REGDADOS* rd;
	long int pos = 0;
	long int pos_prox = 0;

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		pos = ftell(bin);
		leUmRegistroBin(bin, rd, &lixo);
		pos_prox = ftell(bin);
		if(strcmp(rd->nomeServidor, nome) == 0){
			(*achei) = 1;
			//====atualizando registro====
			int tam_reg_aux = rd->tamanhoRegistro;
			//printf("|ANTES| tam do reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O NOME Q QUERO ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			atualiza_UmRegistro(rd, nomeCampoAtualiza, valorCampoAtualiza);
			//printf("DEPOIS DE ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			//printf("|DEPOIS| tam do reg eh %d\n", rd->tamanhoRegistro);
			//fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			
			if(tam_reg_aux >= rd->tamanhoRegistro){ //se o tamanho antigo do registro for maior ou igual, então o registro atualizado permanece ali
				//printf("Atualizando sem remover e inserir\n");
				insere_umReg_naPos(bin, rd, pos);
			}
			//long int ultimo_reg = 0;
			else{
				//printf("Atualizando COM inserção e remoção!\n");
				//senão o registro é removido e inserido em outro lugar
				remove_registro_generico(bin, tam_reg_aux);
				achaFirstFit_eInsereReg(bin, rd, ultimo_reg);				
			}

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
			fseek(bin, pos_prox, SEEK_SET);

		}
		free(rd);
	}
}

/*
 * atualiza_cargo
 * Função: Busca dados do tipo cargoServidor em um dado arquivo binário e atualiza os registros encontrados
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				cargo: Cargo que quero buscar
 				achei: variavel que indica se encontrei registros ou não
 				nomeCampoAtualiza: nome do campo que quero atualizar
 				valorCampoAtualiza: valor do campo que quero atualizar
 				ultimo_reg: posição do ultimo registro (antes de inserir o novo)
*/
void atualiza_cargo (FILE* bin, int tam_bin, char* cargo, int *achei, char nomeCampoAtualiza[MAX], char valorCampoAtualiza[MAX], long int* ultimo_reg){
	//printf("Quero atualizar o cargo |%s|\n", cargo);
	int lixo = 0;
	REGDADOS* rd;
	long int pos = 0;
	long int pos_prox = 0;

	while(ftell(bin) != tam_bin){
		
		rd = calloc(1, sizeof(REGDADOS));
		pos = ftell(bin);
		leUmRegistroBin(bin, rd, &lixo);
		pos_prox = ftell(bin);
		if(strcmp(rd->cargoServidor, cargo) == 0){
			(*achei) = 1;
			//====atualizando registro====
			int tam_reg_aux = rd->tamanhoRegistro;
			//printf("|ANTES| tam do reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O TEL Q QUERO ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			atualiza_UmRegistro(rd, nomeCampoAtualiza, valorCampoAtualiza);
			//printf("DEPOIS DE ATUALIZAR!\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			//printf("|DEPOIS| tam do reg eh %d\n", rd->tamanhoRegistro);
			//fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			
			if(tam_reg_aux >= rd->tamanhoRegistro){ //se o tamanho antigo do registro for maior ou igual, então o registro atualizado permanece ali
				//printf("Atualizando sem remover e inserir\n");
				insere_umReg_naPos(bin, rd, pos);
			}
			//long int ultimo_reg = 0;
			else{
			//	printf("Atualizando COM inserção e remoção!\n");
				//senão o registro é removido e inserido em outro lugar
				remove_registro_generico(bin, tam_reg_aux);
				achaFirstFit_eInsereReg(bin, rd, ultimo_reg);				
			}

			//teste
			//printf("\n\t### Testando Registro depois de modificado ###\n");
			//volta_le_Eprinta_umRegistro(bin, rd->tamanhoRegistro);
			fseek(bin, pos_prox, SEEK_SET);
		}
		free(rd);
	}
}