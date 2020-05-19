
#include <stdio.h>
#include <string.h>
#include <libxml2/libxml/parser.h>
#include <map>
#include <iostream>
#include <iterator>
#include <fstream>

using namespace std;

// On considère qu'une ligne est vide si sa première case est vide
// (permet d'éviter de considérer les lignes avec des commentaires)
bool is_ligne_vide(xmlNode* node){
    xmlNode* cell = node->children;
    int i=0, k;

    while (i<5){
        if(cell->children){
            return false;
        }

        if(xmlHasProp(cell,xmlCharStrdup("number-columns-repeated"))){
            k = stoi((char*)xmlGetProp(cell, xmlCharStrdup("number-columns-repeated")));
            i+=k;
        }
        else i++;
        cell = cell->next;
    }
    return true;
}

// Vérifie la couleur de la case : si gris, alors c'est un jour de congé
int is_jour_conge(xmlNode *node, xmlNode *styles){
    xmlChar* attribut = xmlCharStrdup("style-name");
    if(! xmlHasProp(node, attribut)){
        xmlFree(attribut);
        return 0;
    }

    xmlChar* cell = xmlGetProp(node,attribut);
    xmlNode* type = styles->children;
    xmlChar* attribut2 = xmlCharStrdup("name");

    while(type && xmlStrEqual(cell, xmlGetProp(type,attribut2))==0){
        type = type->next;
    }
    if (!type){
        xmlFree(attribut);
        xmlFree(attribut2);
        xmlFree(cell);
        return 0;
    }
    type = type->children;

    if(xmlHasProp(type, xmlCharStrdup("background-color"))){
        char* colour = (char*)xmlGetProp(type, xmlCharStrdup("background-color"));
        int red = colour[1]+colour[2];
        int green = colour[3]+colour[4];
        int blue = colour[5]+colour[6];
        if(red == green){
            if(green == blue){
                if(green < 200)
                    return 1;
                return 2;
            }
        } 
    }

    xmlFree(attribut);
    xmlFree(attribut2);
    xmlFree(cell);

    return 0;
}

bool is_debutante(xmlNode* node, xmlNode* styles){
    xmlChar* attribut = xmlCharStrdup("style-name");
    if(! xmlHasProp(node, attribut)){
        xmlFree(attribut);
        printf("ici\n");
        return false;
    }

    xmlChar* cell = xmlGetProp(node,attribut);
    xmlNode* type = styles->children;
    xmlChar* attribut2 = xmlCharStrdup("name");

    while(type && xmlStrEqual(cell, xmlGetProp(type,attribut2))==0){
        type = type->next;
    }
    if (!type){
        xmlFree(attribut);
        xmlFree(attribut2);
        xmlFree(cell);
        printf("là\n");
        return false;
    }
    type = type->children;

    // Pas besoin de vérifier la couleur : un agent n'a une couleur que si c'est un(e) débutant(e)
    if(xmlHasProp(type, xmlCharStrdup("background-color"))){
        // Si besoin de tester les couleurs, réutiliser le code de is_jour_conge
        // en changeant les taux : red>200, blue<100, green>100 = statut débutant
        return true;
    }

    xmlFree(attribut);
    xmlFree(attribut2);
    xmlFree(cell);

    return false;
}

string update_content(xmlNode* node, xmlNode* styles){
    string res;
    int i = is_jour_conge(node, styles);
    if(i == 1){
        res = "Congé";
    } else {
        if(i == 2){
            res = "*";
        } else{
        res = (char*)xmlNodeGetContent(node);
        }
    }
    res += " | ";
    return res;
}

string generate(xmlNode* tmp, xmlNode* styles, int nbJours){
    int i=0, k;
    xmlNode* tmp2 = tmp->children;
    while(i<4){
        if(xmlHasProp(tmp2,xmlCharStrdup("number-columns-repeated"))){
            k = stoi((char*)xmlGetProp(tmp2, xmlCharStrdup("number-columns-repeated")));
            i+=k;
        }
        else i++;
        tmp2 = tmp2->next;
    }
    // printf("Nom de l'agent : %s\n", xmlNodeGetContent(tmp2));
    string content = "Agent ";
    content+= ((char*)xmlNodeGetContent(tmp2));
    if (is_debutante(tmp2, styles)){
        content += " (deb)";
    }
    content += ": ";
    i=0;

    tmp2 = tmp2->next;
    while(tmp2 && i<nbJours){
        if(xmlHasProp(tmp2,xmlCharStrdup("number-columns-repeated"))){
            k = stoi((char*)xmlGetProp(tmp2, xmlCharStrdup("number-columns-repeated")));
            for(int j=0; j<k; j++){
                content += update_content(tmp2, styles);
                i++;
                if(i>=nbJours){
                    break;
                }
            }
        }
        else{
            content += update_content(tmp2, styles);
            i++;
        }
        tmp2 = tmp2->next;
    }
    return content;
}

xmlNode* create_service(xmlNode* node, xmlNode* styles){
    // On skippe les lignes vides
    bool res = is_ligne_vide(node);
    int i=0 ;

    while(res && node->next && i<5){
        node = node->next;
        res = is_ligne_vide(node);
        i++;
    }

    if(! node->next || i>=5 ){
        return NULL;
    }
    // tmp parcourra les cellules de la ligne node
    xmlNode* tmp = node->children;

    // récupérer le nom du mois
    for(i=0; i<4; i++){
        tmp = tmp->next;
    }

    char* mois = (char*)xmlNodeGetContent(tmp);

    // récupérer le nombre de jours
    while(xmlHasProp(tmp->next,xmlCharStrdup("value-type"))){
        tmp = tmp->next;
    }

    xmlChar* key = xmlNodeGetContent(tmp);
    int nbJours = stoi((char*)key);

    // récupérer le nom du service
    node = node->next;
    tmp = node->children;
    for(i=0; i<4; i++){
        tmp = tmp->next;
    }
    string name((char*)xmlNodeGetContent(tmp));
    name += "_";
    name += mois;
    name += ".txt";
    // récupérer le premier jour du mois
    tmp = tmp->next;
    key = xmlNodeGetContent(tmp);
    char* firstDay = (char*)key;

    node = node->next;
    ofstream file(name, ios::out);
    file << mois << "\n";
    file << nbJours <<"\n";
    file << firstDay << "\n";
    string content;
    
    // On tolère une ligne vide (notamment pour CS et SDC)
    i = 0;
    while(i<2){
        while(node && ! is_ligne_vide(node)){
            content = generate(node, styles, nbJours);
            file << content << "\n";
            node = node->next;
        }
        if(!node) break;
        node = node->next;
        i++;
    }

    file.close();
    xmlFree(key);
    return node->next;
}

int main(int argc, char **argv)
{
    xmlDoc         *document;
    xmlNode        *root, *scripts, *fonts, *styles, *body;
    char           *filename;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename.xml\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);
    // fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    
    scripts = root->children;
    fonts = scripts->next;
    styles = fonts->next;
    body = styles->next;

    xmlNode* table = body->children->children->next;
    xmlNode* tmp;
    
    int i;

    // S'il y a plusieurs edt sur un même excel
    // table = balise <table:table>
    while(table){
        i=0;
        // tmp = balise <office:forms> ou <table:table-column/row>
        tmp = table->children;
        while (tmp && ! xmlStrEqual(tmp->name, xmlCharStrdup("table-row"))){
            tmp = tmp->next;
        }

        while(tmp){
            tmp = create_service(tmp, styles);
            // printf("SERVICE FINI\n\n");
            i++;
        }
        table = table->next;
    }

    xmlFreeDoc(document);
    xmlCleanupParser();

    return 0;
}