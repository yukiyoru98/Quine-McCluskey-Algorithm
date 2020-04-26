#ifndef cmbterm_h
#define cmbterm_h

#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <set>
#include <map>

#include "OtherFunctions.h"

using namespace std;
//prototypes
void printSet(set<int>);

struct cmbterm {
    string binary = "0000000000";//default value
    set<int> decimal;//saves the minterms set in decimal numbers
    char type = 'm';//m for minterm, d for don't care
    bool checked = false;//default value
    
    void print() const {
        cout << (checked ? 'v' : ' ') << ' ' << binary << ' ';
        printSet(decimal);
        //cout << ' '<<type;
    }
    
    void printC() const{
        char a = 'a';
        for(int i=0; i<10; i++){
            if(binary[i] == '-'){
                continue;
            }
            
            cout << (char)(a+i);
            if(binary[i] == '0'){
                cout << "'";
            }
            else{
                cout << ' ';
            }
        }
    }
    
    friend bool operator<(const cmbterm& l, const cmbterm& r)
    {
        return std::tie(l.binary, l.decimal, l.type, l.checked)
        < std::tie(r.binary, r.decimal, r.type, r.checked); // keep the same order
    }
};

#endif /* cmbterm_h */
