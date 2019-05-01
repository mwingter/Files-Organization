#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rdados.h"


void structToBin(int* tamPagina, int *tamRegAnterior, REGDADOS* r, REGCAB* c, FILE* bin){

	if (r->idServidor == -1){
		return;
	}

	//long posicao;
	//char buffer[50];
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

	int tamanhoPagina = 0;
	int numPaginasAcessadas = 1; //a pagina do cabeçalho esta na primeira pagina, os dados começam na segunda pagina

	FILE *ptr;
	ptr = fopen(nome,"rb");  // r for read, b for binary

	if(ptr == NULL){
		//fprintf(stderr, "Falha no carregamento do arquivo.");
		printf("Falha no processamento do arquivo.\n");
		//printf("Registro inexistente.\n");
		return;
	}
	char tag;
	int tam;


	char status = fgetc(ptr);
	//fread(&status,sizeof(char),1,ptr); 
	if (status == '0') {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	ungetc(status, ptr);
	fseek(ptr, 32000, SEEK_CUR);

	while(1)
	//for(int i = 0; i < 500; i++)
	{
		t->tamanhoRegistro = 0;


		t = calloc (1, sizeof(REGDADOS));
		fread(&t->removido,sizeof(char),1,ptr); 
	//	printf("%c ", t->removido);
			
		if(t->removido == '-'){

			fread(&t->tamanhoRegistro,sizeof(int),1,ptr); 
			//printf("%d ", t->tamanhoRegistro);
			//printf("tamanho do reg INICIAL %d  ", t->tamanhoRegistro);

			tamanhoPagina += (t->tamanhoRegistro+5);
			//printf("tamanho da pagina esta %d\n", tamanhoPagina);

			fread(&t->encadeamentoLista,sizeof(long int),1,ptr); 
			//printf("%ld ", t->encadeamentoLista);
			t->tamanhoRegistro -= sizeof(long int);

			fread(&t->idServidor,sizeof(int),1,ptr); 
			printf("%d", t->idServidor);
			t->tamanhoRegistro -= sizeof(int);


			fread(&t->salarioServidor,sizeof(double),1,ptr); 
			if(t->salarioServidor != -1){
				printf(" %.2lf", t->salarioServidor);
			}
			else{
				printf("         ");
			}
			t->tamanhoRegistro -= sizeof(double);


			fread(&t->telefoneServidor,14,1,ptr); 
			if(t->telefoneServidor[0] == '\0'){
	
				
				sprintf(t->telefoneServidor, "%s", "              ");
				
			}
			printf(" %s", t->telefoneServidor);

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
					t->tamanhoRegistro -= (t->tamNomeServidor);
					t->tamanhoRegistro -= sizeof(int);
					//t->tamanhoRegistro -= sizeof(char);

					printf(" %d %s", t->tamNomeServidor-2, t->nomeServidor);

				}else if (tag == 'c'){ //se for c é pq nao tem nome, e o campo ja eh cargo
					sprintf(t->nomeServidor, "%s", "");
					
					t->tamCargoServidor = tam; //entao o tamanho que li não era do nome pq não tem nome, mas sim do cargo
					//fread(&tag,sizeof(char),1,ptr); // read 10 bytes to our buffer
					//printf("nao tem nome %c ", tag);

					fread(&t->cargoServidor,(t->tamCargoServidor - 1),1,ptr); // read 10 bytes to our buffer
					//printf("%s ", t->cargoServidor);
					t->tamanhoRegistro -= (t->tamCargoServidor);
					t->tamanhoRegistro -= sizeof(int);

					printf(" %d %s", t->tamCargoServidor-2, t->cargoServidor);


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

						printf(" %d %s", t->tamCargoServidor-2, t->cargoServidor);

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

			//printf("%d %.2lf %s %d %s %d %s\n", t->idServidor, t->salarioServidor, t->telefoneServidor, t->tamNomeServidor-2, t->nomeServidor, t->tamCargoServidor-2, t->cargoServidor);
		}
		if(tamanhoPagina > 0){
			printf("\n");
		}

		if(tamanhoPagina >= 32000){
			numPaginasAcessadas++;
			tamanhoPagina = 0;
		}

		free(t);
		if(feof(ptr)){
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

int tamArquivo(FILE *arq) {
	int tam;
	fseek(arq, 0, SEEK_END);
	tam = ftell(arq);
	rewind(arq);

	return tam;
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

void buscaRegistro(FILE *bin, char nomeCampo[MAX], char valor[MAX], int tam_bin, REGCAB *rc, int pagAtual) {	
	int numPaginasAcessadas = pagAtual;

	if(nomeCampo == rc->campos[0]){
		int id = atoi(valor);
		busca_id(bin, tam_bin, id, numPaginasAcessadas, rc);
	}
	else if(nomeCampo == rc->campos[1]){
		double sal = atof(valor);
		busca_salario(bin, tam_bin, sal, numPaginasAcessadas, rc);
	}
	else if(nomeCampo == rc->campos[2]){
		char tel[TEL_TAM];
		strcpy(tel, valor);
		busca_telefone(bin, tam_bin, tel, numPaginasAcessadas, rc);
	}
	else if(nomeCampo == rc->campos[3]){
		char* nome;
		nome = calloc(strlen(valor), sizeof(char));
		strcpy(nome, valor);
		busca_nome(bin, tam_bin, nome, numPaginasAcessadas, rc);

	}
	else if(nomeCampo == rc->campos[4]){
		char* cargo;
		cargo = calloc(strlen(valor), sizeof(char));
		busca_cargo(bin, tam_bin, cargo, numPaginasAcessadas, rc);
	}
	
	
	printf("Número de páginas de disco acessadas: %d\n", numPaginasAcessadas);

	free(h); free(nome); free(cargo);
	fclose(bin);
}

void printaReg_Busca(REGCAB* rc, REGDADOS* rd) {
	printf("%s: ", rc->campos[0]);
	if (rd->idServidor != 1){
		printf("%d\n", d->idServidor);
	}
	else{
		printf("valor nao declarado\n");
	}
	
	printf("%s: ", rc->campos[1]);
	if (rd->salarioServidor != -1){
		printf("%.2lf\n", d->salarioServidor);
	}
	else{
		printf("valor nao declarado\n");
	}
	
	printf("%s: ", rc->campos[2]);
	if ((int) strlen(rd->telefoneServidor)){
		printf("%s\n", d->telefoneServidor);
	}
	else{
		printf("valor nao declarado\n");
	}

	printf("%s: ", rc->campos[3]);
	if (rd->tamNomeServidor){
		printf("%s\n", d->nomeServidor);
	}
	else{
		printf("valor nao declarado\n");			
	}

	printf("%s: ", rc->campos[4]);
	if (rd->tamCargoServidor){
		printf("%s\n\n", d->cargoServidor);
	}
	else{
		printf("valor nao declarado\n\n");
	}
}

void leRegistroDoArqBin(FILE *bin, REGDADOS *rd){	
	rd = calloc(1, sizeof(REGDADOS));

	strcpy(rd->removido, "-");
	rd->encadeamentoLista = -1;
	rd->idServidor = -1;

	rd->salarioServidor = -1;
	strcpy(rd->telefoneServidor, "\0@@@@@@@@@");
	rd->tamNomeServidor = 0;
	rd->nomeServidor = NULL;
	rd->tamCargoServidor = 0;
	rd->cargoServidor = NULL;
	rd->tamanhoRegistro = ID_TAM + SAL_TAM + TEL_TAM + ENC_TAM;

	
	fread(&rd->removido, TAM_REM, 1, bin);
	
	fread(&rd->tamanhoRegistro, TAM_TAM, 1, bin);

	fread(&rd->encadeamentoLista, ENC_TAM, 1, bin);

	fread(&rd->idServidor, ID_TAM, 1, bin);
	
	fread(&rd->salarioServidor, SAL_TAM, 1, bin);
	
	fread(&rd->telefoneServidor, TEL_TAM, 1, bin);

	int tamReg = rd->tamanhoRegistro - ID_TAM - SAL_TAM - TEL_TAM - ENC_TAM ;
	
	int tam;
	char tag;
	fread(&size, TAM_TAM, 1, bin);
	fread(&tag, TAG_TAM, 1, bin);
	
	if (tag == 'n') {
		rd->tamNomeServidor = tam;
		rd->nomeServidor = (char*) calloc(tam-1, tamof(char));
		fread(rd->nomeServidor, tam-1, 1, bin);
		tamReg -= TAM_TAM + tam;
	} else if (tag == 'c') {
		rd->tamCargoServidor = tam;
		rd->cargoServidor = (char*) calloc(tam-1, tamof(char));
		fread(rd->cargoServidor, tam-1, 1, bin);
		tamReg -= TAM_TAM + tam;
	} else {
		fseek(bin, -(TAM_TAM + TAG_TAM), SEEK_CUR);
	}

	fread(&tam, TAM_TAM, 1, bin);
	if (fread(&tag, TAG_TAM, 1, bin)) {
		if (tag == 'c') {
			rd->tamCargoServidor = tam;
			rd->cargoServidor = (char*) calloc(tam-1, tamof(char));
			fread(rd->cargoServidor, tam-1, 1, bin);
			tamReg -= TAM_TAM + tam;
		} else {
			fseek(bin, -(TAM_TAM + TAG_TAM), SEEK_CUR);
		}

		if (tamReg > 0) {
			char lixo;
			while (tamReg) {
				fread(lixo, sizeof(char), 1, bin);
				tamReg--;
			}
		}
	}
}

int busca_id(FILE *bin, int id, int numPaginasAcessadas, REGCAB *rc, int paginaAtual){
	int regEncontrado = 0;
	int tam_bin = tamArquivo(bin);

	while (ftell(bin) != tam_bin) {

		REGDADOS* rd;
		leRegistroDoArqBin(bin, rd);
		
		if (id == rd->idServidor && strcmp(rd->removido, "-") == 0) {
			regEncontrado = 1;
			printaReg_Busca(rc, rd);
			printf("Número de páginas de disco acessadas: %d\n", *numPaginasAcessadas);

			return 1;
		}

		// get_page_info(d, found, paginaAtual, numPaginasAcessadas, tam_bin);
		if(*paginaAtual - REM_TAM - TAM_TAM - rd->tamanhoRegistro == 0) {
			*numPaginasAcessadas = *numPaginasAcessadas + 1;
			*paginaAtual = NUM_PAG_DISCO;
		} else 
			*paginaAtual = *paginaAtual - REM_TAM - TAM_TAM - rd->tamanhoRegistro;

		// if (ftell(bin) == tam_bin) {
			
		// 	if (regEncontrado == 0)
		// 		printf("Registro inexistente.\n");
			
		// 	printf("\nNúmero de páginas de disco acessadas: %d\n", *numPaginasAcessadas);
		// 	return;

		// }
		
		free(rd);
	}

	return 0;
}