#include <iostream>
#include <cmath>
#include <cstring>
using namespace std ; 

struct Node {
    int key ; 
    string value ; 
    int height = 0 ;
    Node* left = nullptr ; 
    Node* right = nullptr ; 
} ; 


class AVLTree {

Node* ROOT = nullptr ; 

int getHeight( Node* node ) {
    if( node == nullptr ) {
        return 0;
    }
    return node -> height ; 
}

int getBalanceFactor( Node* node ) {
    if( node == nullptr ) {
        return 0 ; 
    }
    return getHeight( node -> left ) - getHeight( node -> right ) ; 
}

void updateHeight( Node* node ) {
    node -> height = 1 + max( getHeight( node -> left ) , getHeight( node -> right ) )  ; 
}

Node* rotateLL( Node* node ) {
    Node* newRoot = node -> left ; 
    newRoot -> right = node;
    return newRoot ;  
}

Node* rotateRR( Node* node ) {
    Node* newRoot = node -> right ; 
    newRoot -> left = node ; 
    return newRoot ; 
}

Node* rotateRL( Node* node ) {
    node -> right = rotateRR( node -> right ) ; 
    return rotateLL( node ) ; 
}

Node* rotateLR( Node* node ) {
    node -> left = rotateLL( node -> left ) ; 
    return rotateRR( node ) ; 
}

Node* balance( Node* node ) {
    if( getBalanceFactor( node ) == 2 ) {
        // Left subtree has a greater height
        // Possible rotations: LL, LR
        if( getBalanceFactor( node -> left ) > 0 ) {
            // For left child, left subtree has greater height. Hence, use LL rotation
            node = rotateLL( node ) ;
        }
        else {
            // For left child, right subtree has greater height. Hence, use LR rotation
            node = rotateLR( node ) ;
        }
    }
    else if( getBalanceFactor( node ) == -2 ) {
        // Right subtree has a greater height
        // Possible rotations: RR, RL
        if( getBalanceFactor( node -> right ) < 0 ) {
            // For left child, left subtree has greater height. Hence, use RR rotation
            node = rotateRR( node ) ;
        }
        else {
            // For left child, right subtree has greater height. Hence, use RL rotation
            node = rotateRL( node ) ; 
        }
    }
    return node ; 
}

void inorderRecursive( Node* curr ) {
    if( curr == nullptr ) {
        return ; 
    }
    inorderRecursive( curr -> left ) ; 
    cout << curr -> key << " " ; 
    inorderRecursive( curr -> right ) ; 
}

Node* insertSubTree( Node* curr , int key , string value ) {
    if( curr == nullptr ) {
        Node* newNode = new Node() ; 
        newNode -> key = key ; 
        newNode -> value = value ; 
        return newNode ; 
    }
    if( curr -> key > key ) {
        curr -> left = insertSubTree( curr -> left , key , value ) ; 
    }
    else if( curr -> key < key ) {
        curr -> right = insertSubTree( curr -> right , key , value ) ; 
    }
    else {
        curr -> value = value ; 
        return curr ;
    }
    return balance( curr ) ; 
}

public:

void insert( int key , string value ) {
    ROOT = insertSubTree( ROOT , key , value ) ;
}

void inorder() {
    inorderRecursive( ROOT ) ; 
    cout << "\n" ; 
}

Node* search( int key ) {
    Node* curr = ROOT ; 
    while( curr != nullptr ) {
        if( curr -> key > key ) {
            curr = curr -> left ; 
        }
        else if( curr -> key < key ) {
            curr = curr -> right ; 
        }
        else {
            return curr ; 
        }
    }
}

} ;


int main() {
    AVLTree tree ; 
    while( true ) {
        int option ; 
        cout << "Enter option: " << "\n" ; 
        cout << "1. Insert key, value" << "\n" ; 
        cout << "2. Inorder traversal" << "\n" ;
        cout << "3. Search" << "\n" ;
        cout << "4. Exit" << "\n" ;
        cin >> option ; 
        if( option == 1 ) {
            int key ; 
            cout << "Enter key: " << "\n" ; cin >> key ; 
            string value ; 
            cout << "Enter value: " << "\n" ; cin >> value ; 
            tree.insert( key , value ) ; 
        }
        else if( option == 2 ) {
            cout << "Inorder traversal -> " ;
            tree.inorder() ; 
        }
        else if( option == 3 ) {
            int key ; 
            cout << "Enter key to search: " << "\n" ; cin >> key ; 
            Node* result = tree.search( key ) ; 
            if( result == nullptr ) {
                cout << "Key not found" << "\n" ; 
            }
            else {
                cout << "Key found, value is " << result -> value << "\n" ; 
            }
        }
        else if( option == 4 ){
            break ; 
        }
    }

    return 0 ; 
}