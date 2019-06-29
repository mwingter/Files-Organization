/*
	TRABALHO 5 - ORGANIZAÇÃO DE ARQUIVOS
	Obs: Para a continuação apartir do T3, foi utilizado o código de Michelle wingter da Silva.

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218
*/

#include "funcionalidades.h"
#include <stdio.h>

void menu(){
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
		case 7:
			ordenacao_interna();
			break;
		case 8:
			merge_twoFiles();
			break;
		case 9:
			match_twoFiles();
			break;
		case 10:
			cria_arquivoIndice();
			break;
		case 11:
			recuperaDados();
			break;
		case 12:
			removeChave();
			break;
		case 13:
			insereChave();
			break;
		case 14:
			estatisticas();
			break;
		case 15:
			cria_indice_arvoreB();
			break;
		case 16:
			recuperaDados_arvoreB();
			break;

		default:
			printf("Opcao invalida.\n");
	}
}

int main(int argc, char const *argv[]){

	menu();
	
	return 0;
}