#include <iostream>
#include <string>  // Para usar std::string
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()

using namespace std;

// Para esta versión simple, definimos los tipos de llave y valor
// en lugar de usar templates. [cite: 80]
typedef int Key;
typedef string Value;

/**
 * @brief Nodo para el TreapMap.
 * Almacena llave, valor asociado y prioridad.
 */
struct Node {
    Key key;
    Value value;
    int priority;
    Node *left, *right;

    // Constructor: asigna llave, valor y prioridad aleatoria
    Node(Key k, Value v) {
        key = k;
        value = v;
        priority = rand();
        left = nullptr;
        right = nullptr;
    }
};

/**
 * @brief Implementa el concepto de Asociación (Mapa) usando un Treap.
 * 
 */
class TreapMap {
private:
    Node* root; // Raíz del árbol

    // --- Funciones de Rotación ---
    // Idénticas a las de TreapSet.

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }

    // --- Funciones Recursivas ---

    /**
     * @brief Ayudante recursivo para insertar (llave, valor).
     * Si la llave ya existe, actualiza el valor.
     */
    Node* insertRecursive(Node* node, Key key, Value value) {
        // 1. Caso base: Insertar nuevo nodo
        if (node == nullptr) {
            return new Node(key, value);
        }

        // 2. Bajar por el árbol (Propiedad BST)
        if (key < node->key) {
            node->left = insertRecursive(node->left, key, value);
            // 3. Chequear propiedad MaxHeap
            if (node->left != nullptr && node->left->priority > node->priority) {
                node = rotateRight(node);
            }
        } else if (key > node->key) {
            node->right = insertRecursive(node->right, key, value);
            // 3. Chequear propiedad MaxHeap
            if (node->right != nullptr && node->right->priority > node->priority) {
                node = rotateLeft(node);
            }
        } else {
            // Llave encontrada: actualizar el valor
            node->value = value;
        }
        
        return node;
    }

    /**
     * @brief Ayudante recursivo para eliminar (por llave).
     * Lógica idéntica a TreapSet.
     */
    Node* removeRecursive(Node* node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            node->left = removeRecursive(node->left, key);
        } else if (key > node->key) {
            node->right = removeRecursive(node->right, key);
        } else {
            // --- Nodo encontrado ---
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            
            // 2 hijos: rotar
            if (node->left->priority > node->right->priority) {
                node = rotateRight(node);
                node->right = removeRecursive(node->right, key);
            } else {
                node = rotateLeft(node);
                node->left = removeRecursive(node->left, key);
            }
        }
        return node;
    }

    /**
     * @brief Ayudante recursivo para buscar un nodo por su llave.
     * Retorna el nodo (o nullptr) para que find() pueda extraer el valor.
     */
    Node* findNode(Node* node, Key key) {
        if (node == nullptr) {
            return nullptr;
        }
        if (key == node->key) {
            return node;
        }
        if (key < node->key) {
            return findNode(node->left, key);
        } else {
            return findNode(node->right, key);
        }
    }

    // Libera toda la memoria
    void clearRecursive(Node* node) {
        if (node != nullptr) {
            clearRecursive(node->left);
            clearRecursive(node->right);
            delete node;
        }
    }

public:
    // Constructor de un mapa vacío [cite: 83]
    TreapMap() {
        root = nullptr;
    }

    // Destructor [cite: 83]
    ~TreapMap() {
        clear();
    }

    // Vacía el mapa
    void clear() {
        clearRecursive(root);
        root = nullptr;
    }

    // Inserta una llave y su respectivo dato [cite: 85]
    void insert(Key key, Value value) {
        root = insertRecursive(root, key, value);
    }

    // Elimina de la estructura la llave y su elemento asociado [cite: 86]
    void remove(Key key) {
        // No necesitamos chequear si existe; si no existe,
        // la función recursiva simplemente no hará nada.
        root = removeRecursive(root, key);
    }

    /**
     * @brief Recibe una llave y retorna el valor asociado
     * o un valor por defecto si no se encuentra. [cite: 87]
     */
    Value find(Key key, Value defaultValue) {
        Node* result = findNode(root, key);
        if (result == nullptr) {
            return defaultValue;
        }
        return result->value;
    }

    // Función extra (muy útil) para saber si una llave existe
    bool contains(Key key) {
        return findNode(root, key) != nullptr;
    }
};

// --- Ejemplo de Uso ---
int main() {
    // Inicializar la semilla aleatoria
    srand(time(NULL));

    cout << "--- Ejemplo de TreapMap (int -> string) ---" << endl;
    TreapMap miMapa;

    // insert
    miMapa.insert(50, "Alejandro");
    miMapa.insert(30, "Beatriz");
    miMapa.insert(70, "Carlos");
    
    cout << "Insertando (50, A), (30, B), (70, C)..." << endl;

    // find
    cout << "find(30, 'N/A'): " << miMapa.find(30, "N/A") << endl;
    cout << "find(99, 'N/A'): " << miMapa.find(99, "N/A") << endl;

    // insert (actualizar)
    miMapa.insert(50, "Ana");
    cout << "Actualizando valor de 50 a 'Ana'..." << endl;
    cout << "find(50, 'N/A'): " << miMapa.find(50, "N/A") << endl;

    // remove
    miMapa.remove(70);
    cout << "remove(70)..." << endl;
    cout << "contains(70)? " << (miMapa.contains(70) ? "Si" : "No") << endl;

    return 0;
}