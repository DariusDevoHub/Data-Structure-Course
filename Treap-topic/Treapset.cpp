#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

using namespace std;

/**
 * @brief Nodo para el TreapSet.
 * Almacena el elemento (llave) y su prioridad aleatoria.
 */
struct Node {
    int key;          // El elemento (llave del BST)
    int priority;   // Prioridad aleatoria (valor del MaxHeap)
    Node *left, *right;

    // Constructor: asigna la llave y una prioridad aleatoria
    Node(int k) {
        key = k;
        priority = rand(); // Prioridad aleatoria
        left = nullptr;
        right = nullptr;
    }
};

/**
 * @brief Implementa el concepto de Conjunto (Set) usando un Treap.
 * [cite: 73]
 * Esta versión simple usa 'int' como tipo de dato.
 */
class TreapSet {
private:
    Node* root;      // Raíz del árbol
    int nodeCount;   // Contador para la cardinalidad [cite: 76]

    // --- Funciones de Rotación ---
    // Se usan para mantener la propiedad de MaxHeap.

    /**
     * @brief Rotación a la derecha.
     * Sube al hijo izquierdo (x) y baja al nodo actual (y).
     *
     * y           x
     * / \         / \
     * x   T3  ->  T1  y
     * / \             / \
     * T1  T2          T2  T3
     */
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Realizar rotación
        x->right = y;
        y->left = T2;

        return x; // 'x' es la nueva raíz del subárbol
    }

    /**
     * @brief Rotación a la izquierda.
     * Sube al hijo derecho (y) y baja al nodo actual (x).
     *
     * x             y
     * / \           / \
     * T1  y    ->   x   T3
     * / \       / \
     * T2  T3    T1  T2
     */
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Realizar rotación
        y->left = x;
        x->right = T2;

        return y; // 'y' es la nueva raíz del subárbol
    }

    // --- Funciones Recursivas ---

    /**
     * @brief Ayudante recursivo para insertar.
     * 1. Inserta como un BST normal (por 'key').
     * 2. Si la propiedad de MaxHeap (por 'priority') se rompe, rota.
     */
    Node* insertRecursive(Node* node, int key) {
        // 1. Caso base: Encontramos dónde insertar
        if (node == nullptr) {
            nodeCount++; // Aumentamos el tamaño
            return new Node(key);
        }

        // 2. Bajar por el árbol (Propiedad BST)
        if (key < node->key) {
            node->left = insertRecursive(node->left, key);
            // 3. Chequear propiedad MaxHeap
            if (node->left != nullptr && node->left->priority > node->priority) {
                node = rotateRight(node); // Rotar para subir al hijo
            }
        } else if (key > node->key) {
            node->right = insertRecursive(node->right, key);
            // 3. Chequear propiedad MaxHeap
            if (node->right != nullptr && node->right->priority > node->priority) {
                node = rotateLeft(node); // Rotar para subir al hijo
            }
        }
        // Si key == node->key, no hacemos nada (es un conjunto).

        return node;
    }

    /**
     * @brief Ayudante recursivo para eliminar.
     * 1. Busca el nodo como un BST.
     * 2. Si tiene 2 hijos, lo rota "hacia abajo" (hacia el hijo de mayor prioridad).
     * 3. Cuando tiene 0 o 1 hijo, se elimina fácilmente.
     */
    Node* removeRecursive(Node* node, int key) {
        if (node == nullptr) {
            return nullptr; // No se encontró
        }

        // 1. Buscar el nodo
        if (key < node->key) {
            node->left = removeRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = removeRecursive(node->right, key);
        } else {
            // --- Nodo encontrado ---

            // Caso 1: 0 hijos (hoja) o 1 hijo derecho
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                nodeCount--; // Reducimos el tamaño
                return temp; // El hijo derecho (o nullptr) lo reemplaza
            }
            // Caso 2: 1 hijo izquierdo
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                nodeCount--; // Reducimos el tamaño
                return temp; // El hijo izquierdo lo reemplaza
            }
            
            // Caso 3: 2 hijos. Rotar hacia abajo.
            // Movemos el nodo hacia la rama del hijo con MAYOR prioridad.
            if (node->left->priority > node->right->priority) {
                node = rotateRight(node);
                // Continuamos eliminando recursivamente en la *nueva* rama derecha
                node->right = removeRecursive(node->right, key);
            } else {
                node = rotateLeft(node);
                // Continuamos eliminando recursivamente en la *nueva* rama izquierda
                node->left = removeRecursive(node->left, key);
            }
        }
        return node;
    }

    /**
     * @brief Ayudante recursivo para buscar (member).
     * Es una búsqueda de BST estándar.
     */
    bool memberRecursive(Node* node, int key) {
        if (node == nullptr) {
            return false;
        }
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            return memberRecursive(node->left, key);
        } else {
            return memberRecursive(node->right, key);
        }
    }

    // Libera toda la memoria del árbol
    void clearRecursive(Node* node) {
        if (node != nullptr) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }

public:
    // Constructor de un conjunto vacío [cite: 75]
    TreapSet() {
        root = nullptr;
        nodeCount = 0;
    }

    // Destructor [cite: 75]
    ~TreapSet() {
        clear();
    }

    // Vacía el árbol
    void clear() {
        clearRecursive(root);
        root = nullptr;
        nodeCount = 0;
    }

    // Retorna la cardinalidad del conjunto [cite: 76]
    int size() {
        return nodeCount;
    }

    // Adiciona un elemento al conjunto [cite: 76]
    void insert(int key) {
        // Solo insertamos si no es miembro, para no tener duplicados
        // y para que 'nodeCount' sea correcto.
        if (!member(key)) {
            root = insertRecursive(root, key);
        }
    }

    // Retira un elemento del conjunto [cite: 77]
    void remove(int key) {
        // Solo removemos si es miembro, para que 'nodeCount' sea correcto.
        if (member(key)) {
            root = removeRecursive(root, key);
        }
    }

    // Retorna si éste hace parte del conjunto [cite: 78]
    bool member(int key) {
        return memberRecursive(root, key);
    }
};

// --- Ejemplo de Uso ---
int main() {
    // Inicializar la semilla aleatoria (importante para las prioridades)
    srand(time(NULL));

    cout << "--- Ejemplo de TreapSet (Conjunto de ints) ---" << endl;
    TreapSet miSet;
    
    // insert
    miSet.insert(50);
    miSet.insert(30);
    miSet.insert(70);
    miSet.insert(20);
    
    cout << "Insertando 50, 30, 70, 20..." << endl;

    // member
    cout << "member(30)? " << (miSet.member(30) ? "Si" : "No") << endl;
    cout << "member(99)? " << (miSet.member(99) ? "Si" : "No") << endl;

    // size
    cout << "size(): " << miSet.size() << endl;

    // remove
    miSet.remove(30);
    cout << "remove(30)..." << endl;
    cout << "member(30)? " << (miSet.member(30) ? "Si" : "No") << endl;
    cout << "size() despues de remover: " << miSet.size() << endl;

    return 0;
}