#include "OtherFunctions.h"



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
        cout << "====";
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
        cout << "----";
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
                cout << setw(4) << "x";
            }
            else {
                cout << setw(4) << "";
            }
        }
        cout << endl;
    }
    
    //last line
    for (int i = 0; i < max_l + 5; i++) {
        cout << "=";
    }
    
    for (int i = 0; i < minterms.size(); i++) {
        cout << "====";
    }
    cout << endl;
    
}
