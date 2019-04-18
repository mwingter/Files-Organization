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
#define MAX 100

//Struct do Registro de dados
typedef struct regdados_ REGDADOS; //organização híbrida de campos e registros									
struct regdados_{			
	//Campos de tam fixo = 26 bytes:
	int idServidor; 		//4 bytes
	double salarioServidor; //8 bytes
	char telefoneServidor[14]; //14 bytes, no formato (DD)NNNNN-NNNN
	//Campos de tam variavel:
	int tamNomeServidor; //4 bytes
	char nomeServidor[MAX]; //tam variavel
	
	int tamCargoServidor; //4 bytes
	char cargoServidor[MAX]; //tam variavel
	
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

	double topoLista; //sempre igual à -1

	char tags[6];  /*	*tagCampo1: valor resumido da tag idServidor. Deve assumir o valor i – tam: string de 1 byte.
						*tagCampo2: " " " salarioServidor. Deve assumir o valor s – tam: string de 1 byte.
						*tagCampo3: " " " telefoneServidor. Deve assumir o valor t – tam: string de 1 byte.
						*tagCampo4: " " " nomeServidor. Deve assumir o valor n – tam: string de 1 byte.
						*tagCampo5: " " " cargoServidor. Deve assumir o valor c – tam: string de 1 byte.
					*/
					
	char campos[5][40]; /*	*desCampo1: idServidor. Deve assumir o valor ‘numero de identificacao do servidor’ – tam: string de 40 bytes.
							*desCampo2: salarioServidor. Deve assumir o valor ‘salario do servidor’ – tam: string de 40 bytes.
							*desCampo3: telefoneServidor. Deve assumir o valor ‘telefone celular do servidor’ – tam: string de 40 bytes.
							*desCampo4: nomeServidor. Deve assumir o valor ‘nome do servidor’ – tam: string de 40 bytes.
							*desCampo5: cargoServidor. Deve assumir o valor ‘cargo do servidor’ – tam: string de 40 bytes.
						*/
};

typedef struct pagina_ PAGINA;
struct pagina_ {
	char bytes[32000];
	int nBytes;
};

//Struct do Buffer-Pool
typedef struct buffer_ BUFFER;
struct buffer_{
	int nPaginas;
	PAGINA* paginas;
};


/*Criar função que escreve direto no disco!!!! Deletar essa struct de pagina de bosta, não precisa disso
*/

void salvaNaPagina(REGDADOS* r, REGCAB* c, BUFFER* b){
	//verificar se o tamanho do registro não ultrapassa o tamanho restante da pagina
	int tamRestantePagina = 32000 - b->paginas[b->nPaginas-1].nBytes;
	int tamRegAtual = r->tamanhoRegistro + 5;
//	printf("\n\t\ttam restante da pag antes= %d\n", tamRestantePagina);
//	printf("\t\t\ttam do registro + 5 = %d\n", r->tamanhoRegistro + 5);
//	printf("\tnPaginas antes= %d\n", b->nPaginas);
	if(tamRegAtual > tamRestantePagina){
		//printf("tam registro > tam restante da pag");
		//preenche o resto dos bytes da pagina com '@'
		//memset(&b->paginas->bytes[b->paginas->nBytes], '@', tamRestantePagina);
		b->nPaginas++;
		//printf("\tnNova pagina. Paginas depois= %d", b->nPaginas);
		b->paginas = realloc(b->paginas, (b->nPaginas)*sizeof(PAGINA));
		b->paginas[b->nPaginas - 1].nBytes = 0;
	}

	/*O tamanho do registro inclui: encadeamentoLista (8 bytes) + idServidor (4 bytes) + salarioServidor
(8 bytes) + telefoneServidor (14 bytes) + indicador de tamanho do campo (4
bytes) + tagCampo4 (1 byte) + nomeServidor (tamanho variável, incluindo o
'\0') + indicador de tamanho (4 bytes) + tagCampo5 (1 byte) + cargoServidor
(tamanho variável, incluindo o '\0').*/

	//r->removido(1), r->tamanhoRegistro(4), r->encadeamentoLista(8), r->idServidor(4), 
	//r->salarioServidor(8), r->telefoneServidor(14), r->tamNomeServidor(4), c->tags[3](1), 
	//r->nomeServidor(variavel), r->tamCargoServidor(4), c->tags[4](1), r->cargoServidor(variavel)
	b->paginas[b->nPaginas - 1].nBytes += 1; //nao conta no tam do registro
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->removido;
	//sprintf(b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->removido);

	b->paginas[b->nPaginas - 1].nBytes += 4; //nao conta no tam do registro
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->tamanhoRegistro;
	sprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->tamanhoRegistro);

	b->paginas[b->nPaginas - 1].nBytes += 8;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->encadeamentoLista;
	//sprintf(b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->encadeamentoLista);
	snprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes], 8, "%f", r->encadeamentoLista);

	b->paginas[b->nPaginas - 1].nBytes += 4;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->idServidor;
	sprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->idServidor);

	b->paginas[b->nPaginas - 1].nBytes += 8;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->salarioServidor;
	snprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes], 8, "%f", r->salarioServidor);

	b->paginas[b->nPaginas - 1].nBytes += 14;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = *r->telefoneServidor;
	//memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes], &r->telefoneServidor, 14);

	b->paginas[b->nPaginas - 1].nBytes += 4;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->tamNomeServidor;
	sprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->tamNomeServidor);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = c->tags[3];

	b->paginas[b->nPaginas - 1].nBytes += r->tamNomeServidor;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = *r->nomeServidor;
	//memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes], &r->nomeServidor, r->tamNomeServidor);

	b->paginas[b->nPaginas - 1].nBytes += 4;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = r->tamCargoServidor;
	sprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes],"%d",r->tamCargoServidor);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = c->tags[4];

	b->paginas[b->nPaginas - 1].nBytes += r->tamCargoServidor;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes] = *r->cargoServidor;
	//memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes], &r->cargoServidor, r->tamCargoServidor);

/*	for(int i = 0; i < b->paginas[b->nPaginas - 1].nBytes; i++){
		printf("%c",b->paginas[b->nPaginas - 1].bytes[i]);
	}*/

	//printf("%s",b->paginas[0].bytes);


	//memcpy(void *dest, const void *src, size_t n);

	tamRestantePagina = 32000 - b->paginas[b->nPaginas-1].nBytes;
//	printf("\t\ttam restante da pag depois= %d\n\n", tamRestantePagina);

}

void criaRegCabecalho(FILE* f, REGCAB* rc, BUFFER* b, FILE* bin){
	char ch = 'a';
	int contaVirgula = 0;
	int count = 0;
	char* c1 = malloc(sizeof(char));
	char* c2 = malloc(sizeof(char));
	char* c3 = malloc(sizeof(char));
	char* c4 = malloc(sizeof(char));
	char* c5 = malloc(sizeof(char));

	while(contaVirgula < 5){
		ch = fgetc(f);
		if(ch == '\n'){
			break;
		}
		if(ch != ','){
			if(contaVirgula == 0){
				c1 = realloc(c1, (count+2)*sizeof(char));
				c1[count] = ch;
				c1[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 1){
				c2 = realloc(c2, (count+2)*sizeof(char));
				c2[count] = ch;
				c2[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 2){
				c3 = realloc(c3, (count+2)*sizeof(char));
				c3[count] = ch;
				c3[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 3){
				c4 = realloc(c4, (count+2)*sizeof(char));
				c4[count] = ch;
				c4[count+1] = '\0';
				count++;
			}
			else if(contaVirgula == 4){
				c5 = realloc(c5, (count+2)*sizeof(char));
				c5[count] = ch;
				c5[count+1] = '\0';
				count++;
			}
		}
		else if(ch == ','){
			contaVirgula++;
			count = 0;
		}
	}

	strcpy(rc->campos[0], c1);
	strcpy(rc->campos[1], c2);
	strcpy(rc->campos[2], c3);
	strcpy(rc->campos[3], c4);
	strcpy(rc->campos[4], c5);
	//printf("c5 = %s, e no rc fica = %s\n", c5, rc->campos[4]);

	//completando os vazios com '@'
	for(int j = 0; j < 5; j++){
		for(int i = strlen(rc->campos[j]) + 1; i < 40; i++){
			rc->campos[j][i] = '@';
		}
	}

	//printf("\tREG CABEÇALHO: \n");
	//printf("\t%.40s\n\t%.40s\n\t%.40s\n\t%.40s\n\t%.40s\n\n", rc->campos[0], rc->campos[1], rc->campos[2], rc->campos[3], rc->campos[4]);
	
	rc->tags[0] = 'i';
	rc->tags[1] = 's';
	rc->tags[2] = 't';
	rc->tags[3] = 'n';
	rc->tags[4] = 'c';

	rc->status = '0'; //Ao se abrir um arquivo para escrita, seu status deve ser 0 
					//e, ao finalizar o uso desse arquivo, seu status deve ser 1

	rc->topoLista = -1;

	//status(1), topoLista(8), tag1(1), campo1(40), tag2(1), campo2(40),......., tag5(1), campo5(40) => totalBytes= 10+5+200 = 215
	//printf("|%c|%lf|%c|%s|%c|%s|%c|%s|%c|%s|%c|%s|\n", rc->status, rc->topoLista, rc->tags[0], rc->campos[0], rc->tags[1], rc->campos[1], rc->tags[2], rc->campos[2], rc->tags[3], rc->campos[3], rc->tags[4], rc->campos[4]);

	free(c1);free(c2);free(c3);free(c4);free(c5);

	//adicionar registro de cabeçalho na primeira página de disco (nessa pagina deve conter apenas o cabeçalho)
	b->nPaginas = 1;
	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->status;
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->status, 1);
	
	b->paginas[b->nPaginas - 1].nBytes += 8;
	//sprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1],"%lf",rc->topoLista);
	//snprintf(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], 8, "%f", rc->topoLista);
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->topoLista, 8);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->tags[0];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->tags[0], 1);

	b->paginas[b->nPaginas - 1].nBytes += 40;
	b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = *rc->campos[0];
	//memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->campos[0], 40);


	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->tags[1];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->tags[1], 1);

	b->paginas[b->nPaginas - 1].nBytes += 40;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = *rc->campos[1];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->campos[1], 40);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->tags[2];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->tags[2], 1);

	b->paginas[b->nPaginas - 1].nBytes += 40;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = *rc->campos[2];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->campos[2], 40);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->tags[3];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->tags[3], 1);

	b->paginas[b->nPaginas - 1].nBytes += 40;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = *rc->campos[3];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->campos[3], 40);

	b->paginas[b->nPaginas - 1].nBytes += 1;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = rc->tags[4];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->tags[4], 1);

	b->paginas[b->nPaginas - 1].nBytes += 40;
	//b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1] = *rc->campos[4];
	memcpy(&b->paginas[b->nPaginas - 1].bytes[b->paginas[b->nPaginas - 1].nBytes - 1], &rc->campos[4], 40);

	//printf("\nno vetor de struct: %s\n", rc->campos[0]);
	//printf("\nno buffer: %s\n\n", b->paginas[b->nPaginas - 1].bytes);

	int tamRegistroCab = b->paginas[b->nPaginas - 1].nBytes;
	int restoPagina = 32000 - tamRegistroCab;

	//*tamPagina += tamRegistroCab;

	fwrite(&rc->status, sizeof(char), 1, bin);
	fwrite(&rc->topoLista, sizeof(double), 1, bin);
	fwrite(&rc->tags[0], sizeof(char), 1, bin);
	fwrite(&rc->campos[0], 40*sizeof(char), 1, bin);
	fwrite(&rc->tags[1], sizeof(char), 1, bin);
	fwrite(&rc->campos[1], 40*sizeof(char), 1, bin);
	fwrite(&rc->tags[2], sizeof(char), 1, bin);
	fwrite(&rc->campos[2], 40*sizeof(char), 1, bin);
	fwrite(&rc->tags[3], sizeof(char), 1, bin);
	fwrite(&rc->campos[3], 40*sizeof(char), 1, bin);
	fwrite(&rc->tags[4], sizeof(char), 1, bin);
	fwrite(&rc->campos[4], 40*sizeof(char), 1, bin);


	char arroba = '@';
	for(int i = 0; i < restoPagina; i++){
		fwrite(&arroba, sizeof(char), 1, bin);
	}



	//printf("\tR.Cabeçalho - paginaDisco = %d,\t tamRegCab = %d,\t tamRestantePagina= %d \n\n", b->nPaginas, tamRegistroCab, restoPagina);

/*
	for(int i = 0; i < b->paginas[b->nPaginas - 1].nBytes; i++){
		printf("%c",b->paginas[b->nPaginas - 1].bytes[i]);
	}
*/
}

void structToBin(int* tamPagina, REGDADOS* r, REGCAB* c, FILE* bin){
	if((*tamPagina + r->tamanhoRegistro) > 32000){
		*tamPagina = 0;
		char arroba = '@';

		for(int i = 0; i < r->tamanhoRegistro; i++){
			fwrite(&arroba, sizeof(char), 1, bin);
		}
	}

	tamPagina += r->tamanhoRegistro;

	fwrite(&r->removido, sizeof(char), 1, bin);
	fwrite(&r->tamanhoRegistro, sizeof(int), 1, bin);
	fwrite(&r->encadeamentoLista, sizeof(double), 1, bin);
	fwrite(&r->idServidor, sizeof(int), 1, bin);
	fwrite(&r->salarioServidor, sizeof(double), 1, bin);
	fwrite(&r->telefoneServidor, 14*sizeof(char), 1, bin);

	if(r->nomeServidor != '\0'){
		fwrite(&r->tamNomeServidor, sizeof(int), 1, bin);
		fwrite(&c->tags[3], sizeof(char), 1, bin);
		fwrite(&r->nomeServidor, r->tamNomeServidor, 1, bin);
	}

	if(r->cargoServidor != '\0'){
		fwrite(&r->tamCargoServidor, sizeof(int), 1, bin);
		fwrite(&c->tags[4], sizeof(char), 1, bin);
		fwrite(&r->cargoServidor, r->tamCargoServidor, 1, bin);
	}

}

//função melhor que salva os registros do arquivo pras structs
void salvaRegistroNaStruct(FILE* f, REGDADOS* r){

	fscanf(f, "%d", &r->idServidor); //salvando o id

	fscanf(f, "%*c%lf", &r->salarioServidor); //pulando a virgula, e salvando o salario

	fscanf(f, "%*c%[^,]", r->telefoneServidor); //lendo string até a virgula e salvando o telefone

	fscanf(f, "%*c%[^,]", r->nomeServidor); //lendo string até a virgula e salvando o nome

	fscanf(f, "%*c%[^\n\r]", r->cargoServidor); //lendo string até o \n e \r e salvando o cargo

	//r->nomeServidor[strlen(r->nomeServidor)] = '\0';
	//r->cargoServidor[strlen(r->cargoServidor)] = '\0';

	if(r->idServidor == 0){
		r->idServidor = -1;
	}

	if(r->salarioServidor == 0){
		r->salarioServidor = -1;
	}

	if(r->telefoneServidor[0] != '('){
		r->telefoneServidor[0] = '\0';
		for(int i = 1; i < 14; i++){
			r->telefoneServidor[i] = '@';
		}
	}

	r->tamanhoRegistro = 8 + 4 + 8 + 14;

	if(r->nomeServidor[0] != '\0'){
		r->nomeServidor[strlen(r->nomeServidor)] = '\0';
		r->tamNomeServidor = 1 + strlen(r->nomeServidor);
		r->tamanhoRegistro += 4 + 1 + r->tamNomeServidor;
	}

	if(r->cargoServidor[0] != '\0'){
		r->cargoServidor[strlen(r->cargoServidor)] = '\0';
		r->tamNomeServidor = 1 + strlen(r->cargoServidor);
		r->tamanhoRegistro += 4 + 1 + r->tamCargoServidor;
	}


	// sizeof(tag) = 1
	//r->tamNomeServidor = 1 + strlen(r->nomeServidor);
	//r->tamCargoServidor = 1 + strlen(r->cargoServidor);
	
	r->removido = '-';
	r->encadeamentoLista = -1;

//	r->tamanhoRegistro = 8 + 4 + 8 + 14 + 4 + 1 + r->tamNomeServidor + 4 + 1 + r->tamCargoServidor;

}

/*
void carregaRegistros(FILE *f, REGDADOS* r, REGCAB* c, BUFFER* b, FILE* bin){
	char ch = 'a';
	int contaVirgula = 0;
	int count = 0;

	char* id = malloc(sizeof(char));
	id[0] = '\0';
	char* sal = malloc(sizeof(char));
	sal[0] = '\0';

	r->cargoServidor = malloc(sizeof(char));
	r->cargoServidor[0] = '\0';

	r->nomeServidor = malloc(sizeof(char));
	r->nomeServidor[0] = '\0';
	r->telefoneServidor[0] = '\0';


	//lendo o arquivo char a char
	while(contaVirgula < 5){
		ch = fgetc(f);
		//printf("ch = %c\n", ch);
		if(ch == '\n'){
			break;
		}

		if(ch != ','){
		//campos de tam fixo: idServidor(4b), salarioServidor(8b), telefoneServidor(14b)	=> total(26b)
			if(contaVirgula == 0){ //id
				id = realloc(id, (count+2)*sizeof(char));
				id[count] = ch;
				id[count+1] = '\0';
				//printf("Printando id....ch= %c, id= %s, count = %d\n", ch, id, count);
				count++;
			}	
			else if(contaVirgula == 1){//salario
				sal = realloc(sal, (count+2)*sizeof(char));
				sal[count] = ch;
				sal[count+1] = '\0';
				//printf("Printando salario.... sal= %s, count = %d\n", sal, count);
				count++;
			}
			else if(contaVirgula == 2){//telefone
				r->telefoneServidor[count] = ch;
				r->telefoneServidor[count+1] = '\0';
				//printf("Printando telefone.... tel= %s, count = %d\n", r->telefoneServidor, count);
				count++;
			}
			
		//campos de tam variavel
			else if(contaVirgula == 3){//nome
				r->nomeServidor = realloc(r->nomeServidor, (count+2)*sizeof(char));
				r->nomeServidor[count] = ch;
				r->nomeServidor[count+1] = '\0';
				count++;
			}	
			else if(contaVirgula == 4){ //cargo
				r->cargoServidor = realloc(r->cargoServidor, (count+2)*sizeof(char));
				r->cargoServidor[count] = ch;
				r->cargoServidor[count+1] = '\0';
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
	r->salarioServidor = atof(sal);
	//r->nomeServidor = realloc(r->nomeServidor, r->tamNomeServidor);
	//strcpy(r->nomeServidor, nome);
	r->tamNomeServidor = strlen(r->nomeServidor);
	r->tamCargoServidor = strlen(r->cargoServidor);
	
	r->removido = '-';
	r->encadeamentoLista = -1;
	//O tamanho do registro inclui: encadeamentoLista (8 bytes) + idServidor (4 bytes) + salarioServidor
//(8 bytes) + telefoneServidor (14 bytes) + indicador de tamanho do campo (4
//bytes) + tagCampo4 (1 byte) + nomeServidor (tamanho variável, incluindo o
//'\0') + indicador de tamanho (4 bytes) + tagCampo5 (1 byte) + cargoServidor
//(tamanho variável, incluindo o '\0').
	r->tamanhoRegistro = 8 + 4 + 8 + 14 + 4 + 1 + r->tamNomeServidor + 4 + 1 + r->tamCargoServidor;

	

	//removido, tamRegistro, encadeamentoLista, idServidor, salarioServidor, telServidor, tamNome, tag3, nome, tamCargo, tag4, cargo
	//printf("|%c|%d|%f|%d|%lf|%s|%d|%c|%s|%d|%c|%s|\n\n", r->removido, r->tamanhoRegistro, r->encadeamentoLista, r->idServidor, 
	//	r->salarioServidor, r->telefoneServidor, r->tamNomeServidor, c->tags[3], r->nomeServidor, r->tamCargoServidor, c->tags[4], r->cargoServidor);

	salvaNaPagina(r, c, b);
	free(id); free(sal);
}*/

	/* FUNCIONALIDADES
[1] Permita a leitura de vários registros obtidos a partir de um arquivo de entrada
(arquivo no formato CSV) e a gravação desses registros em um arquivo de dados de
saída. Esse arquivo de dados é binário e deve ter, obrigatoriamente, o nome
arquivoTrab1.bin. O arquivo de entrada é fornecido juntamente com a especificação
do projeto, enquanto que o arquivo de dados de saída deve ser gerado como parte deste
trabalho prático.

[2] Permita a recuperação dos dados, de todos os registros, armazenados no arquivo de
dados, mostrando os dados de forma organizada na saída padrão para permitir a
distinção dos campos e registros. O tratamento de ‘lixo’ deve ser feito de forma a
permitir a exibição apropriada dos dados. Depois de mostrar todos os registros, deve
ser mostrado na saída padrão o número de páginas de disco acessadas.

[3] Permita a recuperação dos dados de todos os registros que satisfaçam um critério
de busca determinado pelo usuário. Por exemplo, o usuário pode solicitar a exibição
de todos os registros de um determinado número de identificação do servidor.
Qualquer campo pode ser utilizado como forma de busca. Essa funcionalidade pode
retornar 0 registros (quando nenhum satisfaz ao critério de busca), 1 registro ou vários
registros.*/



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

	//fp = fopen ("SCC0215012019trabalho1BCC.csv", "r");
	fp = fopen ("teste.csv", "r");

	// TESTE PRA VER SE O ARQUIVO FOI ABERTO
	if (fp == NULL) {
       printf ("Falha no carregamento do arquivo.");
       return 1;
    }
    printf ("arquivoTrab1.bin.\n");
	

	/*********CRIANDO REGISTRO DE CABEÇALHO********/
	REGCAB *RC;
	RC = calloc (1, sizeof(REGCAB));

	
	/*printf("Campo: %s\n", &(RC->campos[1][0]));
	for (int i = 0; i < 40; ++i)
	{
		printf("%c", RC->campos[1][i]);
	}
	printf("\n");*/

	/*********CRIANDO REGISTRO DE DADOS********/
	REGDADOS* RD;
	RD = calloc (1, sizeof(REGDADOS));


	BUFFER *B;
	B = calloc (1, sizeof(BUFFER));
	B->nPaginas = 0;
	B->paginas = calloc(1, sizeof(PAGINA));
	B->paginas->nBytes = 0;


	//char texto_str[32000];

	//enquanto não for fim de arquivo o looping será executado e será impresso o texto
	//while(fgets(texto_str, 20, fp) != NULL)
	//	printf("%s", texto_str);

	//fgets(texto_str, 40, fp);
	//printf("%s\n", texto_str);
	/*
	fgets(texto_str, 100, fp);
	printf("%s", texto_str);
	*/

	//fread (void *buffer, int numero_de_bytes, int count, FILE *fp);
	//fread(&conteudo, sizeof(char), 100,pf); /* Le em conteudo o valor da variável armazenada anteriormente pf */


	
	//return 0;
	B->nPaginas = 2; //o cabeçalho deve ficar na primeira pagina sozinho, então ja incremento a pagina aqui
	B->paginas = realloc(B->paginas, 2*sizeof(PAGINA));
	B->paginas[1].nBytes = 0;
	

	FILE *arqBinario;
	arqBinario = fopen ("programaTrab1.bin", "wb");

	criaRegCabecalho(fp, RC, B, arqBinario);
	//fseek de 32000 pra pular a primeira pagina (nela deve ter soh o reg de cabeçalho)




	//carregaRegistros(fp, RD, RC);
	int countReg = 0;
	char c;
	/*
	while ((c = fgetc(fp)) != EOF){
		ungetc(c, fp);
		RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
		carregaRegistros(fp, &RD[countReg], RC, B, arqBinario);
		countReg++;
	}
	*/


	int tamPag = 0;

	/*
	while(!feof(fp)){
		RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
		salvaRegistroNaStruct(fp, RD);
		printf("|%c|%d|%f|%d|%lf|%s|%d|%c|%s|%d|%c|%s|\n\n", RD->removido, RD->tamanhoRegistro, RD->encadeamentoLista, RD->idServidor, 
		RD->salarioServidor, RD->telefoneServidor, RD->tamNomeServidor, RC->tags[3], RD->nomeServidor, RD->tamCargoServidor, RC->tags[4], RD->cargoServidor);

		structToBin(&tamPag, RD, RC, arqBinario);
	}
	*/

	salvaRegistroNaStruct(fp, RD);
	while(!feof(fp)){
		printf("|%c|%d|%f|%d|%lf|%s|%d|%c|%s|%d|%c|%s|\n\n", RD->removido, RD->tamanhoRegistro, RD->encadeamentoLista, RD->idServidor, 
		RD->salarioServidor, RD->telefoneServidor, RD->tamNomeServidor, RC->tags[3], RD->nomeServidor, RD->tamCargoServidor, RC->tags[4], RD->cargoServidor);
		structToBin(&tamPag, RD, RC, arqBinario);
		free(RD);
		RD = calloc (1, sizeof(REGDADOS));
		//RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
		salvaRegistroNaStruct(fp, RD);

	}

	//fprintf(arqBinario, "%s", oqprintar); //printa no arquivo



	
	for(int i = 0; i < countReg; i++){
		free(RD[i].cargoServidor);
		free(RD[i].nomeServidor);
	}
	/*for(int i = 0; i < B->nPaginas; i++){
		free(&B->paginas[i]);
	}*/ 

	free(RD); free(RC); free(B);
	fclose(fp);
	printf("\nListar na saída padrão o arquivo binário gerado.\n");
	return 0;
}