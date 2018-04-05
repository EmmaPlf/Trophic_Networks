#include "graph.h"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;
/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
:m_presence(true)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de r�glage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Nombre de gens
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if(pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );
}

bool VertexInterface::getPresence() const
{
    return m_presence;
}

void VertexInterface::setPresence(bool presence)
{
    m_presence = presence;
}

/// Gestion du Vertex avant l'appel � l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_value vers le slider associ�
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donn�e m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex apr�s l'appel � l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les �l�ments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }

    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de r�glage associ�s
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de r�glage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, � adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel � l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donn�e m_weight vers le slider associ�
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donn�e m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge apr�s l'appel � l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donn�e m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de pr�parer un cadre d'accueil des
/// �l�ments qui seront ensuite ajout�s lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(1000,740);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(80,720);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(908,720);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);

    m_tool_box.add_child(asterix);
    asterix.set_frame(0,0,85,80);
    asterix.set_bg_color(VERTSOMBRE);

    m_tool_box.add_child(livre);
    livre.set_frame(0,80,85,80);
    livre.set_bg_color(BLANCROSE);

    m_tool_box.add_child(sauvegarder);
    sauvegarder.set_frame(0,160,85,80);
    sauvegarder.set_bg_color(JAUNESOMBRE);

    m_tool_box.add_child(supprimer);
    supprimer.set_frame(0,320,85,80);
    supprimer.set_bg_color(VERTCLAIR);
}


/// M�thode sp�ciale qui construit un graphe arbitraire (d�mo)
/// Cette m�thode est � enlever et remplacer par un syst�me
/// de chargement de fichiers par exemple.
/// Bien s�r on ne veut pas que vos graphes soient construits
/// "� la main" dans le code comme �a.
void Graph::make_example()
{
 m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne pr�c�dente est en gros �quivalente � :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    /// Les sommets doivent �tre d�finis avant les arcs
    // Ajouter le sommet d'indice 0 de valeur 30 en x=200 et y=100 avec l'image clown1.jpg etc...
//    add_interfaced_vertex(0, 30.0, 200, 100, "clown1.jpg");
//    add_interfaced_vertex(1, 60.0, 400, 100, "clown2.jpg");
//    add_interfaced_vertex(2,  50.0, 200, 300, "clown3.jpg");
//    add_interfaced_vertex(3,  0.0, 400, 300, "clown4.jpg");
//    add_interfaced_vertex(4,  100.0, 600, 300, "clown5.jpg");
//    add_interfaced_vertex(5,  0.0, 100, 500, "bad_clowns_xx3xx.jpg", 0);
//    add_interfaced_vertex(6,  0.0, 300, 500, "bad_clowns_xx3xx.jpg", 1);
//    add_interfaced_vertex(7,  0.0, 500, 500, "bad_clowns_xx3xx.jpg", 2);

    /// Les arcs doivent �tre d�finis entre des sommets qui existent !
    // AJouter l'arc d'indice 0, allant du sommet 1 au sommet 2 de poids 50 etc...
//    add_interfaced_edge(0, 1, 2, 50.0);
//    add_interfaced_edge(1, 0, 1, 50.0);
//    add_interfaced_edge(2, 1, 3, 75.0);
//    add_interfaced_edge(3, 4, 1, 25.0);
//    add_interfaced_edge(4, 6, 3, 25.0);
//    add_interfaced_edge(5, 7, 3, 25.0);
//    add_interfaced_edge(6, 3, 4, 0.0);
//    add_interfaced_edge(7, 2, 0, 100.0);
//    add_interfaced_edge(8, 5, 2, 20.0);
//    add_interfaced_edge(9, 3, 7, 80.0);
}

/// La m�thode update � appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();

    for (auto &elt : m_vertices)
        elt.second.post_update();

    for (auto &elt : m_edges)
        elt.second.post_update();

}

/// Aide � l'ajout de sommets interfac�s // value nb individus
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        m_vertices.erase (idx);
        m_vertices.clear();

        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }

    // Cr�ation d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);

    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);

    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

void Graph::sauvegarder(std::string file_name)
{
    int x = 0, y = 0, id_sommet = 0, num_arete = 0, connexe=0;
    double nb_individus = 0;
    std::string pic_name;

    ///OUVERTURE MODE LECTURE
    ofstream fichier(file_name, ios::out | ios::trunc);

    //OUVERTURE FONCTIONNE
    if(fichier)
    {
        fichier << m_vertices.size() << std::endl;

           std::map<int, Vertex>::iterator it;
           for(it = m_vertices.begin(); it != m_vertices.end(); it ++)
           {
                /// RECUPERE LES DONNEES
                x = it->second.m_interface->m_top_box.get_frame().pos.x;
                y= it->second.m_interface->m_top_box.get_frame().pos.y;
                nb_individus = it->second.m_value;
                pic_name = it->second.m_interface->m_img.get_pic_name();

                /// ECRIT LES DONNEES
                fichier << it->first << " ";
                fichier << x << " ";
                fichier << y << " ";
                fichier << nb_individus << ".0" << " ";
                fichier << pic_name << std::endl;
           }

           fichier << m_edges.size() << std::endl;

           std::map<int, Edge>::iterator it2;
           for(it2 = m_edges.begin(); it2 != m_edges.end(); it2 ++)
               {
                    /// RECUPERE LES SOMMETS D ARETES
                    fichier << it2->first << " ";
                    fichier << it2->second.m_from << " ";
                    fichier << it2->second.m_to << std::endl;
               }
//                   for(int i=0; i<ordre; i++)
//                   {
//                       for(int j=0; j<ordre; j++)
//                       {
//                           if(id_to == i && id_from == j)
//                           matrice[i][j]=1;
//                       }
//                   }
//           }
//
//            for(int i=0; i<ordre; i++)
//                   {
//                       for(int j=0; j<ordre; j++)
//                       {
//                           if(matrice[i][j] == 1)
//                            fichier << "1" << " ";
//                           else fichier << "0" << " ";
//                       }

//                       fichier << std::endl;
//                   }

        fichier.close();
    }

    else
    {
        cerr <<"Impossible d'ouvrir le fichier" << std::endl;
        fichier.close();
        exit (0);
    }


}

void Graph::chargement_fichier(std::string file_name)
{
    int x = 0, y = 0, id_sommet = 0, num_arete = 0, connexe=0, s1=0, s2=0;
    double nb_individus = 0;
    unsigned int ordre = 0, nb_aretes;
    std::string pic_name;

    m_vertices.clear();
    m_edges.clear();

    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);

    ///OUVERTURE MODE LECTURE
    ifstream fichier(file_name, ios::in);

    ///OUVERTURE FONCTIONNE
    if(fichier)
    {
        fichier >> ordre;

        /// REMPLIR LE VECTEUR DE SOMMETS
        for(int i=0; i<ordre; i++)
        {
            fichier >> id_sommet;
            fichier >> x;
            fichier >> y;
            fichier >> nb_individus;
            fichier >> pic_name;

            add_interfaced_vertex(id_sommet, nb_individus, x, y, pic_name, i);
        }

        fichier >> nb_aretes;
        std::cout << nb_aretes;

        // REMPLIR LE VECTEUR D ARETES
        for(int i=0; i<nb_aretes; i++)
        {
                fichier >> num_arete;
                std::cout << num_arete;
                fichier >> s1;
                std::cout << s1;
                fichier >> s2;
                std::cout << s2 << " ";

                add_interfaced_edge(num_arete, s1, s2, 0.0);
        }

        fichier.close();
    }

    else
    {
        cerr <<"Impossible d'ouvrir le fichier" << std::endl;
        fichier.close();
        exit (0);
    }
}

/// Aide � l'ajout d'arcs interfac�s
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        m_edges.erase (idx);
        m_edges.clear();

        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
         m_edges.erase (id_vert1);
         m_edges.erase (id_vert2);

        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(id_vert2);
    m_vertices[id_vert2].m_in.push_back(id_vert1);
}

void Graph::ajouter_sommet()///modif
{
    if ((mouse_b&1)&&(mouse_x>700)&&(mouse_x<770)&&(mouse_y>140)&&(mouse_y<150))
           {

               add_interfaced_vertex(12, 700, 200, 100, "Emeu.jpg");
               add_interfaced_edge(20,12,1,20.0);
            // std::cout<<m_vertices[8].m_interface->m_top_box.get_posx()<<std::endl;
//             std::cout<<m_vertices[8].m_interface->m_top_box.get_posy()<<std::endl;

               update();///MERCI THIBAUD
           }
}

void Graph::supprimer_sommet(int id_sommet, std::string file_name)///modif
{

    std::vector<int> ind_aretes;

    m_interface->m_main_box.remove_child(m_vertices[id_sommet].m_interface->m_top_box);
    m_vertices.erase(id_sommet);

    std::map<int, Edge>:: iterator it;
    for(it=m_edges.begin(); it!=m_edges.end(); it++)
    {
        if(it->second.m_to == id_sommet || it->second.m_from == id_sommet)
        {
            ind_aretes.push_back(it->first); // Pas le choix sinon il parcours des artes qui ont �t� supprim�es auparavant
            m_interface->m_main_box.remove_child(it->second.m_interface->m_top_edge); // on supprime de l'interface
        }
    }

    if(ind_aretes.size()!=0)
    {
        for(int i=0; i<ind_aretes.size(); i++)
        {
            m_edges.erase(ind_aretes[i]);
        }
    }

    sauvegarder(file_name);
    chargement_fichier(file_name);
    update();
}

 std::shared_ptr<GraphInterface>& Graph::getInterface()
 {
     return m_interface;
 }

 grman::WidgetButton& GraphInterface::getAsterix()
 {
     return asterix;
 }

grman::WidgetButton& GraphInterface::getLivre()
 {
     return livre;
 }
