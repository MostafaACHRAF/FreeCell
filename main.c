#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/** La structure d'une carte ********************************************************************************************************/
typedef struct Carte {
    char num; /*Le numéro de la carte : A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K*/
    char type; /*La nature de la carte : coeur, carreau, trefle, ou pique*/
    char couleur; /*La couleur de la carte Noire (N) ou Rouge (R)*/
    struct Carte *suiv; /*Un pointeur sur la carte qui suit*/
}Carte;


/** La structure d'un groupe ********************************************************************************************************/
typedef struct Groupe {
    int id; /*L'identifiant du Groupe 1 pour Coeur, 2 pour Carreau, 3 pour Trefle, 4 pour Pique*/
    Carte *suiv; /*Un pointeur sur la liste des cartes*/
}Groupe;

/** La structure de la liste des groupes aléatoires *********************************************************************************/
typedef struct Liste {
    int id; /*Le numéro du groupe choisi par rand()*/
    struct Liste *suiv; /*le prochain élément dans la liste*/
}Liste;

/** La structure des piles *********************************************************************************************************/
typedef struct Pile {
    char nom; /*Le nome de la pile, pour ce Jeu on a besoins de 8 piles*/
    struct Pile *suiv; /*la pile qui suite*/
    Carte *next; /*Un pointeur sur le début de la liste des cartes*/
}Pile;

/** *********************************************************************************************************************************
*********************************************************** Fonctions *********************************************************** **/
void AjouterCartes(Groupe **E) {
    Carte *temp;
    Carte *elem;
    int i;
    char couleur; /*La couleur de la carte*/
    char type;
    char num[13] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K'}; /*Les numéros des cartes possibles*/

    /*Déterminer le groupe où elle appartient la carte*/
    switch((*E)->id) {
    case 1: type = 3; couleur = 'R'; break;
    case 2: type = 4; couleur = 'R'; break;
    case 3: type = 5; couleur = 'N'; break;
    case 4: type = 6; couleur = 'N'; break;
    }

    /*Remplir la liste*/
    for (i = 0; i < 13; i++) {
        elem = (Carte*)malloc(sizeof(Carte));
        elem->num = num[i];
        elem->type = type;
        elem->couleur = couleur;
        elem->suiv = NULL;

        if (i == 0) {
            temp = elem;
            (*E)->suiv = elem;
        }

        else {
            temp->suiv = elem;
            temp = temp->suiv;
        }
    }

    temp->suiv = NULL;
}

void InsererElem(Liste **l, int val) {
    Liste *elem;
    Liste *temp;

    elem = (Liste*)malloc(sizeof(Liste));
    elem->id = val;
    elem->suiv = NULL;

    if (*l == NULL) {
        *l = elem;
    }

    else {
        temp = *l;

        while (temp->suiv != NULL) {
            temp = temp->suiv;
        }

        temp->suiv = elem;
    }
}

void RandGroupe(Liste **l) {
    int r; /*Une variable pour stocker le résultat de rand()*/
    int i; /*Un compteur*/

    srand(time(NULL)); /*Initialiser la fonction rand()*/
    for (i = 0; i < 4; i++) {
        r = rand()%4;
        InsererElem(l, r); /*Insérer la valeur de r dans la liste des groupes aléatoires*/
    }
}

void CreerPile(Pile **P) {
    Pile *temp;
    Pile *elem;
    int i;
    char noms[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

    for (i = 0; i < 8; i++) {
        elem = (Pile*)malloc(sizeof(Pile));
        elem->nom = noms[i];
        elem->next = NULL;
        elem->suiv = NULL;
        if (i == 0) {
            temp = elem;
            *P = temp;
        }

        else {
            temp->suiv = elem;
            temp = temp->suiv;
        }
    }
}

int CarteExisteDeja(Pile *P, Carte *C) {
    Carte *temp;

    temp = P->next;

    while (temp != NULL) {
        if (temp->type == C->type && temp->num == C->num && temp->couleur == C->couleur) {
            printf("CartePile[%c%c%c] et CarteAdd:[%c%c%c] redendance \n", temp->num, temp->type, temp->couleur, C->num, C->type, C->couleur);
            return 1; /*La carte est déjà existe dans la pile*/
        }

        temp = temp->suiv;
    }
    return 0;
}

void EmpilerCarte(Pile *P ,Carte *nouv, Carte **lastCarte) {

    /*Si la carte n'existe pas encore on l'ajoute*/
    if (!CarteExisteDeja(P, nouv)) {
        Pile *temp1;

        temp1 = P;

        if (temp1->next == NULL) {
                temp1->next = nouv;
                *lastCarte = nouv;
        }

        else {
                (*lastCarte)->suiv = nouv;
                *lastCarte = nouv;
        }
        printf("(%c, %c, %c) a ete bien empile!\n", nouv->num, nouv->type, nouv->couleur);
    }
    else {printf("(%c, %c, %c) existe deja!\n", nouv->num, nouv->type, nouv->couleur);}
}

void SupprimerCarte(Groupe **G, char num, char couleur) {
    Carte *temp; /*Un pointeur sur l'élément*/
    Carte *tempp; /*Un pointeur sur le précédent de l'élément recherché*/
    int supprimer = 0; /*l'élément recherché n'est pas encore supprimer*/

    temp = (*G)->suiv; /*le pointeur temp*/
    tempp = (*G)->suiv; /*tempp : temp précédent*/

    while (temp != NULL && supprimer == 0) {

        if (temp->num == num && temp->couleur == couleur) {

            printf("la carte (%c%c%c) a ete bien supprime\n", temp->num, temp->type, temp->couleur);

            if (temp == tempp) {
                /*Si on veut supprimer le premièr élément*/
                (*G)->suiv = temp->suiv;
                free(temp);
                free(tempp);
            }

            else {
                tempp->suiv = temp->suiv;
                free(temp);
            }

            supprimer = 1;
        }

        tempp = temp;
        temp = temp->suiv;
    }
}

void Affichage (Pile *p) {
    printf("===================================================================\n");
    printf("[]\t[]\t[]\t[]\t|\t[]\t[]\t[]\t[]\n");
    printf("===================================================================\n");
    Pile *piles = p;
    Carte *cartes;
    while (piles != NULL) {
        printf("%c\t", piles->nom);
        piles = piles->suiv;
    }
    printf("\n");
    piles = p;
    while (piles != NULL) {
        cartes = piles->next;
        while (cartes != NULL) {
            printf("[%c%c%c](%c)\n", cartes->num, cartes->type, cartes->couleur, piles->nom);
            cartes = cartes->suiv;
        }
        piles = piles->suiv;
    }
}

void RemplirPile(Liste *l, Pile *P, Groupe **Co, Groupe **Ca, Groupe **Tr, Groupe **Pi) {
    Liste *temp;
    Carte *newCarte;
    int r, i = 1;

    temp = l;

    //while (*Co != NULL && *Ca != NULL && *Tr != NULL && *Pi != NULL) {
    //RandGroupe(&l);
    srand(time(NULL));
        while (temp != NULL) {
            newCarte = (Carte*)malloc(sizeof(Carte));

            switch  (temp->id) {
            case 0: newCarte->type = 3; newCarte->couleur = 'R'; break;
            case 1: newCarte->type = 4; newCarte->couleur = 'R'; break;
            case 2: newCarte->type = 5; newCarte->couleur = 'N'; break;
            case 3: newCarte->type = 6; newCarte->couleur = 'N'; break;
            }

            r = rand()%13;

            switch (r) {
            case 0: newCarte->num = 'A'; break;
            case 1: newCarte->num = '2'; break;
            case 2: newCarte->num = '3'; break;
            case 3: newCarte->num = '4'; break;
            case 4: newCarte->num = '5'; break;
            case 5: newCarte->num = '6'; break;
            case 6: newCarte->num = '7'; break;
            case 7: newCarte->num = '8'; break;
            case 8: newCarte->num = '9'; break;
            case 9: newCarte->num = '0'; break;
            case 10: newCarte->num = 'J'; break;
            case 11: newCarte->num = 'Q'; break;
            case 12: newCarte->num = 'K'; break;
            }

            i++;

            Carte *lastCarte;

            EmpilerCarte(P, newCarte, &lastCarte);

            switch (temp->id) {
            case 0: SupprimerCarte(Co, newCarte->num, newCarte->couleur); break;
            case 1: SupprimerCarte(Ca, newCarte->num, newCarte->couleur); break;
            case 2: SupprimerCarte(Tr, newCarte->num, newCarte->couleur); break;
            case 3: SupprimerCarte(Pi, newCarte->num, newCarte->couleur); break;
            }

            temp = temp->suiv;
        }
    //}
}

void NouvellePartie() {
    Groupe *Co; /*Un pointeur pour les Coeurs*/
    Groupe *Ca; /*Un pointeur pour les Carreaus */
    Groupe *Tr; /*Un pointeur pour les Treflet*/
    Groupe *Pi; /*Un pointeur pour les piques*/
    Liste *l = NULL; /*La liste des groupes aléatoire*/
    Pile *p;


    /*Alouer l'espace mémoire pour les Coeurs*/
    Co = (Groupe*)malloc(sizeof(Groupe));
    Co->id = 1;
    Co->suiv = NULL; /*Accune cartes*/

    AjouterCartes(&Co); /*Remplir la liste des cartes*/

    /*Alouer l'espace mémoire pour les Carreaux*/
    Ca = (Groupe*)malloc(sizeof(Groupe));
    Ca->id = 2;
    Ca->suiv = NULL; /*Accune cartes*/

    AjouterCartes(&Ca); /*Remplir la liste des cartes*/

    /*Alouer l'espace mémoire pour les Treflets*/
    Tr = (Groupe*)malloc(sizeof(Groupe));
    Tr->id = 3;
    Tr->suiv = NULL; /*Accune cartes*/

    AjouterCartes(&Tr); /*Remplir la liste des cartes*/

    /*Alouer l'espace mémoire pour les Piques*/
    Pi = (Groupe*)malloc(sizeof(Groupe));
    Pi->id = 4;
    Pi->suiv = NULL;/*Accune cartes*/

    AjouterCartes(&Pi); /*Remplir la liste des cartes*/

    RandGroupe(&l);

    CreerPile(&p);

    printf("\n***Avant***\n");
    Affichage(p);

    RemplirPile(l, &p, &Co, &Ca, &Tr, &Pi);

    printf("\n***Apres***\n");
    Affichage(p);
}


/** *********************************************************************************************************************************
********************************************************* Main ****************************************************************** **/
int main()
{
    NouvellePartie();
    return 0;
}
