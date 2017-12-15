/**********************************************/
/***    Programme général regroupant les    ***/
/***  différents cas possibles de diffusion ***/
/***             de la chaleur              ***/
/**********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


//code pour les couleurs
#define ERREUR "\x1b[01m\x1b[31m" //gras et rouge
#define SUCCES "\x1b[01m\x1b[32m" //gras et vert
#define QUIT "\x1b[01m" //gras
#define RESET "\x1b[0m" //remet la couleur par defaut de la console
#define L 200

int main() {
  system("clear"); //on nettoie la console
  //déclaration des variables
  double U[L], U2[L]; // U[L] chaleur, U2[L] chaleur intermédiaire pour la méthode des différences finies
  double dt; // pas de temps que l'on ajuste en fonction du choix
  double dx = 1.0;

  int i, n;
  int nfinal, modulo; //utile pour le nombre d'itération de la boucle principale et création des fichiers

  double dtsurdx2;
  char  fichier[80];
  int fileNb = 0;

  //températures des sources
  double T0 = 0.0;
  double T1 = 0.0;

  int c;
  int choix, mode, etat;
  double choixdt, choixT0, choixT1;

  //création des fichiers
  FILE *fic1,*fic2,*fic3; //pointeurs pour la création des fichiers

  //ouverture des fichiers initiaux et finaux
  fic1 = fopen("init.dat","w");
  fic2 = fopen("final.dat","w");

  do {
    //menu initial
    printf("SIMULATION NUMERIQUE DE DIFFUSION DE LA CHALEUR\n\n");
    printf("1) Diffusion gaussienne\n");
    printf("2) Diffusion dans un barreau\n");
    printf("3) Diffuison dans un barreau isolé\n");
    printf("4) Diffuison personnalisée\n");
    printf(QUIT "\n5) Quitter\n\n" RESET);
    printf("Sélectionner un choix : ");
    choix = scanf ("%d", &c);
    //on vide le buffer apres la saisie (sécurité)
    scanf ("%*[^\n]");
    getchar ();

      //vérification de la saisie
      if (choix != 1) { //si le choix n'est pas un chiffre
        system("clear");
      	printf (ERREUR "Erreur de saisie" RESET " : un nombre doit être entré.\n");
      	//exit (EXIT_FAILURE);
      } //si c'est bien un chiffre mais qu'il ne respecte pas l'encadrement
      else if(choix == 1 && (c<1 || c>5)) {
        system("clear");
        printf(ERREUR "Erreur de saisie" RESET " : un chiffre entre 1 et 5 doit être entré\n");
      }

  //conditions sur les choix de diffusion
  switch(c) {
  	case 1: //diffusion gaussienne
  	  printf("Diffusion gaussienne\n");
      dt = 0.015;
      //on définit le tableau
      for (i=0; i<L; i++) {
        U[i] = exp(-0.1*(i*dx-L/2.0)*(i*dx-L/2.0));
        //printf("%lf\n",U[i]);
        fprintf(fic1, "%lf\n", U[i]);
      }
  	break;
  	case 2: //diffusion barreau
  	  printf("Diffusion barreau\n");
      //on définit le tableau
      for(i=0; i<L; i++) {
        T0 = 0.0;
        T1 = 1.0;
        dt = 0.15;
        U[i] = (((T1-T0)/2)*tanh(i*dx-L/2)+((T0+T1)/2));
        //printf("%lf\n", U[i]);
        fprintf(fic1, "%lf\n", U[i]);
      }
  	break;
  	case 3: //diffusion barreau isolé
  	  printf("Diffusion barreau isolé\n");
      T0 = 0.0;
      T1 = 1.0;
      dt = 0.35;
      for(i=0; i<L; i++) {
        U[i] = (((T1-T0)/2)*tanh(i*dx-L/2)+((T0+T1)/2));
        //printf("%lf\n", U[i]);
        fprintf(fic1, "%lf\n", U[i]);
      }
  	break;
    case 4: //diffusion personnalisée
      do {
        printf("\nDiffusion personalisée\n");
        //possibilités : gauss (1), tanh (2), D, visqueux, isolé, dt...
        printf("1) Diffusion gaussienne\n");
        printf("2) Diffusion de deux sources\n");
        printf(QUIT "\n3) Quitter\n" RESET);
        printf("\n\tSource de chaleur : ");
        choix = scanf("%d", &mode);
        scanf("%*[^\n]");
        getchar();
        //on vérifie la saisie
        if (choix != 1) {
        	printf (ERREUR "\nErreur de saisie" RESET " : un nombre doit être entré.\n");
        	//exit (EXIT_FAILURE);
        }
        else if(choix == 1 && (mode>3 || mode<1)) {
          printf(ERREUR "\nErreur de saisie" RESET " : choisir 1 ou 2.\n");
        }
        //la saisie est passée
        switch (mode) {
          case 1: //gaussienne
            printf(SUCCES "\t\tDiffusion gaussienne.\n" RESET);
            do {
              printf("\tIntervalle de temps (entre 0.0 et 0.5) : ");
              choixdt = scanf("%lf", &dt);
              scanf("%*[^\n]");
              getchar();
                if(choix != 1) {
                  printf(ERREUR "Erreur de saisie" RESET " : un nombre doit être entré.\n");
                }
                else if(choix == 1 && (dt>0.5 || dt<=0.0)) {
                  printf(ERREUR "Erreur de saisie" RESET " : dt doit être compris entre 0.0 et 0.5\n" RESET);
                }
            } while(dt>0.5 || dt<=0.0);
            printf(SUCCES "\t\tdt = %lf\n" RESET, dt);
            //printf("nfinal = %d et modulo = %d\n", nfinal, modulo);
            for (i=0; i<L; i++) {
              U[i] = exp(-0.1*(i*dx-L/2.0)*(i*dx-L/2.0));
              //printf("%lf\n",U[i]);
              fprintf(fic1, "%lf\n", U[i]);
            }
          break;
          case 2: //tanh
            printf(SUCCES "\t\tDiffusion de deux sources.\n" RESET);
            do {
              printf("\n\tEtat du barreau :\n");
              printf("\t\t1) Barreau non isolé.\n");
              printf("\t\t2) Barreau isolé\n");
              printf("\n\t\tEtat du barreau : ");
              choix = scanf("%d", &etat);
              scanf("%*[^\n]");
              getchar();
              //on vérifie la saisie
              if (choix != 1) {
              	printf (ERREUR "\nErreur de saisie" RESET " : un nombre doit être entré.\n");
              	//exit (EXIT_FAILURE);
              }
              else if(choix == 1 && (etat>3 || etat<1)) {
                printf(ERREUR "\nErreur de saisie" RESET " : choisir 1 ou 2.\n");
              }
              //la saisie est passée
            } while(etat>3 || etat<1);
            if(etat == 1) { //non isolé
              printf(SUCCES "\t\tBarreau non isolé.\n" RESET);
            }
            else {
              printf(SUCCES "\t\tBarreau isolé.\n" RESET);
            }
            do { //on demande l'intervalle de temps
              printf("\tIntervalle de temps : ");
              choixdt = scanf("%lf", &dt);
              scanf("%*[^\n]");
              getchar();
                if(choixdt != 1) {
                  printf(ERREUR "Erreur de saisie" RESET " : un nombre doit être entré.\n");
                }
                else if(choixdt == 1 && (dt>0.5 || dt<=0.0)) {
                  printf(ERREUR "Erreur de saisie" RESET " : dt doit être compris entre 0.0 et 0..5\n" RESET);
                }
            } while(dt>0.5 || dt<=0.0);
            printf(SUCCES "\t\tdt = %lf\n" RESET, dt);
            do { //on demande T0 et puis T1
              printf("\tTempérature T0 : ");
              choixT0 = scanf("%lf", &T0);
              scanf("%*[^\n]");
              getchar();
                if(choixT0 != 1) {
                  printf(ERREUR "Erreur de saisie" RESET " : un nombre doit être entré.\n");
                }
            } while(choixT0 != 1);
            printf(SUCCES "\t\tT0 = %lf\n" RESET, T0);
            do {
              printf("\tTempérature T1 : ");
              choixT1 = scanf("%lf", &T1);
              scanf("%*[^\n]");
              getchar();
                if(choixT1 != 1) {
                  printf(ERREUR "Erreur de saisie" RESET " : un nombre doit être entré.\n");
                }
            } while(choixT1 != 1);
            printf(SUCCES "\t\tT1 = %lf\n" RESET, T1);
            for(i=0; i<L; i++) {
              U[i] = (((T1-T0)/2)*tanh(i*dx-L/2)+((T0+T1)/2));
              //printf("%lf\n", U[i]);
              fprintf(fic1, "%lf\n", U[i]);
            }
          break;
          case 3: //on quitte
            exit(EXIT_FAILURE); //commande pour sortir du programme
          break;
        }
      } while(mode>3 || mode<1);
    break;
    case 5: //on quitte le Programme
      system("clear");
      exit(EXIT_FAILURE);
    break;
    }
    printf("\n");
  } while(c>5 || c<1);

  //en fonction de dt, on impose nfinal
  nfinal = dt*100000;
  modulo = nfinal/5;

  dtsurdx2 = dt/(dx*dx);

  for (n=1; n<=nfinal; n++) {
    //méthode des différences finies
      	for(i=1; i<L-1; i++) {
      	  U2[i] = U[i] + dtsurdx2*(U[i+1] + U[i-1] - 2.0*U[i]);
      	  //printf("%lf\n", U2[i]);
      	}
        for (i=1; i<L-1; i++) {
  	      U[i] = U2[i];
        }

    //on déffinit les conditions aux bords en rapport avec le choix de diffusion
    switch (c) {
      case 1: //diffusion gaussienne
        //printf("%d", c);
        U[0] = 0.0;
        U[L-1] = 0.0;
      break;
      case 2: //diffusion barreau
        //printf("%d", c);
        U[0] = T0;
        U[L-1] = T1;
      break;
      case 3: //diffusion barreau isolé
        //printf("%d", c);
        U[0] = U[1];
        U[L-1] = U[L-2];
      case 4: //diffusion personnalisée
        switch (mode) {
          case 1: //gaussienne
            U[0] = 0.0;
            U[L-1] = 0.0;
          break;
          case 2: //tanh
            if(etat == 1) { //non isolé
              U[0] = T0;
              U[L-1] = T1;
            }
            else if(etat == 2) { //isolé
              U[0] = U[1];
              U[L-1] = U[L-2];
            }
          break;
        }
      break;
    }

    //on créer les fichiers intermédiaires
    if(n%modulo == 0) {
      fileNb = fileNb + 1;
      printf("Numero du fichier  %d\n",fileNb);
      sprintf(fichier, "inter%d.dat",fileNb);
      printf("\tNom du fichier " SUCCES "%s\n" RESET,fichier);

      fic3 = fopen(fichier, "w");

      for(i=0; i<L; i++) {
        fprintf(fic3, "%lf\n", U[i]);
      }
      fclose(fic3);
    }
  } //fin de la boucle
  printf("dt = %lf, nfinal : %d, modulo = %d\n", dt, nfinal, modulo);
  for (i=0; i<L; i++) {
    fprintf(fic2, "%lf\n", U[i]);
  }
  fclose(fic1);
  fclose(fic2);
}
