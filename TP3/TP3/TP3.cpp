// TP3.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
// Auteurs: Frederick Simard, David Mongeau, Benjamin Gelineau

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<sys/timeb.h>
#include<time.h>

//ClearBuffer
void clearStdinBuffer()
{
	char c;
	//while (getchar() != '\n')
	//while ((c = getchar()) != '\n' && c != EOF) { }
	while ((c = getchar()) != '\n' && c != '\r') {}

}
/* Titre: lireEntierValidationETConversion
Description: permet d'introduire un nombre entier (0 et (2^31) - 1) au clavier
en faire la conversion et en faire la validation

INPUT: aucun
OUTPUT: valeur entiere convertie

*/
int lireValidationETConversion()
{
	char chaineAconvertir[100];
	int i, imin, nbCar = 0;
	int retVal = 0;
	int valide = 0;
	int chiffre = 0;
	int negatif = 0;
	int positif = 0;

	while (!valide)
	{
		negatif = 0;
		positif = 0;
		retVal = 0;

		printf("-> ");

		if (fgets(chaineAconvertir, sizeof(chaineAconvertir), stdin) != NULL)
		{
			nbCar = strlen(chaineAconvertir);

			if (chaineAconvertir[0] == '-')
				negatif = 1;
			if (chaineAconvertir[0] == '+')
				positif = 1;

			if (nbCar <= 11) // nombre decimal trop grand
			{
				i = nbCar - 2;
				if (negatif || positif)
					imin = 1;
				else
					imin = 0;
				while (i >= imin)
				{
					if (chaineAconvertir[i] >= '0' && chaineAconvertir[i] <= '9')
					{
						chiffre = chaineAconvertir[i] - '0';
						retVal += chiffre * int(pow(10.0, nbCar - i - 2));
						if ((negatif && (retVal >= 0)) || (!negatif && (retVal > (INT_MAX - 1))))
						{
							i = 0;
							valide = 0;
							retVal = 0;
						}
						else
						{
							valide = 1;
						}
					}
					else
					{
						i = 0;
						valide = 0;
						retVal = 0;
					}
					i--;
				}
			}

		}
	}
	if (!negatif)
		return(retVal);
	else
		return(-retVal);
}
typedef unsigned char UCHAR;
//Hauteur et largeur
int h, l;
struct IMAGERAW {
	int hauteur, largeur;
	UCHAR* image;
};
typedef struct IMAGERAW IMAGE;

//Fonction lire_image() qui permettra de lire le fichier image.raw et retourne le pointeur im sur IMAGE
//Input: l,h, fpI
//Output: Rien
IMAGE* lire_image() {

	char nomFichier[80];
	IMAGE* im;
	FILE* fpI;
	//Demander nom de fichier, ouvrir le fichier en lecture
	printf("Veuillez entrer le nom du fichier\n");
	scanf("%s", &nomFichier);
	fpI = fopen(nomFichier, "rb");
	while (!fpI)
	{
		printf("\aimpossible d'ouvrire le fichier, veuillez reessayer: \n");
		scanf("%s", &nomFichier);
		fpI = fopen(nomFichier, "rb");
	}
	//Demande a l'utilisateur d'inscire la largeur et la hauteur du fichier raw
	clearStdinBuffer();
	printf("Veuillez entrer la largeur du fichier raw entre 0 et (2^31) - 1 ");
	l = lireValidationETConversion();
	printf("Veuillez entrer la hauteur du fichier raw entre 0 et (2^31) - 1 ");
	h = lireValidationETConversion();
	//Regarde le nombre d'octets du document
	fseek(fpI, 0, 2);
	long int res = ftell(fpI);
	fseek(fpI, 0, 0);
	//Validation de la largeur et hauteur en fonction du nombre d'octets
	while (res != h * l) {
		printf("Largeurs et hauteur d'images raw invalides...\n");
		printf("Veuillez entrer la largeur du fichier raw entre 0 et (2^31) - 1 ");
		l = lireValidationETConversion();
		printf("Veuillez entrer la hauteur du fichier raw entre 0 et (2^31) - 1 ");
		h = lireValidationETConversion();
		fseek(fpI, 0, 2);
		res = ftell(fpI);
		fseek(fpI, 0, 0);
	}
	//Reserver un espace memoire pour im
	im = (IMAGE*)malloc(sizeof(IMAGE));
	//Initialisation de hauteur et largeur en fonction de l'entrée des utilisateurs
	im->largeur = l;
	im->hauteur = h;
	im->image = (UCHAR*)malloc(im->hauteur * im->largeur);

	//Lire l’image sur le disque 
	fread(im->image, sizeof(UCHAR), im->hauteur * im->largeur, fpI);
	fclose(fpI);
	/* RETOURNER LE POINTEUR SUR IMAGE */
	return(im);

}
//Fonction de cryptage et decryptage
//Input: cle, choix de la direction de calcul de la matrice
//Output:none
IMAGE* crypter_decrypter(IMAGE* imageS) {
	int i, j, L, m, n, H, num;
	struct _timeb start, finish;
	unsigned long duration;
	UCHAR pixel, cle;
	/* Acces avec positionnement spatial dans une image */
	L = imageS->largeur; // largeur de l’image
	H = imageS->hauteur; // hauteur de l’image
	i = j = 1;
	pixel = imageS->image[i * L + j];
	//Entrer par l'utilisateur de la cle
	printf("Veuillez entrer la valeur de la cle entre 0 et 255 ");
	num = lireValidationETConversion();
	//Validation
	while (num > UCHAR_MAX) {
		printf("Veuillez entrer la valeur de la cle entre 0 et 255");
		num = lireValidationETConversion();
	}
	cle = num;
	printf("1- Par rangee\n");
	printf("2- Par colone\n");
	printf("Veuillez choisir de quelle facon(associer au numero) voulez-vous traiter l'image en entrant le numero:\n");
	scanf_s("%d", &num);
	//Validation
	while (num < 1 || num > 2) {
		printf("Entrer un choix valide: ");
		scanf_s("%d", &num);
	}
	if (num == 1) {
		_ftime(&start);
		for (m = 0; m < H; m++) // Acces avec positionnement spatial
			for (n = 0; n < L; n++)
				imageS->image[m * L + n] = imageS->image[m * L + n] ^ cle; /*XOR*/

		_ftime(&finish);
		duration = ((unsigned long)finish.time * 1000L + (unsigned long)finish.millitm) - ((unsigned long)start.time * 1000L + (unsigned long)start.millitm);
		printf("Temps d'execution : %d ms\n", duration);

	}
	if (num == 2) {
		_ftime(&start);
		for (m = 0; m < L; m++) // Acces avec positionnement spatial
			for (n = 0; n < H; n++)
				imageS->image[n * L + m] = imageS->image[n * L + m] ^ cle; /*XOR*/

		_ftime(&finish);
		duration = ((unsigned long)finish.time * 1000L + (unsigned long)finish.millitm) - ((unsigned long)start.time * 1000L + (unsigned long)start.millitm);
		printf("Temps d'execution : %d ms\n", duration);
	}
	return(imageS);

}
//Fonction qui permet d'ecrire et de sauvegarder l'image crypter ou decrypter
//Input: None
//Output: Fichier sauvegarder
void ecire_image(IMAGE* imageD) {
	char nomFichier[80];
	FILE* fpD;
	//Demander a l'utilisateur le nom du fichier
	printf("Veuillez entrer le nom du fichier\n");
	scanf("%s", &nomFichier);
	fpD = fopen(nomFichier, "wb");
	fwrite(imageD->image, sizeof(UCHAR), imageD->hauteur * imageD->largeur, fpD); // voir si fwrite() retourne une valeur valide
	fclose(fpD);
}
//main
int main()
{
	IMAGE* imageS, * imageD;
	//Lit imageS
	imageS = lire_image();
	printf("L'image commence le processus de cryptage\n");
	//Crypte imageS
	crypter_decrypter(imageS);
	printf("L'image a ete crypter\n");
	imageD = imageS;
	//Ecriture du fichier
	ecire_image(imageS);
	printf("Image a ete ecrit et sauvegarder dans le fichier source avec le nom donne\n");
	free(imageS);
}
