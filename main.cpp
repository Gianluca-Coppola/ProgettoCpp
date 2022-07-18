/**
@file main.cpp
@brief test d'uso della classe graph
**/
#include "graph.h"
#include "Persona.h"
#include <iostream>
#include <cassert> //assert

struct Equal_int;
struct Equal_string;
struct Equal_Persona;
void test_construttori();
void test_assegnamento();
void test_add_node();
void test_remove_node();
void test_add_edge();
void test_remove_edge();
void test_iteratori();


/**
  @brief Funtore di eguaglianza int
**/
struct Equal_int{
  bool operator()(int i1, int i2) const{
    return i1 == i2;
  }
};

/**
  @brief Funtore di eguaglianza string
**/
struct Equal_string{
  bool operator()(std::string s1, std::string s2) const{
    return s1 == s2;
  }
};

/**
  @brief Funtore di eguaglianza Persona
**/
struct Equal_Persona{
  bool operator()(const Persona p1, const Persona p2) const{
    return p1.codiceFiscale() == p2.codiceFiscale();
  }
};

int main() {

  std::cout << "Test Costruttori: " << std::endl;
  test_construttori();
  std::cout << "Ok" << std::endl;
  std::cout << "Test Operatore di Assegnamento: " << std::endl;
  test_assegnamento();
  std::cout << "Ok" << std::endl;
  std::cout << "Test add_node: " << std::endl;
  test_add_node();
  std::cout << "Ok" << std::endl;
  std::cout << "Test remove_node: " << std::endl;
  test_remove_node();
  std::cout << "Ok" << std::endl;
  std::cout << "Test add_edge: " << std::endl;
  test_add_edge();
  std::cout << "Ok" << std::endl;
  std::cout << "Test remove_edge: " << std::endl;
  test_remove_edge();
  std::cout << "Ok" << std::endl;
  std::cout << "Test Iteratori: " << std::endl;
  test_iteratori();
  std::cout << "Ok" << std::endl;

  return 0;
}

void test_construttori(){           // Se flag C_DEBUG è stato definito stampa:
  graph<int, Equal_int> iG1;                       //graph()
  graph<int, Equal_int> iG2(iG1);                  //graph(const graph &other())

  graph<std::string, Equal_string> sG1;            //graph()
  graph<std::string, Equal_string> sG2(sG1);       //graph(const graph &other())

  graph<std::string, Equal_Persona> pG1;           //graph()
  graph<std::string, Equal_Persona> pG2(pG1);      //graph(const graph &other())
                                                   //~graph() x6
}

void test_assegnamento(){           // Se flag C_DEBUG è stato definito stampa:
  graph<int, Equal_int> iG1;                       //graph()
  graph<int, Equal_int> iG2;                       //graph()
  iG1 = iG2;                                       //operator=()
                                                   //   graph(const graph &other)
                                                   //   ~graph()

  graph<std::string, Equal_string> sG1;            //graph()
  graph<std::string, Equal_string> sG2;            //graph()
  sG1 = sG2;                                       //operator=()
                                                   //   graph(const graph &other)
                                                   //   ~graph()

  graph<std::string, Equal_Persona> pG1;           //graph()
  graph<std::string, Equal_Persona> pG2;           //graph()
  pG1 = pG2;                                       //operator=()
                                                   //   graph(const graph &other)
                                                   //   ~graph()
                                                   //~graph() x6
}

void test_add_node(){
  graph<int, Equal_int> iG;     //Grafo di Int
  assert(iG.sizeN() == 0);      //Non è presente alcun elemento nel grafo.

  int n1 = 123;                 //Aggiungo un elemento
  iG.add_node(n1);
  assert(iG.exists(n1));
  assert(iG.sizeN() == 1);

  for(int i = 0; i < 50; ++i){  //Aggiungo più elementi
    iG.add_node(i);
    assert(iG.exists(i));
  }

  try{                          //Provo ad aggiungere un duplicato.
    iG.add_node(n1);
    assert(false);
  }catch(DuplicateNodeException exc) {
    // Ho catturato l'eccezione, ok.
  }
}

void test_remove_node(){
  graph<std::string, Equal_string> sG; //Grafo di String
  sG.add_node("Test");                 //Aggiungo la frase
  sG.add_node("metodo");               //"Test metodo remove con
  sG.add_node("remove");               //grafo di stringhe" nel
  sG.add_node("node");                 //grafo parola per parola.
  sG.add_node("con");
  sG.add_node("grafo");
  sG.add_node("di");
  sG.add_node("stringhe");

  assert(sG.sizeN() == 8);             //Il grafo contiene 8 identificatori.

  sG.add_edge("remove", "node");
  sG.add_edge("grafo", "stringhe");
  assert(sG.sizeE() == 2);             //Il grafo contiene due archi
                                       //tra le stringhe remove -> _node
                                       // e tra grafo -> stringhe.

  sG.remove_node("di");                //Rimuovo i nodi "di" e "stringhe"
  sG.remove_node("stringhe");
  assert(sG.sizeN() == 6);             //Il grafo contiene ora 6 nodi
  assert(sG.sizeE() == 1);             // Ed un solo arco


  try{                                 //Provo a rimuovere un nodo inesistente.
    sG.remove_node("stringhe");
    assert(false);
  }catch(NonExistentNodeException exc) {
    // Ho catturato l'eccezione, ok.
  }

}

void test_add_edge(){
  graph<Persona, Equal_Persona> pG;               //Grafo di Persona
  assert(pG.sizeN() == 0);                        //Vuoto

  Persona p1("EJ139", "Eren", "Jaeger");          // Creo e Aggiungo al grafo
  Persona p2("JK278", "Jean", "Kirstein");        // 5 persone
  Persona p3("RA001", "Rivaille", "Ackerman");
  Persona p4("AA101", "Armin", "Arlert");
  Persona p5("MA102", "Mikasa", "Ackerman");

  pG.add_node(p1);
  pG.add_node(p2);
  pG.add_node(p3);
  pG.add_node(p4);
  pG.add_node(p5);

  assert(pG.sizeN() == 5);                        // Il grafo ha 5 nodi
  assert(pG.sizeE() == 0);                        // e 0 archi

  pG.add_edge(p5, p1);                            //Aggiungo 4 archi
  pG.add_edge(p4, p1);
  pG.add_edge(p1, p3);
  pG.add_edge(p2, p1);

  assert(pG.sizeE() == 4);                        //Il grafo ha 4 archi

  try{                                            //Provo ad aggiungere un arco
    Persona p27("CG278", "Coppola", "Gianluca");  // su un nodo inesistente.
    pG.add_edge(p27, p3);
    assert(false);
  }catch (NonExistentNodeException exc) {
    //Ho catturato l'eccezione, ok.
  }

  try{
    Persona p90("nen1", "N", "E");
    Persona p91("nen2", "N", "E");
    pG.hasEdge(p90, p91);
    assert(false);
  } catch (NonExistentNodeException exc) {
    //Ho catturato l'eccezione, ok.
  }
}

void test_remove_edge(){
  graph<int, Equal_int> iG;            //Creo grafo di interi vuoto

  for(int i = 0; i < 5; ++i)           // Inserisco 5 interi
    iG.add_node(i);

  for(int i = 0; i < 5; ++i)           //Collego tutti i nodi con archi
    for(int j = 0; j < 5; ++j)
      iG.add_edge(i, j);

  assert(iG.sizeE() == 5 * 5);

  for(int i = 0; i < 5; ++i)           //Rimuovo tutti gli archi del grafo
    for(int j = 0; j < 5; ++j){
      assert(iG.hasEdge(i, j));
      iG.remove_edge(i, j);
      assert(!iG.hasEdge(i, j));
    }

  assert(iG.sizeE() == 0);

  try{                                 //Provo a rimuovere un nodo non esisente.
    iG.remove_edge(-1, 4);
    assert(false);
  }catch(NonExistentNodeException exc){
    //Ho catturato l'eccezione, ok.
  }

}

void test_iteratori(){

  graph<Persona, Equal_Persona> pG;
  assert(pG.sizeN() == 0);

  Persona p1("EJ139", "Eren", "Jaeger");
  Persona p2("JK278", "Jean", "Kirstein");
  Persona p3("RA001", "Rivaille", "Ackerman");
  Persona p4("AA101", "Armin", "Arlert");
  Persona p5("MA102", "Mikasa", "Ackerman");

  pG.add_node(p1);
  pG.add_node(p2);
  pG.add_node(p3);
  pG.add_node(p4);
  pG.add_node(p5);

  graph<Persona, Equal_Persona>::const_iterator is, ie;
  ie = pG.end();
                                        //Test di stampa tramite const_iterator.
  for(is = pG.begin(); is != ie; is++) {
    if(is->cognome() == "Ackerman")
      std::cout << *is << " ";
  }
  std::cout << std::endl;


}
