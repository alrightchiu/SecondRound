#include <iostream>
#include "hash.h"
using std::cout;
using std::endl;


/*----- -----class Dict----- -----*/
Dict::Dict(){
	m_key = "";
	m_value = "";
}

Dict::Dict(string k, string v){
	m_key = k;
	m_value = v;	

}


/*----- -----class HashChain----- -----*/
//private member functions

//preHashing: turn string key to int key
int HashChain::preHashing(string key_str){
	int exp = 9;
	int key_int = 0;
	int p = 1;

	for(int i=(int)key_str.length()-1; i>=0; i--){
		key_int += key_str[i]*p;
		p *= exp;
	}
	return key_int;
}

int HashChain::hashFunc(string key_str){

	int division = preHashing(key_str) % (m_size);
	return division;
}


//public member functions

//constructor
HashChain::HashChain(){

}


//constructor overload
HashChain::HashChain(int m){
	m_size = m;
	m_count = 0;
	mv_table.resize(m_size);  //reszie the container
}


void HashChain::insertData(Dict o_data){
	//get index from the hash function
	int index = hashFunc(o_data.m_key); 

	//insert data at the front of linked list
	mv_table[index].push_front(o_data); 
}

         
void HashChain::deleteData(string key_str){
	//get index from the hash function
	int index = hashFunc(key_str);
	
	list<Dict>::iterator i;   //delcare a list iterator
	for(i=mv_table[index].begin(); i!=mv_table[index].end(); i++){
		if( (*i).m_key == key_str ){
			mv_table[index].erase(i);
			return;  //return after delete data
		}	
	} 

}


string HashChain::search(string key_str){
	//get index from the hash function
	int index = hashFunc(key_str);
	list<Dict>::iterator i;   //delcare a list iterator
	for(i=mv_table[index].begin(); i!=mv_table[index].end(); i++){
		if( (*i).m_key == key_str )  //equivalent to i->m_key
			return (*i).m_value;

	} 

	return "...\n NO SUCH DATA";
}


void HashChain::displayTb(){

	for(unsigned int i=0; i<mv_table.size(); i++){
		cout << "slot#" << i << ": ";
		
		list<Dict>::iterator itr;   //delcare a list iterator
		for(itr=mv_table[i].begin(); itr!=mv_table[i].end(); itr++){
			cout << "(" << itr->m_key << ", " << itr->m_value << ")";
		} 
		cout << endl;
	}
	cout << endl;
}


/*-----Note-----*/
//See vector_basic.cpp in vector project to know more about vector iterators