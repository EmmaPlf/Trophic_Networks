#include "grman/grman.h"
#include <iostream>

#include "graph.h"

//BITMAP* charger = load_bitmap("Charger.bmp", NULL);

int main()
{
    bool asterix = false, livre = false;

    /// A appeler en 1er avant d'instancier des objets graphiques etc...
    grman::init();

    /// Le nom du r�pertoire o� se trouvent les images � charger
    grman::set_pictures_path("pics");

    /// CHARGEMENT DU GRAPHE
    Graph g;
    g.make_example();

    ///g.chargement_fichier("Asterix.txt");

    /// Vous gardez la main sur la "boucle de jeu"
    /// ( contrairement � des frameworks plus avanc�s )
    while ( !key[KEY_ESC] )
    {
        /// Il faut appeler les m�thodes d'update des objets qui comportent des widgets
        g.update();

     //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);
     //grman::show_picture(grman::page,"Asterix.bmp", 23,5,0);


    g.getInterface()->getAsterix().interact_focus();

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
                g.getInterface()->getSauvegarder().interact_focus();

                if(g.getInterface()->getSauvegarder().clicked()==true && livre == false)
                {
                    g.sauvegarder("Asterix.txt");
                }

                g.getInterface()->getSauvegarder().interact_focus();

                if(g.getInterface()->getSauvegarder().clicked()==true && asterix == false)
                {
                    g.sauvegarder("Livre_jungle.txt");
                }
            }

        }


        /// Mise � jour g�n�rale (clavier/souris/buffer etc...)
        grman::mettre_a_jour();
    }

    grman::fermer_allegro();

    return 0;
}
END_OF_MAIN();


