#ifndef GRAPH_H
#define GRAPH_H

#include "graph.h"
#include <iostream> // std::cout std::endl
#include <algorithm> //std::swap
#include <stdexcept> // std::logic_error

/**
  @file grafo.h
  @brief Dichiarazione della classe grafo ed eccezioni lanciate
*/

/**
  @exception DuplicateNodeException
  @brief Eccezione per nodi duplicati

  Eccezione custom, che viene lanciata nel caso si cerca di aggiungere un nodo
  già presente nel grafo.
*/
struct DuplicateNodeException : public std::logic_error{
  DuplicateNodeException(const std::string &msg) : std::logic_error(msg) { }
};

/**
  @exception DuplicateNodeException
  @brief Eccezione per nodi inesistenti.

  Eccezione custom, che viene lanciata nel caso si cerca di usare un nodo
  non presente nel grafo.
*/
struct NonExistentNodeException : public std::logic_error{
  NonExistentNodeException(const std::string &msg) : std::logic_error(msg) { }
};


/**
  @class graph
  @brief Grafo orientato

  Classe che rappresenta un grafo orientato, implementato tramite matrici di adiacenza.
  La classe necessita di un funtore Equal per il confronto tra tipi generici T.
  La classe possiede un forward const_iterator, che itera sul identificatore dei nodi del grafo.

*/
template <typename T, typename Eq>
class graph {
public:
  typedef T value_type;
  typedef unsigned int size_type;

  /**
    @brief Costruttore di default

    Costruttore di default per istanziare un grafo vuoto.

    @post _nodes == nullptr
    @post _edges == nullptr
    @post _sizeN == 0
    @post _sizeE == 0
  */
  graph() : _sizeN(0), _sizeE(0), _nodes(nullptr), _edges(nullptr) {
    #ifdef C_DEBUG
    std::cout << "graph()" << std::endl;
    #endif
  }

  /**
    @brief Copy constructor

    Costruttore di copia che instanzia un grafo contenente i valori di un altro
    grafo other.

    @param other Grafo da copiare
  */
  graph(const graph &other) : _nodes(nullptr), _edges(nullptr), _sizeN(0), _sizeE(0) {
		init(other._sizeN);
    try{
      for(size_type i = 0; i < other._sizeN; ++i){
        _nodes[i] = other._nodes[i];
        for(size_type j = 0; j < other._sizeN; ++j)
          _edges[i][j] = other._edges[i][j];
      }
      _sizeE = other._sizeE;
    }catch(...) {
      reset();
      throw;
    }

    #ifdef C_DEBUG
    std::cout << "graph(const graph &other)" << std::endl;
    #endif
  }

  /**
    @brief Operatore di assegnamento

    Operatore di assegnamento che serve per copiare il contenuto di un other in *this.

    @param other Graph da copiare
    @return Reference a graph
  */

  graph& operator=(const graph &other) {
    #ifdef C_DEBUG
    std::cout << "operator=()" << std::endl;
    #endif

    if(&other != this) {
      graph tmp(other);
      swap(tmp);
    }

    return *this;
  }

  /**
    @brief Distruttore della classe

    Distruttore della classe. Rimuove dallo heap tutto il grafo allocato
    in precedenza
  */
  ~graph() {
    reset();
    #ifdef C_DEBUG
    std::cout << "~graph()" << std::endl;
    #endif
  }

	/**
	  @brief Funzione che scambia il contenuto di due grafi.

	  Classe che permette lo scambio del contenuto di un grafo this con un grafo other
    @param other graph con cui effettuare lo scambio
	*/
  void swap(graph &other){
    std::swap(_nodes, other._nodes);
    std::swap(_edges, other._edges);
    std::swap(_sizeN, other._sizeN);
    std::swap(_sizeE, other._sizeE);
    std::swap(_eq, other._eq);
  }

  /**
    @brief Esistenza nodo

    Funzione che verifica l'esistenza di un nodo. Se il nodo esiste la funzione
    ritorna true, altrimenti false.

    @param node Nodo di cui verificare l'esistenza
    @return true se il nodo esiste, false altrimenti
  */
  bool exists(const value_type &node){
    return find(node) != -1;
  }

  /**
    @brief Esistenza arco

    Funzione che verifica l'esistenza di un arco tra due nodi. La Funzione
    ritorna il valore boolean contenuto nella cella della matrice _edges
    che corrisponde all'arco tra due nodi.

    @param n1 Nodo di partenza dell'arco
    @param n2 Nodo di destinazione dell'arco
    @return true se l'arco esiste, false altrimenti
  */
  bool hasEdge(const value_type &n1, const value_type &n2){
    if(!exists(n1) || !exists(n2))
      throw NonExistentNodeException("Uno dei due nodi non è presente nel grafo.");
    return _edges[find(n1)][find(n2)];
  }

  /**
    @brief Aggiunge un nodo al grafo.

    Funzione che aggiunge un nodo al grafo.
    Se si tenta di aggiungere un nodo già presente nel grafo verrà lanciata
    un eccezione.

    @param node Nodo da aggiungere al grafo.
    @throws DuplicateNodeException
  */
  void add_node(const value_type &node) {
    if(exists(node))
      throw DuplicateNodeException("Il nodo è già presente nel grafo.");
	  graph tmp (*this);
	  reset();
    try{
  	  init(tmp._sizeN+1);
  	  for(size_type i = 0; i < tmp._sizeN; ++i){
  	    _nodes[i] = tmp._nodes[i];
  	    for(size_type j = 0; j < tmp._sizeN; ++j)
  	      _edges[i][j] = tmp._edges[i][j];
  	  }
      _sizeE = tmp._sizeE;
    }catch(...){
      this->swap(tmp);
      throw;
    }
	  _nodes[_sizeN - 1] = node;
  }

  /**
    @brief Rimuove un nodo dal grafo

    Funzione che rimuove un nodo dal grafo. Se si tenta di rimuovere un nodo che
    non è presente nel grafo verrà lanciata un eccezione.

    @param node Nodo da rimuovere dal grafo.
    @throws NonExistentNodeException
  */
  void remove_node(const value_type &node) {
    if(!exists(node))
      throw NonExistentNodeException("Il nodo da rimuovere non esiste nel grafo.");

	  graph tmp (*this);
    int index = find(node);
	  reset();
    try{
  	  init(tmp._sizeN-1);
  	  size_type s = 0, t = 0;
      for(size_type i = 0; i < _sizeN; ++i){
        if(s == index)
  	       s++;
  	    _nodes[i] = tmp._nodes[s];
  	    t = 0;
  	    for(size_type j = 0; j < _sizeN; ++j){
  	      if(t == index)
  	        t++;
  	      _edges[i][j] = tmp._edges[s][t];
  	      t++;
  	    }
  	    s++;
  	  }

      for(size_type i = 0; i < _sizeN; ++i)
        for(size_type j = 0; j < _sizeN; ++j)
          if(_edges[i][j])
            ++_sizeE;


    }catch (...){
        this->swap(tmp);
        throw;
      }
  }

  /**
    @brief Aggiunge un arco al grafo.

    Funzione che aggiunge un arco tra due nodi del grafo.
    Se si tenta di aggiungere arco tra nodi non esistenti nel grafo verrà lanciata
    un eccezione.

    @param n1 Nodo di partenza dell'arco
    @param n2 Nodo di destinazione dell'arco
    @throws NonExistentNodeException
  */
  void add_edge(const value_type &n1, const value_type &n2){
    if(!exists(n1) || !exists(n2))
      throw NonExistentNodeException("Uno dei due nodi non esiste nel grafo.");
    size_type i = find(n1);
    size_type j = find(n2);
    if(!_edges[i][j]){
      _edges[i][j] = true;
      _sizeE++;
    }
  }

  /**
    @brief Rimuove un arco al grafo.

    Funzione che rimuove un arco tra due nodi del grafo.
    Se si tenta di rimuovere un arco tra nodi non esistenti nel grafo verrà lanciata
    un eccezione.

    @param n1 Nodo di partenza dell'arco
    @param n2 Nodo di destinazione dell'arco
    @throws NonExistentNodeException
  */
  void remove_edge(const value_type &n1, const value_type &n2){
    if(!exists(n1) || !exists(n2))
      throw NonExistentNodeException("Uno dei due nodi non esiste nel grafo.");
    size_type i = find(n1);
    size_type j = find(n2);
    if(_edges[i][j]){
      _edges[i][j] = false;
      _sizeE--;
    }
  }

  /**
    @brief Getter dimensione insieme dei nodi.

    Ritorna il numero di nodi presenti nel grafo.
    @return Dimensione dell'insieme di Nodi.
  */
  size_type sizeN() const{
    return _sizeN;
  }

  /**
    @brief Getter dimensione insieme degli archi.

    Ritorna il numero di archi presenti nel grafo.
    @return Dimensione dell'insieme degli archi presenti nel grafo.
  */
  size_type sizeE() const{
    return _sizeE;
  }

friend std::ostream &operator<<(std::ostream& os, const graph<T, Eq>& g) {

  os << "Numero di Nodi: " << g.sizeN() << ", Numero di Archi: " << g.sizeE() << std::endl;
  os << "Nodi = { ";
  for(size_type i = 0; i < g.sizeN(); ++i)
    os << g._nodes[i] << " ";
  os << "}" << std::endl;
  os << "Archi = { ";
      for(size_type i = 0; i < g.sizeN(); ++i) {
          for(size_type j = 0; j < g.sizeN(); ++j) {
              if(g._edges[i][j]) {
                  os << "(" << g._nodes[i] << "," << g._nodes[j] << ") ";
              }
          }
      }
  os << "}" << std::endl;
  os << "Matrice d'Adiacenza =  " << std::endl;
  for(size_type i = 0; i < g.sizeN(); ++i){
    for(size_type j = 0; j < g.sizeN(); ++j){
      if(g._edges[i][j])
        os << "1 ";
      else
        os << "0 ";
    }
    os << std::endl;
  }
  return os;
}



private:
  value_type* _nodes;   ///< Insieme dei Nodi
  bool** _edges;        ///< Matrice di Adiacenza.
  Eq _eq;               ///< Funtore di confronto tra tipi T.
  size_type _sizeN;     ///< Dimensione dell'insieme di Nodi
  size_type _sizeE;     ///< Dimensione dell'insieme degli archi

  /**
    @brief Funzione ausiliaria per allocare in memoria un grafo di determinata
    size.
    @parameter size Dimensione con cui inizializzare il grafo.
  */
  void init(size_type size){
    _nodes = new value_type[size];
    try{
      _edges = new bool*[size];
      for(size_type i = 0; i < size; ++i)
        _edges[i] = new bool[size];
      }catch(...) {
        reset();
        throw;
      }

    for(size_type i = 0; i < size; ++i)
      for(size_type j = 0; j < size; ++j)
        _edges[i][j] = false;

    _sizeE = 0;
    _sizeN = size;
  }

  /**
    @brief Funzione ausiliaria per deallocare la memoria e resettare
    il grafo in uno stato vuoto.
  */
  void reset(){
    delete[] _nodes;
    _nodes = nullptr;
    for(size_type i = 0; i < _sizeN; i++){
      delete [] _edges[i];
      _edges[i] = nullptr;
    }
    delete[] _edges;
    _edges = nullptr;
    _sizeN = 0;
    _sizeE = 0;
  }

  /**
    @brief Funzione ausiliaria per la ricerca posizione nodo

    Funzione che restituisce la posizione di un nodo passato come parametro.

    @param node Nodo di cui si vuol sapere la posizione.
    @return Posizione del nodo passato come parametro. (-1 se non è presente)
  */
  int find(value_type node){
    for(size_type i = 0; i < _sizeN; ++i)
      if(_eq(_nodes[i], node))
        return i;
    return -1;
  }

//Iterator
public:
  /**
    @class Forward const_iterator

    @brief Iteratore Const che permette di ciclare sull'insieme dei nodi.

 */
  class const_iterator {
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;


    const_iterator() : ptr(nullptr){}

    const_iterator(const const_iterator &other) :ptr(other.ptr){}

    const_iterator& operator=(const const_iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    ~const_iterator() {}

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return *ptr;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return ptr;
    }

    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(*this);
      ++ptr;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    const_iterator& operator++() {
      ++ptr;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return (ptr == other.ptr);
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return (ptr != other.ptr);
    }

  private:
    const T* ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class graph;

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const T* p) : ptr(p) {}

  }; // classe const_iterator

  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_nodes);
  }

  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(_nodes + _sizeN);
  }


};//graph

#endif//GRAPH_H
