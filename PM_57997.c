/***************************************************************************************************
Autor : David Laranjinho
Numero : 57997
Projeto : Gestao Maritima
Nome Projeto : PM_57997.c
***************************************************************************************************/

/**************************************************************************************************
Para correr na vm deve se trocar os fflush por __fpurge e adicioanr a biblioteca <stdio_ext.h>
**************************************************************************************************/

#include <stdio.h>
// #include <stdio_ext.h> // para usar __fpurge (stdin)
#include <stdlib.h>
#include <string.h>

#define bool int
#define FALSE 0
#define TRUE 1

#define NUM_TEXTFILES 3

#define COLOR_RED     "\x1b[31m"    // Altera a cor do output no terminal para vermelho
#define COLOR_GREEN   "\x1b[32m"    // Altera a cor do output no terminal para verde
#define COLOR_BLUE    "\x1b[34m"    // Altera a cor do output no terminal para azul
#define COLOR_RESET   "\x1b[0m"     // Reset

/***************************************************************************************************
struct _navio
    - Esta estrutura ira armazenar as informacoes dos navios
    - Implementando com uma lista ligada simples
***************************************************************************************************/

struct _navio{
    int id;
    char *nome;
    char *tipo;
    int capacidade;
    int ano;
    char *pais;
    float consumo_combustivel;
    float custo_pessoal;
    struct _navio *seguinte;
};

/***************************************************************************************************
struct _porto
    - Esta estrutura ira armazenar as informacoes dos portos
    - Implementado com uma lista ligada simples
***************************************************************************************************/

struct _porto{
    int id;
    char *nome;
    char *pais;
    float custo_docking;
    struct _porto *seguinte;
};

/***************************************************************************************************
struct _viagem
    - Esta estrutura ira armazenar as informacoes das viagens
    - Decidi usar apontadores para os navios e portos em vez de armazenar os ids devido a maior rapidez
    de acesso a informacao dos mesmos uma evz que elimino o processo de procurar atraves da lista respetiva
    - Implementado com uma lista ligada simples
***************************************************************************************************/

struct _viagem{
    int id;
    struct _navio *navio;
    struct _porto *porto_origem;
    struct _porto *porto_destino;;
    int data_partida;
    int data_chegada;
    int carga;
    float pagamento;
    struct _viagem *seguinte;
};

/***************************************************************************************************
struct _lucro
    - Esta estrutura ira armazenar as informacoes dos lucros
    - Decidi usar um apontador para o navio em vez de armazenar o seu id devido a maior rapidez de acesso
    - Decidi que seria benefico usar uma quarta estrutura auxiliar a funcao 9, particularmente para armazenar 
    os navios e rendimentos para depois ordenar
    - Implementado com uma lista ligada simples
***************************************************************************************************/

struct _lucro{
    struct _navio *navio;
    float rendimento;
    struct _lucro *seguinte;
};

typedef struct _navio *NAVIO;
typedef struct _porto *PORTO;
typedef struct _viagem *VIAGEM;
typedef struct _lucro *LUCRO;

/***************************************************************************************************
Funcoes sobre navios : 

    - Novo_navio
    - Destroi_Navios
    - Obtem_Navio
    - Ler_Navios
***************************************************************************************************/

/***************************************************************************************************
Novo_Navio
    - Esta funcao recebe as informacoes necessarias a criacao de um novo navio, aloca o espaco necessario
    e retorna esta estrutura criada
***************************************************************************************************/

NAVIO Novo_Navio (int id, char *nome, char *tipo, int capacidade, int ano, char *pais, float consumo_combustivel, float custo_pessoal){
    NAVIO navio = (NAVIO) malloc (sizeof (struct _navio));

    navio -> id = id;
    navio -> nome = (char*) malloc (sizeof (char) * (strlen (nome) + 1));
    strcpy (navio -> nome, nome);
    navio -> tipo = (char*) malloc (sizeof (char) * (strlen (tipo) + 1));
    strcpy (navio -> tipo, tipo);
    navio -> capacidade = capacidade;
    navio -> ano = ano;
    navio -> pais = (char*) malloc (sizeof (char) * (strlen (pais) + 1));
    strcpy (navio -> pais, pais);
    navio -> consumo_combustivel = consumo_combustivel;
    navio -> custo_pessoal = custo_pessoal;
    navio -> seguinte = NULL;

    return navio;
}

/***************************************************************************************************
Destroi_Navios
    - Esta funcao liberta a memoria alocada (durante o Novo_Navio)
***************************************************************************************************/

void Destroi_Navios (NAVIO navio){
    NAVIO aux = navio -> seguinte;

    while (navio != NULL){
        aux = navio -> seguinte;
        free (navio -> nome);
        free (navio -> tipo);
        free (navio -> pais);
        free (navio);
        navio = aux;
    }
}

/***************************************************************************************************
Obtem_Navio
    - Esta funcao recebe o primeiro elemento da lista ligada de navios, e um id. Apos iterar sobre a lista
    devolve o navio caso exista ou devolve NULL nao
***************************************************************************************************/

NAVIO Obtem_Navio (NAVIO navio, int id){

    while (navio != NULL){
        if (navio -> id == id)
            return navio;
        navio = navio -> seguinte;
    }
    return NULL;
}

/***************************************************************************************************
Ler_Navios
    - Esta funcao le o ficheiro de texto correspondente aos navios e retorna FALSE se falhou a abrir
    o ficheiro ou TRUE se teve sucesso.
***************************************************************************************************/

bool Ler_Navios (NAVIO *navio, int *nr_navios){
    int id, capacidade, ano;
    float consumo_combustivel, custo_pessoal;
    char nome[40], tipo[40], pais[40];
    char *textfile = "textfiles/ships.txt";
    NAVIO navio_ant, navio_seg;
    FILE *fp = fopen (textfile, "r");

    printf ("A abrir %s... ", textfile);
    if (fp == NULL){
        printf (COLOR_RED "Erro ao abrir o ficheiro!\n" COLOR_RESET);
        return FALSE;
    }
    printf (COLOR_GREEN "Sucesso!\n" COLOR_RESET);

    while (fscanf (fp, "%d;%[^;];%[^;];%d;%d;%[^;];%f;%f;", &id, nome, tipo, &capacidade, &ano, pais, &consumo_combustivel, &custo_pessoal) == 8){
        navio_seg = Novo_Navio (id, nome, tipo, capacidade, ano, pais, consumo_combustivel, custo_pessoal);
        if (*nr_navios == 0){
            *navio = navio_seg;
            navio_ant = navio_seg;
        }
        else{
            navio_ant -> seguinte = navio_seg;
            navio_ant = navio_seg;
        }
        (*nr_navios)++;
    }
    fclose (fp);
    return TRUE;
}

/***************************************************************************************************
Funcoes sobre portoss : 

    - Novo_Porto
    - Destroi_Portos
    - Obtem_Porto
    - Ler_Portos
***************************************************************************************************/

/***************************************************************************************************
Novo_Porto
    - Esta funcao recebe as informacoes necessarias a criacao de um porto, aloca o espaco necessario
    e retorna essa estrutura criada
***************************************************************************************************/

PORTO Novo_Porto (int id, char *nome, char *pais, float custo_docking){
    PORTO porto = (PORTO) malloc (sizeof (struct _porto));

    porto -> id = id;
    porto -> nome = (char*) malloc (sizeof (char) * (strlen (nome) + 1));
    strcpy (porto -> nome, nome);
    porto -> pais = (char*) malloc (sizeof (char) * (strlen (pais) + 1));
    strcpy (porto -> pais, pais);
    porto -> custo_docking = custo_docking;
    porto -> seguinte = NULL;

    return porto;
}

/***************************************************************************************************
Destroi_Portos
    - Esta funcao liberta a memoria alocada (durante o Novo_Porto)
***************************************************************************************************/

void Destroi_Portos (PORTO porto){
    PORTO aux = porto -> seguinte;

    while (aux != NULL){
        aux = porto -> seguinte;
        free (porto -> nome);
        free (porto -> pais);
        free (porto);
        porto = aux;
    }
}

/***************************************************************************************************
Obtem_Porto
    - Esta funcao recebe o primeiro elemento da lista ligada de portos e um id. Apos iterar sobre a lista
    e devolve o porto caso exista ou NULL caso nao
***************************************************************************************************/

PORTO Obtem_Porto (PORTO porto, int id){

    while (porto != NULL){
        if (porto -> id == id)
            return porto;
        porto = porto -> seguinte;
    }
    return porto;
}

/***************************************************************************************************
Ler_Portos
    - Esta funcao le o ficheiro de texto correspondente aos navios e retorna FALSE se falhou a abrir
    o ficheiro ou TRUE se teve sucesso
***************************************************************************************************/

bool Ler_Portos (PORTO *porto, int *nr_portos){
    int id;
    float custo_docking;
    char nome[40], pais[40];
    char *textfile = "textfiles/ports.txt";
    PORTO porto_ant, porto_seg;
    FILE *fp = fopen (textfile, "r");

    printf ("A abrir %s... ", textfile);
    if (fp == NULL){
        printf (COLOR_RED "Erro ao abrir o ficheiro\n" COLOR_RESET);
        return FALSE;
    }
    printf (COLOR_GREEN "Sucesso!\n" COLOR_RESET);

    while (fscanf (fp, "%d;%[^;];%[^;];%f;", &id, nome, pais, &custo_docking) == 4){
        porto_seg = Novo_Porto (id, nome, pais, custo_docking);
        if (*nr_portos == 0){
            *porto = porto_seg;
            porto_ant = porto_seg;
        }
        else{
            porto_ant -> seguinte = porto_seg;
            porto_ant = porto_seg;
        }
        (*nr_portos)++;
    }
    fclose (fp);
    return TRUE;
}

/***************************************************************************************************
Funcoes sobre navios : 

    - Nova_Viagem
    - Destroi_Viagens
    - Existe_Viagem
    - Ler_Viagens
    - Obtem_Menor_ID_Viagem
    - Obtem_ID_Viagem
***************************************************************************************************/

/***************************************************************************************************
Nova_Viagem
    - Esta funcao recebe as informacoes necessarias a criacao de uma viagem, aloca o espaco necessario
    e retorna essa estrutura criada
***************************************************************************************************/

VIAGEM Nova_Viagem (int id, NAVIO navio, PORTO porto_origem, PORTO porto_destino, int data_partida, int data_chegada, int carga, float pagamento){
    VIAGEM viagem = (VIAGEM) malloc (sizeof (struct _viagem));

    viagem -> id = id;
    viagem -> navio = navio;
    viagem -> porto_origem = porto_origem;
    viagem -> porto_destino = porto_destino;
    viagem -> data_partida = data_partida;
    viagem -> data_chegada = data_chegada;
    viagem -> carga = carga;
    viagem -> pagamento = pagamento;
    viagem -> seguinte = NULL;

    return viagem;
}

/***************************************************************************************************
Destroi_Viagens
    - Esta funcao liberta a memoria alocada (durante a Nova_Viagem)
***************************************************************************************************/

void Destroi_Viagens (VIAGEM viagem){
    VIAGEM aux = viagem -> seguinte;

    while (viagem != NULL){
        aux = viagem -> seguinte;
        free (viagem);
        viagem = aux;
    }
}

/***************************************************************************************************
Obtem_Viagem
    - Esta funcao recebe o primeiro elemento da lista ligada de viagens e um id. Apos iterar sobre a lista
    e devolve a viagem caso exista ou NULL caso nao
***************************************************************************************************/

VIAGEM Obtem_Viagem (VIAGEM viagem, int id){

    while (viagem != NULL){
        if (viagem -> id == id)
            return viagem;
        viagem = viagem -> seguinte;
    }
    return NULL;
}

/***************************************************************************************************
Existe_Viagem
    - Esta funcao recebe o primeiro elemento da lista ligada de viagens e um id. Apos iterar sobre a lista
    e devolve TRUE caso exista ou FALSE caso nao
***************************************************************************************************/

bool Existe_Viagem (VIAGEM viagem, int id){

    while (viagem != NULL){
        if (viagem -> id == id)
            return TRUE;
        viagem = viagem -> seguinte;
    }
    return FALSE;
}

/***************************************************************************************************
Obtem_Menor_ID_Viagem
    - Esta funcao itera sobre a lista e retorna o menor id (inteiro positivo) que nao esta em uso
***************************************************************************************************/

int Obtem_Menor_ID_Viagem (VIAGEM viagem){
    int menor_id = 1;
    VIAGEM viagem_aux = viagem;

    while (viagem_aux != NULL){
        if (viagem_aux -> id == menor_id){
            menor_id++;
            viagem_aux = viagem;
        }
        viagem_aux = viagem_aux -> seguinte;
    }
    return menor_id;
}

/***************************************************************************************************
Obtem_ID_VIAGEM
    - Esta funcao pede e retorna um id valido para a funcao 9
***************************************************************************************************/

int Obtem_ID_Viagem (VIAGEM viagem, char *text){
    int id, menor_id = Obtem_Menor_ID_Viagem (viagem);
    bool id_em_uso;

    do{
        printf ("Introduza o ID da viagem (%d esta disponivel): ", menor_id);
        while (scanf (" %d", &id) != 1){
            printf (COLOR_RED "Erro na introducao de dados!\n");
            printf ("Introduza o ID da viagem (%d esta disponivel): ", menor_id);
        }
        if ((id_em_uso = Existe_Viagem (viagem, id)) == TRUE)
            printf (COLOR_RED "Uma viagem com esse ID ja existe!\n" COLOR_RESET);
    } while (id_em_uso == TRUE);
    return id;
}

/***************************************************************************************************
Ler_Viagens
    - Esta funcao le o ficheiro de texto correspondente as viagens e retorna FALSE se falhou a abrir
    o ficheiro ou TRUE se teve sucesso
***************************************************************************************************/

bool Ler_Viagens (NAVIO navio, PORTO porto, VIAGEM *viagem, VIAGEM *ultima_viagem, VIAGEM *ultima_viagem_file, int *nr_viagens){
    int id, id_navio, id_porto_origem, id_porto_destino, data_partida, data_chegada, carga;
    float pagamento;
    char *textfile = "textfiles/trips.txt";
    NAVIO navio_aux;
    PORTO porto_origem, porto_destino;
    VIAGEM viagem_ant, viagem_seg;
    FILE *fp = fopen (textfile, "r");

    printf ("A abrir %s... ", textfile);
    if (fp == NULL){
        printf (COLOR_RED "Erro ao abrir o ficheiro\n" COLOR_RESET);
        return FALSE;
    }
    printf (COLOR_GREEN "Sucesso!\n" COLOR_RESET);

    while (fscanf (fp, "%d;%d;%d;%d;%d;%d;%d;%f;", &id, &id_navio, &id_porto_origem, &id_porto_destino, &data_partida, &data_chegada, &carga, &pagamento) == 8){
        if ((navio_aux = Obtem_Navio (navio, id_navio)) == NULL){
            printf (COLOR_RED "O navio com id %d nao existe!\n" COLOR_RESET, id_navio);
            continue;
        }
        if ((porto_origem = Obtem_Porto (porto, id_porto_origem)) == NULL){
            printf (COLOR_RED "O porto com id %d nao existe!\n" COLOR_RESET, id_porto_origem);
            continue;
        }
        if ((porto_destino = Obtem_Porto (porto, id_porto_destino)) == NULL){
            printf (COLOR_RED "O porto com id %d nao existe!\n" COLOR_RESET, id_porto_destino);
            continue;
        }
        viagem_seg = Nova_Viagem (id, navio_aux, porto_origem, porto_destino, data_partida, data_chegada, carga, pagamento);
        if (*nr_viagens == 0){
            *viagem = viagem_seg;
            viagem_ant = viagem_seg;
        }
        else{
            viagem_ant -> seguinte = viagem_seg;
            viagem_ant = viagem_seg;
        }
        (*nr_viagens)++;
    }
    *ultima_viagem = viagem_seg;
    *ultima_viagem_file = viagem_seg;
    fclose (fp);

    return TRUE;
}

/***************************************************************************************************
Funcoes sobre lucros : 

    - Novo_Lucro
    - Destroi_Lucros
    - Listar_Lucros
    - Formar_Lista_Lucros
    - Agrupa_Lista
    - Divide_Lista
    - Ordena_Lucros
***************************************************************************************************/

/***************************************************************************************************
Novo_Lucro
    - Esta funcao recebe as informacoes necessarias a criacao de um lucro, aloca o espaco necessario
    e retorna essa estrutura criada
***************************************************************************************************/


LUCRO Novo_Lucro (NAVIO navio, float rendimento){
    LUCRO lucro = (LUCRO) malloc (sizeof (struct _lucro));

    lucro -> navio = navio;
    lucro -> rendimento = rendimento;
    lucro -> seguinte = NULL;

    return lucro;
}

/***************************************************************************************************
Destroi_Lucro
    - Esta funcao liberta a memoria alocada (durante a Novo_Lucro)
***************************************************************************************************/

void Destroi_Lucro (LUCRO lucro){
    LUCRO aux = lucro -> seguinte;

    while (lucro != NULL){
        aux = lucro -> seguinte;
        free (lucro);
        lucro = aux;
    }
}

/***************************************************************************************************
Listar_Lucros 
    - Esta funcao lista todas as estruturas do tipo lucro presentes na lista ligada
***************************************************************************************************/


void Listar_Lucros (LUCRO lucro){
    
    printf (COLOR_BLUE "Lista de possiveis escolhas : \n" COLOR_RESET);
    while (lucro != NULL){
        printf (COLOR_BLUE "ID : " COLOR_RESET "%5d ", lucro -> navio -> id);
        printf (COLOR_BLUE "Nome : " COLOR_RESET "%-35s ", lucro -> navio -> nome);
        printf (COLOR_BLUE "Pais : " COLOR_RESET "%-35s ", lucro -> navio -> pais);
        printf (COLOR_BLUE "Ano : " COLOR_RESET "%10d ", lucro -> navio -> ano);
        printf (COLOR_BLUE "Lucro : " COLOR_RESET "%.2f\n", lucro -> rendimento);
        lucro = lucro -> seguinte;
    }
}

/***************************************************************************************************
Formar_Lista_Lucros
    - Esta funcao recebe as informacoes necessarias a criar uma lista de lucros e cria essa lista
***************************************************************************************************/


void Formar_Lista_Lucros (LUCRO *lucro, NAVIO navio, PORTO porto_origem, PORTO porto_destino, int data_partida, int data_chegada, int carga, float pagamento, char *tipo){
    LUCRO lucro_ant, lucro_seg;
    int count = 0;
    float rendimento;

    while (navio != NULL){
        if (!strcmp (navio -> tipo, tipo)){
            rendimento = pagamento - ((porto_origem -> custo_docking + porto_destino -> custo_docking) + (navio -> consumo_combustivel + navio -> custo_pessoal) * (data_chegada - data_partida + 1));
            if (rendimento > 0){
                lucro_seg = Novo_Lucro (navio, rendimento);
                if (count == 0){
                    *lucro = lucro_seg;
                    lucro_ant = lucro_seg;
                }
                else{
                    lucro_ant -> seguinte = lucro_seg;
                    lucro_ant = lucro_seg;
                }
                count++;
            }
        }
        navio = navio -> seguinte;
    }
    if (count == 0)
        *lucro = NULL;
}

/***************************************************************************************************
Agrupa_Lista
    - Esta funcao recebe duas estruturas do tipo lucro e agrupa as de acordo com a ordenacao
***************************************************************************************************/


LUCRO Agrupa_Lista(LUCRO aux1, LUCRO aux2) 
{ 
    LUCRO result = NULL; 
  
    if (aux1 == NULL) 
        return (aux2); 
    else if (aux2 == NULL) 
        return (aux1); 
  
    if (aux1 -> rendimento > aux2 -> rendimento) { 
        result = aux1; 
        result -> seguinte = Agrupa_Lista (aux1 -> seguinte, aux2); 
    } 
    else { 
        result = aux2; 
        result -> seguinte = Agrupa_Lista (aux1, aux2 -> seguinte); 
    } 
    return (result); 
}

/***************************************************************************************************
Divide_Lista
    - Esta funcao recebe divide a lista em 2 listas as quais sao divididas recursivamente e ordenadas
    e, por fim, ordenadas
***************************************************************************************************/

void Divide_Lista(LUCRO head, LUCRO* head1, LUCRO* head2) { 
    LUCRO fast; 
    LUCRO slow; 
    slow = head; 
    fast = head ->seguinte; 
  
    while (fast != NULL) { 
        fast = fast -> seguinte; 
        if (fast != NULL) { 
            slow = slow -> seguinte; 
            fast = fast -> seguinte; 
        } 
    } 
  
    *head1 = head ; 
    *head2 = slow -> seguinte; 
    slow -> seguinte = NULL; // cortar a lista a meio
} 

void Ordena_Lucros (LUCRO *lucro){
    LUCRO head = *lucro; 
    LUCRO aux1; 
    LUCRO aux2; 
  
    if ((head == NULL) || (head -> seguinte == NULL)) { 
        return; 
    } 
  
    Divide_Lista (head, &aux1, &aux2); 
  
    Ordena_Lucros(&aux1); 
    Ordena_Lucros(&aux2); 
  
    *lucro = Agrupa_Lista(aux1, aux2);  
}

/********************************************************************************
Funcoes do auxiliares :

    - Obtem_Int
    - Obtem_Int_Intervalo
    - Obtem_Float
    - Obtem_Float_Acima
    - Obtem_Tipo
    - Sair_Funcao
********************************************************************************/

/***************************************************************************************************
Obtem_Int
    - Esta funcao devolve um inteiro apos de ser bem verificado
***************************************************************************************************/

int Obtem_Int (char *text){
    int nro;

    printf ("Introduza o %s: ", text);
    while (scanf (" %d", &nro) != 1){
        fflush (stdin); // __fpurge (stdin);
        printf (COLOR_RED "Erro na introducao de valores\n" COLOR_RESET);
        printf ("Introduza o %s: ", text);
    }
    return nro;
}

/***************************************************************************************************
Obtem_Int_Intervalo
    - Esta funcao devolve um inteiro apos de ser bem verificado e esta num dado intervalo (min, max)
***************************************************************************************************/

int Obtem_Int_Intervalo (int min, int max, char *text){
    int nro;

    nro = Obtem_Int (text);
    while (nro < min || nro > max){
        printf (COLOR_RED "Erro! O valor deve ser entre %d e %d!\n" COLOR_RESET, min, max);
        nro = Obtem_Int (text);
    }
    return nro;
}

/***************************************************************************************************
Obtem_Float
    - Esta funcao devolve um float apos de ser bem verificado
***************************************************************************************************/

float Obtem_Float (char *text){
    float nro;

    printf ("Introduza o %s: ", text);
    while (scanf (" %f", &nro) != 1){
        fflush (stdin); // __fpurge (stdin);
        printf (COLOR_RED "Erro na introducao de valores\n" COLOR_RESET);
        printf ("Introduza o %s: ", text);
    }
    return nro;
}

/***************************************************************************************************
Obtem_Float_Acima
    - Esta funcao devolve um float apos de ser bem verificado e que esta acima de um dado numero (min)
***************************************************************************************************/

float Obtem_Float_Acima (float min, char *text){
    float nro;

    nro = Obtem_Float (text);
    while (nro < min){
        printf (COLOR_RED "Erro! O valor deve ser acima de %f!\n" COLOR_RESET, min);
        nro = Obtem_Float (text);
    }
    return nro;
}

/***************************************************************************************************
Obtem_Tipo
    - Esta funcao pede e verifica se o tipo introduzido e valido
***************************************************************************************************/

void Obtem_Tipo (char *tipo){
    char *tipos[3] = {"Bulker", "Container", "Tanker"};
    int i;
    bool valido = FALSE;

    do{
        printf ("Introduza o tipo [Bulker, Container, Tanker] : ");
        fflush (stdin); // __fpurge (stdin);
        fgets (tipo, 15, stdin);
        tipo [strlen (tipo) - 1] = '\0';
        for (i = 0; i < 3; i++){
            if (!strcmp (tipo, tipos[i])){
                valido = TRUE;
                break;
            }
        }
    } while (valido == FALSE);
}

/***************************************************************************************************
Sair_Funcao
    - Esta funcao esta presente no fim das funcoes do menu para permitir o utilizador sair ao seu ritmo
    apos vizualizar a informacao
***************************************************************************************************/

void Sair_Funcao (){
    
    fflush (stdin); // __fpurge (stdin);
    printf ("Pressione ENTER para continuar\n");
    getchar ();
    fflush (stdin); // __fpurge (stdin);
    
}

/********************************************************************************
Funcoes do menu :
    - Listar_Navios
    - Listar_Portos
    - Listar_Viagens
    - Listar_Carga_Tempo
    - Guardar_Viagens
    - Listar_Carga_Portos
    - Analisar_Rendimento
    - Adicionar_Viagem
    - Adicionar_Viagem_Auto
********************************************************************************/

/***************************************************************************************************
Listar_Navios
    - Esta funcao Lista todos os navios presentes na lista
***************************************************************************************************/

void Listar_Navios (NAVIO navio){

    while (navio != NULL){
        printf (COLOR_BLUE "Id : " COLOR_RESET "%-10d ", navio -> id);
        printf (COLOR_BLUE "Nome : " COLOR_RESET "%-40s ", navio -> nome);
        printf (COLOR_BLUE "Pais : " COLOR_RESET "%-40s ", navio -> pais);
        printf (COLOR_BLUE "Ano : " COLOR_RESET "%-10d", navio -> ano);
        printf (COLOR_BLUE "Consumo : " COLOR_RESET "%-15.2f", navio -> consumo_combustivel);
        printf (COLOR_BLUE "Custo Tripulacao : " COLOR_RESET "%-10.2f\n", navio -> custo_pessoal);

        navio = navio -> seguinte;
    }
    Sair_Funcao ();
}

/***************************************************************************************************
Listar_Portos
    - Esta funcao Lista todos os portos presentes na lista
***************************************************************************************************/

void Listar_Portos (PORTO porto){

    while (porto != NULL){
        printf (COLOR_BLUE "Id : " COLOR_RESET "%-10d ", porto -> id);
        printf (COLOR_BLUE "Nome : " COLOR_RESET "%-40s ", porto -> nome);
        printf (COLOR_BLUE "Pais : " COLOR_RESET "%-40s ", porto -> pais);
        printf (COLOR_BLUE "Custo :" COLOR_RESET "%-20.2f\n", porto -> custo_docking);

        porto = porto -> seguinte;
    }
    Sair_Funcao ();
}

/***************************************************************************************************
Listar_Viagens
    - Esta funcao Lista todos as viagens presentes na lista
***************************************************************************************************/

void Listar_Viagens (VIAGEM viagem){

    while (viagem != NULL){

        printf (COLOR_BLUE "Id Navio : " COLOR_RESET "%-5d ", viagem -> navio -> id);
        printf (COLOR_BLUE "Nome Navio : " COLOR_RESET "%-17s ", viagem -> navio -> nome);
        printf (COLOR_BLUE "Porto Origem : " COLOR_RESET "%-39s ", viagem -> porto_origem -> nome);
        printf (COLOR_BLUE "Porto Destino : " COLOR_RESET "%-39s ", viagem -> porto_destino -> nome);
        printf (COLOR_BLUE "Data Partida : " COLOR_RESET "%-5d ", viagem -> data_partida);
        printf (COLOR_BLUE "Data Chegada : " COLOR_RESET "%-5d ", viagem -> data_chegada);
        printf (COLOR_BLUE "Lotacao : " COLOR_RESET "%-5d\n", viagem -> carga);

        viagem = viagem -> seguinte;
    }
    Sair_Funcao ();
}

/***************************************************************************************************
Listar_Carga_Tempo 
    - Esta funcao lista todas as viagens de um dado navio num intervalo de tempo e apresenta a soma da sua carga
***************************************************************************************************/

void Listar_Carga_Tempo (NAVIO navio, PORTO porto, VIAGEM viagem){
    int data_partida, data_chegada;
    float carga_viagem, carga_total = 0.00;
    NAVIO navio_aux = NULL;

    while ((navio_aux = Obtem_Navio (navio, Obtem_Int ("ID do navio"))) == NULL)
        printf (COLOR_RED "O navio nao existe!\n" COLOR_RESET);
    data_partida = Obtem_Int_Intervalo (1, 30, "dia de partida");
    data_chegada = Obtem_Int_Intervalo (data_partida + 1, 31, "dia de chegada");

    printf (COLOR_BLUE "Lista de toda a carga do navio %d entre os dias %d e %d:\n" COLOR_RESET, navio_aux -> id, data_partida, data_chegada);
    while (viagem != NULL){
        if ((viagem -> navio == navio_aux) && (viagem -> data_partida >= data_partida) && (viagem -> data_chegada <= data_chegada)){
            carga_viagem = navio_aux -> capacidade * (viagem -> carga / 100.);
            carga_total += carga_viagem;
            printf ("\t - Viagem: %s de %s para %s: %d%% --> %.2fKg\n", navio_aux -> nome, viagem -> porto_origem -> nome, viagem -> porto_destino -> nome, viagem -> carga, carga_viagem);
        }
        viagem = viagem -> seguinte;
    }
    printf ("\n%s com carga total de %.2fKg em viagens entre os dias %d e %d\n", navio_aux -> nome, carga_total, data_partida, data_chegada);
    Sair_Funcao ();
}

/***************************************************************************************************
Guardar_Viagens
    - Esta funcao guarda no ficheiro de texto as viagens que estao na lista e ainda nao foram guardadas
***************************************************************************************************/

void Guardar_Viagens (VIAGEM *ultima_viagem_file){
    int count = 0;
    char *textfile = "textfiles/trips.txt";
    FILE *fp = fopen (textfile, "a");

    if (fp == NULL){
        printf (COLOR_RED "Erro a abrir o ficheiro!\n" COLOR_RESET);
        return;
    }

    while ((*ultima_viagem_file) -> seguinte != NULL){
        *ultima_viagem_file = (*ultima_viagem_file) -> seguinte;
        fprintf (fp, "\n%d;%d;%d;%d;%d;%d;%d;%f;", (*ultima_viagem_file) -> id, (*ultima_viagem_file) -> navio -> id, (*ultima_viagem_file) -> porto_origem -> id, (*ultima_viagem_file) -> porto_destino -> id, (*ultima_viagem_file) -> data_partida, (*ultima_viagem_file) -> data_chegada, (*ultima_viagem_file) -> carga, (*ultima_viagem_file) -> pagamento);
        count++;
    }
    if (count)
        printf ("Foram guardadas %d viagens\n", count);
    else
        printf ("Todas as viagens ja estavam guardadas\n");
    fclose (fp);
    Sair_Funcao ();
}

/***************************************************************************************************
Listar_Carga_Porto
    - Esta funcao lista a carga total transportada entre dois portos (unidirecional) 
***************************************************************************************************/

void Listar_Carga_Porto (NAVIO navio, PORTO porto, VIAGEM viagem){
    int count = 0;
    float carga_viagem, carga_total = 0.00;
    PORTO porto_origem = NULL, porto_destino = NULL;

    while ((porto_origem = Obtem_Porto (porto, Obtem_Int ("ID do porto de origem"))) == NULL)
        printf (COLOR_RED "O porto nao existe!\n" COLOR_RESET);
    while ((porto_destino == NULL) || (porto_destino == porto_origem)){
        if ((porto_destino = Obtem_Porto (porto, Obtem_Int ("ID do porto de destino"))) == NULL)
            printf (COLOR_RED "O porto nao existe!\n");
        if (porto_origem == porto_destino)
            printf (COLOR_RED "O porto destino e igual ao de origem!\n" COLOR_RESET);
    }

    while (viagem != NULL){
        if (((viagem -> porto_origem == porto_origem) && (viagem -> porto_destino == porto_destino)) || ((viagem -> porto_origem == porto_destino) && (viagem -> porto_destino == porto_origem))){
            carga_viagem = viagem -> navio -> capacidade * (viagem -> carga / 100.);
            carga_total += carga_viagem;
            count++;
        }
        viagem = viagem -> seguinte;
    }
    printf (COLOR_BLUE "A carga total de %.2fKg transportada entre %s e %s em %d viagens\n" COLOR_RESET, carga_total, porto_origem -> nome, porto_destino -> nome, count);
    Sair_Funcao ();
}

/***************************************************************************************************
Analisar_Rendimento 
    - Esta funcao analisa o rendimento de uma dada viagem
***************************************************************************************************/

void Analisar_Rendimento (VIAGEM viagem){
    VIAGEM viagem_aux = NULL;
    float rendimento;

    while ((viagem_aux = Obtem_Viagem (viagem, Obtem_Int ("ID da viagem"))) == NULL){
        printf (COLOR_RED "A viagem nao existe!\n" COLOR_RESET);
    }
    rendimento = viagem_aux -> pagamento - ((viagem_aux -> porto_origem -> custo_docking + viagem_aux -> porto_destino -> custo_docking) + (viagem_aux -> navio -> consumo_combustivel + viagem_aux -> navio -> custo_pessoal) * (viagem_aux -> data_chegada - viagem_aux -> data_partida + 1));
    if (rendimento < 0)
        printf ("Rendimento da viagem %d: " COLOR_RED "%.2f\n" COLOR_RESET, viagem_aux -> id, rendimento);
    else    
        printf ("Rendimento da viagem %d: " COLOR_GREEN "+%.2f\n" COLOR_RESET, viagem_aux -> id, rendimento);
    Sair_Funcao ();
}

/***************************************************************************************************
Adicionar_Viagem 
    - Esta funcao pede todas as informacoes necessarias a criacao de uma viagem, cria essa viagem e adiciona ao
    fim da lista ligada
***************************************************************************************************/

void Adicionar_Viagem (NAVIO navio, PORTO porto, VIAGEM viagem, VIAGEM *ultima_viagem, int *nr_viagens){
    int id_viagem, data_partida, data_chegada, carga;
    float pagamento;
    NAVIO navio_aux = NULL;
    PORTO porto_origem = NULL, porto_destino = NULL;

    id_viagem = Obtem_ID_Viagem (viagem, "ID da viagem");
    while ((navio_aux = Obtem_Navio (navio, Obtem_Int ("ID do navio"))) == NULL)
        printf (COLOR_RED "O navio nao existe!\n" COLOR_RESET);
    while ((porto_origem = Obtem_Porto (porto, Obtem_Int ("ID do porto de origem"))) == NULL)
        printf (COLOR_RED "O porto nao existe!\n" COLOR_RESET);
    while ((porto_destino == NULL) || (porto_destino == porto_origem)){
        if ((porto_destino = Obtem_Porto (porto, Obtem_Int ("ID do porto de destino"))) == NULL)
            printf (COLOR_RED "O porto nao existe!\n" COLOR_RESET);
        if (porto_origem == porto_destino)
            printf (COLOR_RED "O porto destino e igual ao de origem!\n" COLOR_RESET);
    }
    data_partida = Obtem_Int_Intervalo (1, 30, "dia de partida");
    data_chegada = Obtem_Int_Intervalo (data_partida + 1, 31, "dia de chegada");
    carga = Obtem_Int_Intervalo (0, 100, "ocupacao");
    pagamento = Obtem_Float_Acima (0, "pagamento");

    (*ultima_viagem) -> seguinte = Nova_Viagem (id_viagem, navio_aux, porto_origem, porto_destino, data_partida, data_chegada, carga, pagamento);
    *ultima_viagem = (*ultima_viagem) -> seguinte;
    (*nr_viagens)++;

    printf (COLOR_GREEN "Viagem %d adicionada com sucesso:\n" COLOR_RESET, (*ultima_viagem) -> id);
    printf (COLOR_BLUE "\t - Partida : " COLOR_RESET "%s (%s) no dia %d\n", (*ultima_viagem) -> porto_origem -> nome, (*ultima_viagem) -> porto_origem -> pais, (*ultima_viagem) -> data_partida);
    printf (COLOR_BLUE "\t - Chegada : " COLOR_RESET "%s (%s) no dia %d\n", (*ultima_viagem) -> porto_destino -> nome, (*ultima_viagem) -> porto_destino -> pais, (*ultima_viagem) -> data_chegada);
    printf (COLOR_BLUE "\t - Ocupacao : " COLOR_RESET "%d\n", (*ultima_viagem) -> carga);
    printf (COLOR_BLUE "\t - Pagamento : " COLOR_RESET "%.2f\n", (*ultima_viagem) -> pagamento);
    printf (COLOR_GREEN "Existem %d viagens no manifesto.\n" COLOR_RESET, *nr_viagens);    
    Sair_Funcao ();
}

/***************************************************************************************************
Adicionar_Viagem_Auto
    - Esta funcao pede todas as informacoes necessarias a criacao de uma viagem a excepcao do id do navio (pede o tipo)
    e de um id (usa o menor disponivel) e mostra todas as viagens com rendimento positivo que satisfazem essas condicoes
    e acrescenta a mais lucrativa ao fim da lista ligada simples
***************************************************************************************************/

void Adicionar_Viagem_Auto (NAVIO navio, PORTO porto, VIAGEM viagem, VIAGEM *ultima_viagem, int *nr_viagens){
    int data_partida, data_chegada, carga;
    float pagamento;
    char tipo[15];
    PORTO porto_origem = NULL, porto_destino = NULL;
    LUCRO lucro = (LUCRO) malloc (sizeof (struct _lucro));

    while ((porto_origem = Obtem_Porto (porto, Obtem_Int ("ID do porto de origem"))) == NULL)
        printf (COLOR_RED "O porto nao existe!\n" COLOR_RESET);
    while ((porto_destino == NULL) || (porto_destino == porto_origem)){
        if ((porto_destino = Obtem_Porto (porto, Obtem_Int ("ID do porto de destino"))) == NULL)
            printf (COLOR_RED "O porto nao existe!\n" COLOR_RESET);
        if (porto_origem == porto_destino)
            printf (COLOR_RED "O porto destino e igual ao de origem!\n" COLOR_RESET);
    }
    data_partida = Obtem_Int_Intervalo (1, 30, "dia de partida");
    data_chegada = Obtem_Int_Intervalo (data_partida + 1, 31, "dia de chegada");
    carga = Obtem_Float_Acima (0, "ocupacao");
    pagamento = Obtem_Float_Acima (0, "pagamento");
    Obtem_Tipo (tipo);

    Formar_Lista_Lucros (&lucro, navio, porto_origem, porto_destino, data_partida, data_chegada, carga, pagamento, tipo);
    if (lucro == NULL){
        printf (COLOR_RED "Nao existem viagens lucrativas com essas especificacoes.\n" COLOR_RESET);
        return;
    }
    Ordena_Lucros (&lucro);
    Listar_Lucros (lucro);

    (*ultima_viagem) -> seguinte = Nova_Viagem (Obtem_Menor_ID_Viagem (viagem), lucro -> navio, porto_origem, porto_destino, data_partida, data_chegada, carga, pagamento);
    (*ultima_viagem) = (*ultima_viagem) -> seguinte;
    (*nr_viagens)++;

    printf (COLOR_GREEN "Viagem %d adicionada com sucesso:\n" COLOR_RESET, (*ultima_viagem) -> id);
    printf (COLOR_BLUE "\t - Partida : " COLOR_RESET "%s (%s) no dia %d\n", (*ultima_viagem) -> porto_origem -> nome, (*ultima_viagem) -> porto_origem -> pais, (*ultima_viagem) -> data_partida);
    printf (COLOR_BLUE "\t - Chegada : " COLOR_RESET "%s (%s) no dia %d\n", (*ultima_viagem) -> porto_destino -> nome, (*ultima_viagem) -> porto_destino -> pais, (*ultima_viagem) -> data_chegada);
    printf (COLOR_BLUE "\t - Ocupacao : " COLOR_RESET "%d\n", (*ultima_viagem) -> carga);
    printf (COLOR_BLUE "\t - Pagamento : " COLOR_RESET "%.2f " COLOR_GREEN "(+%.2f)\n", (*ultima_viagem) -> pagamento, lucro -> rendimento);
    printf (COLOR_GREEN "Existem %d viagens no manifesto.\n" COLOR_RESET, *nr_viagens);
    Destroi_Lucro (lucro);
    Sair_Funcao ();
}

/***************************************************************************************************
menu
    - Esta funcao mostra o menu ao utilizador
***************************************************************************************************/

void menu (char *opcao){

    printf (COLOR_BLUE "\t\t\t GESTAO MARITIMA\n\n" COLOR_RESET);
    printf ("\t 1 - Listar Frota\n");
    printf ("\t 2 - Listar Portos\n");
    printf ("\t 3 - Listar Viagens\n");
    printf ("\t 4 - Listar toda a carga de um navio num determinado periodo de dias\n");
    printf ("\t 5 - Guardar Viagens no ficheiro de Texto\n");
    printf ("\t 6 - Carga transportada entre dois pontos\n");
    printf ("\t 7 - Analisar o rendimento economico de uma determinada viagem\n");
    printf ("\t 8 - Adicionar uma viagem\n");
    printf ("\t 9 - Adicionar uma viagem (auto)\n");
    printf ("\t 0 - Sair do programa\n");

    printf ("\tOpcao : ");
    *opcao = getchar ();
    fflush (stdin); // __fpurge (stdin);
}

int main (){
    char *opcao = (char*) malloc (sizeof (char));
    char save;
    int *nr_navios = (int*) calloc (1, sizeof (int));   // inicializacao com calloc para meter *nr_navios = 0
    int *nr_portos = (int*) calloc (1, sizeof (int));   // inicializacao com calloc para meter *nr_portos = 0
    int *nr_viagens = (int*) calloc (1, sizeof (int));  // inicializacao com calloc para meter *nr_viagens = 0

    NAVIO navio = (NAVIO) malloc (sizeof (struct _navio));
    PORTO porto = (PORTO) malloc (sizeof (struct _porto));
    VIAGEM viagem = (VIAGEM) malloc (sizeof (struct _viagem));
    VIAGEM ultima_viagem = (VIAGEM) malloc (sizeof (struct _viagem));
    VIAGEM ultima_viagem_file = (VIAGEM) malloc (sizeof (struct _viagem));

    if ((Ler_Navios (&navio, nr_navios) == FALSE) || (Ler_Portos (&porto, nr_portos) == FALSE) || (Ler_Viagens (navio, porto, &viagem, &ultima_viagem, &ultima_viagem_file, nr_viagens) == FALSE))
        return 0;
    printf ("Existem %d navios, %d portos e %d viagens\n", *nr_navios, *nr_portos, *nr_viagens);

    do{
        menu (opcao);

        switch (*opcao){
            case '1' : Listar_Navios (navio);break;
            case '2' : Listar_Portos (porto);break;
            case '3' : Listar_Viagens (viagem);break;
            case '4' : Listar_Carga_Tempo (navio, porto, viagem);break;
            case '5' : Guardar_Viagens (&ultima_viagem_file);break;
            case '6' : Listar_Carga_Porto (navio, porto, viagem);break;
            case '7' : Analisar_Rendimento (viagem);break;
            case '8' : Adicionar_Viagem (navio, porto, viagem, &ultima_viagem, nr_viagens);break;
            case '9' : Adicionar_Viagem_Auto (navio, porto, viagem, &ultima_viagem, nr_viagens);break;
            case '0' : break;
            default : printf (COLOR_RED "Opcao invalida!\n" COLOR_RESET);
        }
    } while (*opcao != '0');

    if (ultima_viagem != ultima_viagem_file){
        do{
            printf ("Deseja guardar as alteracoes (s/n)? ");
            save = getchar ();
            if (save == 's')
                Guardar_Viagens (&ultima_viagem_file);
            fflush (stdin); // __fpurge (stdin);
            if ((save != 's') && (save != 'n'))
                printf (COLOR_RED "Erro na introducao de carateres!\n" COLOR_RESET);
        } while ((save != 's') && (save != 'n'));
    }

    free (opcao);
    free (nr_navios);
    free (nr_portos);
    free (nr_viagens);

    Destroi_Navios (navio);
    Destroi_Portos (porto);
    Destroi_Viagens (viagem);

    printf ("Adeus!\n");
}