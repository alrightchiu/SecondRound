#include <iostream>
#include "hash.h"
using std::cout;
using std::endl;

int main(){

	HashChain o_hash(5);

	Dict o_dict1("T-Mac", "Magic");
	Dict o_dict2("Bryant", "Lakers");
	Dict o_dict3("Webber", "Kings");
	Dict o_dict4("Arenas", "Wizards");

	o_hash.insertData(o_dict1);
	o_hash.insertData(o_dict2);
	o_hash.insertData(o_dict3);
	o_hash.insertData(o_dict4);
	o_hash.insertData(Dict("Davis", "Clipper"));
	o_hash.insertData(Dict("Kidd", "Nets"));
	o_hash.displayTb();

	cout << "T-Mac is in.. " << o_hash.search("T-Mac") << endl;
	cout << "Arenas is in.. " << o_hash.search("Arenas") << endl;


	o_hash.deleteData("Kidd");
	o_hash.deleteData("T-Mac");
	cout << endl << "After deleting Kidd and T-Mac:" << endl;
	o_hash.displayTb();

	return 0;
}