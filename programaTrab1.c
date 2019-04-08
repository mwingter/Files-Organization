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
#include <stdlib.h>
#include <string.h>
#define MAX 1000

//Struct do Registro de dados
typedef struct regdados_ REGDADOS; //organização híbrida de campos e registros									
struct regdados_{			//Campos de tam fixo = 26 bytes:
	int idServidor; 		//4 bytes
	double salarioServidor; //8 bytes
	char telefoneServidor[14]; //14 bytes, no formato (DD)NNNNN-NNNN
							//Campos de tam variavel:
	int tamNomeServidor; //tam variavel
	char *nomeServidor; //tam variavel
	
	int tamCargoServidor;
	char *cargoServidor;
	
	char removido;	/*removido: indica se o registro se encontra removido ou não. Pode assumir os valores ‘*’, 
					para indicar que o registro é um registro removido, ou ‘-’, para indicar que o registro não 
					é um registro removido – tamanho: string de 1 byte.*/
	int tamanhoRegistro; //indica o tamanho do registro – tamanho: inteiro de 4 bytes.
	double encadeamentoLista; // armazena os byte offsets dos registros logicamente removidos – tamanho: inteiro de 8 bytes.

};


//Struct do Registro de Cabeçalho
typedef struct regcab_ REGCAB; //tam = 214 bytes, e ocupa uma pagina de disco, mas como ele é menor q a pagina, completar a pagina com '@'
struct regcab_{
	char status; /*indica a consistência do arquivo de dados, devido à queda de energia, travamento do programa, etc. 
				  *	Pode assumir os valores 0, para indicar que o arquivo de dados está inconsistente, ou 1, para 
				  *	indicar que o arquivo de dados está consistente. Ao se abrir um arquivo para escrita, seu status 
				  *	deve ser 0 e, ao finalizar o uso desse arquivo, seu status deve ser 1*/

	int topoLista; //sempre igual à -1

	char tags[6];  /*	*tagCampo1: valor resumido da tag idServidor. Deve assumir o valor i – tam: string de 1 byte.
						*tagCampo2: " " " salarioServidor. Deve assumir o valor s – tam: string de 1 byte.
						*tagCampo3: " " " telefoneServidor. Deve assumir o valor t – tam: string de 1 byte.
						*tagCampo4: " " " nomeServidor. Deve assumir o valor n – tam: string de 1 byte.
						*tagCampo5: " " " cargoServidor. Deve assumir o valor c – tam: string de 1 byte.
					*/
					
	char campos[6][40]; /*	*desCampo1: idServidor. Deve assumir o valor ‘numero de identificacao do servidor’ – tam: string de 40 bytes.
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
	char paginas[][32000]; //x a definir
};

void criaRegistros(FILE *f, REGDADOS* r){
	char ch;
	int contaVirgula = 0;
	int count = 0;
	//char* nome = malloc(sizeof(char));
	char* id = malloc(sizeof(char));
	char* sal = malloc(sizeof(char));
	char* tel = malloc(sizeof(char));


	//lendo o arquivo char a char
	while(ch != '\n' && contaVirgula < 5){
		ch = fgetc(f);
		//printf("ch = %c\n", ch);
		if(ch != ','){
		//campos de tam fixo: idServidor(4b), salarioServidor(8b), telefoneServidor(14b)	=> total(26b)
			if(contaVirgula == 0){ //id
				id = realloc(id, (count+1)*sizeof(char));
				id[count] = ch;
				//printf("Printando id....ch= %c, id= %s, count = %d\n", ch, id, count);
				count++;
			}	
			if(contaVirgula == 1){//salario
				sal = realloc(sal, (count+1)*sizeof(char));
				sal[count] = ch;
				//printf("Printando salario.... sal= %s, count = %d\n", sal, count);
				count++;
			}
			if(contaVirgula == 2){//telefone
				r->telefoneServidor[count] = ch;
				//printf("Printando telefone.... tel= %s, count = %d\n", r->telefoneServidor, count);
				count++;
			}
			
		//campos de tam variavel
			if(contaVirgula == 3){//nome
				r->nomeServidor = realloc(r->nomeServidor, (count+1)*sizeof(char));
				r->nomeServidor[count] = ch;
				count++;
			}	
			if(contaVirgula == 4){ //cargo
				r->cargoServidor = realloc(r->cargoServidor, (count+1)*sizeof(char));
				r->cargoServidor[count] = ch;
				count++;
			}

		}
		if(ch == ','){
			count = 0;
			contaVirgula++;
			//printf("\tConta virgula = %d\n", contaVirgula);
		}
		
		if(ch == '\n'){
			//buffer->nPaginas++;
			contaVirgula = 0;
			break;
		}
	}
	
	r->idServidor = atoi(id);
	r->salarioServidor = atoi(sal);
	//r->nomeServidor = realloc(r->nomeServidor, r->tamNomeServidor);
	//strcpy(r->nomeServidor, nome);
	r->tamNomeServidor = strlen(r->nomeServidor);
	r->tamCargoServidor = strlen(r->cargoServidor);
	
	r->removido = '-';
	r->tamanhoRegistro = 26 + r->tamNomeServidor + r->tamCargoServidor;
	r->encadeamentoLista = 0;

	printf("id = %d\nsalario = %lf\ntel = %s\nnome = %s\ncargo = %s\n", r->idServidor, r->salarioServidor, r->telefoneServidor, r->nomeServidor, r->cargoServidor);

	free(id); free(sal); free(tel);
			
}


int main(){
	/* Descrição Geral: Implemente um programa em C por meio do qual o usuário possa
obter dados de um arquivo de entrada e gerar um arquivo binário com esses dados,
bem como exibir os dados armazenados no arquivo binário. Deve-se levar em
consideração a descrição e a organização do arquivo de dados especificados
anteriormente. 
	*Dados no registro: idServidor, salario, telefone, nome, cargo 
	*No arquivo csv, o separador de campos é vírgula (,). 
	
	ORGANIZACAO HIBRIDA DE CAMPOS E REGISTROS:
	Campos
	- tamanho fixo
	- tamanho variável com indicador de	tamanho
	Registros de tamanho variável
	- separados por delimitadores
	- delimitador: # 
*/

	FILE *fp;
	//FILE *fopen (char *nome_do_arquivo, char *modo_de_acesso);
	fp = fopen ("SCC0215012019trabalho1-BCC-A.csv", "r");

	/* TESTE PRA VER SE O ARQUIVO FOI ABERTO
	if (fp == NULL) {
       printf ("Falha no carregamento do arquivo.");
       return 1;
    }
    printf ("Arquivo SCC0215012019trabalho1-BCC-A criado com sucesso.\n");
    fclose (fp);
	*/

	/*********CRIANDO REGISTRO DE CABEÇALHO********/
	REGCAB* RC;
	RC = malloc (sizeof(REGCAB));
	RC->status = 0;
	RC->topoLista = -1;
	RC->tags[1] = 'i';
	RC->tags[2] = 's';
	RC->tags[3] = 't';
	RC->tags[4] = 'n';
	RC->tags[5] = 'c';
	//RC->tags[5] = {'', 'i', 's', 't', 'n', 'c'}
	//RC->campos[1][0] = "numero de identificacao do servidor\0";
	sprintf(RC->campos[1], "numero de identificacao do servidor");
	//strcpy(RC->campos[1], "numero de identificacao do servidor")
	//RC->campos[2][0] = "salario do servidor";
	sprintf(RC->campos[2], "salario do servidor");
	//RC->campos[3][0] = "telefone celular do servidor";
	sprintf(RC->campos[3], "telefone celular do servidor");
	//RC->campos[4][0] = "nome do servidor";
	sprintf(RC->campos[4], "nome do servidor");
	//RC->campos[5][0] = "cargo do servidor";
	sprintf(RC->campos[5], "cargo do servidor");
	/**********************************************/
	
	/*printf("Campo: %s\n", &(RC->campos[1][0]));
	for (int i = 0; i < 40; ++i)
	{
		printf("%c", RC->campos[1][i]);
	}
	printf("\n");*/

	/*********CRIANDO REGISTRO DE DADOS********/
	REGDADOS* RD;
	RD = calloc (1, sizeof(REGDADOS));
	/*
	RD->idServidor = 0;
	RD->salarioServidor = 0;
	RD->tamCargoServidor = 0;
	RD->cargoServidor = malloc(sizeof(char));
	RD->tamNomeServidor = 0;
	RD->nomeServidor = malloc(sizeof(char));
	RD->removido = '-';
	RD->tamanhoRegistro = 0;
	RD->encadeamentoLista = 0;
	/**********************************************/

	BUFFER *B;
	B = malloc (sizeof(BUFFER));
	B->nPaginas = 0;
	B->paginas[B->nPaginas][32000];


	char texto_str[32000];

	//enquanto não for fim de arquivo o looping será executado e será impresso o texto
	//while(fgets(texto_str, 20, fp) != NULL)
	//	printf("%s", texto_str);

	fgets(texto_str, 40, fp);
	printf("%s\n", texto_str);
	/*
	fgets(texto_str, 100, fp);
	printf("%s", texto_str);
	*/

	//fread (void *buffer, int numero_de_bytes, int count, FILE *fp);
	//fread(&conteudo, sizeof(char), 100,pf); /* Le em conteudo o valor da variável armazenada anteriormente pf */


	 

	FILE *arqBinario;
	//arqBinario = fopen ("arqSaidaBinario", "wb");




	//criaRegistros(fp, RD);
	int countReg = 0;
	while (fgetc(fp) != EOF){
		RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
		criaRegistros(fp, &RD[countReg]);
		countReg++;
	}


	//fprintf(arqBinario, "%s", oqprintar); //printa no arquivo



	fclose(fp);
	free(RD->cargoServidor); free(RD->nomeServidor);
	free(RD); free(RC); free(B);
	printf("\nListar na saída padrão o arquivo binário gerado.\n");
	return 0;
}
