/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#ifndef INDICE_H

	#include "organizaArq.h"
	#include "rdados.h"
	#include "rcab.h"
	
	#define INDICE_H
	#define TAM_IND_CAB 5
	#define TAM_IND_DADOS 128
	#define TAM_CHAVE 120
	#define TAM_BYTEOFFSET 8

	// ***** STRUCT DO REGISTRO DE CABEÇALHO *****
	typedef struct regcabind_ REGCABIND; //tamanho total = 5 bytes - ocupa uma pagina de disco, que deve ser preenchida com '@' até completar seu tamanho.
	struct regcabind_{
		char status;			/*status: indica a consistência do arquivo de índice, devido à queda de energia,
									travamento do programa, etc. Pode assumir os valores 0, para indicar que o
									arquivo de dados está inconsistente, ou 1, para indicar que o arquivo de dados
									está consistente. Ao se abrir um arquivo para escrita, seu status deve ser 0 e, ao
									finalizar o uso desse arquivo, seu status deve ser 1 – tamanho: string de 1 byte.*/
		
		int nroRegistros;		/*nroRegistros: indica a quantidade de registros do arquivo de índice (sem contar
									o registro de cabeçalho, ou seja, somente os registros de dados do arquivo de
									índice) – tamanho: inteiro de 4 bytes*/
	};

	typedef struct regdadosind_ REGDADOSIND; //tamanho total = 128 bytes
	struct regdadosind_{
		char chaveBusca[120];	/*chaveBusca: armazena a chave de busca do índice - tamanho: string de 120 bytes.*/
		
		long int byteOffset;	/*byteOffset: armazena o byte offset do registro que corresponde à chave de
									busca associada – tamanho: inteiro de 8 bytes.*/
	};

//================ FUNÇÕES AUXILIARES ==========================================
int int_compare_chave(const void *A, const void *B);
int int_compare_byteOffset(const void *A, const void *B);
void IndCabToArqBin(REGCABIND *rc_ind, FILE *bin_indice);
void regIndiceToArqBin(REGDADOSIND *rd_ind, FILE *bin_indice);

//================ [10] CODIGOS PARA CRIAÇÃO DE NOVO INDICE =====================
void novoIndice(char *nomeBin_in, char *nomeBin_indice);


//================ [11] CODIGOS PARA RECUPERAÇÃO DE DADOS ========================
void busca_eRecupera(char *nomeBin_in, char *nomeBin_indice, char *nomeServidor, char *valor, int estatisticas);


//================ [12] CODIGOS PARA REMOÇÃO NO INDICE ============================
void busca_RemoveChave_indice(REGDADOSIND *rd_ind, REGCABIND *rc_ind, char *valorCampo);
void listaIndice_toArqIndice(char *nomeBin_indice, REGDADOSIND *rd_ind, REGCABIND *rc_ind);


//================ [13] CODIGOS PARA INSERÇÃO NO INDICE ===========================
void firstFit_insereChave_indice(char *nomeBin, REGDADOS *rd, long int *ultimo_reg);


//================ [15] CODIGOS PARA ATUALIZAÇÃO ==================================
void calculaEstatisticas(char *nomeBin_in, char *nomeBin_indice, char *nomeCampo, char *valorCampo);

#endif