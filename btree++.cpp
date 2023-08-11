#include <iostream>
using namespace std;

class hash
{
    hash()
    {
        
    }
};

class reg
{
public:
    int id;
    hash *miembros;
    int bloque;

    reg()
    {
        id = bloque = 0;
    }

    reg(int i)
    {
        id = i;
        bloque = 0;
    }

    reg(int i, int b)
    {
        id = i;
        bloque = b;
    }

    void operator=(const reg &r)
    {
        id = r.id;
        bloque = r.bloque;
    }
};

class nodo
{
public:
    bool esHoja;
    int ocupado;
    reg *clave;
    nodo **puntero;

    nodo(int n)
    {
        clave = new reg[n];
        puntero = new nodo*[n+1];
    }
};

class bptree
{
    nodo *raiz;
    int n;
public:
    bptree(int c)
    {
        raiz = NULL;
        n = c;
    }

    int buscar(int k)
    {
        if (raiz == NULL) return -1;
        else
        {
            nodo* temp = raiz;
            int pos;
            while (temp->esHoja == false)
            {
                for (pos = 0; pos < temp->ocupado; pos++)
                {
                    if (k < temp->clave[pos].id)
                    {
                        temp = temp->puntero[pos];
                        break;
                    }

                    if (pos == temp->ocupado - 1)
                    {
                        temp = temp->puntero[pos+1];
                        break;
                    }
                }
            }

            for (pos = 0; pos < temp->ocupado; pos++)
            {
                if (temp->clave[pos].id == k)
                {
                    return temp->clave[pos].bloque;
                }
            }
            return -1;
        }
    }

    void insertar(int k, int b)
    {
        if (!raiz)
        {
            raiz = new nodo(n);
            raiz->clave[0].id = k;
            raiz->clave[0].bloque = b;
            raiz->esHoja = true;
            raiz->ocupado = 1;
        }
        else
        {
            nodo *temp = raiz;
            nodo *tempPadre;

            while (temp->esHoja == false)
            {
                tempPadre = temp;

                for (int i = 0; i < temp->ocupado; i++)
                {
                    if (k < temp->clave[i].id)
                    {
                        temp = temp->puntero[i];
                        break;
                    }

                    if (i == temp->ocupado - 1)
                    {
                        temp = temp->puntero[i+1];
                        break;
                    }
                }
            }

            if (temp->ocupado < n)
            {
                int pos = 0;

                while (k > temp->clave[pos].id)
                {
                    pos++;
                    if (pos == temp->ocupado) break;
                }

                for (int i = temp->ocupado; i > pos; i--)
                {
                    temp->clave[i].id = temp->clave[i-1].id;
                }

                //Ubicamos el numero en su posición
                temp->clave[pos].id = k;
                temp->clave[pos].bloque = b;
                //Aumentamos el número de claves ocupadas del nodo
                temp->ocupado++;
                //Redirigimos el puntero que apuntaba al siguiente nodo
                temp->puntero[temp->ocupado] = temp->puntero[temp->ocupado-1];
                //Vaciamos el puntero que ya no se usará
                temp->puntero[temp->ocupado-1] = NULL;
            }
            else
            {
                nodo *hojaNueva = new nodo(n);
                //Cremos un nodo temporal donde ordenaremos los valores
                reg nodoTemp[n+1];

                for (int i = 0; i < n; i++)
                {
                    nodoTemp[i] = temp->clave[i];
                }

                int pos1 = 0, pos2;
                while (k > nodoTemp[pos1].id && pos1 < n) pos1++;

                for (pos2 = n+1; pos2 > pos1; pos2--)
                {
                    nodoTemp[pos2] = nodoTemp[pos2-1];
                }
                nodoTemp[pos1].id = k;
                nodoTemp[pos1].bloque = b;
                hojaNueva->esHoja = true;
                
                //Se reapartirán las claves entre ambas hojas
                temp->ocupado = (n+1)/2;
                hojaNueva->ocupado = n+1 - (n+1)/2;

                //Redirigimos los punteros
                temp->puntero[temp->ocupado] = hojaNueva;
                hojaNueva->puntero[hojaNueva->ocupado] = temp->puntero[n];
                temp->puntero[n] = NULL;

                //Llenamos el nodo antiguo con los nuevos valores
                for (pos1 = 0; pos1 < temp->ocupado; pos1++)
                {
                    temp->clave[pos1] = nodoTemp[pos1];
                }

                //Llneamos el nodo nuevo con los nuevos valores
                for (pos2 = 0; pos2 < hojaNueva->ocupado; pos1++,pos2++)
                {
                    hojaNueva->clave[pos2] = nodoTemp[pos1];
                }

                //Es necesario cambiar al padre cuando se agrega un nodo nuevo
                //Si fuera raiz o no
                if (temp == raiz)
                {
                    nodo *raizNueva = new nodo(n);
                    //Se le agrega una clave
                    raizNueva->clave[0].id = hojaNueva->clave[0].id;

                    //Se direcciona sus hojas
                    raizNueva->puntero[0] = temp;
                    raizNueva->puntero[1] = hojaNueva;

                    //La convertimos en la nueva raíz
                    raizNueva->esHoja = false;
                    raizNueva->ocupado = 1;
                    raiz = raizNueva;
                }
                else
                {
                    insertarRama(hojaNueva->clave[0].id, tempPadre, hojaNueva);
                }
            }
        }
        //std::cout << "Insertado en hoja!\n";
    }

    void insertarRama(int k, nodo* temp, nodo* hoja)
    {
        //Si el nodo padre no está lleno
        if (temp->ocupado < n)
        {
            int pos1 = 0, pos2;
            //Encontramos la posicion donde debe estar la nueva clave
            while (k > temp->clave[pos1].id && pos1 < temp->ocupado) pos1++;
            
            //Reubicamos las claves que se encuentran delante de la nueva clave
            //En pocas palabras le hacemos espacio
            for (pos2 = temp->ocupado; pos2 > pos1; pos2--)
            {
                temp->clave[pos2] = temp->clave[pos2-1];
            }
            //Lo mismo con los punteros
            for (pos2 = temp->ocupado + 1; pos2 > pos1+1; pos2--)
            {
                temp->puntero[pos2] = temp->puntero[pos2-1];
            }

            //Ubicamos la nueva clave
            temp->clave[pos1].id = k;
            temp->ocupado++;
            temp->puntero[pos1+1] = hoja;
        }
        else
        {
            //Si el nodo padre está lleno habrá que crear una nueva
            //Y buscar al abuelo
            nodo *ramaNueva = new nodo(n);
            reg ramaTemp[n+1];
            nodo *ramaPtrTemp[n+2];
            
            //Copiamos los datos a nuestro temporal
            for (int i = 0; i < n; i++)
            {
                ramaTemp[i] = temp->clave[i];
                ramaPtrTemp[i] = temp->puntero[i];
            }
            ramaPtrTemp[n] = temp->puntero[n];

            int pos1 = 0, pos2;
            while (k > ramaTemp[pos1].id && pos1 < n) pos1++;
            for (pos2 = n + 1; pos2 > pos1; pos2--)
            {
                ramaTemp[pos2] = ramaTemp[pos2-1];
            }
            for (pos2 = n + 2; pos2 > pos1 + 1; pos2--)
            {
                ramaPtrTemp[pos2] = ramaPtrTemp[pos2-1];
            }
            ramaTemp[pos1].id = k;
            ramaPtrTemp[pos1+1] = hoja;

            ramaNueva->esHoja = false;
            temp->ocupado = (n+1)/2;
            ramaNueva->ocupado = n - (n+1)/2;

            for (pos1 = 0, pos2 = temp->ocupado + 1; pos1 < ramaNueva->ocupado; pos1++,pos2++)
            {
                ramaNueva->clave[pos1] = ramaTemp[pos2];
                ramaNueva->puntero[pos1] = ramaPtrTemp[pos2];
            }
            ramaNueva->puntero[++pos1] = ramaPtrTemp[++pos2];

            if (temp == raiz)
            {
                nodo *raizNueva = new nodo(n);
                raizNueva->clave[0].id = temp->clave[temp->ocupado].id;
                raizNueva->puntero[0] = temp;
                raizNueva->puntero[1] = ramaNueva;
                raizNueva->esHoja = false;
                raizNueva->ocupado = 1;
                raiz = raizNueva;
            }
            else
            {
                insertarRama(temp->clave[temp->ocupado].id, encontrarPadre(raiz,temp), ramaNueva);
            }
        }
        //std::cout << "Insertado en rama!\n";
    }

    nodo* encontrarPadre(nodo* temp, nodo* hijo)
    {
        if (temp->esHoja || (temp->puntero[0])->esHoja) return NULL;
        nodo* padre = NULL;

        for (int i = 0; i < temp->ocupado+1; i++)
        {
            if (temp->puntero[i] == hijo)
            {
                padre = temp;
                return padre;
            }
            else
            {
                padre = encontrarPadre(temp->puntero[i], hijo);
                
                if (padre != NULL) return padre;
            }
        }

        /*
        while (padre->esHoja == false)
        {
            for (int i = 0; i < tempHijo->ocupado; i++)
            {
                    if (hijo->clave[0].id < tempHijo->clave[i].id)
                    {
                        temp = temp->puntero[i];
                        break;
                    }

                    if (i == temp->ocupado - 1)
                    {
                        temp = temp->puntero[i+1];
                        break;
                    }
                }
            }
        */
        return padre;
    }
};

int main()
{
    bptree e(3);

	e.insertar(6,1);
	e.insertar(16,2);
	e.insertar(26,3);
	e.insertar(36,4);
	e.insertar(46,5);
    
    std::cout << e.buscar(6) << endl;
    std::cout << e.buscar(16) << endl;
    std::cout << e.buscar(26) << endl;
    std::cout << e.buscar(36) << endl;
    std::cout << e.buscar(46) << endl;
	return 0;
}