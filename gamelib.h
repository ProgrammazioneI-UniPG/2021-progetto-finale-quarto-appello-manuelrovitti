void imposta_gioco();
void gioca();
void termina_gioco();


typedef enum {Giallo,Rosso,Blu,Verde,Nero,Bianco,Viola,Marrone,Arancione,Ciano}Nome_giocatore;

typedef enum {Astronauta,Impostore,Assasinato,Defenestrato}Stato_giocatore;

typedef enum{vuota, quest_semplice, quest_complicata, botola}Tipo_stanza;

typedef enum{false,true}boolean;


struct Giocatore{
  struct Stanza *posizione;
  Stato_giocatore Stato_giocatore;
  Nome_giocatore Nome_giocatore;
};

typedef struct Giocatore Giocatore;



struct Stanza{
  struct Stanza *avanti;
  struct Stanza *destra;
  struct Stanza *sinistra;
  struct Stanza *stanza_precedente;
  Tipo_stanza Tipo_stanza;
  boolean emergenza_chiamata;
  struct Stanza *s_successiva;
};

typedef struct Stanza Stanza;

