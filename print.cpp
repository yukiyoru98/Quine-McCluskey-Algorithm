#include "print.hpp"

using namespace std;

//print func. definition

void printSet(set<int> set) {//print the no. of prime implicants
    
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
        printSet(*it);
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

void printKmap(const int min_sop[16]) {//print Kmap
    //change to char
    char c[16] = {};
    for (int i = 0; i < 16; i++) {
        switch (min_sop[i])
        {
            case 1:
                c[i] = '1';
                break;
                
            case 0:
                c[i] = '0';
                break;
                
            case -1:
                c[i] = 'X';
                break;
                
            default:
                break;
        }
    }
    cout << "======= K Map ========\n";
    cout << "  \\ab|\n";
    cout << "cd \\ | 00  01  11  10\n";
    cout << "-----+----------------\n";
    cout << "  00 | " << c[0] << " | " << c[4] << " | " << c[12] << " | " << c[8] << " |" << endl;
    cout << "-----+---+---+---+---|\n";
    cout << "  01 | " << c[1] << " | " << c[5] << " | " << c[13] << " | " << c[9] << " |" << endl;
    cout << "-----+---+---+---+---|\n";
    cout << "  11 | " << c[3] << " | " << c[7] << " | " << c[15] << " | " << c[11] << " |" << endl;
    cout << "-----+---+---+---+---|\n";
    cout << "  10 | " << c[2] << " | " << c[6] << " | " << c[14] << " | " << c[10] << " |" << endl;
    cout << "----------------------\n";
}

void printGroup(const vector< vector<Cmbterms> > &G) {//print combined minterms in groups
    for (int i = 0; i < G.size(); i++) {
        cout << "\n<Group " << i << ">" << endl;
        for (int j = 0; j < G[i].size(); j++) {
            G[i][j].print();
        }
        cout << "------------------------";
    }
}

void printPiChart(const vector< Cmbterms > &pi_list, const vector<int> &minterms, const vector<int> &isPruned_pi) {//print prime implicant chart
    
    //convert the minterms covered by this pi into string for conveniency (format input)
    int max_l = 0;
    vector<string> pi_str;
    std::set<int>::iterator it;
    for (int i = 0; i < pi_list.size(); i++) {
        string s;
        stringstream ss;
        set<int> m = pi_list[i].getMinterm();
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
        cout << setw(4) << minterms[i];
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
            if ((pi_list[i].getMinterm()).count(minterms[j])) {
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
