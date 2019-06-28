/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "funcionalidades.h"

#include "rdados.h"
#include "manipulaReg.h"
#include "organizaArq.h"
#include "indice.h"
#include "arvoreB.h"



/* Funcionalidade [1]: Permite a leitura de vários registros obtidos a partir de um arquivo de entrada
(arquivo no formato CSV) e a gravação desses registros em um arquivo de dados de
saída.
*/
void leCsv_SalvaBin(){
	char nomeDoArquivo[MAX];
	scanf(" %[^\n\r]", nomeDoArquivo);
	FILE *fp;
	fp = fopen (nomeDoArquivo, "r");
	
	if (fp == NULL) {
		printf("Falha no carregamento do arquivo.\n");	
		return;
	}
	
	/*********CRIANDO REGISTRO DE CABEÇALHO********/
	REGCAB *RC;
	RC = calloc (1, sizeof(REGCAB));
	
	criaRegCabecalho(fp, RC);

	char nome_bin[MAX];
	strcpy(nome_bin, "arquivoTrab1.bin");
	FILE *arqBinario = fopen(nome_bin, "wb+");

	regCabToArqBin(RC, arqBinario);

	int tamPag = 32000;
	int tamRegAnt = 0;

	/*********CRIANDO REGISTRO DE DADOS********/
	REGDADOS* RD;
	RD = calloc (1, sizeof(REGDADOS));

	while(!feof(fp))
	{
		salvaRegistroNaStruct(fp, RD);
		structToBin(&tamPag, &tamRegAnt, RD, RC, arqBinario);
		free(RD);
		RD = calloc (1, sizeof(REGDADOS));
	}
	atualizaStatus(RC, arqBinario);

	printf("%s",nome_bin);
	free(RD); free(RC);
	fclose(fp); fclose(arqBinario);
}

/* Funcionalidade [2]: Permite a recuperação dos dados, de todos os registros, armazenados no arquivo de
dados, mostrando os dados de forma organizada na saída padrão para permitir a
distinção dos campos e registros.
*/
void leBin_PrintBin(){
	char nomeDoArquivo[MAX];
	scanf(" %[^\n\r]", nomeDoArquivo);

	lePrintaArqBin(nomeDoArquivo);
}

/* Funcionalidade [3]: Permite a recuperação dos dados de todos os registros que satisfaçam um critério
de busca determinado pelo usuário.
*/
void buscaBin_Print(){
	char nomeBin[MAX], nomeCampo[MAX], valor[MAX];
	scanf(" %s %s %[^\r\n]", nomeBin, nomeCampo, valor);

	FILE* bin = fopen(nomeBin, "rb");

	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	REGCAB* RC;
	RC = calloc(1, sizeof(REGCAB));

	leCabecalho(bin, RC); //lendo o cabeçalho da primeira pagina do arquivo

	if (RC->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		free(RC); fclose(bin);
		return;
	}

	buscaRegBin(bin, RC, nomeCampo, valor); //buscando o registro

	free(RC);
	fclose(bin);
}


/* Funcionalidade [4]: Permite a remoção lógica de registros, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.
*/
void remove_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	char nomeCampo[MAX];
	char valorCampo[MAX];

	scanf(" %s %d", nomeBin, &n);

	for (int i = 0; i < n; ++i){
		scanf("%s", nomeCampo);
		scan_quote_string(valorCampo);
		busca_RemoveReg(nomeBin, nomeCampo, valorCampo);
	}
	binarioNaTela2(nomeBin);
}


/* Funcionalidade [5]: Permite a inserção de registros adicionais, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.
*/
void insere_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	scanf(" %s %d", nomeBin, &n);

	char idStr[MAX], salStr[MAX], tel[MAX], nome[MAX], cargo[MAX];
	REGDADOS *rd;

	long int ultimo_reg = -1;

	for (int i = 0; i < n; ++i){
		scanf("%s ", idStr);
		scanf("%s ", salStr);
		scan_quote_string(tel);
		scan_quote_string(nome);
		scan_quote_string(cargo);

		rd = calloc(1, sizeof(REGDADOS));
		
		criaNovoRegDados2(rd, idStr, salStr, tel, nome, cargo);
		firstFit_insere(nomeBin, rd, &ultimo_reg);
		
		free(rd);
	}

	binarioNaTela2(nomeBin);
}


/* Funcionalidade [6]: Permite a atualização de registros. Os registros a serem atualizados devem ser
aqueles que satisfaçam um critério de busca determinado pelo usuário.
*/
void atualiza_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	long int ultimo_reg = -1;
	scanf(" %s %d", nomeBin, &n);

	char nomeCampoBusca[MAX]; char valorCampoBusca[MAX];
	char nomeCampoAtualiza[MAX]; char valorCampoAtualiza[MAX];

	for (int i = 0; i < n; ++i){
		scanf("%s", nomeCampoBusca);
		if(strcmp(nomeCampoBusca, "idServidor") == 0 || strcmp(nomeCampoBusca, "salarioServidor") == 0){
			scanf("%s", valorCampoBusca);
		}
		else{
			scan_quote_string(valorCampoBusca);
		}
		scanf("%s", nomeCampoAtualiza);
		if(strcmp(nomeCampoAtualiza, "idServidor") == 0 || strcmp(nomeCampoAtualiza, "salarioServidor") == 0){
			scanf("%s", valorCampoAtualiza);
		}
		else{
			scan_quote_string(valorCampoAtualiza);
		}

		if(strcmp(nomeCampoBusca, nomeCampoAtualiza) != 0 && strcmp(valorCampoBusca, valorCampoAtualiza) != 0){ //verifica se os campos são o mesmo, se for não faz nada
			busca_atualiza_campo(nomeBin, nomeCampoBusca, valorCampoBusca, nomeCampoAtualiza, valorCampoAtualiza, &ultimo_reg);
		}
	}
	binarioNaTela2(nomeBin);
}


/* Funcionalidade [7] Realiza a ordenação interna dos dados de um arquivo de dados, considerando os
valores do campo idServidor.
*/
void ordenacao_interna(){

	char nomeBin_in[MAX]; /*nome do arquivo binário de entrada que segue as mesmas especificações do primeiro trabalho prático, 
							e que contém dados desordenados e registros logicamente removidos.*/
	
	char nomeBin_out[MAX];/*nome do arquivo binário de saída que segue as mesmas especificações definidas no primeiro trabalho prático, e que contém dados 
							ordenados de forma crescente nos valores do campo idServidor e não contém registros logicamente removidos.*/

	scanf(" %s %s", nomeBin_in, nomeBin_out);

	ordena_por_id(nomeBin_in, nomeBin_out);

	binarioNaTela2(nomeBin_out);
}


/* Funcionalidade [8] Realiza a operação cosequencial de merging (união) de dois arquivos de dados,
considerando os valores do campo idServidor.
*/
void merge_twoFiles(){
	
	char nomeBin_in1[MAX]; /*- arquivoEntrada1.bin e arquivoEntrada2.bin são arquivos binários de entrada que seguem as mesmas especificações 
							do primeiro trabalho prático, e que foram ordenados de acordo com as especificações da funcionalidade [7].*/
	char nomeBin_in2[MAX];
	
	char nomeBin_out[MAX]; /*nome do arquivo binário de saída que segue as mesmas especificações definidas no primeiro trabalho prático, e que 
						contém dados ordenados de forma crescente nos valores do campo idServidor e não contém registros logicamente removidos.*/

	scanf(" %s %s %s", nomeBin_in1, nomeBin_in2, nomeBin_out);

	read_andMerge(nomeBin_in1, nomeBin_in2, nomeBin_out);

	binarioNaTela2(nomeBin_out);
}


/* Funcionalidade [9] Realiza a operação cosequencial de matching (interesecção) de dois arquivos de
dados, considerando os valores do campo idServidor.
*/			
void match_twoFiles(){

	char nomeBin_in1[MAX]; /*- arquivoEntrada1.bin e arquivoEntrada2.bin são arquivos binários de entrada que seguem as mesmas especificações 
							do primeiro trabalho prático, e que foram ordenados de acordo com as especificações da funcionalidade [7].*/
	char nomeBin_in2[MAX];
	
	char nomeBin_out[MAX]; /*nome do arquivo binário de saída que segue as mesmas especificações definidas no primeiro trabalho prático, 
							e que contém apenas todos os registros presentes em ambos arquivoEntrada1 e arquivoEntrada2 . Os registros de 
							arquivoSaida .bin encontram-se ordenados de forma crescente nos valores do campo idServidor. Não devem existir 
							registros repetidos no arquivo de saída..*/

	scanf(" %s %s %s", nomeBin_in1, nomeBin_in2, nomeBin_out);

	read_andMatch(nomeBin_in1, nomeBin_in2, nomeBin_out);

	binarioNaTela2(nomeBin_out);
}

/* Funcionalidade [10]: Cria um arquivo de índice secundário fortemente ligado para um arquivo de
dados de entrada já existente. O campo a ser indexado é nomeServidor.*/
void cria_arquivoIndice(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; //arquivo binário de índice secundário fortemente ligado que indexa o campo nomeServidor.

	scanf(" %s %s", nomeBin_in, nomeBin_indice);

	novoIndice(nomeBin_in, nomeBin_indice);

	binarioNaTela2(nomeBin_indice);
}

/* Funcionalidade [11]: Permite a recuperação dos dados de todos os registros que satisfaçam um critério
de busca determinado pelo usuário sobre o campo nomeServidor, usando o índice secundário fortemente ligado 
criado na funcionalidade [10].*/
void recuperaDados(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; //arquivo binário de índice secundário fortemente ligado que indexa o campo nomeServidor.
	char nomeServidor[MAX];
	char valor[MAX];

	scanf(" %s %s %s %[^\r\n]", nomeBin_in, nomeBin_indice, nomeServidor, valor);

	busca_eRecupera(nomeBin_in, nomeBin_indice, nomeServidor, valor, 0);
}

/* Funcionalidade [12]: Estenda a funcionalidade [4] descrita no segundo trabalho prático de forma que,
depois de cada remoção lógica no arquivo de dados, a chave de busca referente ao registro logicamente 
removido seja removida do índice secundário fortemente ligado criado na funcionalidade [10].*/
void removeChave(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; //arquivo binário de índice secundário fortemente ligado que indexa o campo nomeServidor.
	int n = 0; //n = numero de vezes que a funcionalidade 12 será executada
	char nomeCampo[MAX];
	char valorCampo[MAX];

	scanf(" %s %s %d", nomeBin_in, nomeBin_indice, &n);

	FILE* bin_indice = fopen(nomeBin_indice, "rb");
	check_file_status(bin_indice);

	//primeiro, carrega-se o indice do disco para a memoria primaria
	REGCABIND *rc_ind = calloc(1, sizeof(REGCABIND));

	rewind(bin_indice);

	fread(&rc_ind->status,STATUS_TAM,1,bin_indice);
	fread(&rc_ind->nroRegistros,TAM_TAM,1,bin_indice);

	REGDADOSIND *rd_ind = calloc(rc_ind->nroRegistros, sizeof(REGDADOSIND));
	
	fseek(bin_indice, TAM_PAG_DISCO, SEEK_SET);
	
	for (int i = 0; i < rc_ind->nroRegistros; ++i){
		fread(&rd_ind[i].chaveBusca,TAM_CHAVE,1,bin_indice);		
		fread(&rd_ind[i].byteOffset,TAM_BYTEOFFSET,1,bin_indice);
	}
	fclose(bin_indice);
	//=============================================================


	for (int i = 0; i < n; ++i){
		scanf("%s", nomeCampo);
				if(strcmp(nomeCampo, "idServidor") == 0 || strcmp(nomeCampo, "salarioServidor") == 0){
			scanf("%s", valorCampo);
		}
		else{
			scan_quote_string(valorCampo);
		}

		busca_RemoveReg(nomeBin_in, nomeCampo, valorCampo); //rodando a funcionalidade 4
		busca_RemoveChave_indice(rd_ind, rc_ind, valorCampo);
	}

	listaIndice_toArqIndice(nomeBin_indice, rd_ind, rc_ind); //reescrevendo o indice do zero

	free(rc_ind); free(rd_ind);

	binarioNaTela2(nomeBin_indice);
}

/* Funcionalidade [13]: Estende a funcionalidade [5] descrita no segundo trabalho prático de forma que,
depois de cada inserção de registro adicional no arquivo de dados, a chave de busca referente ao registro 
inserido seja inserida no índice secundário fortemente ligado criado na funcionalidade [10]. */
void insereChave(){
	char nomeBin[MAX];
	char nomeBin_indice[MAX]; //arquivo binário de índice secundário fortemente ligado que indexa o campo nomeServidor.
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	scanf(" %s %s %d", nomeBin, nomeBin_indice, &n);

	//primeiro, carrega-se o indice do disco para a memoria primaria
	FILE* bin_indice = fopen(nomeBin_indice, "rb");
	check_file_status(bin_indice);

	REGCABIND *rc_ind = calloc(1, sizeof(REGCABIND));

	rewind(bin_indice);
	fread(&rc_ind->status,STATUS_TAM,1,bin_indice);
	fread(&rc_ind->nroRegistros,TAM_TAM,1,bin_indice);

	REGDADOSIND *rd_ind = calloc(rc_ind->nroRegistros, sizeof(REGDADOSIND));
	
	fseek(bin_indice, TAM_PAG_DISCO, SEEK_SET);
	
	for (int i = 0; i < rc_ind->nroRegistros; ++i){
		fread(&rd_ind[i].chaveBusca,TAM_CHAVE,1,bin_indice);		
		fread(&rd_ind[i].byteOffset,TAM_BYTEOFFSET,1,bin_indice);
	}
	fclose(bin_indice);
	//=============================================================

	char idStr[MAX], salStr[MAX], tel[MAX], nome[MAX], cargo[MAX];
	REGDADOS *rd = NULL;
	REGDADOSIND *novo = NULL;

	long int ultimo_reg = -1;

	FILE *bin_in = fopen(nomeBin, "rb");
	fseek(bin_in, 0, SEEK_END);
	long int pos = ftell(bin_in);
	fclose(bin_in);

	for (int i = 0; i < n; ++i){
		scanf("%s ", idStr);
		scanf("%s ", salStr);
		scan_quote_string(tel);
		scan_quote_string(nome);
		scan_quote_string(cargo);

		if(strcmp(nome, "") != 0){
			rd = calloc(1, sizeof(REGDADOS));
			novo = calloc(1, sizeof(REGDADOSIND));
			
			criaNovoRegDados2(rd, idStr, salStr, tel, nome, cargo);
			firstFit_insere(nomeBin, rd, &ultimo_reg); //inserindo no arquivo de dados

			strcpy(novo->chaveBusca, rd->nomeServidor);
			novo->byteOffset = pos;
			rc_ind->nroRegistros++;
			
			rd_ind = realloc(rd_ind, rc_ind->nroRegistros * sizeof(REGDADOSIND));
			strcpy(rd_ind[rc_ind->nroRegistros - 1].chaveBusca, novo->chaveBusca);
			rd_ind[rc_ind->nroRegistros - 1].byteOffset = novo->byteOffset;
			pos = pos + rd->tamanhoRegistro;
			
			free(rd); free(novo);			
		}
	}

	//ordenando a lista de indices
	MS_sort(rd_ind, rc_ind->nroRegistros, sizeof(REGDADOSIND), int_compare_byteOffset);
	MS_sort(rd_ind, rc_ind->nroRegistros, sizeof(REGDADOSIND), int_compare_chave);

	//passando a lista de indice para o arquivo de indice
	listaIndice_toArqIndice(nomeBin_indice, rd_ind, rc_ind);

	binarioNaTela2(nomeBin_indice);
	free(rc_ind); free(rd_ind);
}

/* Funcionalidade [14]: Permite a realização de estatísticas considerando a recuperação dos dados de
todos os registros que satisfaçam um critério de busca determinado pelo usuário sobre o campo nomeServidor.*/
void estatisticas(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; //arquivo binário de índice secundário fortemente ligado que indexa o campo nomeServidor.
	char nomeCampo[MAX];
	char valorCampo[MAX];

	scanf(" %s %s %s %[^\r\n]", nomeBin_in, nomeBin_indice, nomeCampo, valorCampo);

	calculaEstatisticas(nomeBin_in, nomeBin_indice, nomeCampo, valorCampo);
}

/* Funcionalidade [15]: Cria um arquivo de índice árvore-B para um arquivo de dados de entrada já existente. */
void cria_indice_arvoreB(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; 

	scanf(" %s %s", nomeBin_in, nomeBin_indice);

	cria_arvoreB(nomeBin_in, nomeBin_indice);



	binarioNaTela2(nomeBin_indice);
}

/* Funcionalidade [16]: Permite a recuperação dos dados de todos os registros que satisfaçam um critério de busca determinado 
pelo usuário sobre o campo idServidor, usando o índice árvore-B criado na funcionalidade [15].*/
void recuperaDados_arvoreB(){
	char nomeBin_in[MAX];
	char nomeBin_indice[MAX]; 
	char idServidor[MAX];
	int valor;

	scanf(" %s %s %s %d", nomeBin_in, nomeBin_indice, idServidor, &valor);

	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);
	int nivel = 0;

	long int ind = busca_e_recupera_arvoreB(nomeBin_indice, valor, &nivel);
	
	if (ind != -1)
	{
		REGCAB *rc = calloc(1, sizeof(REGCAB));
		rewind(bin_in);
		leCabecalho(bin_in, rc);

		fseek(bin_in, ind, SEEK_SET);

		int tam_pagina = 0;
		REGDADOS *rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin_in, rd, &tam_pagina);
		printaRegEncontrado(rc, rd);
		printf("Número de níveis do índice árvore-B percorridos: %d\n", nivel);
	}
	else{
		printf("Registro inexistente.\n");	
	}
	fclose(bin_in);
	
}