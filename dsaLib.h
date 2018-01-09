/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>

using namespace std;


class DSAException {
    int     _error;
    string  _text;
public:

    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
    L1Item<T>   *_pHead;// The head pointer of linked list
    size_t      _size;// number of elements in this list
public:
    L1List() : _pHead(NULL), _size(0) {}
    ~L1List();

    void    clean();
    bool    isEmpty() {
        return _pHead == NULL;
    }
    size_t  getSize() {
        return _size;
    }

    T&      at(int i);
    T&      operator[](int i);

    bool    find(T& a, int& idx);
    T*      find(T& a);
    T*      findLast(T& a);
    int     insert(int i, T& a);
    int     remove(int i);
    int     removeAll(T& a);

    int     push_back(T& a);
    int     insertHead(T& a);

    int     removeHead();
    int     removeLast();

    void    reverse();

    void    traverse(void (*op)(T&)) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data);
            p = p->pNext;
        }
    }
    void    traverse(void (*op)(T&, void*), void* pParam) {
        L1Item<T>   *p = _pHead;
        while (p) {
            op(p->data, pParam);
            p = p->pNext;
        }
    }
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
    if (_pHead == NULL) {
        _pHead = new L1Item<T>(a);
    }
    else {
        L1Item<T>   *p = _pHead;
        while (p->pNext) p = p->pNext;
        p->pNext = new L1Item<T>(a);
    }

    _size++;
    return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
    L1Item<T>   *p = new L1Item<T>(a);
    p->pNext = _pHead;
    _pHead = p;
    _size++;
    return 0;
}

/// Remove the first item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeHead() {
    if(_pHead) {
        L1Item<T>* p = _pHead;
        _pHead = p->pNext;
        delete p;
        _size--;
        return 0;
    }
    return -1;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
    if(_pHead) {
        if(_pHead->pNext) {
            L1Item<T>* prev = _pHead;
            L1Item<T>* pcur = prev->pNext;
            while(pcur->pNext) {
                prev = pcur;
                pcur = pcur->pNext;
            }
            delete pcur;
            prev->pNext = NULL;
        }
        else {
            delete _pHead;
            _pHead = NULL;
        }
        _size--;
        return 0;
    }
    return -1;
}

/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
enum BFactor{EH = 0, LH = -1,RH = 1};
template <class T>
struct AVLNode {
    T           _data;
    AVLNode<T>   *_pLeft, *_pRight;
#ifdef AVL_USE_HEIGHT
    int         _height;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
#else
    int        _bFactor;
    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _bFactor(0) {}
#endif
};

template <class T>
class AVLTree {
    AVLNode<T> *_pRoot;
public:
    AVLTree() : _pRoot(NULL) {}
    ~AVLTree() { destroy(_pRoot); }

    //bool find(T& key, T* &ret) { return find(_pRoot, key, ret); }
	bool find(T& key) { return find(_pRoot, key); }
    bool insert(T& key) { return insert(_pRoot, key); }
    bool remove(T& key) { return remove(_pRoot, key); }
    void traverseNLR(void (*op)(T&)) { traverseNLR(_pRoot, op); }
    void traverseLNR(void (*op)(T&)) { traverseLNR(_pRoot, op); }
    void traverseLRN(void (*op)(T&)) { traverseLRN(_pRoot, op); }

protected:
    void destroy(AVLNode<T>* &pR);
	//bool find(AVLNode<T> *pR, T& key, T* &ret)
	bool find(AVLNode<T> *pR, T& key){
		if (_pRoot == NULL) return false;
		else if (key < pR->_data)
		{
			return search(key, pR->_pLeft);
		}
		else if (pR->data < key)
		{
			return search(key, pR->_pRight);
		}
	}
	bool insert(AVLNode<T>* &pR, T& a){
		if (pR == NULL){
			pR = new AVLNode<T>(a);
			return true;
		}
		if (a < pR->_data){
			if (insert(a, pR->_pLeft) == false) return false;
			balanceLeft(pR->_pLeft);
		}
		if (a > pR->_data){
			if (insert(a, pR->_pRight) == false) return false;
			balanceLeft(pR->_pRight);
		}
	}

	bool remove(AVLNode<T>* &pR, T& a){
		if (pR == NULL) return false;
		if (a < pR->_data){
			if (remove(a, pR->_pLeft) == false) return false;
			if (pR->_bFactor == LH){
				pR->_bFactor = EH; 
				return true;
			}
			if (pR->_bFactor == EH){
				pR->_bFactor = RH;
				return false;
			}
			return !balanceRight(a, pR);
		}
		if (a > pR->_data){
			if (remove(a, pR->_pRight) == false) return false;
			if (pR->_bFactor == RH){
				pR->_bFactor = EH;
				return true;
			}
			if (pR->_bFactor == EH){
				pR->_bFactor = LH;
				return false;
			}
			return !balanceLeft(a, pR);
		}
		if (pR->_pLeft == NULL && pR->_pRight == NULL){
			delete pR;
			pR = NULL;
			return true;
		}
		if (pR->_pLeft == NULL{
			AVLNode<T>* p = pR;
			pR = pR->_pRight;
			delete p;
			return true;
		}
		if (pR->_pRight == NULL{
			AVLNode<T>* p = pR;
			pR = pR->_pLeft;
			delete p;
			return true;
		}
		AVLNode<T> *p = pR->_pRight;
		while (p->_pLeft) p = p->_pLeft;
		pR->_data = p->_data;
		if (remove(p->_data, pR->_pRight) == false) return false;
		if (pR->_bFactor == RH){
			pR->_bFactor = EH;
			return true;
		}
		if (pR->_bFactor == EH){
			pR->_bFactor = LH;
			return false;
		}
	}
	void traverseNLR(AVLNode<T> *pR, void(*op)(T&)){
		if (pR == NULL) return;
		else{
			traverseNLR(pR->_pLeft);
			traverseNLR(pR->_pRight);
		}
	}
    void c;
	void traverseLRN(AVLNode<T> *pR, void(*op)(T&)){
		if (pR == NULL) return;
		else{
			traverseLRN(pR->_pLeft);
			traverseLRN(pR->_pRight);
			traverseLRN(pR->_pRoot);
		}
	}

	void rotLeft(AVLNode<T>* &pR){
		if (pR == NULL) return;
		if (pR->_pLeft == NULL) return;
		AVLNode<T> *p = pR->_pLeft;
		pR->_pLeft = pR->_pRight;
		p->_pRight = pR;
		pR = p;
	}
	void rotRight(AVLNode<T>* &pR){
		if (pR == NULL) return;
		if (pR->_pRight == NULL) return;
		AVLNode<T> *p = pR->_pRight;
		pR->_pRight = pR->_pLeft;
		p->_pLeft = pR;
		pR = p;
	}

	void rotLR(AVLNode<T>* &pR){
		rotLeft(pR->_pLeft);
		rotRight(pR);
	}
	void rotRL(AVLNode<T>* &pR){
		rotRight(pR->_pRight);
		rotLeft(pR);
	}

	bool balanceLeft(AVLNode<T>* &pR){
		if (pR->_bFactor == EH){
			pR->_bFactor = LH;
			return true;
		}
		if (pR->_bFactor == RH){
			pR->_bFactor = EH;
			return false;
		}
		if (pR->_pLeft->_bFactor == LH){
			rotRight(pR);
			pR->_bFactor = pR->_pRight->_bFactor = EH;
			return false;
		}
		rotLR(pR);
		if (pR->_bFactor == LH){
			pR->_bFactor = pR->_pLeft->_bFactor = EH;
			pR->_pLeft->_bFactor = RH;
			return false;
		}
		pR->_bFactor = RH;
		pR->_bFactor = pR->_pRight->_bFactor = EH;
		pR->_pLeft->_bFactor = LH;
		return false;
	}

	bool balanceRight(AVLNode<T>* &pR){
		if (pR->_bFactor == EH){
			pR->_bFactor = LH;
			return true;
		}
		if (pR->_bFactor == LH){
			pR->_bFactor = EH;
			return false;
		}
		if (pR->_pRight->_bFactor == RH){
			rotLeft(pR);
			pR->_bFactor = pR->_pLeft->_bFactor = EH;
			return false;
		}
		rotRL(pR);
		if (pR->_bFactor == RH){
			pR->_bFactor = pR->_pRight->_bFactor = EH;
			pR->_pRight->_bFactor = LH;
			return false;
		}
		pR->_bFactor = LH;
		pR->_bFactor = pR->_pLeft->_bFactor = EH;
		pR->_pRight->_bFactor = LH;
		return false;
	}
};

#endif //A02_DSALIB_H
