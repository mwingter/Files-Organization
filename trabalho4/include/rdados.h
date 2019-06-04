/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#ifndef RDADOS_H

	#define RDADOS_H

	#include "rcab.h"

	#define MAX 150

	#define ID_TAM 	4
	#define SAL_TAM 8
	#define TEL_TAM 14
	#define TAM_TAM 4
	#define REM_TAM 1
	#define ENC_TAM	8

	// ***** STRUCT DO REGISTRO DE DADOS *****
	typedef struct regdados_ REGDADOS; //organização híbrida de campos e registros									
	struct regdados_{			
		//Campos de tam fixo = 26 bytes:
		int idServidor; 		//4 bytes
		double salarioServidor; //8 bytes
		char telefoneServidor[TEL_TAM]; //14 bytes, no formato (DD)NNNNN-NNNN
		//Campos de tam variavel:
		int tamNomeServidor; //4 bytes
		char nomeServidor[MAX]; //tam variavel
		
		int tamCargoServidor; //4 bytes
		char cargoServidor[MAX]; //tam variavel
		
		char removido;	/*removido: indica se o registro se encontra removido ou não. Pode assumir os valores ‘*’, 
						para indicar que o registro é um registro removido, ou ‘-’, para indicar que o registro não 
						é um registro removido – tamanho: string de 1 byte.*/
		int tamanhoRegistro; //indica o tamanho do registro – tamanho: inteiro de 4 bytes.
		long int encadeamentoLista; // armazena os byte offsets dos registros logicamente removidos – tamanho: inteiro de 8 bytes.

	};

	void salvaRegistroNaStruct(FILE* f, REGDADOS* r);
	void structToBin(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin);
	void lePrintaArqBin(char nome[MAX]);
	int le_ePrinta_UmRegistro(FILE *bin, REGDADOS* t, int *tam_pag);
	int tamArquivo(FILE *arq);
	
	void buscaRegBin(FILE* bin, REGCAB* rc, char nomeCampo[MAX], char valor[MAX]);
	void busca_id (FILE* bin, int tam_bin, int id, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei);
	void busca_salario (FILE* bin, int tam_bin, double sal, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei);
	void busca_telefone (FILE* bin, int tam_bin, char tel[TEL_TAM], int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei);
	void busca_nome (FILE* bin, int tam_bin, char* nome, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei);
	void busca_cargo (FILE* bin, int tam_bin, char* cargo, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei);
	
	void limpa_registro_dados(REGDADOS *rd);
	void freadString(FILE* bin, char* str);
	int leUmRegistroBin(FILE*bin, REGDADOS* t, int *tamanhoPagina);
	void printaRegEncontrado(REGCAB* rc, REGDADOS* rd);
	void criaNovoRegDados(REGDADOS* rd, int id, double sal, char* tel, char* nome, char* cargo);
	void criaNovoRegDados2(REGDADOS* rd, char* idStr, char* salStr, char* tel, char* nome, char* cargo);

#endif