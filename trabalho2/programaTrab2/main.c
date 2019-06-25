#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdados.h"
#include "manipulaReg.h"

/*
 * Funcionalidade [1]: Permite a leitura de vários registros obtidos a partir de um arquivo de entrada
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
 * Funcionalidade [2]: Permite a recuperação dos dados, de todos os registros, armazenados no arquivo de
dados, mostrando os dados de forma organizada na saída padrão para permitir a
distinção dos campos e registros.
*/
void leBin_PrintBin(){
	char nomeDoArquivo[MAX];
	scanf(" %[^\n\r]", nomeDoArquivo);

	lePrintaArqBin(nomeDoArquivo);

}
/*
 * Funcionalidade [3]: Permite a recuperação dos dados de todos os registros que satisfaçam um critério
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

/*
 * Funcionalidade [4]: Permite a remoção lógica de registros, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.
*/
void remove_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	char nomeCampo[MAX];
	char valorCampo[MAX];

	//int n_rem = 0;
	//REG_REM* listaRem = NULL;

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
 * Funcionalidade [5]: Permite a inserção de registros adicionais, baseado na abordagem dinâmica de
reaproveitamento de espaços de registros logicamente removidos.
*/
void insere_registro(){
	char nomeBin[MAX];
	int n; //n = numero de vezes que a funcionalidade 4 será executada
	scanf(" %s %d", nomeBin, &n);

	char idStr[MAX], salStr[MAX], tel[MAX], nome[MAX], cargo[MAX];
	//int id; double sal;
	REGDADOS *rd;

	long int ultimo_reg = -1;

	for (int i = 0; i < n; ++i)
	{
		scanf("%s ", idStr);
		scanf("%s ", salStr);
		scan_quote_string(tel);
		scan_quote_string(nome);
		scan_quote_string(cargo);
		//printf("id|%s|, sal|%s|, tel|%s|, nome|%s|, cargo|%s|\n", idStr, salStr, tel, nome, cargo);

		rd = calloc(1, sizeof(REGDADOS));
		//criaNovoRegDados(rd, id, sal, tel, nome, cargo);
		criaNovoRegDados2(rd, idStr, salStr, tel, nome, cargo);
//		printf("\n==========INSERINDO: \n\tid|%d|, sal|%lf|, tel|%s|, nome|%s|, cargo|%s|\n", rd->idServidor, rd->salarioServidor, rd->telefoneServidor, rd->nomeServidor, rd->cargoServidor);
//		printf("TAMANHOS: \n\ttel = %ld\n\tnome = %d\n\tcargo = %d\n\t", strlen(tel), rd->tamNomeServidor, rd->tamCargoServidor);
		firstFit_insere(nomeBin, rd, &ultimo_reg);
		free(rd);
	}

	binarioNaTela2(nomeBin);
}

/*
 * Funcionalidade [6]: Permite a atualização de registros. Os registros a serem atualizados devem ser
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
			//printf("\n====Quero atualizar:\n\tDe:\tCampo: %s, Valor: %s\n\tPara:\tCampo: %s, Novo Valor: %s====\n\n", nomeCampoBusca, valorCampoBusca, nomeCampoAtualiza, valorCampoAtualiza);
			busca_atualiza_campo(nomeBin, nomeCampoBusca, valorCampoBusca, nomeCampoAtualiza, valorCampoAtualiza, &ultimo_reg);
		}
	}

	binarioNaTela2(nomeBin);
}

void menu()
{
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