#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdados.h"

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
	/*********CRIANDO REGISTRO DE DADOS********/
	REGDADOS* RD;
	RD = calloc (1, sizeof(REGDADOS));
	
	criaRegCabecalho(fp, RC);

	char nome_bin[MAX];
	strcpy(nome_bin, "arquivoTrab1.bin");
	FILE *arqBinario = fopen(nome_bin, "wb+");

	regCabToArqBin(RC, arqBinario);

	int tamPag = 32000;
	int tamRegAnt = 0;


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

void leBin_PrintBin(){
	char nomeDoArquivo[MAX];
	scanf(" %[^\n\r]", nomeDoArquivo);

	lePrintaArqBin(nomeDoArquivo);

}

void buscaBin_Print(){
	char nomeBin[MAX], nomeCampo[MAX], valor[MAX];
	scanf(" %[^\n\r] %[^\n\r] %[^\r\n]", nomeBin, nomeCampo, valor);

	FILE* bin = fopen(nomeBin, "r");

	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	

	int paginaAtual = 0;
	REGCAB* rc;
	leCabecalho(bin, &paginaAtual, rc);
	if (rc->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	//buscando registro e printando se encontrar
	buscaRegistro(bin, nomeDoCampo, valor, rc, &paginaAtual);

	free(h); fclose(bin);
}

void menu()
{
		/* FUNCIONALIDADES
[1] Permita a leitura de vários registros obtidos a partir de um arquivo de entrada
(arquivo no formato CSV) e a gravação desses registros em um arquivo de dados de
saída. Esse arquivo de dados é binário e deve ter, obrigatoriamente, o nome
arquivoTrab1.bin. O arquivo de entrada é fornecido juntamente com a especificação
do projeto, enquanto que o arquivo de dados de saída deve ser gerado como parte deste
trabalho prático.

[2] Permita a recuperação dos dados, de todos os registros, armazenados no arquivo de
dados, mostrando os dados de forma organizada na saída padrão para permitir a
distinção dos campos e registros. O tratamento de ‘lixo’ deve ser feito de forma a
permitir a exibição apropriada dos dados. Depois de mostrar todos os registros, deve
ser mostrado na saída padrão o número de páginas de disco acessadas.

[3] Permita a recuperação dos dados de todos os registros que satisfaçam um critério
de busca determinado pelo usuário. Por exemplo, o usuário pode solicitar a exibição
de todos os registros de um determinado número de identificação do servidor.
Qualquer campo pode ser utilizado como forma de busca. Essa funcionalidade pode
retornar 0 registros (quando nenhum satisfaz ao critério de busca), 1 registro ou vários
registros.*/
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
		default:
			printf("Opcao invalida.\n");
	}

}

int main(){

	menu();
	
	return 0;
}