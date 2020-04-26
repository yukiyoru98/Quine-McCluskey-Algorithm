#include "OtherFunctions.h"
using namespace std;

int main()
{
    //open file
    ifstream read("input.txt", ios::in);
    if (!read) {//failed to read file
        cerr << "File cannot be opened.\n";
        exit(1);
    }
    
    int min_sop[1024] = {0};//2^10//minterm=1; dc = -1; else 0
    vector<int> minterms;
    vector< vector<cmbterm> > adjGroup(11);
    string str;
    int m;
    
    //read each line
    while (getline(read, str)) {
        cmbterm term;
        //cout << "get " << str << endl;
        if (str[0] == '(') {
            
            term.type = 'd';//set as don't care
            str.erase(str.begin());//remove '('
            str.erase(str.begin()+str.size()-1);//remove ')'
        }
        else {
            term.type = 'm';//set as minterm
        }
        m = std::stoi(str);//convert string to integer
        //cout << "m" << m << endl;
        term.decimal.insert(m);//put into cmbterm
        term.checked = false;//set as unchecked
        if(term.type == 'm'){
            min_sop[m] = 1;
            minterms.push_back(m);
        }
        else{
            min_sop[m] = -1;
        }
        
        //convert to binary
        //term.binary="0000000000";
        int flag = 9;//record bit position
        int numOf_1 = 0;//count how many 1's are there
        while (m != 0) {
            if (m % 2 == 0) {
                term.binary.at(flag) = '0';
            }
            else {
                term.binary.at(flag) = '1';
                numOf_1++;
            }
            m /= 2;
            flag--;
        }
        // cout << term.binary;
        adjGroup.at(numOf_1).push_back(term);//put term into group
        //printAdjG(adjGroup, 0);
    }
    
    cout << "Combine terms that have the same '-' and differ one in the number of 1's.\n";
    cout << "Check off the combined terms.\n";
    //combination process
    
    int round = 0;
    
    for (int i = 0; i < adjGroup.size() - 1; i++) {
        vector<cmbterm> temp;//saves all combined terms
        
        std::vector<cmbterm>::iterator it;
        std::vector<cmbterm>::iterator it2;
        
        //compare with the next group
        for (it = adjGroup[i].begin(); it != adjGroup[i].end(); it++) {
            //cout << "G" << i << "cmb with G" << i+1 << endl;
            
            
            //cout << "term A:\n";
            //it->print();
            
            //start from the first term in group[i]
            for (it2 = adjGroup[i + 1].begin(); it2 != adjGroup[i + 1].end(); it2++) {
                
                //cout << "term B:\n";
                //it2->print();
                
                //try combine with adjGourp[i+1];
                bool can_combine = true;
                int dif_pos = -1;
                for (int j = 0; j < 10; j++) {//compare each bit
                    //cout << "cmp bit[" << j<<"]:"<<it->binary[j]<<"&"<<it2->binary[j]<<"\n";
                    if (it->binary[j] != it2->binary[j]) {
                        if (dif_pos != -1) {//already diff. at other position
                            can_combine = false;
                            //cout << "already dif, can_combine = "<< can_combine << endl;
                            break;
                        }
                        else if ( !( (it->binary[j] != '-')&&(it2->binary[j] != '-') ) ) {
                            //shall not diff. at '-'
                            //cout << "dif at  - "<<endl;
                            can_combine = false;
                            //cout << " can_combine = "<< can_combine << endl;
                            break;
                        }
                        else{
                            //cout << " can_combine = "<< can_combine << endl;
                            dif_pos = j;//record position of different bit
                            //cout << "dif_pos = " << dif_pos << endl;
                            
                        }
                    }
                }
                if (can_combine) {
                    //check the source terms
                    //cout << "check termA\n";
                    it->checked = true;
                    //cout <<"check termB\n";
                    it2->checked = true;
                    
                    cmbterm new_combined;
                    new_combined.binary = it->binary;//copy from source
                    new_combined.binary.at(dif_pos) = '-';//change diff. to '-'
                    //merge the minterms(decimal)
                    std::set_union(it->decimal.begin(), it->decimal.end(), it2->decimal.begin(), it2->decimal.end(), inserter(new_combined.decimal, new_combined.decimal.begin()));
                    //check if already exists
                    for(int i=0; i<temp.size(); i++){
                        if(temp[i].decimal == new_combined.decimal){
                            can_combine = false;
                        }
                    }
                    if(can_combine == true){//skip repeated
                        
                        //cout << "new cmb:\n";
                        //new_combined.print();
                        
                        //push into new group
                        temp.push_back(new_combined);
                    }
                }
            }
        }
        if (!temp.empty()) {
            adjGroup.push_back(temp);//push into adjGroup to form new group
            round ++;
            cout << "Round " << round << endl;
            printAdjG(adjGroup);
            cout << "\nPress ENTER to see the next step...\n";
            char enter;
            cin.get(enter);
            system("cls");
        }
        
    }
    
    ////////////////////////////////////////////////////////////////////////////////////////
    
    //create pi_list
    vector<cmbterm> pi_list;
    for (int i = 0; i < adjGroup.size(); i++) {
        if (adjGroup[i].empty())    continue;
        for(int j=0; j<adjGroup[i].size(); j++){
            if(adjGroup[i][j].checked == false){
                if(adjGroup[i][j].type == 'd'){//for don't cares
                    continue;
                }
                pi_list.push_back(adjGroup[i][j]);
                //cout << "insert";
                //pi_list.back().print();
                //cout << endl;
            }
        }
    }
    vector<int> isPruned_pi(pi_list.size(), 0);//for printing solution step-by-step
    
    //make a table with minterms and the prime implicants
    //the set<int> in min_pi[i] records the prime implicants that includes the i-th minterm
    //but non-minterms are still in the table for conveniency, they will be pruned later
    vector<set<int>> min_pi(1024);//2^10
    for (int i = 0; i < pi_list.size(); i++) {
        std::set<int>::iterator it;
        set<int> m = pi_list[i].decimal;//make a copy for convenience
        for (it = m.begin(); it != m.end(); it++) {//put the minterm set of Cmbterms into table
            if (min_sop[*it] == 1) {//*it is a minterm
                min_pi[*it].insert(i);//put all minterms in the minterm set sequentially into table
            }
        }
    }
    //erase non-minterms from min_pi table
    std::vector<set<int>>::iterator it_min_pi;
    int i_min_pi;
    for (it_min_pi = min_pi.begin(), i_min_pi = 0; it_min_pi != min_pi.end();) {
        if (min_pi[i_min_pi].empty()) {
            it_min_pi = min_pi.erase(it_min_pi);
        }
        else {
            it_min_pi++;
            i_min_pi++;
        }
    }
    vector<set<int>> all_solutions;//save all possible solutions
    set<int> solution;//save solution of QM
    
    //initial pi chart
    printPiChart(pi_list, minterms, isPruned_pi);
    
    //USER selection:
    cout << "Please select...\n";
    cout << "(1)Quine-McClusky Method\n";
    cout << "(2)Petrick's Method\n";
    char choice, enter;
    cin >> choice;
    //illegal input
    while (choice != '1' && choice != '2') {
        cout << "Please try again...\n";
        cin >> choice;
    }
    
    //apply QM
    if (choice == '1') {
        
        for (int i = 0; i < min_pi.size();) {//look for essential prime implicant in the remaining pi chart
            int ess_pi;
            if (min_pi[i].size() == 1) {
                ess_pi = *(min_pi[i].begin());
                solution.insert(ess_pi);//record ess_pi in solution
                isPruned_pi[ess_pi] = 1;//mark ess_pi as pruned in isPruned_pi
                cout << "\nMinterm" << minterms[i] << " is only covered by " << " P" << ess_pi << ".\n";
                cout << "P" << ess_pi << " has to be chosen. Delete P" << ess_pi << " and the other minterms it covers.\n\n";
                i = 0;//start from first min_pi again next round
            }
            else {
                i++;
                continue;
            }
            
            //prune min_pi table
            std::vector<set<int>>::iterator it_min_pi;
            std::set<int>::iterator it_solution;
            std::vector<int>::iterator itv;
            int i_min_pi;//counter in the for-loop
            for (it_min_pi = min_pi.begin(), i_min_pi = 0, itv = minterms.begin(); it_min_pi != min_pi.end(); ) {
                if (min_pi[i_min_pi].count(ess_pi)) {//find minterms that are covered by ess_pi
                    //erase them from min_pi and from the minterms table
                    it_min_pi = min_pi.erase(it_min_pi);
                    itv = minterms.erase(itv);
                }
                else {
                    it_min_pi++;
                    i_min_pi++;
                    itv++;
                }
            }
            printPiChart(pi_list, minterms, isPruned_pi);
            cout << "\nPress any key to see the next step...\n";
            system("pause");
        }
        
        if (minterms.empty()) {//no more minterms uncovered
            //QM solution is a complete solution, no need to apply petrick's method
            all_solutions.push_back(solution);//put solution into all_solutions
        }
        
    }//end of QM
    
    //apply petrick's method
    if (all_solutions.size() == 0) {//QM reaches here if it doesn't generate a complete sol((because the remaining chart is cyclic
        
        //message to USER who selects QM
        if (choice == '1') {
            cout << "The remaining chart is CYCLIC, apply Petrick's Method to find solution.\n";
        }
        
        set< set< set<int> > > sum;//used to save the equation(SOP)
        
        for (int i = 0; i < min_pi.size(); i++) {//form the initial petrick's method equation EX:(P1+P2)(P2+P3(P3+P4)
            set<int> s = min_pi[i];//s is the set of prime implicants that covers the i-th  minterm
            
            set<set<int>> proterm;//product term (P1+P2+...Pn)
            std::set<int>::iterator it;
            for (it = s.begin(); it != s.end(); it++) {//insert the prime implicants in s seperately to form a product term
                set<int> pi;//save as set<int> because each P inside the product term might not be a single prime implicant after consequent calculation
                pi.insert(*it);
                proterm.insert(pi);
            }
            sum.insert(proterm);
        }
        cout << "\n< Equation >\n   ";
        printSum(sum);
        cout << "\nPress ENTER to see the next step...\n";
        cin.get(enter);
        
        //iterators declaration
        std::set< set< set<int> > >::iterator its;
        std::set< set< set<int> > >::iterator its2;
        std::set< set<int> >::iterator itp;
        std::set< set<int> >::iterator itp2;
        
        for (its = sum.begin(); its != sum.end();) {
            bool modified = false;
            for (its2 = its; its2 != sum.end(); ) {
                if (its2 == its) {//skip itself
                    its2++;
                    continue;
                }
                //reduce redundance term(two conditions because in the function "includes(a_begin, a_end, b_begin, b_end)", a must be larger than b)
                //condition1:(X+Y) X = X
                if (includes(its->begin(), its->end(), its2->begin(), its2->end())) {
                    its = sum.erase(its);
                    modified = true;
                    its = sum.begin();
                    break;
                }
                //condition2:X (X+Y) = X
                if (includes(its2->begin(), its2->end(), its->begin(), its->end())) {
                    its2 = sum.erase(its2);
                    modified = true;
                    its = sum.begin();
                    break;
                }
                //or else
                //factorization: (X+Y)(X+Z)=X+YZ
                //X is intersection part, YZ is product of different parts
                //partA: find X
                set< set<int> > inter;
                set_intersection((*its).begin(), (*its).end(), (*its2).begin(), (*its2).end(), inserter(inter, inter.begin()));
                
                if (!inter.empty()) {//can factorize the two sets if intersection is not empty
                    //partB:find YZ
                    //remove intersection from origin to get the remaining different parts
                    set< set<int> > dif;
                    set< set<int> > dif2;
                    set_difference((*its).begin(), (*its).end(), inter.begin(), inter.end(), inserter(dif, dif.begin()));
                    set_difference((*its2).begin(), (*its2).end(), inter.begin(), inter.end(), inserter(dif2, dif2.begin()));
                    
                    for (itp = dif.begin(); itp != dif.end(); itp++) {//merge dif "one by one"(because Y and Z could be POS) EX: Y=(A+B)-> YZ=(A+B)Z=AZ+BZ
                        for (itp2 = dif2.begin(); itp2 != dif2.end(); itp2++) {
                            set< set<int> > merge_dif;
                            set<int> merge_pi;
                            
                            set_union((*itp).begin(), (*itp).end(), (*itp2).begin(), (*itp2).end(), inserter(merge_pi, merge_pi.begin()));
                            //convert into set< set<int> > form
                            merge_dif.insert(merge_pi);
                            //merge partA and partB to get (X+YZ)
                            set< set<int> > lastMerge;
                            set_union(merge_dif.begin(), merge_dif.end(), inter.begin(), inter.end(), inserter(lastMerge, lastMerge.begin()));//unite inter and merge dif and put into sum
                            //put the result into sum
                            sum.insert(lastMerge);
                            
                            if (choice == '2') {
                                cout << " = ";
                                printSum(sum);
                                cout << "\nPress ENTER to see the next step...\n";
                                cin.get(enter);
                            }
                        }
                    }
                    //remove original pos term (X+Y)and (X+Z) after factorizing
                    its2 = sum.erase(its2);
                    its = sum.erase(its);
                    modified = true;
                    if (choice == '2') {
                        cout << " = ";
                        printSum(sum);
                        cout << "\nPress ENTER to see the next step...\n";
                        cin.get(enter);
                    }
                    its = sum.begin();//check from the beginning again since sum has been modified
                    break;
                }
                else {//nothing erased->iterator move on
                    its2++;
                }
            }
            if (modified == false) {//nothing erased->iterator move on
                its++;
            }
        }//factorization is done
        
        //next: multiply all out to get all possible solutions: (W+X)(Y+Z)=WY+WZ+XY+XZ
        for (its = sum.begin(); its != sum.end(); its = sum.begin()) {
            its2 = its;
            its2++;//don't multiply itself
            
            if (its2 == sum.end()) {//multiplying out procedure is done
                break;
            }
            
            set< set<int> > lastMerge;
            for (itp = (*its).begin(); itp != (*its).end(); itp++) {//merge different parts
                for (itp2 = (*its2).begin(); itp2 != (*its2).end(); itp2++) {
                    set<int> merge_pi;
                    set_union((*itp).begin(), (*itp).end(), (*itp2).begin(), (*itp2).end(), inserter(merge_pi, merge_pi.begin()));
                    //convert into set< set<int> > form
                    lastMerge.insert(merge_pi);
                }
            }
            //erase after multiplying out
            its2 = sum.erase(its2);
            its = sum.erase(its);
            
            //put back into sum
            sum.insert(lastMerge);
            //print sum
            if (choice == '2') {
                cout << " = ";
                printSum(sum);
                cout << "\nPress ENTER to see the next step...\n";
                cin.get(enter);
            }
        }
        
        //find solution with fewest product terms
        cout << "\n< SOLUTION >\n";
        cout << "(Pick the terms with fewest prime implicants.)\n";
        if (choice == '1') {
            
            
            cout << "(Combine with Quine McClasky's solution.)\n";
            
        }
        its = sum.begin();
        itp = its->begin();
        size_t min_size = itp->size();
        for (int i = 0; itp != its->end(); i++, itp++) {
            
            if (itp->size() < min_size) {//found solution with smaller size
                min_size = itp->size();
                all_solutions.clear();//clear all bigger solutions recorded
            }
            
            if (itp->size() == min_size) {
                
                if (choice == '1') {//merge with solution found in QM if neccessary
                    set<int> merge_pi;
                    set_union((*itp).begin(), (*itp).end(), solution.begin(), solution.end(), inserter(merge_pi, merge_pi.begin()));
                    all_solutions.push_back(merge_pi);//record solutions with the currrently minimum size
                }
                else {
                    all_solutions.push_back(*itp);//record solutions with the currrently minimum size
                }
            }
        }
        
    }//solution found, end of Petrick's Method
    
    //print solution
    for (int i = 0; i < all_solutions.size(); i++) {
        if (i > 0) {//if more than one solution
            cout << "\n\n or\n\n";
        }
        
        cout << "F(a,b,c,d,e,f,g,h,i,j) = ";
        std::set<int>::iterator it;
        for (it = all_solutions[i].begin(); it != all_solutions[i].end(); it++) {
            pi_list[*it].printC();//print the prime implicants in characters
            if (it != --all_solutions[i].end()) {
                cout << " + ";
            }
        }
    }
    cout << endl << endl;
    
    
    system("pause");
    return 0;
}
