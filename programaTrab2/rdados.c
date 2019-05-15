#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdados.h"


/*
 * salvaRegistroNaStruct
 * Função: Salva os registros do arquivo csv nas structs de registro de dados
 * Parâmetros: 	
 				f: Arquivo csv
 				r: Registro de dados
*/
void salvaRegistroNaStruct(FILE* f, REGDADOS* r){

	fscanf(f, "%d", &r->idServidor); //salvando o id
	fscanf(f, "%*c%lf", &r->salarioServidor); //pulando a virgula, e salvando o salario
	fscanf(f, "%*c%[^,]", r->telefoneServidor); //lendo string até a virgula e salvando o telefone
	fscanf(f, "%*c%[^,]", r->nomeServidor); //lendo string até a virgula e salvando o nome
	fscanf(f, "%*c%[^\n\r]", r->cargoServidor); //lendo string até o \n e \r e salvando o cargo

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

	r->tamanhoRegistro = ENC_TAM + ID_TAM + SAL_TAM + TEL_TAM;

	if(r->nomeServidor[0] != '\0'){
		r->nomeServidor[strlen(r->nomeServidor)] = '\0';
		r->tamNomeServidor = 1 + strlen(r->nomeServidor); //esse +1 é pra contar o \0
		r->tamanhoRegistro += TAM_TAM + TAG_TAM + r->tamNomeServidor;
	}

	if(r->cargoServidor[0] != '\0'){
		r->cargoServidor[strlen(r->cargoServidor)] = '\0';
		r->tamCargoServidor = 1 + strlen(r->cargoServidor); //esse +1 é pra contar o \0
		r->tamanhoRegistro += TAM_TAM + TAG_TAM + r->tamCargoServidor;
	}
	
	r->removido = '-';
	r->encadeamentoLista = -1;
}

/*
 * structToBin
 * Função: passa os registros salvos nas structs para o arquivo binário
 * Parâmetros: 	
 				tamPagina: Tamanho da pagina de disco
				tamRegAnterior: Tamanho do registro anterior passado para o arquivo binário
				r: Struct do registro de dados
				c: Struct do registro de cabeçalho
				bin: Arquivo binário
*/
void structToBin(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin){

	if (r->idServidor == -1){
		return;
	}

	int aux = r->tamanhoRegistro +  5;

	if((*tamPagina - aux) < 0){
		char arroba = '@';
		
		for(int i = 0; i < *tamPagina; i++){
			fwrite(&arroba, sizeof(char), 1, bin);
			(*tamRegAnterior) = (*tamRegAnterior) + 1;
		}

		//adicionar o tamanho desses arrobas no ultimo registro da pagina
		fseek(bin, -(*tamRegAnterior + 4), SEEK_CUR);
		fwrite(&(*tamRegAnterior), sizeof(int), 1, bin);

		fseek(bin, 0, SEEK_END);

		*tamPagina = 32000;
	}

	*tamPagina -= aux;

	fwrite(&r->removido, REM_TAM, 1, bin);
	fwrite(&r->tamanhoRegistro, TAM_TAM, 1, bin);
	fwrite(&r->encadeamentoLista, ENC_TAM, 1, bin);
	fwrite(&r->idServidor, ID_TAM, 1, bin);
	fwrite(&r->salarioServidor, SAL_TAM, 1, bin);
	fwrite(&r->telefoneServidor, TEL_TAM * sizeof(char), 1, bin);

	if(r->nomeServidor[0] != '\0'){
		int tamanho2 = strlen(r->nomeServidor) + 2;
		fwrite(&tamanho2, TAM_TAM, 1, bin);
		fwrite(&c->tags[3], TAG_TAM, 1, bin);
		fwrite(&r->nomeServidor, strlen(r->nomeServidor) * sizeof(char) + 1, 1, bin);
	}

	if(r->cargoServidor[0] != '\0'){
		int tamanho = strlen(r->cargoServidor) + 2;
		fwrite(&tamanho, TAM_TAM, 1, bin);
		fwrite(&c->tags[4], TAG_TAM, 1, bin);
		fwrite(&r->cargoServidor, strlen(r->cargoServidor)  * sizeof(char) + 1, 1, bin);
	}

	if(*tamPagina == 0){
		*tamPagina = 32000;
	}
	
	(*tamRegAnterior) = r->tamanhoRegistro;
}

/*
 * lePrintaArqBin
 * Função: Abre um arquivo binário e printa na tela os registros presentes
 * Parâmetros: 	
 				nome: Nome do arquivo binário a ser aberto
*/
void lePrintaArqBin(char nome[MAX]){
	REGDADOS *t = calloc (1, sizeof(REGCAB));

	int tamanhoPagina = 0;
	int numPaginasAcessadas = 1; //a pagina do cabeçalho esta na primeira pagina, os dados começam na segunda pagina

	FILE *bin;
	bin = fopen(nome,"rb");  // r for read, b for binary

	if(bin == NULL){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	char tag;
	int tam;

	char status = fgetc(bin);

	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	ungetc(status, bin);
	fseek(bin, 32000, SEEK_CUR);

	while(1)
	{
		t->tamanhoRegistro = 0;

		t = calloc (1, sizeof(REGDADOS));
		fread(&t->removido,REM_TAM,1,bin);
			
		if(t->removido == '-'){
			//coletando os dados do arquivo binário e salvando na struct
			fread(&t->tamanhoRegistro,TAM_TAM,1,bin); 
			tamanhoPagina += (t->tamanhoRegistro+5);

			fread(&t->encadeamentoLista,ENC_TAM,1,bin);
			t->tamanhoRegistro -= ENC_TAM;

			fread(&t->idServidor,ID_TAM,1,bin); 
			printf("%d", t->idServidor);
			t->tamanhoRegistro -= ID_TAM;

			fread(&t->salarioServidor,SAL_TAM,1,bin); 
			if(t->salarioServidor != -1){
				printf(" %.2lf", t->salarioServidor);
			}
			else{
				printf("         ");
			}
			t->tamanhoRegistro -= SAL_TAM;

			fread(&t->telefoneServidor,14,1,bin); 
			if(t->telefoneServidor[0] == '\0'){			
				sprintf(t->telefoneServidor, "%s", "              ");		
			}
			printf(" %s", t->telefoneServidor);

			t->tamanhoRegistro -= 14;

			 //se o registro ainda tem mais dados, continuar
			fread(&tam,TAM_TAM,1,bin); // lendo o tamanho da proxima string, q pode ser nome ou campo

			fread(&tag,TAG_TAM,1,bin); // lendo a tag da proxima string, q pode ser nome ou campo
			
			//verificando se o campo será de nome ou campo
			if(tag == 'n' ){ //se for n é pq tem nome
				t->tamNomeServidor = tam; //entao o tamanho que li é do nome mesmo
				fread(&t->nomeServidor,(t->tamNomeServidor - 1),1,bin); //lendo o nome
				t->tamanhoRegistro -= (t->tamNomeServidor);
				t->tamanhoRegistro -= TAM_TAM;

				printf(" %d %s", t->tamNomeServidor-2, t->nomeServidor);

			}else if (tag == 'c'){ //se for c é pq nao tem nome, e o campo ja eh cargo
				sprintf(t->nomeServidor, "%s", "");
				
				t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo

				fread(&t->cargoServidor,(t->tamCargoServidor - 1),1,bin); // read 10 bytes to our buffer

				t->tamanhoRegistro -= (t->tamCargoServidor);
				t->tamanhoRegistro -= TAM_TAM;

				printf(" %d %s", t->tamCargoServidor-2, t->cargoServidor);


			}
			else{
				fseek(bin, -(TAM_TAM+TAG_TAM), SEEK_CUR); // VOLTA TAG
			}
				
			fread(&tam, TAM_TAM, 1, bin);

			if(fread(&tag,TAG_TAM,1,bin)){

				if (tag == 'c'){ 
				//se for c é pq nao tem nome, e o campo ja eh cargo
					t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo

					fread(&t->cargoServidor,(t->tamCargoServidor - 1),1,bin); // read 10 bytes to our buffer

					t->tamanhoRegistro -= (t->tamCargoServidor);
					t->tamanhoRegistro -= TAM_TAM;

					printf(" %d %s", t->tamCargoServidor-2, t->cargoServidor);
				}
				else {
					fseek(bin, -(TAM_TAM+TAG_TAM), SEEK_CUR);
				}	
			
				if(t->tamanhoRegistro > 0){
					char lixo;
					for(int i = 0; i < t->tamanhoRegistro; i++){
						fread(&lixo, 1, 1, bin);
					}
				}
			}		
		}
		if(tamanhoPagina > 0){
			printf("\n");
		}
		if(tamanhoPagina >= 32000){
			numPaginasAcessadas++;
			tamanhoPagina = 0;
		}
		free(t);

		if(feof(bin)){
			break;
		}
	}
	if(tamanhoPagina > 0 && tamanhoPagina < 32000){
		numPaginasAcessadas++;
	}

	if(numPaginasAcessadas == 1){
		printf("Registro inexistente.\n");
	}
	else{
		printf("Número de páginas de disco acessadas: %d\n", numPaginasAcessadas);
	}
}

/*
 * tamArquivo
 * Função: Calcula o tamanho de um arquivo
 * Parâmetros: 	
 				arq: Arquivo que quero descobrir o tamanho

 * Retorno:	Tamanho do arquivo dado
*/
int tamArquivo(FILE *arq){
	int tam;
	fseek(arq, 0, SEEK_END);
	tam = ftell(arq);
	rewind(arq);

	return tam;
}

/*
 * buscaRegBin
 * Função: Busca dados em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				rc: Registro de cabeçalho
 				nomeCampo: Nome do campo que quero buscar
 				valor: Valor do campo que quero buscar
*/
void buscaRegBin(FILE* bin, REGCAB* rc, char nomeCampo[MAX], char valor[MAX]){

	int achei = 0;
	int tam_bin = tamArquivo(bin);
	int tam_pag = 0;
	fseek(bin, TAM_PAG_DISCO, SEEK_CUR);
	int numPaginasAcessadas = 1; //ja acessei o Registro de cabeçalho na primeira pagina

	if(strcmp(nomeCampo,"idServidor") == 0){
		//printf("vou procurar o id");
		int id = atoi(valor);
		busca_id(bin, tam_bin, id, &numPaginasAcessadas, &tam_pag, rc, &achei);
	}
	else if(strcmp(nomeCampo, "salarioServidor") == 0){
		double sal = atof(valor);
		busca_salario(bin, tam_bin, sal, &numPaginasAcessadas, &tam_pag, rc, &achei);
	}
	else if(strcmp(nomeCampo, "telefoneServidor") == 0){
		char tel[TEL_TAM];
		strcpy(tel, valor);
		busca_telefone(bin, tam_bin, tel, &numPaginasAcessadas, &tam_pag, rc, &achei);
	}
	else if(strcmp(nomeCampo,"nomeServidor") == 0){
		char* nome;
		nome = calloc(strlen(valor)+1, sizeof(char));
		strcpy(nome, valor);
		busca_nome(bin, tam_bin, nome, &numPaginasAcessadas, &tam_pag, rc, &achei);
		free(nome);

	}
	else if(strcmp(nomeCampo, "cargoServidor") == 0){
		char* cargo;
		cargo = calloc(strlen(valor)+1, sizeof(char));
		strcpy(cargo, valor);
		busca_cargo(bin, tam_bin, cargo, &numPaginasAcessadas, &tam_pag, rc, &achei);
		free(cargo);
	}
	
	if(tam_pag > 0 && tam_pag < 32000){
		numPaginasAcessadas++;
	}
	if(achei > 0){
		printf("Número de páginas de disco acessadas: %d\n", numPaginasAcessadas);
	}
	else{
		printf("Registro inexistente.\n");
	}
}

/*
 * busca_id
 * Função: Busca dados do tipo idServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				id: Valor do id que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void busca_id (FILE* bin, int tam_bin, int id, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){

	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(rd->idServidor == id){
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
			free(rd);
			return;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * busca_salario
 * Função: Busca dados do tipo salarioServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				sal: Valor do salario que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void busca_salario (FILE* bin, int tam_bin, double sal, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(rd->salarioServidor == sal){
			//printf("cargoooo = %s, tam = %d\n", rd->cargoServidor, rd->tamCargoServidor);
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * busca_telefone
 * Função: Busca dados do tipo telefoneServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				tel: Numero do telefone que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void busca_telefone (FILE* bin, int tam_bin, char tel[TEL_TAM], int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->telefoneServidor, tel) == 0){
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * busca_nome
 * Função: Busca dados do tipo nomeServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				nome: Nome que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void busca_nome (FILE* bin, int tam_bin, char* nome, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->nomeServidor, nome) == 0){
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * busca_cargo
 * Função: Busca dados do tipo cargoServidor em um dado arquivo binário
 * Parâmetros: 	
 				bin: Arquivo binário
 				tam_bin: Tamanho do arquivo binário passado
 				rc: Registro de cabeçalho
 				cargo: Cargo que quero buscar
 				numPaginasAcessadas: ponteiro que guarda quantas páginas do arquivo acessei na busca
 				tam_pag: ponteiro para controlar o tamanho da pagina
 				achei: variavel que indica se encontrei registros ou não
*/
void busca_cargo (FILE* bin, int tam_bin, char* cargo, int *numPaginasAcessadas, int *tam_pag, REGCAB* rc, int *achei){
	//printf("CARGO A PESQUISARRRR : %s\n", cargo);
	while(ftell(bin) != tam_bin){
		
		REGDADOS* rd = calloc(1, sizeof(REGDADOS));
		leUmRegistroBin(bin, rd, tam_pag);
		if(strcmp(rd->cargoServidor, cargo) == 0){
			//printa o registro encontrado
			printaRegEncontrado(rc, rd);
			(*achei) = 1;
			break;
		}

		if((*tam_pag) == 32000){
			(*tam_pag) = 0;
			//printf("Acessei uma pagina, proxima pagina\n");
			(*numPaginasAcessadas)++;
		}
		free(rd);
	}
}

/*
 * freadString
 * Função: lê uma string de um arquivo binário e armazena em uma string
 * Parâmetros: 	
 				bin: arquivo binário.
 				str: string que será lida do arquivo
*/
void freadString(FILE* bin, char* str){
	int i =-1;
	do
	{
		i++;
		str[i] = getc(bin);
	} while (str[i] != '\0'  && str[i] != EOF);
	str[i] = '\0';
	return;
}

/*
 * leUmRegistroBin
 * Função: lê um registro de um arquivo binário e armazena em uma struct de registro de dados
 * Parâmetros: 	
 				bin: arquivo binário.
 				t: Registro de dados.
 				tam_pag: ponteiro para controlar o tamanho da pagina.
*/
void leUmRegistroBin(FILE *bin, REGDADOS* t, int *tam_pag){
	t->tamanhoRegistro = 0;
	int tam = 0;
	char tag;

	t->tamNomeServidor = 0;
	t->tamCargoServidor = 0;

	fread(&t->removido,REM_TAM,1,bin);
	fread(&t->tamanhoRegistro,TAM_TAM,1,bin); 

	int tamanhoReg = t->tamanhoRegistro;
	if(t->removido == '*'){
		fseek(bin, t->tamanhoRegistro, SEEK_CUR);
		return;
	}
	(*tam_pag) += (t->tamanhoRegistro + TAM_TAM + REM_TAM);
		
	//coletando os dados do arquivo binário e salvando na struct
	fread(&t->encadeamentoLista,ENC_TAM,1,bin); 
	tamanhoReg -= ENC_TAM;

	fread(&t->idServidor,ID_TAM,1,bin); 
	tamanhoReg -= ID_TAM;

	fread(&t->salarioServidor,SAL_TAM,1,bin);
	tamanhoReg -= SAL_TAM;

	fread(t->telefoneServidor,TEL_TAM,1,bin);
	tamanhoReg -= TEL_TAM;

	 //se o registro ainda tem mais dados, continuar
	fread(&tam,TAM_TAM,1,bin); // lendo o tamanho da proxima string, q pode ser nome ou campo
	fread(&tag,TAG_TAM,1,bin); // lendo a tag da proxima string, q pode ser nome ou campo	

	//verificando se o campo será de nome ou campo
	if(tag == 'n' ){ //se for n é pq tem nome
		t->tamNomeServidor = tam; //entao o tamanho que li é do nome mesmo
		freadString(bin, &(t->nomeServidor[0]));
		tamanhoReg -= t->tamNomeServidor;
		tamanhoReg -= TAM_TAM;	

		fread(&tam,TAM_TAM,1,bin); // lendo o tamanho da proxima string, q pode ser nome ou campo
		fread(&tag,TAG_TAM,1,bin); // lendo a tag da proxima string, q pode ser nome ou campo	

		if (tag == 'c'){ 
		//se for c é pq nao tem nome, e o campo ja eh cargo
			t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo
			freadString(bin, &(t->cargoServidor[0]));
			tamanhoReg -= t->tamCargoServidor;
			tamanhoReg -= TAM_TAM;
		}
		else {
			fseek(bin, -5, SEEK_CUR);
		}
	}
	else if (tag == 'c'){ //se for c é pq nao tem nome, e o campo ja eh cargo
		t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo
		freadString(bin, &(t->cargoServidor[0]));
		tamanhoReg -= t->tamCargoServidor;
		tamanhoReg -= TAM_TAM;
	}
	else{
		fseek(bin, -5, SEEK_CUR); // VOLTA TAG
	}
		
	char lixo;
	do{
		
		lixo = getc(bin);
		if (lixo == EOF){
			return;
		}
	}while(lixo != '-' && lixo != '*');

	fseek(bin, -1, SEEK_CUR);
}


/*
 * printaRegEncontrado
 * Função: Printa um registro de dados encontrado na busca
 * Parâmetros: 	
 				rc: Registro de cabeçalho
 				rd: Registro de dados
*/
void printaRegEncontrado(REGCAB* rc, REGDADOS* rd) {
	printf("%s: ", rc->campos[0]);
	if (rd->idServidor != 1){
		printf("%d\n", rd->idServidor);
	}
	else{
		printf("valor nao declarado\n");
	}
	
	printf("%s: ", rc->campos[1]);
	if (rd->salarioServidor != -1){
		printf("%.2lf\n", rd->salarioServidor);
	}
	else{
		printf("valor nao declarado\n");
	}
	
	printf("%s: ", rc->campos[2]);
	if ((int) strlen(rd->telefoneServidor)){
		printf("%s\n", rd->telefoneServidor);
	}
	else{
		printf("valor nao declarado\n");
	}

	printf("%s: ", rc->campos[3]);
	if (rd->tamNomeServidor){
		printf("%s\n", rd->nomeServidor);
	}
	else{
		printf("valor nao declarado\n");			
	}

	printf("%s: ", rc->campos[4]);
	if (rd->tamCargoServidor){
		printf("%s\n\n", rd->cargoServidor);
	}
	else{
		printf("valor nao declarado\n\n");
	}
}

void criaNovoRegDados(REGDADOS* rd, int id, double sal, char* tel, char* nome, char* cargo){
	rd->idServidor = id;
	rd->salarioServidor = sal;
	strcpy(rd->telefoneServidor,tel);
	strcpy(rd->nomeServidor,nome);
	strcpy(rd->cargoServidor,cargo);

	rd->tamNomeServidor = strlen(nome) + 1;
	rd->tamCargoServidor = strlen(cargo) + 1;
	rd->removido = '-';
	rd->tamanhoRegistro = ENC_TAM + ID_TAM + SAL_TAM + TEL_TAM + rd->tamNomeServidor + rd->tamCargoServidor;
	rd->encadeamentoLista = -1;
}