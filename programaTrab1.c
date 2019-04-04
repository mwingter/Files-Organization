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
	typedef struct regdados_ REGDADOS;
	struct regdados_{
		int idServidor;
		double salarioServidor;
		char telefoneServidor[14];
		int tamNomeServidor;
		char *nomeServidor;
		int tamCargoSuperior;
		char *cargoSuperior;
	};


	//Struct do Registro de Cabeçalho
	typedef struct redcab_ REGCAB;
	struct redcab_{
		char status;
		int topoLista; //sempre igual à -1
		char tags[5];
		char campos[5][40];
	};

	//Struct do Buffer-Pool
	typedef struct buffer_ BUFFER;
	struct buffer_{
		int nPaginas;
		char paginas[10][32000]; //x a definir
	};




int main(){
	FILE *fp;
	//FILE *fopen (char *nome_do_arquivo, char *modo_de_acesso);
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



	return 0;
}
