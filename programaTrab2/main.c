#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdados.h"
#include "manipulaReg.h"

/*
 * Funcionalidade 1
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
		//printf("|%c|%d|%ld|%d|%lf|%s|%d|%c|%s|%d|%c|%s|\n\n", RD->removido, RD->tamanhoRegistro, RD->encadeamentoLista, RD->idServidor, 
		//RD->salarioServidor, RD->telefoneServidor, RD->tamNomeServidor, RC->tags[3], RD->nomeServidor, RD->tamCargoServidor, RC->tags[4], RD->cargoServidor);
		structToBin(&tamPag, &tamRegAnt, RD, RC, arqBinario);
		free(RD);
		RD = calloc (1, sizeof(REGDADOS));
		//RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
	}
	atualizaStatus(RC, arqBinario);

	printf("%s",nome_bin);
	free(RD); free(RC);
	fclose(fp); fclose(arqBinario);
}
/*
 * Funcionalidade 2
*/
void leBin_PrintBin(){
	char nomeDoArquivo[MAX];
	scanf(" %[^\n\r]", nomeDoArquivo);

	lePrintaArqBin(nomeDoArquivo);

}
/*
 * Funcionalidade 3
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

/*
 * Funcionalidade 1
*/
void remove_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	char nomeCampo[MAX];
	char valorCampo[MAX];

	scanf(" %s %d", nomeBin, &n);

	//criando lista de removidos
	//NO_REG_REM* lista_rem;

	for (int i = 0; i < n; ++i)
	{
		//scanf("%s %s", nomeCampo, valorCampo);
		scanf("%s", nomeCampo);
		scan_quote_string(valorCampo);
		//printf("nome e valor|%s|%s|\n", nomeCampo, valorCampo);
		//lista_rem = calloc(1, sizeof(NO_REG_REM));
		busca_RemoveReg(nomeBin, nomeCampo, valorCampo);
	}
	//fclose(bin); 
	
	binarioNaTela2(nomeBin);
	//printf("\nListar o arquivo binário %s.", nomeBin);


	//free(lista_rem);
	
}

/*
 * Funcionalidade 1
 valorIdServidor 1 valorSalarioServidor 1 valorTelefoneServidor 1
valorNomeServidor 1 valorCargoServidor 1
*/
void insere_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	scanf(" %s %d", nomeBin, &n);

	char idStr[MAX], salStr[MAX], tel[MAX], nome[MAX], cargo[MAX];
	int id; double sal;
	REGDADOS *rd;

	for (int i = 0; i < n; ++i)
	{
		scanf("%s ", idStr);
		scanf("%s ", salStr);
		scan_quote_string(tel);
		scan_quote_string(nome);
		scan_quote_string(cargo);
		printf("id|%s|, sal|%s|, tel|%s|, nome|%s|, cargo|%s|\n", idStr, salStr, tel, nome, cargo);
		if(strcmp(idStr, "NULO") == 0){
			id = 0;
		}
		else{
			id = atoi(idStr);
		}
		if(strcmp(salStr, "NULO") == 0){
			sal = 0;
		}
		else{
			sal = atof(salStr);
		}

		rd = calloc(1, sizeof(REGDADOS));
		criaNovoRegDados(rd, id, sal, tel, nome, cargo);
		bestFit_insere(nomeBin, rd);
		free(rd);
	}

	//binarioNaTela2(nomeBin);
	//printf("Listar o arquivo binário %s.", nomeBin);

}

/*
 * Funcionalidade 1
*/
void atualiza_registro(){

}

void menu()
{
		/* FUNCIONALIDADES
	[4] Permita a remoção lógica de registros, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.

	[5] Permita a inserção de registros adicionais, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.

	[6] Permita a atualização de registros. Os registros a serem atualizados devem ser
aqueles que satisfaçam um critério de busca determinado pelo usuário.
*/
	int op;
	scanf("%d", &op);

	switch (op) {
		case 1:
			leCsv_SalvaBin();
			break;
		case 2:
			leBin_PrintBin();
			break;
		case 3:
			buscaBin_Print();
			break;
		case 4:
			remove_registro();
			break;
		case 5:
			insere_registro();
			break;
		case 6:
			atualiza_registro();
			break;
		default:
			printf("Opcao invalida.\n");
	}

}

int main(int argc, char const *argv[])
{
	menu();
	
	return 0;
}