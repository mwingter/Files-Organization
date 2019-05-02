

#define CAB_TAM		214
#define STATUS_TAM 	1
#define TOPO_TAM 	8
#define TAG_TAM 	1
#define CAMPO_TAM	40

#define TAM_PAG_DISCO 32000


// ***** STRUCT DO REGISTRO DE CABEÇALHO *****
typedef struct regcab_ REGCAB; //tam = 214 bytes, e ocupa uma pagina de disco, mas como ele é menor q a pagina, completar a pagina com '@'
struct regcab_{
	char status; /*indica a consistência do arquivo de dados, devido à queda de energia, travamento do programa, etc. 
				  *	Pode assumir os valores 0, para indicar que o arquivo de dados está inconsistente, ou 1, para 
				  *	indicar que o arquivo de dados está consistente. Ao se abrir um arquivo para escrita, seu status 
				  *	deve ser 0 e, ao finalizar o uso desse arquivo, seu status deve ser 1*/

	long int topoLista; //sempre igual à -1

	char tags[5];  /*	*tagCampo1: valor resumido da tag idServidor. Deve assumir o valor i – tam: string de 1 byte.
						*tagCampo2: " " " salarioServidor. Deve assumir o valor s – tam: string de 1 byte.
						*tagCampo3: " " " telefoneServidor. Deve assumir o valor t – tam: string de 1 byte.
						*tagCampo4: " " " nomeServidor. Deve assumir o valor n – tam: string de 1 byte.
						*tagCampo5: " " " cargoServidor. Deve assumir o valor c – tam: string de 1 byte.
					*/
					
	char campos[5][CAMPO_TAM]; /*	*desCampo1: idServidor. Deve assumir o valor ‘numero de identificacao do servidor’ – tam: string de 40 bytes.
							*desCampo2: salarioServidor. Deve assumir o valor ‘salario do servidor’ – tam: string de 40 bytes.
							*desCampo3: telefoneServidor. Deve assumir o valor ‘telefone celular do servidor’ – tam: string de 40 bytes.
							*desCampo4: nomeServidor. Deve assumir o valor ‘nome do servidor’ – tam: string de 40 bytes.
							*desCampo5: cargoServidor. Deve assumir o valor ‘cargo do servidor’ – tam: string de 40 bytes.
						*/
};

void criaRegCabecalho(FILE* f, REGCAB* rc);
void regCabToArqBin(REGCAB* c, FILE* bin);
void atualizaStatus(REGCAB* rc, FILE* bin);
void leCabecalho(FILE* bin, REGCAB *rc);