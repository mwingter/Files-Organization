/*
	TRABALHO 5 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "indice.h"

//================ FUNÇÕES AUXILIARES ==========================================
/*
 * int_compare_chave
 * Função: Compara dois valores de chave (duas strings).
 * Parâmetros: 	
 				A: string 1
 				B: string 2

 * Retorno: 0 se forem iguais, 1 se B é maior que A, ou -1 se A é maior que B.
*/
int int_compare_chave(const void *A, const void *B) { // para ordenar de forma crescente
	REGDADOSIND *pA, *pB;
	pA = (REGDADOSIND *) A;
	pB = (REGDADOSIND *) B;
	return strcmp(pB->chaveBusca, pA->chaveBusca);
}
/*
 * int_compare_byteOffset
 * Função: Compara dois valores de byteOffset.
 * Parâmetros: 	
 				A: valor 1
 				B: valor 2

 * Retorno: Resultado de B - A.
*/
int int_compare_byteOffset(const void *A, const void *B) { // para ordenar de forma crescente
	REGDADOSIND *pA, *pB;
	pA = (REGDADOSIND *) A;
	pB = (REGDADOSIND *) B;
	return pB->byteOffset - pA->byteOffset;
}

/*
 * IndCabToArqBin
 * Função: Escreve um dado indice de cabeçalho em um dado arquivo de índice.
 * Parâmetros: 	
 				rc_ind: struct do indice de cabeçalho
 				bin_indice: arquivo de indice
*/
void IndCabToArqBin(REGCABIND *rc_ind, FILE *bin_indice){
	int tamRegistroCab = 5;
	int restoPagina = TAM_PAG_DISCO - tamRegistroCab;

	fwrite(&rc_ind->status, STATUS_TAM, 1, bin_indice);
	fwrite(&rc_ind->nroRegistros, TAM_TAM, 1, bin_indice);
	
	char arroba = '@';
	for(int i = 0; i < restoPagina; i++){ //preenchendo com @ o resto da pagina
		fwrite(&arroba, sizeof(char), 1, bin_indice);
	}
}

/*
 * regIndiceToArqBin
 * Função: Escreve um dado registro de dados de indice em um dado arquivo de índice.
 * Parâmetros: 	
 				rd_ind: struct do registro de dados de indice
 				bin_indice: arquivo de indice
*/
void regIndiceToArqBin(REGDADOSIND *rd_ind, FILE *bin_indice){
	int tam_nome = strlen(rd_ind->chaveBusca);

	rd_ind->chaveBusca[strlen(rd_ind->chaveBusca)] = '\0';
	tam_nome++;
	while(tam_nome < TAM_CHAVE){
		rd_ind->chaveBusca[tam_nome] = '@';
		tam_nome++;
	}

	fwrite(&rd_ind->chaveBusca, TAM_CHAVE, 1, bin_indice);
	fwrite(&rd_ind->byteOffset, TAM_BYTEOFFSET, 1, bin_indice);
}

/*
 * buscaBinaria
 * Função: Busca uma chave em uma lista de registros de dados de indice.
 * Parâmetros: 	
 				chave: chave a ser buscada na lista.
 				lista: lista de struct do registro de dados de indice.
 				low: indice do primeiro slot da lista.
 				high: indice do ultimo slot da lista.

 * Retorno: indice da chave encontrada, ou -1 caso não seja encontrada.
*/
int buscaBinaria(char chave[120], REGDADOSIND* lista, int low, int high){
   int mid, cmp;

   if (low > high) return -1;
   else{
	   mid = (low + high) / 2;
	   cmp = strcmp(chave, lista[mid].chaveBusca);
	   if (cmp == 0 )
	      return mid;
	   if (cmp < 0) 
	      return buscaBinaria(chave, lista, low, mid - 1); 
	   else 
	      return buscaBinaria(chave, lista, mid + 1, high);   	
   }
}

//================ [10] CODIGOS PARA CRIAÇÃO DE NOVO INDICE =====================
/*
 * novoIndice
 * Função: Cria um novo arquivo de indice
 * Parâmetros: 	
 				nomeBin_in: nome do arquivo de dados de referencia para o indice
 				nomeBin_indice: nome do arquivo de indice que será criado
*/
void novoIndice(char *nomeBin_in, char *nomeBin_indice){
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);

	REGDADOSIND *rd_ind = calloc(1, sizeof(REGDADOSIND));
	REGCABIND *rc_ind = calloc(1, sizeof(REGCABIND));
	FILE* bin_indice = fopen(nomeBin_indice, "wb");
	check_file_status(bin_indice);
	REGDADOS *rd = calloc(1, sizeof(REGDADOS));
	REGCAB *rc = calloc(1, sizeof(REGCAB));
	
	int tam_bin_in = tamArquivo(bin_in);
	int tam_pagina = 0;
	int pos = 0;
	int n_reg = 0;

	leCabecalho(bin_in, rc);

	rc_ind->status = 0;
	rc_ind->nroRegistros = 0;

	IndCabToArqBin(rc_ind, bin_indice); //primeira pagina preenchida

	while(ftell(bin_in) < tam_bin_in){
		limpa_registro_dados(rd);
		pos = ftell(bin_in);
		rd->nomeServidor[0] = '\0';
		leUmRegistroBin(bin_in, rd, &tam_pagina);
		if(rd->removido == '-' && rd->nomeServidor[0] != '\0'){
			n_reg++;
			rd_ind = realloc(rd_ind, n_reg*sizeof(REGDADOSIND));
			strcpy(rd_ind[n_reg-1].chaveBusca, rd->nomeServidor);
			rd_ind[n_reg-1].byteOffset = pos;
		}
	}

	MS_sort(rd_ind, n_reg, sizeof(REGDADOSIND), int_compare_byteOffset);
	MS_sort(rd_ind, n_reg, sizeof(REGDADOSIND), int_compare_chave);

	//passando os registros de dados do indice para o arquivo de indice	
	for (int i = 0; i < n_reg; ++i){
		regIndiceToArqBin(&rd_ind[i], bin_indice);
	}

	rc_ind->status = '1';
	rc_ind->nroRegistros = n_reg;

	//atualizando o registro de cabeçalho do indice no arquivo de indice
	fseek(bin_indice, 0, SEEK_SET); //voltando pro começo do arquivo
	fwrite(&rc_ind->status, STATUS_TAM, 1, bin_indice);
	fwrite(&rc_ind->nroRegistros, TAM_TAM, 1, bin_indice);

	fclose(bin_in); fclose(bin_indice);
	free(rc_ind); free(rd_ind);
	free(rc); free(rd);

}


//================ [11] CODIGOS PARA RECUPERAÇÃO DE DADOS ========================
/*
 * busca_eRecupera
 * Função: Busca um dado nomeServidor em um arquivo de indices e o recupera do arquivo de dados caso encontrado.
 * Parâmetros: 	
 				nomeBin_in: nome do arquivo de dados
 				nomeBin_indice: nome do arquivo de indice
 				nomeServidor: tipo da chave que será buscado
 				valor: valor da chave que será buscada
 				estatisticas: indica se devem ser realidadas estatisticas, se for 0 não serão realidades, e se for 1 serão realizadas.
*/
void busca_eRecupera(char* nomeBin_in, char *nomeBin_indice, char *nomeServidor, char *valor, int estatisticas){
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);
	FILE* bin_indice = fopen(nomeBin_indice, "rb+");
	check_file_status(bin_indice);

	int tam_bin_in = tamArquivo(bin_in);
	int *paginasAcessadas = calloc(sizeof(int), (tam_bin_in/TAM_PAG_DISCO + 1));
	int numPaginasAcessadas = 1;
	int n_paginas_dados = 1;
	int n_paginas_indice;
	int tam_pag = 0;
	int achei = 0;

	REGCABIND *rc_ind = calloc(1, sizeof(REGCABIND));
	REGCAB *rc = calloc(1, sizeof(REGCAB));
	
	leCabecalho(bin_in, rc);
	fseek(bin_in, TAM_PAG_DISCO, SEEK_SET);
	busca_nome(bin_in, tam_bin_in, valor, &numPaginasAcessadas, &tam_pag, rc, &achei);
	numPaginasAcessadas++;	

	rewind(bin_indice);

	fread(&rc_ind->status,STATUS_TAM,1,bin_indice);
	fread(&rc_ind->nroRegistros,TAM_TAM,1,bin_indice);

	REGDADOSIND *rd_ind = calloc(rc_ind->nroRegistros, sizeof(REGDADOSIND));
	
	fseek(bin_indice, TAM_PAG_DISCO, SEEK_SET);

	//carregando o indice para a memoria
	for (int i = 0; i < rc_ind->nroRegistros; ++i){
		fread(&rd_ind[i].chaveBusca,TAM_CHAVE,1,bin_indice);		
		fread(&rd_ind[i].byteOffset,TAM_BYTEOFFSET,1,bin_indice);
		if(strcmp(rd_ind[i].chaveBusca, valor) == 0){
			paginasAcessadas[rd_ind[i].byteOffset/TAM_PAG_DISCO] = 1;
		}
	}

	n_paginas_indice = ceil((128.0 * (double)rc_ind->nroRegistros) / 32000.0);

	for(int i=0; i <= (tam_bin_in/TAM_PAG_DISCO); i++){
		if(paginasAcessadas[i]){
			n_paginas_dados++;
		} 
	}
	
	int tam_lista = rc_ind->nroRegistros;
	n_paginas_indice++; //contando a pagina de cabeçalho
	int ind = buscaBinaria(valor, rd_ind, 0, tam_lista-1); //fazendo busca binaria pra achar o nome

	//buscar no arquivo de dados o registro encontrado
	if(estatisticas == 1){
		int p_dados = n_paginas_dados;
		if(ind == -1){
			printf("Registro inexistente.\n");
			p_dados = 1; //pra contar a pagina de cabeçalho
		}
		printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", n_paginas_indice);
		printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", p_dados);
		int estat = numPaginasAcessadas - p_dados;
		printf("\nA diferença no número de páginas de disco acessadas: %d\n", estat);
	}
	else{
		if(ind != -1){
			printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", n_paginas_indice);
			printf("Número de páginas de disco para acessar o arquivo de dados: %d", n_paginas_dados);
		}
		else{
			printf("Registro inexistente.\n");
		}	
	}

	fclose(bin_indice);
	free(rc_ind); free(rd_ind);
	free(rc); fclose(bin_in); free(paginasAcessadas);
}


//================ [12] CODIGOS PARA REMOÇÃO NO INDICE ============================
/*
 * busca_RemoveChave_indice
 * Função: Busca um dado nomeServidor em um arquivo de indices e, se encontrado, o remove do arquivo de indice e de dados.
 * Parâmetros: 	
 				nomeBin_in: nome do arquivo de dados
 				nomeBin_indice: nome do arquivo de indice
 				nomeServidor: tipo da chave que será buscado
 				valor: valor da chave que será buscada
 				estatisticas: indica se devem ser realidadas estatisticas, se for 0 não serão realidades, e se for 1 serão realizadas.
*/
void busca_RemoveChave_indice(REGDADOSIND *rd_ind, REGCABIND *rc_ind, char *valorCampo){
	int ind = buscaBinaria(valorCampo, rd_ind, 0, rc_ind->nroRegistros-1);

	if(ind != -1){
		for (int i = ind; i < (rc_ind->nroRegistros-1); ++i)
		{
			rd_ind[i] = rd_ind[i+1];
		}
		rc_ind->nroRegistros--;
	}
}

/*
 * listaIndice_toArqIndice
 * Função: Transcreve uma lista de structs de indices em um arquivo de indices.
 * Parâmetros: 	
 				nomeBin_indice: nome do arquivo de indice
 				rd_ind: struct do registro de dados de indice
 				rc_ind: struct do registro de cabeçalho de indice
*/
void listaIndice_toArqIndice(char *nomeBin_indice, REGDADOSIND *rd_ind, REGCABIND *rc_ind){
	FILE* bin_indice = fopen(nomeBin_indice, "wb");

	IndCabToArqBin(rc_ind, bin_indice); //primeira pagina preenchida

	//passando os registros de dados do indice para o arquivo de indice	
	for (int i = 0; i < rc_ind->nroRegistros; ++i){
		regIndiceToArqBin(&rd_ind[i], bin_indice);
	}

	fclose(bin_indice);
}

//================ [14] CODIGOS PARA ESTATISTICAS ==================================
/*
 * calculaEstatisticas
 * Função: Calcula estatísticas considerando a recuperação dos dados de todos os registros 
 		que satisfaçam um critério de busca determinado pelo usuário sobre o campo nomeServidor.

 * Parâmetros: 	
 				nomeBin_in: nome do arquivo de dados.
 				nomeBin_indice: nome do arquivo de indice.
 				nomeCampo: nome do campo a ser buscado.
 				valorCampo: valor do campo a ser buscado.
*/
void calculaEstatisticas(char *nomeBin_in, char *nomeBin_indice, char *nomeCampo, char *valorCampo){
	printf("*** Realizando a busca sem o auxílio de índice\n");
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);

	int tam_bin_in = tamArquivo(bin_in);
	int numPaginasAcessadas = 1;
	int tam_pag = 0;
	int achei = 0;

	REGCAB *rc = calloc(1, sizeof(REGCAB));
	leCabecalho(bin_in, rc);
	fseek(bin_in, TAM_PAG_DISCO, SEEK_SET);

	busca_nome(bin_in, tam_bin_in, valorCampo, &numPaginasAcessadas, &tam_pag, rc, &achei);
	if(achei == 0){
		printf("Registro inexistente.\n");
	}
	printf("Número de páginas de disco acessadas: %d\n", numPaginasAcessadas+1);

	printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
	busca_eRecupera(nomeBin_in, nomeBin_indice, nomeCampo, valorCampo, 1);

	free(rc); fclose(bin_in);
}