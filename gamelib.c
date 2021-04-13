#include "gamelib.h" //libreria creata da me
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// variabili globali

// array di giocatori creati (4-10)
struct Giocatore *giocatori;
// quest da completare per vincere
unsigned short quest_da_finire = 0;
// stanza di partenza
static Stanza *stanza_inizio;
// lista stanze
static Stanza *lista_stanze;

/*                        IMPOSTA GIOCO
  Qui viene scelto il numero di giocatori, impostori, il colore,
  il numero di quest, generazione stanza iniziale.*/


// numero di giocatori
static int numero_giocatori = 0;
// generazione di un nome random per ogni giocatore
static Nome_giocatore *nome_random = NULL;
// numero di impostori
static int numero_impostori = 0;


/*                          GIOCO
Qui inizia il gioco vero e proprio, l'ordine del turno è randomico, ogni
giocatore può fare una sola mossa a turno, si creano le nuove stanze e
si cambiano i puntatori delle stanze precedenti.*/


// numero di astronati vivi
static int numero_giocatori_vivi = 0;
// numero di quest finite
static int quest_finite = 0;
// creazione nuova stanza
static Stanza *nuova_stanza;
// numero di stanze create
static int numero_stanze = 1;




/*                        TERMINA GIOCO */

// Funzioni

// random numero==colore giocatore
static void num_random_giocatore();
// ordine del turno
static void random_turni();
// stampa elenco dei giocatori e il loro ruolo
static void stampa_lista_giocatori();
// permette di stampare il nome/colore del giocatore
static char *stampa_nome();
// permette di stampare lo stato del giocatore
static char *stampa_stato();
// stampa il tipo di stanza in cui si trovano
static char *stampa_tipo_stanza();
// serve per sapere chi c'è nella stanza
static void giocatori_nella_stanza();
// serve per muoversi tra le stanza
static void avanza();
// creazione nuova stanza
static Stanza *creazione_stanza();
// inserimento in testa della lista
static void inserimentoTesta ();
// serve per eseguire la quest
static void esegui_quest();
//  serve agli impostori per eliminare la quest dalla stanza
static void sabotare();
// serve agli assaasini per uccidere
static void uccidi();
// serve per fare la chiamata d'emergenza
static void chiamata_emergenza();
//serve per calcolare la probabilità per ogni giocatore di essere espulso
static int probPerGiocatore();
// serve per far usare la botola agli impostori
static void usa_botola();
// serve per randomizzare la stanza con botola
static Stanza *stanzaRandomBotola();
// serve per randomizzare la stanza
static Stanza *stanzaRandom();
// controllo se esiste + di 1 una stanza botola
static int controlloBotola();

/*  Serve per decidere le impostazioni del gioco*/
void imposta_gioco()
{

  // impostore1
  int impostore1 = 100;
  // impostore2
  int impostore2 = 100;
  // impostore3
  int impostore3 = 100;
  // scelta per decidere cosa fare dopo aver impostato il gioco
  int scelta_fine_impostazioni = 0;

  printf("\n--> ɪᴍᴘᴏsᴛᴀᴢɪᴏɴɪ ᴅɪ ɢɪᴏᴄᴏ\n");

  printf("\nIl numero degli impostori può variare in base al numero di "
         "giocatori.\n");
  printf("\nSe i giocatori sono:\n");
  printf("4/5, ci sarà un solo impostore.\n");
  printf("6/7/8, ci saranno 1 o 2 impostori.\n");
  printf("9/10, ci saranno 1, 2 o 3 impostori.\n");

  do
  {

    printf("\nNumero di giocatori ? ( Min 4 - Max 10 )\n");
    printf("\n-->");
    scanf("%d", &numero_giocatori);
    numero_giocatori_vivi = numero_giocatori;
    if (numero_giocatori < 4 || numero_giocatori > 10)
    {
      printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
      printf("\nReinserisci il numero di giocatori.\n");
    }
    else
    {
      giocatori = (Giocatore *)calloc(numero_giocatori, sizeof(Giocatore));
      nome_random = (Nome_giocatore *)calloc(numero_giocatori, sizeof(int));
      num_random_giocatore();
    }
  } while (numero_giocatori < 4 || numero_giocatori > 10);

  numero_impostori =
      numero_giocatori < 6
          ? 1
          : ((numero_giocatori < 9 ? rand() % 2 : rand() % 3) + 1);

  if (numero_impostori == 1)
  {
    impostore1 = rand() % numero_giocatori;
    giocatori[impostore1].Stato_giocatore = 1;
  }
  else if (numero_impostori == 2)
  {
    do
    {
      impostore1 = rand() % numero_giocatori;
      giocatori[impostore1].Stato_giocatore = 1;
      impostore2 = rand() % numero_giocatori;
      giocatori[impostore2].Stato_giocatore = 1;
    } while (impostore1 == impostore2);
  }
  else if (numero_impostori == 3)
  {
    do
    {
      impostore1 = rand() % numero_giocatori;
      giocatori[impostore1].Stato_giocatore = 1;
      impostore2 = rand() % numero_giocatori;
      giocatori[impostore2].Stato_giocatore = 1;
      impostore3 = rand() % numero_giocatori;
      giocatori[impostore3].Stato_giocatore = 1;
    } while (impostore1 == impostore2 || impostore1 == impostore3 ||
             impostore2 == impostore3);
  }
  for (int i = 0; i < numero_giocatori; i++)
  {
    if (giocatori[i].Stato_giocatore != 1)
    {
      giocatori[i].Stato_giocatore = 0;
    }
  }

  printf("\n");
  do
  {
    sleep(1);
    system("clear");
    printf("\n\nTutti gli astronati dovranno completare un numero di quest predefinito "
           "per vincere la partita\n");
    printf("\nInserisci il numero di quest da completare. (Min %d - Max "
           "%d)\n\n-->",
           numero_giocatori, (numero_giocatori)*5);
    scanf("%hu", &quest_da_finire);
    if (quest_da_finire >= (numero_giocatori) && quest_da_finire <= (numero_giocatori)*5)
    {
      printf("\nIl numero di quest da completare è: %d\n", quest_da_finire);
    }
    else
    {
      printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
    }
  } while (quest_da_finire < (numero_giocatori) ||
           quest_da_finire > (numero_giocatori)*5);

  printf("\n");

  do
  {
    sleep(1);
    system("clear");
    printf("Ora puoi decidere:\nDigita 1 per vedere chi sono gli impostori.\n");
    printf("Digita 2 per tornare al menu e poter giocare\n\n-->");
    scanf("%d", &scelta_fine_impostazioni);
    if (scelta_fine_impostazioni < 1 || scelta_fine_impostazioni > 2)
    {
      printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
    }
    else
    {

      switch (scelta_fine_impostazioni)
      {

      case 1:
      {
        stampa_lista_giocatori();
        printf("\nPronto ad arrivare a destinazione o ad uccidere tutti?\n");
        break;
      }
      case 2:
      {
        printf("\nPronto ad arrivare a destinazione o ad uccidere tutti?\n");
        break;
      }
      }
    }
  } while (scelta_fine_impostazioni < 1 || scelta_fine_impostazioni > 2);

  stanza_inizio = (Stanza *)malloc(sizeof(Stanza));
  stanza_inizio->avanti = NULL;
  stanza_inizio->sinistra = NULL;
  stanza_inizio->destra = NULL;
  stanza_inizio->stanza_precedente = NULL;
  stanza_inizio->Tipo_stanza = 0;
  stanza_inizio->emergenza_chiamata = false;
  stanza_inizio->s_successiva = NULL;

  for (int i = 0; i < numero_giocatori; i++)
  {
    // dare come posizione del giocatore la stanza di inizio
    giocatori[i].posizione = stanza_inizio;
  }

  return;
}

/* Serve per dare al giocatore di poter giocare, prendere decisioni*/
void gioca()
{
  // tiene conto dei turni
  int turno = 1;
  // mi permette di ristampare le azioni eseguibili
  int menu_flag = 0;
  // scelta dell'azione del giocatore
  int scelta_azione = 0;
  
  int nem = 0;
  int ass = 0;
  int que = 0;
  int fine = 0;

  numero_giocatori_vivi -= numero_impostori;

  system("clear");

  printf("\nBenvenuto sulla nave.\n");
  printf("\nOgni giocatore può eseguire una singola mossa a turno.\n");
  printf("\nLa stanza iniziale è vuota\n");

  do
  {
    // randomizza il turno
    random_turni (giocatori,numero_giocatori);

    printf("\n\nTurno %d°.\n", turno++);

    for (int i = 0; i < numero_giocatori; i++)
    {

      nem = 0;
      ass = 0;
      que = 0;

      printf("\n\nCambio giocatore.\n");
      sleep(2);
      system("clear");
      printf("\n\nTocca a %s.\n", stampa_nome(giocatori[i].Nome_giocatore));

      do
      {
        if (giocatori[i].Stato_giocatore == 2 || giocatori[i].Stato_giocatore == 3)
        {
          printf("\n\nSei morto.\n\n");
          printf("X X\n---\n\n\n");
          menu_flag = 0;
        }
        else if (giocatori[i].Stato_giocatore == 0 || giocatori[i].Stato_giocatore == 1)
        {
          menu_flag = 0;
          printf("\nTi trovi nella stanza %p,", giocatori[i].posizione);
          printf(" %s.\n", stampa_tipo_stanza(giocatori[i].posizione->Tipo_stanza));
          giocatori_nella_stanza(giocatori[i]);

          printf("\n\n");

          printf("\nDigita 1 per andare avanti.\n");
          if (giocatori[i].posizione->Tipo_stanza == 1 || giocatori[i].posizione->Tipo_stanza == 2)
          {
            if (giocatori[i].Stato_giocatore == 0)
            {
              printf("Digita 2 per eseguire la quest.\n");
              que = 1;
            }
            else if (giocatori[i].Stato_giocatore == 1)
            {
              printf("Digita 2 per sabotare la quest.\n");
              que = 1;
            }
          }
          else if (giocatori[i].posizione->Tipo_stanza == 3 && giocatori[i].Stato_giocatore == 1)
          {
            printf("Digita 2 per usare la botola.\n");
            que = 1;
          }
          if (giocatori[i].Stato_giocatore == 1)
          {
            if (turno > 2)
            {
              for (int k = 0; k < numero_giocatori; k++)
              {
                if (giocatori[i].posizione == giocatori[k].posizione && giocatori[k].Stato_giocatore == 0)
                {
                  nem = 1;
                }
              }
              if (nem == 1)
              {
                printf("Digita 3 per uccidere.\n");
              }
            }
          }
          if (turno > 2)
          {
            for (int k = 0; k < numero_giocatori; k++)
            {
              if (giocatori[i].posizione == giocatori[k].posizione && giocatori[i].posizione->emergenza_chiamata == false)
              {
                if (giocatori[k].Stato_giocatore == 2)
                {
                  ass = 1;
                }
              }
            }
            if (ass == 1)
            {
              printf("Digita 4 per effettuare la chiamata d'emergenza.\n");
            }
          }
          printf("-->");
          scanf("%d", &scelta_azione);
          getchar();

          if (scelta_azione < 1 || scelta_azione > 4)
          {
            menu_flag = 1;
            printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
          }
          else if (scelta_azione == 4 && ass != 1)
          {
            printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
            menu_flag = 1;
          }
          else if (scelta_azione == 3 && nem != 1)
          {
            printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
            menu_flag = 1;
          }
          else if (scelta_azione == 2 && que != 1)
          {
            printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
            menu_flag = 1;
          }

          if (scelta_azione == 1)
          {
            avanza(giocatori[i]);
            giocatori[i].posizione = nuova_stanza;
          }
          else if (scelta_azione == 2 && que == 1)
          {
            if (giocatori[i].posizione->Tipo_stanza == 1 || giocatori[i].posizione->Tipo_stanza == 2)
            {
              if (giocatori[i].Stato_giocatore == 0)
              {
                esegui_quest(giocatori[i]);
              }
              else if (giocatori[i].Stato_giocatore == 1)
              {
                sabotare(giocatori[i]);
              }
            }
            else if (giocatori[i].posizione->Tipo_stanza == 3 && giocatori[i].Stato_giocatore == 1)
            {
              usa_botola(&giocatori[i]);
            }
          }
          else if (scelta_azione == 4 && ass == 1)
          {
            chiamata_emergenza(giocatori[i]);
          }
          else if (giocatori[i].Stato_giocatore == 1 && scelta_azione == 3 &&
                   nem == 1)
          {
            uccidi(giocatori[i]);
          }
        }
      } while (menu_flag == 1);

      if (quest_finite >= quest_da_finire)
      {
        fine = 1;
        break;
      }
      else
      {
        fine = 0;
      }
      if (numero_giocatori_vivi <= numero_impostori)
      {
        fine = 1;
        break;
      }
      else
      {
        fine = 0;
      }
      if (numero_impostori == 0)
      {
        fine = 1;
        break;
      }
      else
      {
        fine = 0;
      }
    }

  } while (fine == 0);

  if (quest_finite >= quest_da_finire || numero_impostori == 0)
  {
    printf("\n\n\n\n\t\t\t░█████╗░░██████╗████████╗██████╗░░█████╗░███╗░░██╗░█████╗░██╗░░░██╗████████╗██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n");
    printf("\t\t\t██╔══██╗██╔════╝╚══██╔══╝██╔══██╗██╔══██╗████╗░██║██╔══██╗██║░░░██║╚══██╔══╝██║  ░██║░░██╗░░██║██║████╗░██║\n");
    printf("\t\t\t███████║╚█████╗░░░░██║░░░██████╔╝██║░░██║██╔██╗██║███████║██║░░░██║░░░██║░░░██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n");
    printf("\t\t\t██╔══██║░╚═══██╗░░░██║░░░██╔══██╗██║░░██║██║╚████║██╔══██║██║░░░██║░░░██║░░░██║  ░░████╔═████║░██║██║╚████║\n");
    printf("\t\t\t██║░░██║██████╔╝░░░██║░░░██║░░██║╚█████╔╝██║░╚███║██║░░██║╚██████╔╝░░░██║░░░██║  ░░╚██╔╝░╚██╔╝░██║██║░╚███║\n");
    printf("\t\t\t╚═╝░░╚═╝╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝░╚════╝░╚═╝░░╚══╝╚═╝░░╚═╝░╚═════╝░░░░╚═╝░░░╚═╝  ░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝\n");
  }
  else if (numero_giocatori_vivi <= numero_impostori)
  {
    printf("\n\n\n\n\t\t\t██╗███╗░░░███╗██████╗░░█████╗░░██████╗████████╗░█████╗░██████╗░██╗  ░██╗░░░░░░░██╗██╗███╗░░██╗\n");
    printf("\t\t\t██║████╗░████║██╔══██╗██╔══██╗██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██║  ░██║░░██╗░░██║██║████╗░██║\n");
    printf("\t\t\t██║██╔████╔██║██████╔╝██║░░██║╚█████╗░░░░██║░░░██║░░██║██████╔╝██║  ░╚██╗████╗██╔╝██║██╔██╗██║\n");
    printf("\t\t\t██║██║╚██╔╝██║██╔═══╝░██║░░██║░╚═══██╗░░░██║░░░██║░░██║██╔══██╗██║  ░░████╔═████║░██║██║╚████║\n");
    printf("\t\t\t██║██║░╚═╝░██║██║░░░░░╚█████╔╝██████╔╝░░░██║░░░╚█████╔╝██║░░██║██║  ░░╚██╔╝░╚██╔╝░██║██║░╚███║\n");
    printf("\t\t\t╚═╝╚═╝░░░░░╚═╝╚═╝░░░░░░╚════╝░╚═════╝░░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝╚═╝  ░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚══╝\n");
  }

  return;
}

/* Libera lo spazio in memoria dinamica*/
void termina_gioco()
{

  free(stanza_inizio);
  free(giocatori);
  free(nome_random);

  printf("\n\n\n\n\t\t\t░█████╗░██████╗░██████╗░██╗██╗░░░██╗███████╗██████╗░███████╗██████╗░░█████╗░██╗  ██╗██╗\n");
  printf("\t\t\t██╔══██╗██╔══██╗██╔══██╗██║██║░░░██║██╔════╝██╔══██╗██╔════╝██╔══██╗██╔══██╗██║  ██║██║\n");
  printf("\t\t\t███████║██████╔╝██████╔╝██║╚██╗░██╔╝█████╗░░██║░░██║█████╗░░██████╔╝██║░░╚═╝██║  ██║██║\n");
  printf("\t\t\t██╔══██║██╔══██╗██╔══██╗██║░╚████╔╝░██╔══╝░░██║░░██║██╔══╝░░██╔══██╗██║░░██╗██║  ╚═╝╚═╝\n");
  printf("\t\t\t██║░░██║██║░░██║██║░░██║██║░░╚██╔╝░░███████╗██████╔╝███████╗██║░░██║╚█████╔╝██║  ██╗██╗\n");
  printf("\t\t\t╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░░╚═╝░░░╚══════╝╚═════╝░╚══════╝╚═╝░░╚═╝░╚════╝░╚═╝  ╚═╝╚═╝\n");

  return;
}

/* Randomizza il nome*/
static void num_random_giocatore()
{

  time_t t;
  srand((unsigned)time(&t));
  // array che serve per memorizzare qualcosa a breve termine
  static int array[10];

  for (int i = 0; i < 10; i++)
  {
    array[i] = i;
  }
  for (int i = 0; i < 10; i++)
  {
    int temp = array[i];
    int random = rand() % 10;

    array[i] = array[random];
    array[random] = temp;
  }
  for (int i = 0; i < numero_giocatori; i++)
  {
    nome_random[i] = array[i];
    for (int k = 0; k < numero_giocatori; k++)
    {
      // associare il colore al nome del giocatore
      giocatori[i].Nome_giocatore = nome_random[i];
    }
  }
}

/* Randomizza il turno -------------------->risolvere stampa nome */
static void random_turni(Giocatore* giocatori,size_t n)
{
  time_t t;
  srand((unsigned)time(&t));

  // array che serve per memorizzare qualcosa a breve termine
  static int array[10];
  static Giocatore fake [10];

  for (int i = 0; i < numero_giocatori; i++)
  {
    array[i] = rand() % numero_giocatori;
    for (int k = 0; k < i; k++)
    {
      if (array[i] == array[k])
      {
        i--;
        break;
      }
    }
  }


  for (int j = 0; j < numero_giocatori; j++)
  {
    fake[j] = giocatori[array[j]];
  }
  for (int k = 0; k < numero_giocatori; k++)
  {
    giocatori[k] = fake[k];
  }
}

/* Serve per stampare la lista dei giocatori*/
static void stampa_lista_giocatori()
{
  printf("\nEcco qua la lista dei giocatori:\n\n");
  for (int i = 0; i < numero_giocatori; i++)
  {
    printf("%d)Il %s è un %s.\n", i + 1,
           stampa_nome(giocatori[i].Nome_giocatore),
           stampa_stato(giocatori[i].Stato_giocatore));
  }
  return;
}

/* Serve per stampare in char in colore*/
static char *stampa_nome(Nome_giocatore nome_giocatore)
{

  switch (nome_giocatore)
  {

  case 0:
    return "Giallo";
    break;
  case 1:
    return "Rosso";
    break;
  case 2:
    return "Blu";
    break;
  case 3:
    return "Verde";
    break;
  case 4:
    return "Nero";
    break;
  case 5:
    return "Bianco";
    break;
  case 6:
    return "Viola";
    break;
  case 7:
    return "Marrone";
    break;
  case 8:
    return "Arancione";
    break;
  case 9:
    return "Ciano";
    break;
  default:
    break;
  }
}

/*  Serve per stampare lo stato del giocatore*/
static char *stampa_stato(Stato_giocatore stato_giocatore)
{

  switch (stato_giocatore)
  {

  case Astronauta:
    return "Astronauta";
    break;
  case Impostore:
    return "Impostore";
    break;
  case Assasinato:
    return "Assasinato";
    break;
  case Defenestrato:
    return "Defenestrato";
    break;
  default:
    break;
  }
}

/* Serve per stampare il tipo della stanza*/
static char *stampa_tipo_stanza(Tipo_stanza tipo_stanza)
{

  switch (tipo_stanza)
  {

  case vuota:
    return "vuota";
    break;
  case quest_semplice:
    return "quest semplice";
    break;
  case quest_complicata:
    return "quest complicata";
    break;
  case botola:
    return "botola";
    break;
  }
}

/*Serve per stampare i giocatori presenti nella stanza */
static void giocatori_nella_stanza(Giocatore giocatori1)
{
  int giocatoriStanza = 0;
  printf("\n");

  for (int r = 0; r < numero_giocatori; r++)
  {

    if (giocatori1.posizione == giocatori[r].posizione)
    {

      if (giocatori[r].Stato_giocatore == 0 ||
          giocatori[r].Stato_giocatore == 1)
      {
        if (giocatori1.Nome_giocatore != giocatori[r].Nome_giocatore)
        {
          printf(" ° Il giocatore %s è nella stanza.\n",
                 stampa_nome(giocatori[r].Nome_giocatore));
          giocatoriStanza = giocatoriStanza + 1;
        }
      }
      else if (giocatori[r].Stato_giocatore == 2 ||
               giocatori[r].Stato_giocatore == 3)
      {
        printf("\n°Il giocatore %s è stato %s.\n",
               stampa_nome(giocatori[r].Nome_giocatore), stampa_stato(giocatori[r].Stato_giocatore));
      }
    }
  }
}

/* Il giocatore sceglie in quale direzione andare */
static void avanza(Giocatore giocatori)
{

  int direzione = 0;

  do
  {
    printf("\nIn quale direzioni desideri andare?\n");
    printf("\nDigita 1 per andare avanti.\nDigita 2 per andare a destra.\n");
    printf(
        "Digita 3 per andare a sinistra.\nDigita 4 per rimanere fermo.\n\n-->");
    scanf("%d", &direzione);

    switch (direzione)
    {

    case 1:
    {
      printf("\n\nHai deciso di andare avanti.\n");
      if (giocatori.posizione->avanti == NULL)
      {
        creazione_stanza(direzione, giocatori);
        giocatori.posizione = nuova_stanza;
        printf("\nLa stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      else
      {
        giocatori.posizione = giocatori.posizione->avanti;
        printf("\nLa stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      break;
    }
    case 2:
    {
      printf("\n\nHai deciso di andare a destra.\n");
      if (giocatori.posizione->destra == NULL)
      {
        creazione_stanza(direzione, giocatori);
        giocatori.posizione = nuova_stanza;
        printf("\nLa stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      else
      {
        giocatori.posizione = giocatori.posizione->destra;
        printf("La stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      break;
    }
    case 3:
    {
      printf("\n\nHai deciso di andare a sinistra.\n");
      if (giocatori.posizione->sinistra == NULL)
      {
        creazione_stanza(direzione, giocatori);
        giocatori.posizione = nuova_stanza;
        printf("\nLa stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      else
      {
        giocatori.posizione = giocatori.posizione->sinistra;
        printf("\nLa stanza: %p", giocatori.posizione);
        printf(" ha %s.\n",
               stampa_tipo_stanza(giocatori.posizione->Tipo_stanza));
      }
      break;
    }
    case 4:
    {
      nuova_stanza = giocatori.posizione;
      printf("Sei rimasto fermo nella stanza.\n");
      break;
    }
    }
  } while (direzione > 4 || direzione < 1);

  return;
}

/* Crea una nuova stanza, randomizza il tipo */
static struct Stanza *creazione_stanza(int direzione, Giocatore giocatore)
{
  int tipo_stanza = rand() % 101;

  if (tipo_stanza <= 30)
  {
    tipo_stanza = 0;
  }
  else if (tipo_stanza > 30 && tipo_stanza <= 60)
  {
    tipo_stanza = 1;
  }
  else if (tipo_stanza > 60 && tipo_stanza <= 75)
  {
    tipo_stanza = 2;
  }
  else if (tipo_stanza > 75)
  {
    tipo_stanza = 3;
  }

  nuova_stanza = (Stanza *)malloc(sizeof(Stanza));
  nuova_stanza->avanti = NULL;
  nuova_stanza->destra = NULL;
  nuova_stanza->sinistra = NULL;
  nuova_stanza->stanza_precedente = giocatore.posizione;
  nuova_stanza->Tipo_stanza = tipo_stanza;
  nuova_stanza->emergenza_chiamata = false;
  nuova_stanza->s_successiva = NULL;

  if (direzione == 1)
  {
    giocatore.posizione->avanti = nuova_stanza;
  }
  else if (direzione == 2)
  {
    giocatore.posizione->destra = nuova_stanza;
  }
  else if (direzione == 3)
  {
    giocatore.posizione->sinistra = nuova_stanza;
  }

  inserimentoTesta(nuova_stanza);

  if (direzione == 1)
  {
    giocatore.posizione = giocatore.posizione->avanti;
  }
  else if (direzione == 2)
  {
    giocatore.posizione = giocatore.posizione->destra;
  }
  else if (direzione == 3)
  {
    giocatore.posizione = giocatore.posizione->sinistra;
  }

  numero_stanze += 1;
  return nuova_stanza;
}

/* serve per l'inserimento in testa*/
static void inserimentoTesta (struct Stanza* nuovaStanza)
{
  if (stanza_inizio == NULL) {
    stanza_inizio = nuovaStanza;
  } else {
    nuovaStanza->s_successiva = stanza_inizio;
    stanza_inizio = nuovaStanza;
  }
}

/* Esegue la quest, aggiorna il numero di quest finite e modifica la stanza*/
static void esegui_quest(Giocatore giocatori)
{

  if (giocatori.posizione->Tipo_stanza == 1)
  {
    quest_finite = quest_finite + 1;
    printf("\n\nHai eseguito una quest semplice.\n");
    printf("%d quest fatte su %d.\t", quest_finite, quest_da_finire);
    printf("[");
    for (int i = 0; i < quest_finite; i++)
    {
      printf("/");
    }
    printf("]");
  }
  else if (giocatori.posizione->Tipo_stanza == 2)
  {
    quest_finite = quest_finite + 2;
    printf("\nHai eseguito una quest complicata.\n");
    printf("%d quest fatte su %d.\t", quest_finite, quest_da_finire);
    printf("[");
    for (int i = 0; i < quest_finite; i++)
    {
      printf("/");
    }
    printf("]");
  }
  giocatori.posizione->Tipo_stanza = 0;
}

/* permette agli impostore di rallentare gli astronauti*/
static void sabotare(Giocatore giocatori)
{

  printf("Hai sabotato la quest.\n");
  giocatori.posizione->Tipo_stanza = 0;
}

/* Permette agli impostori di uccidere un astronauta che si trova nella stanza*/
static void uccidi(Giocatore giocatori1)
{

  int risposta = 0;
  int loop = 0;
  int s = 0;
  int p = 0;
  int def = 0;
  int pos = 0;
  int r = 0;

  do
  {
    loop = 0;
    printf("\n\nChi desideri uccidere?\n");
    for (r = 0; r < numero_giocatori; r++)
    {

      if (giocatori1.posizione == giocatori[r].posizione && giocatori1.Nome_giocatore != giocatori[r].Nome_giocatore)
      {
        if (giocatori[r].Stato_giocatore == 0 || giocatori[r].Stato_giocatore == 1)
        {
          printf("%d) Il giocatore %s è nella stanza.\n", r, stampa_nome(giocatori[r].Nome_giocatore));
        }
      }
    }
    printf("\n-->");
    scanf("%d", &risposta);
    s = risposta;

    if (s < 0 || s > r)
    {
      printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
      loop = 1;
    }
    else if (giocatori1.Stato_giocatore == giocatori[s].Stato_giocatore)
    {
      printf("\nNon puoi ucccidere un alleato\n");
      loop = 1;
    }
    else if (giocatori[s].Stato_giocatore == 0)
    {
      printf("\n%s è stato ucciso\n", stampa_nome(giocatori[s].Nome_giocatore));
      giocatori[s].Stato_giocatore = 2;
      loop = 0;
      numero_giocatori_vivi = numero_giocatori_vivi - 1;
    }
    else if (giocatori[s].posizione != giocatori1.posizione)
    {
      printf("\nSᴘɪᴀᴄᴇɴᴛᴇ, ɴᴜᴍᴇʀᴏ ɴᴏɴ ᴠᴀʟɪᴅᴏ\n");
      loop = 1;
    }
  } while (loop == 1);

  if (numero_impostori >= numero_giocatori_vivi)
  {
    return;
  }

  for (s = 0; s < numero_giocatori; s++)
  {
    if (giocatori[s].Stato_giocatore == 0)
    {
      if (giocatori[s].posizione == giocatori1.posizione->stanza_precedente)
      {
        p = p + 20;
        def = 1;
      }
      else if (giocatori1.posizione == giocatori[s].posizione &&
               giocatori[s].Stato_giocatore == 0)
      {
        p = p + 50;
        def = 1;
      }
    }
  }
  if (def == 1)
  {
    if (p >= 100)
    {
      printf("\nSei stato scoperto.\nSei stato defenestrato");
      giocatori1.Stato_giocatore = 3;
      numero_impostori = numero_impostori - 1;
    }
    else if (p < 100)
    {
      pos = rand() % 100 + 1;
      if (pos > p)
      {
        printf("\nNessuno ti ha visto.\n");
      }
      else if (p >= pos)
      {
        printf("\nSei stato scoperto.\nSei stato defenestrato");
        giocatori1.Stato_giocatore = 3;
        numero_impostori = numero_impostori - 1;
      }
    }
  }
  else if (def == 0)
  {
    printf("\nNessuno ti ha visto.\n");
  }
}

/* Serve per effettuare la chiamata d'emergenza, in caso trovassero un corpo
 assasinato*/
static void chiamata_emergenza(Giocatore giocatori1)
{
  time_t t;
  srand((unsigned)time(&t));

  int totale = 0;
  int inizioRango;
  int fineRango;
  int risultato;

  for (int i = 0; i < numero_giocatori; i++)
  {
    if (giocatori1.posizione == giocatori[i].posizione)
    {
      totale += probPerGiocatore(giocatori[i]);
    }
  }
  if (totale <= 0)
  {
    printf("Nessuno verrà defenestrato.");
    return;
  }
  risultato = rand() % totale + 1;

  inizioRango = 0;
  fineRango = 0;

  for (int i = 0; i < numero_giocatori; i++)
  {
    if (giocatori1.posizione == giocatori[i].posizione && giocatori[i].Stato_giocatore != 2 && giocatori[i].Stato_giocatore != 3)
    {
      inizioRango = fineRango;
      fineRango += probPerGiocatore(giocatori1);
      if (inizioRango < risultato || risultato < fineRango)
      {
        printf("Verra defenestrato il giocatore %s.", stampa_nome(giocatori[i].Nome_giocatore));

        if (giocatori[i].Stato_giocatore == 0)
        {
          numero_giocatori_vivi -= 1;
        }
        else if (giocatori[i].Stato_giocatore == 1)
        {
          numero_impostori -= 1;
        }



        giocatori[i].Stato_giocatore = 3;
        giocatori1.posizione->emergenza_chiamata = true;

        break;
      }
    }
  }
}

/*Serve per calcolare la probalità di ogni giocatore di uscire*/
static int probPerGiocatore(Giocatore giocatori1)
{

  int a = 0;

  for (int i = 0; i < numero_giocatori; i++)
  {
    if (giocatori1.posizione == giocatori[i].posizione)
    {
      if (giocatori1.Stato_giocatore == 0)
      {
        a = 30;
        if (giocatori[i].Stato_giocatore == 0 && giocatori[i].Nome_giocatore != giocatori1.Nome_giocatore)
        {
          a = a - 30;
        }
        else if (giocatori[i].Stato_giocatore == 1)
        {
          a = a + 20;
        }
      }
      else if (giocatori1.Stato_giocatore == 1)
      {
        a = 30;
        if (giocatori[i].Stato_giocatore == 0)
        {
          a = a + 20;
        }
        else if (giocatori[i].Stato_giocatore == 1 && giocatori[i].Nome_giocatore != giocatori1.Nome_giocatore)
        {
          a = a - 30;
        }
      }
    }
  }
  return a;
}

/*Serve agli impstori per spostarsi piu velocemente in maniera randomica*/
static void usa_botola(Giocatore *giocatore)
{
  int ass = 0;
  ass = controlloBotola(ass);

  if (ass > 1)
  {
    printf("\nTi sposterai in una stanza con botola.\n");
    giocatore->posizione = stanzaRandomBotola();
    printf("\nTi sei spostato nella stanza: %p, %s.\n", giocatore->posizione, stampa_tipo_stanza(giocatore->posizione->Tipo_stanza));
    return;
  }
  else
  {
    printf("\nTi sposterai in una stanza random.\n");
    giocatore->posizione = stanzaRandom();
    printf("\nTi sei spostato nella stanza: %p, %s.\n", giocatore->posizione, stampa_tipo_stanza(giocatore->posizione->Tipo_stanza));
    return;
  }
}

/*Serve per randomizare in quale stanza con botola si deve spostare impostore*/
static Stanza *stanzaRandomBotola()
{
  time_t t;
  srand((unsigned)time(&t));

  int stanzaRandom;
  lista_stanze = stanza_inizio;

  while (lista_stanze->Tipo_stanza != 3 )
  {
    do
    {
      stanzaRandom = rand() % numero_stanze + 1;
    }while (stanzaRandom == numero_stanze);

    for (int i = 0; i < stanzaRandom; i++)
    {
      if (lista_stanze == NULL) 
      { 
        lista_stanze = stanza_inizio;
        break;
      }
      lista_stanze = lista_stanze->s_successiva;
    }
  }

  return lista_stanze;
}

/* Serve per randomizzare la stanza in cui si dovra spostare*/
static Stanza *stanzaRandom()
{
  time_t t;
  srand((unsigned)time(&t));

  int stanzaRandom;

  lista_stanze = stanza_inizio;
  do
  {
    stanzaRandom = (rand() % numero_stanze) + 1;
  }while (stanzaRandom == numero_stanze);

  for (int i = 0; i < stanzaRandom; i++)
  {
    if (lista_stanze == NULL) { break; }
    lista_stanze = lista_stanze->s_successiva;
  }
  return lista_stanze;
}

/* Serve per controllare se esiste un'altra stanza botola */
static int controlloBotola(int control)
{

  lista_stanze = stanza_inizio;

  while (lista_stanze != NULL)
  {
    if (lista_stanze->Tipo_stanza == 3)
    {
      control+=1;
    }
    lista_stanze = lista_stanze->s_successiva;
  }
  return control;
}
