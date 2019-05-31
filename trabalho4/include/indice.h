/*
	TRABALHO 3 - ORGANIZAÇÃO DE ARQUIVOS

	Nome:	Michelle Wingter da Silva	nUSP:	10783243
			Juliano Fantozzi					9791218

*/

#ifndef INDICE_H
	
	#define INDICE_H
	#define TAM_IND_CAB 5
	#define TAM_IND_DADOS 128

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

#endif