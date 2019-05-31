/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//================ [10] CODIGOS PARA CRIAÇÃO DE NOVO INDICE =====================
void novoIndice(char *nomeBin_in, char *nomeBin_indice){
	FILE* bin_in = fopen(nomeBin_in, "rb");
	check_file_status(bin_in);

	FILE* bin_indice = fopen(nomeBin_indice, "wb");

}


//================ [11] CODIGOS PARA RECUPERAÇÃO DE DADOS ========================
void busca_eRecupera(char *nomeBin_in, char *nomeBin_indice, char *nomeServidor, char *valor){


}


//================ [12] CODIGOS PARA REMOÇÃO NO INDICE ============================
void busca_RemoveChave(char *nomeBin, char *nomeCampo, char *valorCampo){


}


//================ [13] CODIGOS PARA INSERÇÃO NO INDICE ===========================
void firstFit_insereChave(char *nomeBin, REGDADOS *rd, long int *ultimo_reg){


}


//================ [15] CODIGOS PARA ATUALIZAÇÃO ==================================
void calculaEstatisticas(char *nomeBin_in, char *nomeBin_indice, char *nomeCampo, char *valorCampo){


}