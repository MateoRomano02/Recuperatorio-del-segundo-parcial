#include <iostream>
using namespace std;

/*
1)
Primero utilizaremos una lista con un struct para cada persona
para recuperar los datos del archivo "Inscriptos.dat".    
La estructura pensada para procesar información sería un 
una lista. Y en la función mostrarListado solo
tomamos la cantidad de cupos que haya, no todas los personas.
Utilizamos este tipo de estructuras ya que nos permite tener un
manejo dinámico de los datos, es decir, al principio utilizamos
una lista ya que no sabemos la cantidad de registros que tiene el
archivo "Inscriptos.dat", con lo que descartamos cualquier 
estructura de datos con longitad fija(como en los arrays por 
ejemplo). Y en la segunda parte utilizamos una lista con sublista, 
ya que no sabemos la cantidad de creditos que se van a ingresar y
también
*/

//structs
struct Persona{
    int dni, codigo;
    char nombre[25], apellido[25];
    float ingreso;
};

struct Credito{
    int codigo, cupo;
    char tipoDeCredito[50];    
};

struct NodoListaPersonas{
    Persona p;
    NodoListaPersonas*sig;
};

struct NodoListaCreditos{
    Credito c;
    NodoListaCreditos*sig;
};

//prototipo de funciones

void ingresarCreditos(NodoListaCreditos*&listaCreditos);
void insertarEnListaCreditos(NodoListaCreditos*&listaCreditos, Credito credito);
void organizarInscriptos(NodoListaCreditos*&listaC, NodoListaPersonas*&listaP);
void insertarEnListaPersonas(NodoListaPersonas*&lista, Persona persona);
void mostrarListado(NodoListaPersonas*listaP, NodoListaCreditos*listaC);
void crearArchivo();


int main(){ 
   
    crearArchivo();
    cout<<"*************"<<endl;
    NodoListaCreditos*listaCreditos=NULL;
    NodoListaPersonas*listaPersonas=NULL;
    
    ingresarCreditos(listaCreditos);
    cout<<"*************"<<endl;
    FILE*archivo=fopen("data.dat","rb");

    organizarInscriptos(listaCreditos, listaPersonas);    
    mostrarListado(listaPersonas, listaCreditos);
    
    return 0;
}

//funciones
void crearArchivo(){
    
    FILE *archivo=fopen("Inscriptos.dat","wb");
    if(archivo==NULL){
        cout<<"ERROR"<<endl;
    }
    else{
        Persona p;
        cout<<"DNI (cero fin) ";
        cin>>p.dni;
        while(p.dni!=0){
            cout<<"Nombre ";
            cin.ignore();
            cin.getline(p.nombre,35);
            cout<<"Apellido: ";
            cin>>p.apellido;
            cout<<"Ingreso: ";
            cin>>p.ingreso;
            cout<<"Codigo de credito: ";
            cin>>p.codigo;
            fwrite(&p,sizeof(Persona),1,archivo);
            cout<<"DNI (cero fin) ";
            cin>>p.dni;
        }
        fclose(archivo);
    }
}

void ingresarCreditos(NodoListaCreditos*&lista){
    NodoListaCreditos*listaCreditos=NULL;
    Credito credito;

    cout<<"Codigo de credito (0 para finalizar): ";
    cin>>credito.codigo;
    while(credito.codigo!=0){
        cout<<"Cupos: ";
        cin>>credito.cupo;
        cout<<"Tipo de credito: ";
        cin>>credito.tipoDeCredito;
        insertarEnListaCreditos(lista, credito);
        cout<<"Codigo de credito (0 para finalizar): ";
        cin>>credito.codigo;
    }
}

void organizarInscriptos(NodoListaCreditos*&listaC, NodoListaPersonas*&listaP){
    Credito c;
    Persona p;
    NodoListaCreditos*creditos=listaC;
    NodoListaPersonas*personas=listaP;   

    FILE*archivo=fopen("Inscriptos.dat","rb");
    if(archivo==NULL){
        cout<<"Error"<<endl;
    }
    else{
        fread(&p,sizeof(Persona),1,archivo);

        while(!feof(archivo)){

            insertarEnListaPersonas(listaP, p);
            fread(&p,sizeof(Persona),1,archivo);    
        }
        fclose(archivo);
    }
}

void insertarEnListaCreditos(NodoListaCreditos*&lista, Credito credito){
    NodoListaCreditos*n,*r,*anterior; //los punteros
    n=new NodoListaCreditos;
    n->c=credito;
    r=lista;
    while(r!=NULL && r->c.codigo<credito.codigo){
        anterior=r;
        r=r->sig;
    }
    n->sig=r;
    if(r!=lista){
        anterior->sig=n;
    }
    else{
        lista=n;
    }
}

void insertarEnListaPersonas(NodoListaPersonas*&lista, Persona persona){
    NodoListaPersonas*n,*r,*anterior; //los punteros
    n=new NodoListaPersonas;
    n->p=persona;
    r=lista;
    //ingresa ordenado por ingreso
    while(r!=NULL && r->p.ingreso>persona.ingreso){
        anterior=r;
        r=r->sig;
    }
    n->sig=r;
    if(r!=lista){
        anterior->sig=n;
    }
    else{
        lista=n;
    }
}

void mostrarListado(NodoListaPersonas*listaP, NodoListaCreditos*listaC){
    NodoListaPersonas*punteroP; //puntero a la lista //primer elemento de la lista
    punteroP=listaP;
    NodoListaCreditos*punteroC;
    punteroC=listaC;
    
    while(punteroC!=NULL){ // mientras que la lista no esté vacía, muestra todos los elementos de esta

        int codigo=punteroC->c.codigo;
        int cupo=punteroC->c.cupo;     
        int cont=0;
        cout<<"Personas que se les ha otorgado el credito con codigo #"<<codigo<<endl;

        while(cont!=cupo && punteroP!=NULL){            

            if(punteroP->p.codigo==codigo && punteroP!=NULL){                                
                cout<<punteroP->p.dni<<" | "<<punteroP->p.nombre<<" | "<<punteroP->p.apellido<<" | "<<punteroP->p.ingreso<<endl;
                cont++;               
            }
            punteroP=punteroP->sig;
        }       
        punteroP=listaP;
        punteroC=punteroC->sig;    
    }
}
