#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main () {

  int scelta_menu=0;
  int menu_flag=0;
  int impostazioni=0;
  int modifica_impostazioni=0;

  printf ( "\t\t\t\t\t████████╗██████╗░░█████╗░███╗░░██╗███╗░░██╗░█████╗░██╗\n" );
  printf ( "\t\t\t\t\t╚══██╔══╝██╔══██╗██╔══██╗████╗░██║████╗░██║██╔══██╗██║\n" );
  printf ( "\t\t\t\t\t░░░██║░░░██████╔╝███████║██╔██╗██║██╔██╗██║██║░░██║██║\n" );
  printf ( "\t\t\t\t\t░░░██║░░░██╔══██╗██╔══██║██║╚████║██║╚████║██║░░██║██║\n" );
  printf ( "\t\t\t\t\t░░░██║░░░██║░░██║██║░░██║██║░╚███║██║░╚███║╚█████╔╝██║\n" );
  printf ( "\t\t\t\t\t░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚══╝░╚════╝░╚═╝\n" );

  do {
    printf ( "\n\n\n\n\n\t█▀▄▀█ █▀▀ █▄░█ █░█\n" );
    printf ( "\t█░▀░█ ██▄ █░▀█ █▄█\n" );

    printf ( "\nDigita 1 per le impostazioni di gioco.\n" );
    printf ( "Digita 2 per avviare la partita.\n" );
    printf ( "Digita 3 per chiudere il gioco.\n" );
    printf ( "\n--> " );
    scanf ( "%d",&scelta_menu );
    if ( scelta_menu<1 || scelta_menu>3 ) {
      printf ( "\nsᴄᴇʟᴛᴀ ɴᴏɴ ᴠᴀʟɪᴅᴀ\n\n" );

      menu_flag=1;
    }

    switch ( scelta_menu ) {

      case 1: {
        if ( impostazioni==0 ) {
          imposta_gioco ();
          impostazioni=1;
          menu_flag=1;
        } else {
          printf ( "\nSei sicuro di voler modificare le impostazioni?\n" );
          do {
            printf ( "\nDigita 1 per modificare.\n" );
            printf ( "Digita 2 per annulare.\n" );
            printf ( "\n--> " );
            scanf ( "%d",&modifica_impostazioni );
            if ( modifica_impostazioni<1 || modifica_impostazioni>2 ) {
              printf ( "\nsᴄᴇʟᴛᴀ ɴᴏɴ ᴠᴀʟɪᴅᴀ\n\n" );
            } else {
              switch ( modifica_impostazioni ) {

                case 1: {
                  imposta_gioco ();
                  menu_flag=1;
                  break;
                }
                case 2: {
                  printf ( "\nRitorno al menù.\n" );
                  menu_flag=1;
                  break;
                }
              }
            }
          }while ( modifica_impostazioni<1 || modifica_impostazioni>2 );
        }
        break;
      }

      case 2: {
        if ( impostazioni==1 ) {
          gioca ();
          menu_flag=1;
        } else {
          printf ( "\nDevi prima impostare la partita.\n" );

          menu_flag=1;
        }
        break;
      }
      case 3: {
        termina_gioco ();
        menu_flag=0;
        break;
      }
    }
  }while ( menu_flag==1 );
  return 0;
}

