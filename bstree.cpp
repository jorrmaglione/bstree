#include "bstree.h"
#include <iostream>

namespace bstree {

#define NL "\n"

	static void addNode(pNode, pNode);
	static pNode printPostOrden(pNode);
	static void printPreOrden(pNode);
	static void printInOrden(pNode);
	static bool findNode(pNode, int);
	static pNode getNode(pNode, int);
	static pNode findMax(pNode);
	static pNode findMin(pNode);
	static int height(pNode);

#ifdef _AVLT_
	static int updateEf(pNode);
	static int checkEf(pNode);
	static void simpleRotationLeft(pNode);
	static void doubleRotationLeft(pNode);
	static void simpleRotationRight(pNode);
	static void doubleRotationRight(pNode);
	static pNode printPostOrdenAVL(pNode);
#endif // !_AVLT_

	/*
	 * Agrega un dato al arbol
	 */
	void addNode(pTree tree, int data) {
		pNode nNode = new Node; //Creo un nodo nuevo y lo inicializo
		nNode->data = data;
		if (tree->root == nullptr) tree->root = nNode;
		else addNode(tree->root, nNode);
	}

	/*
	 * Agregar un dato a un subarbol
	 */
	static void addNode(pNode cNode, pNode nNode) {
		if (nNode->data < cNode->data) { //Si el dato del nuevo nodo es menor al del nodo actual
			if (cNode->leftSon != nullptr) addNode(cNode->leftSon, nNode); //y si tiene hijo izquierdo le paso el nodo nuevo
			else { //sino lo coloco en el hijo izquierdo
				cNode->leftSon = nNode;
				nNode->father = cNode;
			}
		} else if (nNode->data > cNode->data) { //en caso contrario
			if (cNode->rightSon != nullptr) addNode(cNode->rightSon, nNode);
			else {
				cNode->rightSon = nNode;
				nNode->father = cNode;
			}
		}
	}

	/*
	 * Busca el dato en el arbol
	 */
	bool findNode(pTree tree, int data) {
		if (tree->root != nullptr) return findNode(tree->root, data); //si el arbol no esta vacio busco el nodo
		return false;
	}

	/*
	 * Busca el dato en el nodo
	 */
	bool findNode(pNode cNode, int data) {
		if (data == cNode->data) return true; //si el dato a buscar es igual al del nodo actual
		else if (data > cNode->data && cNode->rightSon != nullptr) return findNode(cNode->rightSon, data); //si es mayor
		else if (data < cNode->data && cNode->leftSon != nullptr) return findNode(cNode->leftSon, data); //si es menor
		return false;
	}

	/*
	 * Impresion PreOrden de un arbol
	 */
	void printPreOrden(pTree tree) {
		if (tree->root != nullptr) {
			printPreOrden(tree->root);
			std::cout << NL;
		} else std::cout << "Empty tree" << NL;
	}

	/*
	 * Impresion PreOrden de un subarbol
	 */
	static void printPreOrden(pNode cNode) {
		std::cout << cNode->data << " ";
		if (cNode->leftSon != nullptr) printPreOrden(cNode->leftSon);
		if (cNode->rightSon != nullptr) printPreOrden(cNode->rightSon);
	}

	/*
	 * Impresion de un arbol
	 */
	void printInOrden(pTree tree) {
		if (tree->root != nullptr) {
			printInOrden(tree->root);
			std::cout << NL;
		} else std::cout << "Empty tree" << NL;
	}

	/*
	 * Impresion de un subarbol
	 */
	static void printInOrden(pNode cNode) {
		if (cNode->leftSon != nullptr) printInOrden(cNode->leftSon);
		std::cout << cNode->data << " ";
		if (cNode->rightSon != nullptr) printInOrden(cNode->rightSon);
	}

	/*
	 * Impresion de un arbol
	 */
	void printPostOrden(pTree tree) {
		if (tree->root != nullptr) {
			printPostOrden(tree->root);
			std::cout << tree->root->data << NL;
		} else std::cout << "Empty tree" << NL;
	}

	/*
	 * Impresion de un subarbol
	 */
	static pNode printPostOrden(pNode cNode) {
		if (cNode->leftSon != nullptr) {
			pNode aux = printPostOrden(cNode->leftSon);
			std::cout << aux->data << " ";
		}
		if (cNode->rightSon != nullptr) {
			pNode aux = printPostOrden(cNode->rightSon);
			std::cout << aux->data << " ";
		}
		return cNode;
	}

	/*
	 *
	 */
	bool deleteNode(pTree tree, int data) {
		if (tree->root != nullptr) { //Si el arbol no esta vacio
			pNode cNode = getNode(tree->root, data); //Busco el nodo a eliminar
			if (cNode != nullptr) { //Si existe el nodo a eliminar
				if ((cNode->leftSon != nullptr && (cNode->rightSon != nullptr || cNode->rightSon == nullptr))) { //y tiene 2 hijos o solo el izquierdo
					pNode maxNode = findMax(cNode->leftSon); //Busco el maximo nodo del hijo izquierdo
					cNode->data = maxNode->data; //intercambio los valores
					if (maxNode->father != cNode) maxNode->father->rightSon = maxNode->leftSon; //si el padre del nodo maximo es distinto al nodo a eliminar
					else cNode->leftSon = maxNode->leftSon; //si son iguales
					delete maxNode; //elimino el nodo maximo
				} else if (cNode->leftSon == nullptr && cNode->rightSon != nullptr) { //y tiene solo el hijo derecho
					pNode minNode = findMin(cNode->rightSon); //busco el minimo nodo del hijo derecho
					cNode->data = minNode->data; //intercambio los valores
					if (minNode->father != cNode) minNode->father->leftSon = minNode->rightSon; //si el padre del nodo minimo es distinto al nodo a eliminar
					else cNode->rightSon = minNode->rightSon; //si son iguales
					delete minNode; //elimino el nodo minimo
				} else { //y si no tienen ningun hijo
					if (cNode->father != nullptr) { //si el nodo a eliminar no es la raiz
						if (cNode->father->leftSon != nullptr && cNode->father->leftSon->data == data) cNode->father->leftSon = nullptr; //es hijo izquierdo?
						else cNode->father->rightSon = nullptr; //es hijo derecho?
						delete cNode; //elimino el nodo
					} else { //si el nodo a eliminar es la raiz
						delete tree->root;
						tree->root = nullptr;
					}
				}
				return true;
			}
		}
		return false;
	}

	/*
	 * Funcion que encuentra el nodo a eliminar
	 */
	static pNode getNode(pNode cNode, int data) {
		if (cNode->data == data) return cNode; //Devuelvo el nodo buscado
		else if (data < cNode->data && cNode->leftSon != nullptr) return getNode(cNode->leftSon, data);
		else if (data > cNode->data && cNode->rightSon != nullptr) return getNode(cNode->rightSon, data);
		return nullptr; //Devuelve NULL en caso de no encontrar el nodo buscado
	}

	/*
	 * Buscar el padre del nodo de valor maximo en un arbol
	 * En un arbol binario los nodos mayores siempre se encuentran
	 * a la derecha
	 */
	static pNode findMax(pNode cNode) {
		if (cNode->rightSon != nullptr) return findMax(cNode->rightSon);
		return cNode;
	}

	/*
	 * Buscar el padre del nodo de valor minimo en un arbol
	 * En un arbol binario los nodos menores siempre se encuentran
	 * a la izquierda
	 */
	static pNode findMin(pNode cNode) {
		if (cNode->leftSon != nullptr) return findMin(cNode->leftSon);
		return cNode;
	}

	/**
	 * Calcular la altura de un arbol
	 */
	int height(pTree tree) {
		return height(tree->root);
	}

	/*
	 * Calcular la altura de un nodo
	 */
	static int height(pNode cNode) {
		if (cNode == nullptr) return -1;
		int hLeft = 1 + height(cNode->leftSon); //Chequeo la altura de mi hijo izquierdo
		int hRight = 1 + height(cNode->rightSon); //Chequeo la altura de mi hijo derecho
		return (hLeft > hRight) ? hLeft : hRight; //Devuelvo la altura correspondiente
	}

	/*
	 * Niveles de un arbol
	 */
	int levels(pTree tree) {
		//Calculo la altura en root y le sumo 1
		return height(tree->root) + 1;
	}

#ifdef _AVLT_
	/*
	 * Agregar un nodo AVL
	 */
	void addNodeAVL(pTree tree, int data) {
		addNode(tree, data); //Agrego un nodo como si fuera un arbol de busqueda binario
		checkEf(tree->root); //Realizo la comprobacion de los factores de equilibrio
	}

	/*
	 * Eliminar un nodo AVL
	 */
	void deleteNodeAVL(pTree a, int data) {
		deleteNode(a, data); //Elimino un nodo como un arbol de busqueda binario
		checkEf(a->root); //Realizo la comprobacion de los factores de equilibrio
	}

	/**
	 * Actualizar los factores de equilibrio del arbol
	 * ver funcion height();
	 */
	static int updateEf(pNode cNode) {
		if (cNode == nullptr) return -1; //Si estoy en un nodo nulo devuelvo -1
		int hLeft = 1 + updateEf(cNode->leftSon); //factor de equilibrio del hijo izquierdo
		int hRight = 1 + updateEf(cNode->rightSon); //factor de equilibrio del hijo derecho
		cNode->eF = hRight - hLeft; //Calcular factor de equilibrio del nodo actual
		return (hLeft > hRight) ? hLeft : hRight; //Devolver altura maxima del nodo
	}

	/*
	 * Chequear factores de equilibrio y realizar las rotaciones
	 * en caso que sea necesario
	 */
	static int checkEf(pNode cNode) {
		if (cNode == nullptr) return -1; //Si estoy en un nodo nulo devuelvo -1
		int hLeft = 1 + checkEf(cNode->leftSon); //factor de equilibrio del hijo izquierdo
		int hRight = 1 + checkEf(cNode->rightSon); //factor de equilibrio del hijo derecho
		cNode->eF = hRight - hLeft; //Calcular factor de equilibrio del nodo actual
		if (cNode->eF == 2) { //Si el factor de equilibrio es +2
			if (cNode->rightSon->eF > 0) simpleRotationLeft(cNode);
			else doubleRotationLeft(cNode);
			return updateEf(cNode);
		} else if (cNode->eF == -2) { //Si el factor de equilibrio es -2
			if (cNode->leftSon->eF > 0) doubleRotationRight(cNode);
			else simpleRotationRight(cNode);
			return updateEf(cNode);
		}
		return (hLeft > hRight) ? hLeft : hRight; //devuelvo la mayor altura del nodo
	}

	/*
	 * Impresion de un arbol AVL
	 */
	void printPostOrdenAVL(pTree tree) {
		if (tree->root != nullptr) {
			printPostOrdenAVL(tree->root);
			std::cout << tree->root->data << "(" << tree->root->eF << ")" << NL;
		} else {
			std::cout << "Empty tree" << NL;
		}
	}

	/*
	 * Impresion de un subarbol AVL
	 */
	pNode printPostOrdenAVL(pNode cNode) {
		if (cNode->leftSon != nullptr) {
			pNode aux = printPostOrdenAVL(cNode->leftSon);
			std::cout << aux->data << "(" << aux->eF << ")" << " ";
		}
		if (cNode->rightSon != nullptr) {
			pNode aux = printPostOrdenAVL(cNode->rightSon);
			std::cout << aux->data << "(" << aux->eF << ")" << " ";
		}
		return cNode;
	}

	/*
	 * Rotacion simple a la izquierda
	 */
	static void simpleRotationRight(pNode cNode) {
		pNode nNode = new Node;
		pNode aux = cNode->leftSon;
		nNode->data = cNode->data;
		nNode->father = cNode;
		nNode->rightSon = cNode->rightSon;
		if (nNode->rightSon != nullptr) nNode->rightSon->father = nNode;
		nNode->leftSon = aux->rightSon;
		if (nNode->leftSon != nullptr) nNode->leftSon->father = nNode;
		cNode->rightSon = nNode;
		cNode->data = aux->data;
		cNode->leftSon = aux->leftSon;
		cNode->leftSon->father = aux->father;
		delete aux;
	}

	/*
	 * Rotacion doble a la izquierda
	 */
	static void doubleRotationRight(pNode cNode) {
		pNode nNode = new Node;
		pNode aux = cNode->leftSon->rightSon;
		nNode->data = cNode->data;
		nNode->father = cNode;
		nNode->rightSon = cNode->rightSon;
		if (nNode->rightSon != nullptr) nNode->rightSon->father = nNode;
		nNode->leftSon = aux->rightSon;
		if (nNode->leftSon != nullptr) nNode->leftSon->father = nNode;
		cNode->rightSon = nNode;
		cNode->data = aux->data;
		cNode->leftSon->rightSon = aux->leftSon;
		if (aux->leftSon != nullptr) aux->leftSon->father = aux->father;
		delete aux;
	}

	/**
	 * Rotacion simple a la derecha
	 */
	static void simpleRotationLeft(pNode cNode) {
		pNode nNode = new Node;
		pNode aux = cNode->rightSon;
		nNode->data = cNode->data;
		nNode->father = cNode;
		nNode->leftSon = cNode->leftSon;
		if (nNode->leftSon != nullptr) nNode->leftSon->father = nNode;
		nNode->rightSon = aux->leftSon;
		if (nNode->rightSon != nullptr) nNode->rightSon->father = nNode;
		cNode->data = aux->data;
		cNode->leftSon = nNode;
		cNode->rightSon = aux->rightSon;
		cNode->rightSon->father = cNode;
		delete aux;
	}

	/*
	 * Rotacion doble a la derecha
	 */
	static void doubleRotationLeft(pNode cNode) {
		pNode nNode = new Node;
		pNode aux = cNode->rightSon->leftSon;
		nNode->data = cNode->data;
		nNode->leftSon = cNode->leftSon;
		if (nNode->leftSon != nullptr) nNode->leftSon->father = nNode;
		nNode->rightSon = aux->leftSon;
		if (nNode->rightSon != nullptr) nNode->rightSon->father = nNode;
		cNode->leftSon = nNode;
		cNode->data = aux->data;
		cNode->rightSon->leftSon = aux->rightSon;
		if (aux->rightSon != nullptr) aux->rightSon->father = aux->father;
		delete aux;
	}
#endif // _AVLT_
}