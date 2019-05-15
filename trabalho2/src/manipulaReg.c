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
	int tam_bin = tamArquivo(bin); //tamanho do arquivo
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR); //fui para a pagina 2, onde começam os registro de dados
	char removido;
	int n_regs = 0;
	int tam_aux = 0;
	long int pos = 0;
	long int proxRem = 0;
	lista = calloc(1, sizeof(REG_REM));

	while(ftell(bin) != tam_bin){
		fread(&removido, REM_TAM, 1, bin);
		fread(&tam_aux, TAM_TAM, 1, bin);
		fread(&proxRem, ENC_TAM, 1, bin);
	//	printf("LENDO : %c %d %ld\n", removido, tam_aux, proxRem);
		if(removido == '*'){
			n_regs++;
			pos = ftell(bin) - ENC_TAM; //posição do inicio desse arquivo, ou seja, no encadeamentoLista
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
	MS_sort(lista, n_regs, sizeof(REG_REM), int_compare);

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

int int_compare(const void *A, const void *B) { // 1, 2, 2, 3, 4
	REG_REM *pA, *pB;
	pA = (REG_REM *) A;
	pB = (REG_REM *) B;
	return pB->tam - pA->tam;
}


void busca_RemoveReg(char* nomeBin, char* nomeCampo, char* valorCampo){
	int achei = 0;
	//=====================
	FILE* bin = fopen(nomeBin, "rb+");
	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	char status;
	fread(&status, STATUS_TAM, 1, bin);
	//printf("status = %c\n", status);
	//long int topoLista = 0;
	//fread(&topoLista, TOPO_TAM, 1, bin);

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

	int n_rem = 0;
	//REG_REM* listaRem;
	//long int topo = -1;
//	printf("achei = %d\n", achei);
	//if(achei > 0){
		//listando os removidos
		REG_REM* listaRem = lista_removidos(bin, listaRem, &n_rem);// = calloc(1, sizeof(REG_REM));
		//printf("tamanho da lista = %d\n", n_rem);
		long int topo = listaRem[0].pos;
		//printf("-----novo topoooo = %ld\n", topo);		
	//}

	fclose(bin);
	
	if(listaRem != NULL){
		//atualizando topoLista
		atualiza_topoLista(nomeBin, topo);

		//atualizando o encadeamentoLista
		atualiza_encadLista(nomeBin, listaRem, n_rem);

		free(listaRem);	
	}

}

void atualiza_topoLista(char* nomeBin, long int topo){
	FILE* bin = fopen(nomeBin, "rb+");

	printf("-----novo topoooo = %ld\n", topo);
	fseek(bin, sizeof(char), SEEK_CUR);
	fwrite(&topo, TOPO_TAM, 1, bin);

	fclose(bin);
}

void atualiza_encadLista(char* nomeBin, REG_REM* lista, int n_rem){
	FILE* bin = fopen(nomeBin, "rb+");
	
	long int encadLista = -1;

	for (int i = 1; i < n_rem; ++i)
	{
		fseek(bin, lista[i-1].pos, SEEK_CUR); //pulando até o registro removido que quero atualizar o encadeamento
	
		encadLista = lista[i].pos + REM_TAM + TAM_TAM; //posição do proximo menor registro removido
		printf("-----novo encadeamento = %ld\n", encadLista);
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
						for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
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
						for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
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
						for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
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
		if(strcmp(rd->nomeServidor, nome) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O NOME Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			} //completando o registro com lixo
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
		if(strcmp(rd->cargoServidor, cargo) == 0){
			//====removendo registro====
			//printf("tam reg eh %d\n", rd->tamanhoRegistro);
			//printf("ACHEI O CARGO Q QUERO REMOVER:\nRemovido: %c\nID: %d\nSalario: %lf\nTel: %s\nNome: %s\nCargo: %s\n", rd->removido, rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
			fseek(bin, -(rd->tamanhoRegistro + TAM_TAM + REM_TAM), SEEK_CUR); //voltando no inicio do registro
			fwrite(&removido, REM_TAM, 1, bin); //reescrevendo o valor de removido para '*', indicando que o registro foi removido
			fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanhoRegistro
			fwrite(&encadeamento, ENC_TAM, 1, bin); //reescrevendo o valor de EncadeamentoLista com o novo valor
			for(int i = 0; i < rd->tamanhoRegistro - ENC_TAM; i++){
				fwrite(&arroba, sizeof(char), 1, bin); //completando o registro com lixo
			}
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
	int tam_bin = tamArquivo(bin);

	char status = 0;
	fread(&status, STATUS_TAM, 1, bin);
	//printf("status = %c\n", status);

	//checando status do arquivo
	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(bin);
		return;
	}
	//=======================

	int tam_lixo = 0;

	long int pos_bestFit = achaBestFit(bin, rd->tamanhoRegistro, tam_bin, &tam_lixo);

	insere_umReg_naPos(bin, rd, pos_bestFit, tam_lixo);



}

void insere_umReg_naPos(FILE* bin, REGDADOS* rd, long int pos, int tam_lixo){
	rewind(bin);

	char tagN = 'n';
	char tagC = 'c';
	char lixo = '@';

	fseek(bin, pos, SEEK_CUR);

	fwrite(&rd->removido, REM_TAM, 1, bin); //reescrevendo o removido de '*' para '-'
	fseek(bin, TAM_TAM, SEEK_CUR); //pulando o tamanho, pois este permanece o mesmo
	fwrite(&rd->encadeamentoLista, ENC_TAM, 1, bin);
	fwrite(&rd->idServidor, ID_TAM, 1, bin);
	fwrite(&rd->salarioServidor, SAL_TAM, 1, bin);
	fwrite(&rd->telefoneServidor, TEL_TAM, 1, bin);

	fwrite(&rd->tamNomeServidor, TAM_TAM, 1, bin);
	fwrite(&tagN, TAG_TAM, 1, bin);
	fwrite(&rd->nomeServidor, rd->tamNomeServidor - 1, 1, bin);
	
	fwrite(&rd->tamCargoServidor, TAM_TAM, 1, bin);
	fwrite(&tagC, TAG_TAM, 1, bin);
	fwrite(&rd->cargoServidor, rd->tamCargoServidor - 1, 1, bin);

	for (int i = 0; i < tam_lixo; ++i)
	{
		fwrite(&lixo, sizeof(char), 1, bin);
	}


}





/*
 * tamanho_bestFit
 * Função: Percorre o vetor ordenado com os tamanhos dos registros removidos existentes, procurando pelo tamanho do melhor bestFit de acordo com o tamanho do registro dado
 * Parâmetros: 	
 				tamanhos_ordenados: Vetor ordenado com os tamanhos dos registros removidos existentes.
 				tam_reg: Tamanho do registro
 				n_reg: numero de itens do vetor

 	Retorno: Tamanho do melhor bestFit 
*/
/*
int tamanho_BestFit(int* tamanhos_ordenado, int tam_reg, int n_reg){
	int tamanho1 = 0;
	int tamanho2 = 0;
	int sobra1 = 0;
	int sobra2 = 0;
	//int menor_sobra = tamanhos_ordenado;
	int best_fit_tam = -1;

	for (int i = 1; i < n_reg; ++i)
	{
		tamanho1 = tamanhos_ordenado[i-1];
		tamanho2 = tamanhos_ordenado[i];
		sobra1 = tamanho1 - tam_reg;
		sobra2 = tamanho2 - tam_reg;
		if(sobra1 < sobra2 && sobra1 < menor_sobra && tam_reg >= sobra1){
			best_fit_tam = tamanho1;
			menor_sobra = sobra1;
		}
		else if(sobra2 < sobra1 && tam_reg >= sobra2){
			best_fit_tam = tamanho2;
			menor_sobra = sobra2;
		}
	}


	return best_fit_tam;
}*/

/*
 * achaBestFit
 * Função: Percorre um vetor de tamanhos de registros (vetor_tamanhos) procurando pelo indice do best fit recebido, 
 			que também é o indice da sua posição no arquivo no vetor vetor_posicao, encontrando a posição do best fit.

 * Parâmetros: 	
 				best_fit_tam: Tamanho do best fit
 				vetor_tamanhos: Vetor com os tamanhos dos registros na ordem em que estão no arquivo
 				vetor_posicao: Vetor com as posições dos registros na ordem em que estão no arquivo
 				tam_vetor: numero de elementos dos vetores vetor_tamanhos e vetor_posicao
 				pos: Posição do best fit

 	Retorno: posição no arquivo do melhor bestFit 
*/
void posicao_BestFit(int best_fit_tam, int *vetor_tamanhos, long int *vetor_posicao, int tam_vetor, long int *pos){
	for (int i = 0; i < tam_vetor; ++i)
	{
		if(best_fit_tam == vetor_tamanhos[i]){
			(*pos) = vetor_posicao[i];
			return;
		}
	}
}

/*
 * achaBestFit
 * Função: Lê um arquivo binário e acha o best Fit para um registro de um dado tamanho e retorna a posição do best fit encontrado
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_reg: Tamanho do registro
 				tam_bin: Tamanho do arquivo binário

 	Retorno: Tamanho do melhor bestFit 
*/
long int achaBestFit(FILE* bin, int tam_reg, int tam_bin, int*tam_lixo){
	int *tamanhos;
	int *tam_aux;
	long int *posicao;
	int countReg = 0;
	char removido;
	int tamRegAux;
	long int bestFit_posicao = 0;

	while(ftell(bin) != tam_bin){
		fread(&removido, REM_TAM, 1, bin);
		fread(&tamRegAux, TAM_TAM, 1, bin); //lê e guarda o tamanho do 
		if(removido == '*'){
			tamanhos = calloc(countReg, sizeof(int));
			tam_aux = calloc(countReg, sizeof(int));
			posicao = calloc(countReg, sizeof(long int));

			tamanhos[countReg] = tamRegAux;
			tam_aux[countReg] = tamRegAux;
			posicao[countReg] = ftell(bin) - REM_TAM; //posição da variavel 'removido' do registro

			countReg++;
		}
		fseek(bin, tamRegAux, SEEK_CUR);
	}

	//ordenando o vetor de tamanhos auxiliar
	MS_sort(tam_aux, countReg, sizeof(int), int_compare);

	//comparando os tamanhos pra achar o best_fit
	//int best_fit_tam = tamanho_BestFit(tam_aux, tam_reg, countReg);

	//procurando o indice do tamanho encontrado
	//posicao_BestFit(best_fit_tam, tamanhos, posicao, countReg, &bestFit_posicao);

	//tamanho que vai sobrar no registro após inserção
	//(*tam_lixo) = best_fit_tam - tamRegAux;

	rewind(bin);

	return bestFit_posicao;
}