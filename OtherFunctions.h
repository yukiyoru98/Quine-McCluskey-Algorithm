#ifndef OtherFunctions_h
#define OtherFunctions_h

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
using namespace std;

//functions
void printPetrickSet(set<int> set) {//print the no. of prime implicants
    
    std::set<int>::iterator it;
    for (it = set.begin(); it != set.end(); it++) {
        cout << "P";
        cout << *it;
        cout << " ";
    }
}

void printSset(set< set<int> > sset) {//print product terms
    std::set<set<int>>::iterator it;
    cout << "( ";
    for (it = sset.begin(); it != sset.end(); it++) {
        printPetrickSet(*it);
        if (it != (--sset.end()))cout << " + ";
    }
    cout << " )";
}

void printSum(set<set<set<int>>> sum) {//print SOP
    std::set<set<set<int>>>::iterator it;
    for (it = sum.begin(); it != sum.end(); it++) {
        printSset(*it);
        if (it != (--sum.end()))cout << " ";
    }
    cout << endl;
}

void printVec(vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << endl;
    }
}
void printSet(set<int> s) {
    std::set<int>::iterator it;
    for (it = s.begin(); it != s.end(); it++) {
        cout << *it << ',';
    }
}

struct cmbterm {
    string binary = "0000000000";//default value
    set<int> decimal;//saves the minterms set in decimal numbers
    char type = 'm';//m for minterm, d for don't care
    bool checked = false;//default value
    
    void print() const {
        if(type == 'd'){
            cout << 'd';
        }
        else{
            cout << (checked ? 'v' : ' ');
        }
        cout << ' ' << binary << ' ';
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

void printAdjG(vector< vector<cmbterm> > G) {//print combined minterms in groups
    
    for (int i = 0; i < G.size(); i++) {
        if (G[i].empty())    continue;
        std::vector<cmbterm>::iterator it;
        for (it = G[i].begin(); it != G[i].end(); it++) {
            it->print();
            cout << endl;
        }
        cout << "------------------------";
        cout << endl;
    }
}

void printPiChart(const vector< cmbterm > &pi_list, const vector<int> &minterms, const vector<int> &isPruned_pi) {//print prime implicant chart
    
    //convert the minterms covered by this pi into string for conveniency (format input)
    int max_l = 0;
    vector<string> pi_str;
    std::set<int>::iterator it;
    for (int i = 0; i < pi_list.size(); i++) {
        string s;
        stringstream ss;
        set<int> m = pi_list[i].decimal;
        for (it = m.begin(); it != m.end(); it++) {
            ss << *it << ',';
        }
        ss >> s;
        pi_str.push_back(s);
        if (s.size() > max_l) {
            max_l = s.size();
        }
    }
    
    //formatted print
    cout << "< Prime Implicant Chart >\n";
    
    //line 1
    for (int i = 0; i < max_l + 5; i++) {
        cout << "=";
    }
    
    for (int i = 0; i < minterms.size(); i++) {
        cout << "=====";
    }
    cout << endl;
    
    //line 2: minterms
    for (int i = 0; i < max_l + 5; i++) {
        cout << ' ';
    }
    
    for (int i = 0; i < minterms.size(); i++) {
        cout << setw(5) << minterms[i];
    }
    cout << endl;
    
    //line 3
    for (int i = 0; i < max_l + 5; i++) {
        cout << "-";
    }
    for (int i = 0; i < minterms.size(); i++) {
        cout << "-----";
    }
    cout << endl;
    
    //put x at the column of the minterms it covers
    for (int i = 0; i < pi_str.size(); i++) {
        if (isPruned_pi[i] == 1) {//don't print pi's that are pruned
            continue;
        }
        cout << "[P" << i << "] ";
        cout << right << setw(max_l) << pi_str[i];
        
        for (int j = 0; j < minterms.size(); j++) {
            if ((pi_list[i].decimal).count(minterms[j])) {
                cout << setw(5) << "x";
            }
            else {
                cout << setw(5) << "";
            }
        }
        cout << endl;
    }
    
    //last line
    for (int i = 0; i < max_l + 5; i++) {
        cout << "=";
    }
    
    for (int i = 0; i < minterms.size(); i++) {
        cout << "=====";
    }
    cout << endl;
    
}

#endif /* OtherFunctions_h */
