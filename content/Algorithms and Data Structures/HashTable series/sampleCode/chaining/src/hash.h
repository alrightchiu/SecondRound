#ifndef HASH_H_
#define HASH_H_

#include <string>
#include <vector>
#include <list>
using std::string;
using std::vector;
using std::list;


//self-defined dictionary
class Dict{
public:
	string m_key;
	string m_value;

	Dict();
	Dict(string k, string v);
};


class HashChain{
private:
	int m_size;
	int m_count;

	vector< list<Dict> > mv_table; //hash table with linked list

	int preHashing(string); //turn string_type_key to int_type_key
	int hashFunc(string); //using Division method
public:

	HashChain(); //constructor
	HashChain(int m); //constuctor overload

	void insertData(Dict);
	void deleteData(string);
	string search(string);
	void displayTb();

};




#endif //HASH_H_