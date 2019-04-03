/*
 *	Trabalho 1
 *	SCC0215 - Organização de Arquivos
 * 	Este trabalho tem como objetivo armazenar dados em um arquivo binário de
 * acordo com uma organização de campos e registros, bem como recuperar os
 * dados armazenados.
 * Nome: Michelle Wingter da Silva
 * nUSP: 10783243
 */

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
		char paginas[x][32000]; //x a definir
	};




int main(){





	return 0;
}
