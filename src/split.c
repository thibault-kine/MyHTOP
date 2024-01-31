#include <stdlib.h>
#include <stdio.h>

#include "../headers/split.h"

// Pour activer les messages de debug, décommentez la ligne suivante:
// #define DEBUG
// ou compiler avec la commande suivante:
// gcc -DDEBUG split.c

int is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ',')
	// Si c'est un espace, une tabulation,
	// un saut de page, un retour chariot ou un retour à la ligne
		return (1);
	return (0);
}

int count_letters(char *str, int i)
 // i est la position du premier caractere d'un mot dans la chaine
{
	int count;

	count = 0;										// Compteur de caractères
	while (str[i] != '\0' && is_space(str[i]) == 0) // Tant qu'on est pas à la fin de la chaîne et qu'on est pas sur un espace
	{
		count++; // On incrémente le compteur de caractères
		i++;
	}
    
	return (count);
}

int count_words(char *str)
{
	int i;
	int count;

	i = 0;				   // Compteur de caractères
	count = 0;			   // Compteur de mots
	while (str[i] != '\0') // Tant qu'on est pas à la fin de la chaîne
	{
		if (is_space(str[i]) == 0) // Si on est pas sur un espace
		{
			count++;										// On incrémente le compteur de mots
			while (str[i] != '\0' && is_space(str[i]) == 0) // Tant qu'on est pas à la fin de la chaîne
															// et qu'on est pas sur un espace
				i++;										// On incrémente le compteur de caractères
		}
		else
			i++; // On passe au caractère suivant
	}

	return (count);
}

char **split(char *str) {

	char **tab;
	int i;
	int j;
	int k;

	i = 0;															// i parcourt la chaîne de caractères
	j = 0;															// j parcourt le tableau de pointeurs
	k = 0;															// k parcourt la chaîne de caractères de destination
	tab = (char **)malloc(sizeof(char *) * (unsigned int)(count_words(str) + 1)); // Allocation du tableau de pointeurs, +1 pour le NULL

	while (str[i] != '\0') // Tant qu'on est pas à la fin de la chaîne
	{
		if (is_space(str[i]) == 0) // Si on est pas sur un espace
		{
			tab[j] = (char *)malloc(sizeof(char) * (unsigned int)(count_letters(str, i) + 1)); // Allocation de la chaîne de caractères, +1 pour le '\0'

			while (str[i] != '\0' && is_space(str[i]) == 0) // Tant qu'on est pas à la fin de la chaîne et qu'on est pas sur un espace
				tab[j][k++] = str[i++];						// On copie le caractère et on incrémente i et k
			tab[j][k] = '\0';								// On termine la chaîne de caractères

			j++;   // On incrémente j pour passer à la chaîne suivante
			k = 0; // On remet k à 0 pour la chaîne suivante
		}
		else
			i++; // On incrémente i pour passer au caractère suivant
	}
	tab[j] = NULL; // On termine le tableau de pointeurs
	return (tab);  // On retourne le tableau de pointeurs
}