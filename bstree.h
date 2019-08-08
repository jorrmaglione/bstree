#pragma once

/*
* Arbol de busqueda binario con implementacion AVL
*/
namespace bstree {

	typedef struct Node {
		Node *leftSon = nullptr;
		Node *rightSon = nullptr;
		Node *father = nullptr;
#ifdef _AVLT_
		int eF = 0;
#endif
		int data = 0;
	} Node, *pNode;

	typedef struct Tree {
		Node *root = nullptr;
	} Tree, *pTree;

	void addNode(pTree, int);
	bool findNode(pTree, int);
	void printPostOrden(pTree);
	void printPreOrden(pTree);
	void printInOrden(pTree);
	bool deleteNode(pTree, int);
	int height(pTree);
	int levels(pTree);

#ifdef _AVLT_
	void addNodeAVL(pTree, int);
	void deleteNodeAVL(pTree, int);
	void printPostOrdenAVL(pTree);
#endif
}
