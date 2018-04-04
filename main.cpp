#include "grman/grman.h"
#include <iostream>
#include "menu.h"
#include "graph.h"

//BITMAP* charger = load_bitmap("Charger.bmp", NULL);

int main()
{
    bool asterix = false, livre = false;
    int compteur_emeu=0;
//    int *compteur_emeu;
    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du répertoire où se trouvent les images à charger
    grman::set_pictures_path("pics");

    /// CHARGEMENT DU GRAPHE
    Graph g;
    g.make_example();
    Menu m;
    m.load();
    ///g.chargement_fichier("Asterix.txt");
    while(m.display()!=1)
    {
        m.display();
        grman::mettre_a_jour();
    }
    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement à des frameworks plus avancés )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les méthodes d'update des objets qui comportent des widgets
        g.update();


        //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);
        //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);

        if (asterix==true)
        {
            grman::show_picture(grman::page,"Emeu2.jpg",21,245,0);
        }

        g.getInterface()->getAsterix().interact_focus();
      //  std::cout << std::endl << g.getInterface()->getAsterix().get_value();

        if(g.getInterface()->getAsterix().clicked()==true &&  asterix == false )
        {
            g.chargement_fichier("Asterix.txt");
            asterix = true;
            livre = false;
        }


        else
        {

            g.getInterface()->getLivre().interact_focus();

            if(g.getInterface()->getLivre().clicked()==true && livre == false)
            {
                g.chargement_fichier("Livre_jungle.txt");
                livre = true;
                asterix = false;
            }
            else
            {
                g.getInterface()->getEmeu().interact_focus();

                if(g.getInterface()->getEmeu().clicked()==true)
                {

                    ///appeler ajouter emeu

                }
            }
        }


        g.ajouter_sommet();
        /// Mise à jour générale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


