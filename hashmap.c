#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
 Pair* aux = createPair (key, value); 
    int i=hash (key, map->capacity);
    if (map->buckets [i]==NULL || map->buckets[i]->key==NULL)
      {
       map->buckets[i]=aux;
       map->size++;
      }
    else
      {
        for(i=i;map->buckets[i]!=NULL;i++,i%=map->capacity){
          if(is_equal(key, map->buckets [i]->key))
            return;
        }
        if(map->buckets[i]==NULL || map->buckets[i]->key==NULL)
        {
          map->buckets[i]=aux;
          map->size++;
        }
      }        
}


void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
  Pair** aux=map->buckets;
  long capOld=map->capacity;
  map->capacity *= 2;
  map->buckets=(Pair**) calloc(map->capacity, sizeof(Pair*));
  map->size=0;
  for(long i=0; i<capOld;i++){
    if(aux[i]!=NULL && aux[i]->key!=NULL){
      insertMap(map, aux[i]->key, aux[i]->value);
      map->size++;
    }
  }
}


HashMap * createMap(long capacity) {
    HashMap *map=(HashMap *) malloc(sizeof(HashMap));
    if (map==NULL){
        printf("No hay Memoria suficiente");
        return NULL;
    }
    map->buckets=(Pair**) calloc(capacity, sizeof(Pair*));
    if(map->buckets==NULL){
      map=NULL;
      return NULL;
    }
    map->capacity=capacity;
    map->size=0;
    map->current=-1;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair* aux=searchMap(map, key);
    if(aux==NULL) return;
    aux->key=NULL;
    map->size-=1;
}

Pair * searchMap(HashMap * map,  char * key) {   
  long i= hash(key, map->capacity);
  for(i=i;map->buckets[i]!=NULL;i++){
    if(is_equal(key,map->buckets[i]->key)!=0){
      map->current=i;
      return map->buckets[i];
    }
    if (i==map->capacity-1) {
        i=0;
    }
    map->current = i;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
    for(long i=0;i<=map->capacity;i++){
      if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
          map->current=i;
         return map->buckets[i];
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
  long i;
    for(i=(map->current)+1;i<=map->capacity;i++){
      if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
         map->current=i;
         return map->buckets[i];
        }
    }
    return NULL;
}
