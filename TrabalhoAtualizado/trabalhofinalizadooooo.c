#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Conven??o de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);
typedef int (CALLCONV *LimpaBufferModoPagina_t)(void);


/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;
static LimpaBufferModoPagina_t        LimpaBufferModoPagina         = NULL;


/* ======================= Configura??o ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver s?mbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Fun??es para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);
    LOAD_FN(g_hDll, LimpaBufferModoPagina);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Fun??es a serem implementadas pelos alunos ======================= */

static void exibirMenu()
{
    // TODO: implementar exibi??o do menu principal com as op??es de impress?o
    
    int opcao;
    
    printf("\n=================MENU DA IMPRESSORA=================\n\n");
    
    printf("1 -Configurar Conexao\n");
    printf("2 -Abrir Conexao\n");
    printf("3 -Impressao Texto\n");
    printf("4 -Impressao QRCode\n");
    printf("5- Impressao Cod Barras\n");
    printf("6- Impressao XML SAT\n");
    printf("7- Impressao XML Canc SAT\n");
    printf("8- Abrir Gaveta Elgin\n");
    printf("9- Abrir Gaveta\n");
    printf("10- Sinal Sonoro\n");
    printf("0- Fechar Conexao e Sair\n\n");
     
    
}

static void configurarConexao(void)
{
    // TODO: pedir ao usu?rio tipo, modelo, conex?o e par?metro
   
    
    printf("\nEscolha o tipo de conexao:\n");
    printf("1 - USB\n");
    printf("2 - RS232\n");
    printf("3 - TCP/IP\n");
    printf("4 - Bluetooth\n");
    printf("5 - Impressoras acopladas(Android)\n");
    
    scanf("%i", &g_tipo);
    
    printf("\nEscolha o modelo:\n");
    printf("i7\n");
    printf("i8\n");
    printf("i9\n");
    
    scanf("%s", &g_modelo);
    
    printf("\nDigite qual e tipo de conexao\n");
    printf("1 - USB\n");
    printf("2 - RS232 \n");
    
    scanf("%s", &g_conexao);
   
   g_parametro = 0;
   
   printf("Dados salvos com sucesso!\n\n\n");
   
  
   
}

static void abrirConexao()
{
    if(g_conectada == 0){
    	
		int ret = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
    	if(ret == 0){
    		g_conectada = 1;
    		LimpaBufferModoPagina();
    		printf("Conexao aberta com sucesso!\n");
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Ja tem uma conexao em aberto\n");
	}
}

static void fecharConexao()
{
   
     if(g_conectada == 1){
    	
		int ret = FechaConexaoImpressora();
    	if(ret == 0){
    		g_conectada = 0;
    		printf("Conexao fechada com sucesso!\n");
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Ja ta fechada seu cabecudo\n");
	}
    	

}
    // TODO: chamar FechaConexaoImpressora e tratar retorno


static void imprimirTexto()
{
    char dados [20];
    int posicao;
    int stilo;
    int tamanho;
    
	if(g_conectada == 1){		
    	
		int ret = ImpressaoTexto("Teste", 1, 4, 0);
		AvancaPapel(2);
		Corte(2);
	
    	if(ret == 0){
    		printf("Impressao realizada com sucesso!\n");
    		
    		
    		
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n");
	}
	
}

static void imprimirQRCode(void)
{
	char dados [20];
	int 	tamanho;
	int 	nivelCorrecao;
	
	if(g_conectada == 1){
    	
		int ret = ImpressaoQRCode("Teste de impressao", 6, 4);
		
	
    	if(ret == 0){
    		printf("Impressao realizada com sucesso!\n");
    		AvancaPapel(2);
    		Corte(2);
    		
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n");
	}
	
    // TODO: solicitar conte?do do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
}

static void imprimirCodigoBarras(void)
{
       
       if(g_conectada == 1){
    	
		int ret = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
		
	
    	if(ret == 0){
    		printf("Impressao realizada com sucesso!\n\n\n");
    		AvancaPapel(2);
    		Corte(2);
    		
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n");
	}
    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLSAT(void)
{
	//char dados[300];
       if(g_conectada == 1){
    	
    	const char* dados = "path=C:./XMLSAT.xml";
		int ret = ImprimeXMLSAT(dados, 0);
		
	
    	if(ret == 0){
    		printf("Impressao realizada com sucesso!\n\n\n");
    		AvancaPapel(2);
    		Corte(2);
    		
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n");
	}
       
    // TODO: ler o arquivo ./XMLSAT.xml e enviar via ImprimeXMLSAT
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLCancelamentoSAT(void)
{
       //const char * dados [300];
       //const char assQRCode [45000];
       int 	param;
       
       if(g_conectada == 1){
    	
    	const char* assQRCode = "path=C:./CANC_SAT.xml";
		
		int ret = ImprimeXMLCancelamentoSAT(assQRCode, "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==", 0);
		
	
    	if(ret == 0){
    		printf("Impressao realizada com sucesso!\n\n\n");
    		AvancaPapel(2);
    		Corte(2);
    		
		}else{
			printf("Deu erro ai. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n");
	}
       
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
	/*usar assinatura abaixo:
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        */
}

static void abrirGavetaElginOpc(void)
{
       
       if(g_conectada == 1){
    	
		int ret = AbreGavetaElgin(1, 50, 50);
		
	
    	if(ret == 0){
    		printf("Gaveta Elgin aberta!\n\n\n");
    		
		}else{
			printf("Erro ao abrir gaveta Elgin. retorno: %d\n\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n\n");
	}
    // TODO: chamar AbreGavetaElgin(1, 50, 50)
}

static void abrirGavetaOpc(void)
{
       
       if(g_conectada == 1){
    	
		int ret = AbreGaveta(1, 5, 10);
		
	
    	if(ret == 0){
    		printf("Gaveta aberta!\n\n\n");
    		
		}else{
			printf("Erro ao abrir gaveta. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n\n");
	}
    // TODO: chamar AbreGaveta(1, 5, 10)
}

static void emitirSinalSonoro(void)
{
       if(g_conectada == 1){
    	
		int ret = SinalSonoro(4, 50, 5);
		
	
    	if(ret == 0){
    		printf("Sianl sonoro emitido!\n\n\n");
    		
		}else{
			printf("Erro ao emitir sinal sonoro. retorno: %d\n", ret);
		}
    	
	}else{
		printf("Nao tem nenhuma conexao em aberto\n\n");
	}
    // TODO: chamar SinalSonoro(4, 50, 5)
}

/* ======================= Fun??o principal ======================= */
int main(void)
{
	int escolha;
    if (!carregarFuncoes()) {    	
        return 1;
    }
   	int opcao;
    while (1) {
        exibirMenu();
        	printf("Escolha a opcao desejada:\n");
        scanf("%i", &opcao);
        
        if(opcao == 0){
        	fecharConexao();
			break;	
		}
        switch (opcao) {
        	case 1:
        		configurarConexao();
        		break;
			case 2:
				abrirConexao();
				break;
			case 3:
				//LimpaBufferModoPagina();
				imprimirTexto();
				break;
			case 4:
				imprimirQRCode();
				break;
			case 5:
                 imprimirCodigoBarras();
                 break;   
            case 6:
                 imprimirXMLSAT();
                 break;     
			case 7:
                 imprimirXMLCancelamentoSAT();
                 break;
            case 8:
                 abrirGavetaElginOpc();
                 break;
            case 9:
                 abrirGavetaOpc();
                 break;
            case 10:
                 emitirSinalSonoro();
                 break;
            default:
                 printf("A opcao escolhida nao existe, escolha outra opcao :)");
                 break;
		}     
        	
        
        
        //construir o menu e chamar as fun?oes aqui!!!             
        
    }
}
