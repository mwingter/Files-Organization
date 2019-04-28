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
	long int encadeamentoLista; // armazena os byte offsets dos registros logicamente removidos – tamanho: inteiro de 8 bytes.

};



//Struct do Registro de Cabeçalho
typedef struct regcab_ REGCAB; //tam = 214 bytes, e ocupa uma pagina de disco, mas como ele é menor q a pagina, completar a pagina com '@'
struct regcab_{
	char status; /*indica a consistência do arquivo de dados, devido à queda de energia, travamento do programa, etc. 
				  *	Pode assumir os valores 0, para indicar que o arquivo de dados está inconsistente, ou 1, para 
				  *	indicar que o arquivo de dados está consistente. Ao se abrir um arquivo para escrita, seu status 
				  *	deve ser 0 e, ao finalizar o uso desse arquivo, seu status deve ser 1*/

	long int topoLista; //sempre igual à -1

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




void criaRegCabecalho(FILE* f, REGCAB* rc){
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

	//completando o primeiro byte com \0
	for(int i = 0; i < 5; i++){
		int j = strlen(rc->campos[i]);
		rc->campos[i][j+1] = '\0';
	}

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

	rc->status = '1'; //Ao se abrir um arquivo para escrita, seu status deve ser 0 
					//e, ao finalizar o uso desse arquivo, seu status deve ser 1

	rc->topoLista = -1;

	//status(1), topoLista(8), tag1(1), campo1(40), tag2(1), campo2(40),......., tag5(1), campo5(40) => totalBytes= 10+5+200 = 215
	//printf("|%c|%lf|%c|%s|%c|%s|%c|%s|%c|%s|%c|%s|\n", rc->status, rc->topoLista, rc->tags[0], rc->campos[0], rc->tags[1], rc->campos[1], rc->tags[2], rc->campos[2], rc->tags[3], rc->campos[3], rc->tags[4], rc->campos[4]);

	free(c1);free(c2);free(c3);free(c4);free(c5);

}


void regCabToArqBin(REGCAB* c, FILE* bin){

	int tamRegistroCab = 214;//1+8+1+40+1+40+1+40+1+40+1+40
	int restoPagina = 32000 - tamRegistroCab;

	fwrite(&c->status, sizeof(char), 1, bin);

	fwrite(&c->topoLista, sizeof(long int), 1, bin);

	fwrite(&c->tags[0], sizeof(char), 1, bin);

	fwrite(&c->campos[0], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[1], sizeof(char), 1, bin);

	fwrite(&c->campos[1], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[2], sizeof(char), 1, bin);

	fwrite(&c->campos[2], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[3], sizeof(char), 1, bin);

	fwrite(&c->campos[3], 40*sizeof(char), 1, bin);

	fwrite(&c->tags[4], sizeof(char), 1, bin);

	fwrite(&c->campos[4], 40*sizeof(char), 1, bin);

	
	char arroba = '@';
	for(int i = 0; i < restoPagina; i++){ //preenchendo com @ o resto da pagina
		fwrite(&arroba, sizeof(char), 1, bin);
	}
}


void structToBin(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin){

	if (r->idServidor == -1){
		return;
	}

	long posicao;
	char buffer[50];
	int aux = r->tamanhoRegistro +  5;
	//printf(" Tamanho nome2  %d - !%d! - %s \n",r->tamanhoRegistro,*tamPagina,r->nomeServidor);
	if((*tamPagina - aux) < 0){
		char arroba = '@';
		//printf("NOMEEE %s",r->nomeServidor);
		//printf("tamReg anterior antes %d\n", *tamRegAnterior);
		
		for(int i = 0; i < *tamPagina; i++){
			fwrite(&arroba, sizeof(char), 1, bin);
			(*tamRegAnterior) = (*tamRegAnterior) + 1;
			//printf("%c",arroba);
		}
		//printf("tamReg anterior %d\n", *tamRegAnterior);

		//adicionar o tamanho desses arrobas no ultimo registro da pagina
		fseek(bin, -(*tamRegAnterior + 4), SEEK_CUR);
		//fseek(bin, -100, SEEK_CUR);
		//printf("PONTEIRO antes %p",bin);
		fwrite(&(*tamRegAnterior), sizeof(int), 1, bin);
		//fread(&buffer, 50, 1, bin);
		//posicao = ftell(bin);
		//fseek(bin, posicao + (*tamRegAnterior + 4), SEEK_CUR);
		fseek(bin, 0, SEEK_END);
		//printf(" PONTEIRO depois %p \n",bin);

		//printf("PAGINA NOVA \n");
		*tamPagina = 32000;
	}

	*tamPagina -= aux;
	//printf("Tamanho da pagina %d \n",*tamPagina);

	//printf("Tamanho da pagina %d \n",r->tamanhoRegistro);

	fwrite(&r->removido, sizeof(char), 1, bin);
	fwrite(&r->tamanhoRegistro, sizeof(int), 1, bin);
	fwrite(&r->encadeamentoLista, sizeof(long int), 1, bin);
	fwrite(&r->idServidor, sizeof(int), 1, bin);

	//if(r->salarioServidor = 0.00)
	//	r->salarioServidor = 0;

	//printf("nome %s e salario %lf \n",r->nomeServidor,r->salarioServidor);
	fwrite(&r->salarioServidor, sizeof(double), 1, bin);

	fwrite(&r->telefoneServidor, 14*sizeof(char), 1, bin);



	if(r->nomeServidor[0] != '\0'){
		int tamanho2 = strlen(r->nomeServidor) + 2;
		fwrite(&tamanho2, sizeof(int), 1, bin);
		//fwrite(&r->tamNomeServidor, sizeof(int), 1, bin);
		fwrite(&c->tags[3], sizeof(char), 1, bin);
		fwrite(&r->nomeServidor, strlen(r->nomeServidor) * sizeof(char) + 1, 1, bin);
		//printf("tam %d e %d\n", strlen(r->nomeServidor), r->tamNomeServidor);
		//printf("Nome do corno %s \n",r->nomeServidor);
	}

	if(r->cargoServidor[0] != '\0'){
		int tamanho = strlen(r->cargoServidor) + 2;
		fwrite(&tamanho, sizeof(int), 1, bin);

		//printf("tam %d\n", r->tamCargoServidor);
		fwrite(&c->tags[4], sizeof(char), 1, bin);
		fwrite(&r->cargoServidor, strlen(r->cargoServidor)  * sizeof(char) + 1, 1, bin);
	}

	if(*tamPagina == 0){
		*tamPagina = 32000;
	}
	
	(*tamRegAnterior) = r->tamanhoRegistro;

}	

void lePrintaArqBin(char nome[MAX]){

	REGDADOS *t = calloc (1, sizeof(REGCAB));
	//unsigned char buffer[32000];

	int tamanhoPagina = 0;
	FILE *ptr;
	ptr = fopen(nome,"rb");  // r for read, b for binary

	char tag;
	int tam;
//	long int total = 32000;
	fseek(ptr, 32000, SEEK_CUR);

	//while(!feof(ptr))
	while(1)
	//for (int i = 0; i < 3; ++i)
	{
		t->tamanhoRegistro = 0;

		t = calloc (1, sizeof(REGDADOS));
		fread(&t->removido,sizeof(char),1,ptr); 
	//	printf("%c ", t->removido);
			
		if(t->removido == '-'){

			fread(&t->tamanhoRegistro,sizeof(int),1,ptr); 
			//printf("%d ", t->tamanhoRegistro);
			//printf("tamanho do reg INICIAL %d  ", t->tamanhoRegistro);

			tamanhoPagina += (t->tamanhoRegistro + 5);
			//printf("tamanho da pagina esta %d\n", tamanhoPagina);

			fread(&t->encadeamentoLista,sizeof(long int),1,ptr); 
			//printf("%ld ", t->encadeamentoLista);
			t->tamanhoRegistro -= sizeof(long int);

			fread(&t->idServidor,sizeof(int),1,ptr); 
			//printf("%d ", t->idServidor);
			t->tamanhoRegistro -= sizeof(int);


			fread(&t->salarioServidor,sizeof(double),1,ptr); 
			//printf("%lf ", t->salarioServidor);
			t->tamanhoRegistro -= sizeof(double);


			fread(&t->telefoneServidor,14,1,ptr); 
			if(t->telefoneServidor[0] == '\0'){
	
				
				sprintf(t->telefoneServidor, "%s", "              ");
				
			}
			//printf("%s ", t->telefoneServidor);

			t->tamanhoRegistro -= 14;

			//printf("REGISTRO tam 1 %d\n", t->tamanhoRegistro);
			//printf("tamanho do reg menos dados de tam fixo %d  ", t->tamanhoRegistro);

			 //se o registro ainda não acabou, continuar
			//	printf("REGISTRO tam %d\n", t->tamanhoRegistro);

				fread(&tam,sizeof(int),1,ptr); // lendo o tamanho da proxima string, q pode ser nome ou campo

				fread(&tag,sizeof(char),1,ptr); // lendo a tag da proxima string, q pode ser nome ou campo
				

				//printf("TAGGG %c\n", tag);
				//verificando se o campo será de nome ou campo
				if(tag == 'n' ){ //se for n é pq tem nome
					t->tamNomeServidor = tam; //entao o tamanho que li é do nome mesmo
					fread(&t->nomeServidor,(t->tamNomeServidor - 1),1,ptr); //lendo o nome
					//printf("nomeee %s ", t->nomeServidor);
					t->tamanhoRegistro -= (t->tamNomeServidor);
					t->tamanhoRegistro -= sizeof(int);
					//t->tamanhoRegistro -= sizeof(char);

					//printf("REGISTRO tam2 %d\n", t->tamanhoRegistro);





				}else if (tag == 'c'){ //se for c é pq nao tem nome, e o campo ja eh cargo
					sprintf(t->nomeServidor, "%s", "");
					
					t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo
					//fread(&tag,sizeof(char),1,ptr); // read 10 bytes to our buffer
					//printf("nao tem nome %c ", tag);

					fread(&t->cargoServidor,(t->tamCargoServidor - 1),1,ptr); // read 10 bytes to our buffer
					//printf("%s ", t->cargoServidor);
					t->tamanhoRegistro -= (t->tamCargoServidor);
					t->tamanhoRegistro -= sizeof(int);
					//printf("REGISTRO tam3 %d\n", t->tamanhoRegistro);


				}
				else{
					fseek(ptr, -5, SEEK_CUR); // VOLTA TAG
					//t-> tamanhoRegistro += 5;


				}
				
				fread(&t->tamCargoServidor, sizeof(int), 1, ptr);
				if(fread(&tag,sizeof(char),1,ptr)){

					if (tag == 'c'){ 
					//se for c é pq nao tem nome, e o campo ja eh cargo
						//sprintf(t->nomeServidor, "%s", "");
						t->tamCargoServidor = t->tamCargoServidor; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo
						//fread(&tag,sizeof(char),1,ptr); // read 10 bytes to our buffer
						//printf("nao tem nome %c ", tag);

						fread(&t->cargoServidor,(t->tamCargoServidor - 1),1,ptr); // read 10 bytes to our buffer
						//printf("%s ", t->cargoServidor);
						t->tamanhoRegistro -= (t->tamCargoServidor);
						t->tamanhoRegistro -= sizeof(int);

					}
					else {
						//printf("Entrei aquiiii");
						fseek(ptr, -5, SEEK_CUR);
						

					}	
				
					//printf("REGISTROOOOOOOOOOOOOOOO %d \n",t->tamanhoRegistro);
					if(t->tamanhoRegistro > 0){
						char lixo;
						for(int i = 0; i < t->tamanhoRegistro; i++){
							fread(&lixo, 1, 1, ptr);
							//printf("ENTREI %d \n", t->tamanhoRegistro);

						}
					}

				}




			
			
			//printf("tamanho do reg FINAL %d\n", t->tamanhoRegistro);

			

			printf("%d %lf %s %s %s\n", t->idServidor, t->salarioServidor, t->telefoneServidor, t->nomeServidor, t->cargoServidor);

		}
		//printf("\n\n");
		free(t);
		if(feof(ptr))
			break;
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

	if(r->salarioServidor == 0.00){
		r->salarioServidor = 0;
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
		r->tamNomeServidor = 1 + strlen(r->nomeServidor); //esse +1 é pra contar o \0
		r->tamanhoRegistro += 4 + 1 + r->tamNomeServidor; // o 4 é do tamNomeServidor e o 1 é da tag
	}

	if(r->cargoServidor[0] != '\0'){
		r->cargoServidor[strlen(r->cargoServidor)] = '\0';
		r->tamCargoServidor = 1 + strlen(r->cargoServidor);
		r->tamanhoRegistro += 4 + 1 + r->tamCargoServidor;
	}


	// sizeof(tag) = 1
	//r->tamNomeServidor = 1 + strlen(r->nomeServidor);
	//r->tamCargoServidor = 1 + strlen(r->cargoServidor);
	
	r->removido = '-';
	r->encadeamentoLista = -1;

//	r->tamanhoRegistro = 8 + 4 + 8 + 14 + 4 + 1 + r->tamNomeServidor + 4 + 1 + r->tamCargoServidor;

}


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


int main(int argc, char const *argv[]){


	int op;
	char nomeDoArquivo[MAX], valor[MAX], nomeDoCampo[MAX];
	scanf("%d %[^\n\r]", &op, nomeDoArquivo); //vale pras 3 funcionalidades
	FILE* fp;

	switch(op){
		case 1:
			//FILE *fp;
			fp = fopen (nomeDoArquivo, "r");	

			if (!fp) {
				printf("Falha no carregamento do arquivo.\n");	
				return 1;
			}
			

			/*********CRIANDO REGISTRO DE CABEÇALHO********/
			REGCAB *RC;
			RC = calloc (1, sizeof(REGCAB));

			/*********CRIANDO REGISTRO DE DADOS********/
			REGDADOS* RD;
			RD = calloc (1, sizeof(REGDADOS));
			

			criaRegCabecalho(fp, RC);
			char nome_bin[MAX];
			strcpy(nome_bin, "arquivoTrab1.bin");
			FILE *arqBinario = fopen(nome_bin, "wb+");

			regCabToArqBin(RC, arqBinario);

			int countReg = 0;
			char c;	

			int tamPag = 32000;
			int tamRegAnt = 0;


			while(!feof(fp))
			{
				salvaRegistroNaStruct(fp, RD);
				//printf("|%c|%d|%ld|%d|%lf|%s|%d|%c|%s|%d|%c|%s|\n\n", RD->removido, RD->tamanhoRegistro, RD->encadeamentoLista, RD->idServidor, 
				//RD->salarioServidor, RD->telefoneServidor, RD->tamNomeServidor, RC->tags[3], RD->nomeServidor, RD->tamCargoServidor, RC->tags[4], RD->cargoServidor);
				structToBin(&tamPag, &tamRegAnt, RD, RC, arqBinario);
				free(RD);
				RD = calloc (1, sizeof(REGDADOS));
				//RD = realloc(RD, (countReg+1)*sizeof(REGDADOS));
			}

			printf("%s",nome_bin);
			free(RD); free(RC);
			fclose(fp); fclose(arqBinario);
		    
		    
			break;


		case 2:

   			//fflush(stdin);
			
			//EGDADOS* T;
			//printf("entrei no caso 2\n");
			lePrintaArqBin(nomeDoArquivo);
			break;



		case 3:
			//printf("buscar coisas\n");
			//char nomeDoCampo[MAX];
			//char valor[MAX];
			scanf("%[^\n\r] %[^\n\r]", nomeDoCampo, valor);
			break;

		default:
			printf("Opcao invalida.\n");

	}

	return 0;
}