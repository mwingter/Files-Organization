#include "rcab.h"


#define MAX 100
#define TAM_PAG_DISCO 32000

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

void structToBin(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin);

void lePrintaArqBin(char nome[MAX]);

int tamArquivo(FILE *arq);

void salvaRegistroNaStruct(FILE* f, REGDADOS* r);

void buscaRegistro(FILE *bin, char nomeCampo[MAX], char valor[MAX], int tam_bin, REGCAB *rc, int pagAtual);

void printaReg_Busca(REGCAB* rc, REGDADOS* rd);

void leRegistroDoArqBin(FILE *bin, REGDADOS *rd);

int busca_id(FILE *bin, int id, int numPaginasAcessadas, REGCAB *rc, int paginaAtual);


