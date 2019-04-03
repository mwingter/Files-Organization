/*
 * Michelle Wingter da Silva
 * 10783243
 */

/*
fopen, fread, fwrite, fseek, fprintf, fscanf, fclose

Funções úteis:
	fread(buffer, n, 1, file)
	fwrite(buffer, n, 1, file)
	fscanf(file, "%14s", str) //Ler data:
	fscanf(file, "%d", n) //Ler indicadores de tamanho
	fseek(file, x, SEEK_CUR | SEEK_SET | SEEK_END) //pular x bytes

	Exemplo de Makefile:
	all:
		gcc *.c -o programaTrab1
	run:
		./programaTrab1
*/

int main(){


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






	 return 0;
}
