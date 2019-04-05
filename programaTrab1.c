/*
 *	Trabalho 1
 *	SCC0215 - Organização de Arquivos
 * 	Este trabalho tem como objetivo armazenar dados em um arquivo binário de
 * acordo com uma organização de campos e registros, bem como recuperar os
 * dados armazenados.
 * Nome: Michelle Wingter da Silva
 * nUSP: 10783243
 */

#include <stdio.h>

	//Struct do Registro de dados
	typedef struct regdados_ REGDADOS; //organização híbrida de campos e registros
										
										
	struct regdados_{			//Campos de tam fixo = 26 bytes:
		int idServidor; 		//4 bytes
		double salarioServidor; //8 bytes
		char telefoneServidor[14]; //14 bytes, no formato (DD)NNNNN-NNNN
								//Campos de tam variavel:
		int tamNomeServidor; //tam variavel
		char *nomeServidor; //tam variavel
		/*
		int tamCargoSuperior;
		char *cargoSuperior;
		*/
		char removido;	/*removido: indica se o registro se encontra removido ou não. Pode assumir os valores ‘*’, 
						para indicar que o registro é um registro removido, ou ‘-’, para indicar que o registro não 
						é um registro removido – tamanho: string de 1 byte.*/
		int tamanhoRegistro; //indica o tamanho do registro – tamanho: inteiro de 4 bytes.
		double encadeamentoLista; // armazena os byte offsets dos registros logicamente removidos – tamanho: inteiro de 8 bytes.

	};


	//Struct do Registro de Cabeçalho
	typedef struct redcab_ REGCAB; //tam = 214 bytes, e ocupa uma pagina de disco, mas como ele é menor q a pagina, completar a pagina com '@'
	struct redcab_{
		char status; /*indica a consistência do arquivo de dados, devido à queda de energia, travamento do programa, etc. 
					  *	Pode assumir os valores 0, para indicar que o arquivo de dados está inconsistente, ou 1, para 
					  *	indicar que o arquivo de dados está consistente. Ao se abrir um arquivo para escrita, seu status 
					  *	deve ser 0 e, ao finalizar o uso desse arquivo, seu status deve ser 1*/

		int topoLista; //sempre igual à -1

		char tags[5];  /*	*tagCampo1: idServidor. Deve assumir o valor i – tam: string de 1 byte.
							*tagCampo2: salarioServidor. Deve assumir o valor s – tam: string de 1 byte.
							*tagCampo3: telefoneServidor. Deve assumir o valor t – tam: string de 1 byte.
							*tagCampo4: nomeServidor. Deve assumir o valor n – tam: string de 1 byte.
							*tagCampo5: cargoServidor. Deve assumir o valor c – tam: string de 1 byte.
						*/
						
		char campos[5][40]; /*	*desCampo1: idServidor. Deve assumir o valor ‘numero de identificacao do servidor’ – tam: string de 40 bytes.
								*desCampo2: salarioServidor. Deve assumir o valor ‘salario do servidor’ – tam: string de 40 bytes.
								*desCampo3: telefoneServidor. Deve assumir o valor ‘telefone celular do servidor’ – tam: string de 40 bytes.
								*desCampo4: nomeServidor. Deve assumir o valor ‘nome do servidor’ – tam: string de 40 bytes.
								*desCampo5: cargoServidor. Deve assumir o valor ‘cargo do servidor’ – tam: string de 40 bytes.
							*/
	};

	//Struct do Buffer-Pool
	typedef struct buffer_ BUFFER;
	struct buffer_{
		int nPaginas;
		char paginas[10][32000]; //x a definir
	};




int main(){
	/* Descrição Geral: Implemente um programa em C por meio do qual o usuário possa
obter dados de um arquivo de entrada e gerar um arquivo binário com esses dados,
bem como exibir os dados armazenados no arquivo binário. Deve-se levar em
consideração a descrição e a organização do arquivo de dados especificados
anteriormente. */

	FILE *fp;
	//FILE *fopen (char *nome_do_arquivo, char *modo_de_acesso);
	//No arquivo csv, o separador de campos é vírgula (,).
	fp = fopen ("SCC0215012019trabalho1-BCC-A.csv", "r");

	/* TESTE PRA VER SE O ARQUIVO FOI ABERTO
	if (fp == NULL) {
       printf ("Houve um erro ao abrir o arquivo.\n");
       return 1;
    }
    printf ("Arquivo SCC0215012019trabalho1-BCC-A criado com sucesso.\n");
    fclose (fp);
    return 0;
	*/

	 

	FILE *arqBinario;
	arqBinario = fopen ("", "wb");

	fprintf(arqBinario, "%s"); //printa no arquivo

	return 0;
}
