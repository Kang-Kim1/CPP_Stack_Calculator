#include "UBVector.h"
#include <iostream>

using namespace std;
int main()

{

	cout << "vec(3,1) -> 1,1,1" << endl;
    UBVector b(3,1);

	cout << "3->push back 1, 1, 1, 3" << endl;
    b.push_back(3); // 1, 1, 1, 3
    cout << "begin : " << b.begin() << "  end : " << b.end() << endl;
    b.insert(b.begin()+1,9); // 1, 9, 1, 1, 3

	cout << "print vector with 1, 9, 1, 1, 3" << endl;

	cout << "***** current vec*****" << endl;
    b.print_vector();

    cout << endl;
	cout<< "pop back! 1,9, 1" << endl;
    b.pop_back(); // 1, 9, 1, 1

	cout << "*****current vec*****" << endl;
    b.print_vector();


	cout << "pop back!! 1,9,1" << endl;
    b.pop_back(); // 1, 9, 1

	cout << "*****current vec*****" << endl;
    b.print_vector();

	cout << "10 -> push back!! 1, 9, 1, 10" << endl;
    b.push_back(10); // 1, 9, 1, 10

	cout << "current vec*****" << endl;
    b.print_vector();

	cout << "new vec(oldvec.begin, oldvec.end)" << endl;
    UBVector aav(b.begin(), b.end());

	cout << "******current new vec*****" << endl;
    aav.print_vector();

	cout << "erase new vec end" << endl;
    aav.erase(aav.end());

	cout << "***** current new vec*****" <<endl;
        aav.print_vector();


	cout << "****current old vec "<<endl;
        b.print_vector();

	cout << "b.end+1" << endl;
        cout << *(b.end()+1)<< endl;
	
	cout << "b.front" <<endl;
	cout << b.front() << endl;
	cout << "b.back" << endl;
	cout << b.back() << endl;

	b.front()= 89;
	b.back() = 97;


	b.print_vector();

	cout << b.at(3) << " " << b.at(0)<<endl;

	UBVector acc(b);

	b.print_vector();
	acc.print_vector();

	UBVector pp(5,8);

	b = pp;	
	
	b.print_vector();

    return 0;
}
