#include "Menu.h"

using namespace std;

//Moteur du jeu
extern Moteur moteur;

//Constructeur
Menu::Menu()
{
	int i;
	for(i=0;i<4;i++)
		images[i] = NULL;

	for(i=0;i<4;i++)
		typejoueur[i] = NULL;

	txtj[0] = NULL;
	txtj[1] = NULL;
	//Par défaut, le joueur 1 est humain et le joueur 2 est de niveau moyen
	typej1=0;
	typej2=1;
}

Menu::~Menu()
{
	int i;
	//Libération des surfaces
	for(i=0;i<4;i++)
		{
		SDL_FreeSurface(images[i]) , images[i] = NULL;
		}
	for(i=0;i<4;i++)
		{
		SDL_FreeSurface(typejoueur[i]) , typejoueur[i] = NULL;
		}
	SDL_FreeSurface(txtj[0]) , txtj[0] = NULL;
	SDL_FreeSurface(txtj[1]) , txtj[1] = NULL;
}

//Fonction d'initialisation
bool Menu::init(string file)
{
	int i;
	ifstream input;
	string nom;

	//Ouverture du fichier de paramétrage
	input.open(file.c_str());

	//Test si le fichier s'est bien ouvert
	if(input.fail())
	        {
        	cout << "Erreur dans l'ouverture du fichier " << file << endl;
	        return false;
        	}

	//Récupération du nom de l'image pour le menu
	input >> nom;

	//On charge l'image de fond
	images[0] = SDL_LoadBMP(nom.c_str());

	//On récupère la surface de l'image "Titre"
	input >> nom;
	images[1] = SDL_LoadBMP(nom.c_str());

	//Initialisation des positions du menu
	input >> titre.x >> titre.y;

	//On récupère la surface de l'image "Nouveau"
	input >> nom;
	images[2] = SDL_LoadBMP(nom.c_str());

	//Initialisation de la position du bouton nouveau
	input >> nouveau.x >> nouveau.y;

	//On récupère la surface de l'image "Quitter"
	input >> nom;
	images[3] = SDL_LoadBMP(nom.c_str());

	//Mise en place de la transparence
	for(i=1;i<4;i++)
		SDL_SetColorKey(images[i],SDL_SRCCOLORKEY,0);

	//On teste le retour du chargement
	for(i=0;i<4;i++)
		if(images[i]==NULL)
		    	{
		    	cerr << "Probleme de chargement d'une des images du menu" << endl;
		    	return false;
		    	}

	//Initialisation de la position du bouton quitter
	input >> quitter.x >> quitter.y;

	//On récupére la taille des images
	nouveau.w = images[2]->w;
	nouveau.h = images[2]->h;

	quitter.w = images[3]->w;
	quitter.h = images[3]->h;

	//On récupère les surfaces des positions textes pour le joueur 1 et pour le joueur 2
	input >> nom;
	txtj[0] = SDL_LoadBMP(nom.c_str());
	input >> postxtj1.x >> postxtj1.y;
	SDL_SetColorKey(txtj[0],SDL_SRCCOLORKEY,0);

	input >> nom;
	txtj[1] = SDL_LoadBMP(nom.c_str());
	input >> postxtj2.x >> postxtj2.y;
	SDL_SetColorKey(txtj[1],SDL_SRCCOLORKEY,0);

	//Chargement des images pour le type des joueurs et leur position
	for(i=0;i<4;i++)
		{
		input >> nom;
		typejoueur[i] = SDL_LoadBMP(nom.c_str());

		//Test sur le chargement
		if(typejoueur[i]==NULL)
            {
            cerr << "Probleme de chargement d'une image typejoueur" << endl;
            return false;
            }

		SDL_SetColorKey(typejoueur[i],SDL_SRCCOLORKEY,0);
		}
	input >> postypej1.x >> postypej1.y;
	input >> postypej2.x >> postypej2.y;

	//Ferme le fichier
	input.close();

	//On retourne vrai
	return true;
}

//Gestion du clic
void Menu::clic(int x, int y)
{
//Est-ce qu'on est dans le bouton nouveau?
if((nouveau.x<x)&&(nouveau.x+nouveau.w>x)&&(nouveau.y<y)&&(nouveau.y+nouveau.h>y))
	{
	moteur.initJeu();
	moteur.setFonctionsJeu();
	}
//Est-ce qu'on est dans le bouton quitter?
else if((quitter.x<x)&&(quitter.x+quitter.w>x)&&(quitter.y<y)&&(quitter.y+quitter.h>y))
	moteur.fin();
//Est-ce qu'on est dans le bouton type joueur1?
else if((postypej1.x<x)&&(postypej1.x+typejoueur[typej1]->w>x)&&(postypej1.y<y)&&(postypej1.y+typejoueur[typej1]->h>y))
	{
	typej1= (typej1+1)%4;
	}
//Est-ce qu'on est dans le bouton type joueur2?
else if((postypej2.x<x)&&(postypej2.x+typejoueur[typej2]->w>x)&&(postypej2.y<y)&&(postypej2.y+typejoueur[typej2]->h>y))
	{
	typej2= (typej2+1)%4;
	}

}

//Fonction retourne le type des joueurs
int Menu::getType(int i)
{
	if(i==1)
		return typej1;
	else
		return typej2;
}

//Fonction d'affichage
void Menu::aff(SDL_Surface *screen)
{
	//Si on a une image, on l'affiche
	if(images[0] && images[1] && images[2] && images[3] && txtj[0] && txtj[1] &&
			typejoueur[typej1] && typejoueur[typej2])
	{
		SDL_BlitSurface(images[0],NULL,screen,NULL);
		SDL_BlitSurface(images[1],NULL,screen,&titre);
		SDL_BlitSurface(images[2],NULL,screen,&nouveau);
		SDL_BlitSurface(images[3],NULL,screen,&quitter);
		SDL_BlitSurface(txtj[0],NULL,screen,&postxtj1);
		SDL_BlitSurface(txtj[1],NULL,screen,&postxtj2);
		SDL_BlitSurface(typejoueur[typej1],NULL,screen,&postypej1);
		SDL_BlitSurface(typejoueur[typej2],NULL,screen,&postypej2);
	}
}
