#include <iostream>
#include <regex>
#include <stdio.h>
#include <bits/stdc++.h>
using namespace std;

#define MAX_LINE_LENGTH 1000

int back_propagate_to_head(string wire, char value_reqd);
int front_propagate(string wire, char value_Reqd);
char wire_val(string gate, char out, char in);
int justify_head1(string wire);
vector<array<string,4>> netlist;
map<string, string> wire_type;
map<string, string> ip_op;
map<string, char> values_for_fault;


int main(){

    FILE* textfile;
    char line[MAX_LINE_LENGTH];

    textfile = fopen("testcircuit.v", "r");
    if(textfile == NULL)
        return 1;
    
    // int var = 0;

    while(fgets(line, MAX_LINE_LENGTH, textfile)){
        if(strlen(line) == 2){
            continue;
        }

        // printf("%s--", line);
        vector <string> tokens;
        stringstream check1(line);
        string intermediate;

        while(getline(check1, intermediate, ' '))
        {
            tokens.push_back(intermediate);
        }

        if (tokens[0] == "module" || tokens[0] == "endmodule"){
            continue;
        } else if (tokens[0] == "input"){
            const char delimit[] = ",;\n";
            
            char *dup = strdup(tokens[1].c_str());
            char *t = strtok(dup, delimit);

            while(t != 0){
                // printf("-%s\n", t);
                if(strlen(t) == 1){
                    break;
                }
                wire_type[t] = "h";
                t = strtok(0, delimit);
            }

            free(dup);
            
        } else if (tokens[0] == "output"){
            const char delimit[] = ",;\n";
            
            char *dup = strdup(tokens[1].c_str());
            char *t = strtok(dup, delimit);

            while(t != 0){
                // printf("--%s\n", t);
                if(strlen(t) == 1){
                    break;
                }
                wire_type[t] = "f";
                t = strtok(0, delimit);
            }

            free(dup);
            
        } else if (tokens[0] == "wire"){
            const char delimit[] = ",;\n";
            
            char *dup = strdup(tokens[1].c_str());
            char *t = strtok(dup, delimit);

            while(t != 0){
                // printf("---%s\n", t);
                if(strlen(t) == 1){
                    break;
                }
                wire_type[t] = "f";
                t = strtok(0, delimit);
            }

            free(dup);
            
        } else{
            array<string,4> arr = {"a", "b", "c", "d"};

             for(int i = 0; i < tokens.size(); i++){

                if (i >= 2){
                    const char delimit[] = "(,;)\n";
                    char *dup = strdup(tokens[i].c_str());
                    char *t = strtok(dup, delimit);
                    arr[i-2] = t;
                    free(dup);
                }

            }

            // for(int i = 0; i < 3; i++){
            //     cout << "*" << arr[i] << endl;
            // }

            string x = arr[0];
            arr[0] = arr[2];
            arr[2] = x;

            // for(int i = 0; i < 3; i++){
            //     cout << "*" << arr[i] << endl;
            // }

            if(tokens[0] == "nand"){
                arr[3] = "nand";
            }else if (tokens[0] == "and"){
                arr[3] = "and";
            }else if (tokens[0] == "or"){
                arr[3] = "or";
            }else if(tokens[0] == "nor"){
                arr[3] = "nor";
            }else if(tokens[0] == "xor"){
                arr[3] = "xor";
            }else if(tokens[0] == "not"){
                arr[3] = "not";
            }else{
                continue;
            }

            if(wire_type[arr[0]] == "h" && wire_type[arr[1]] == "h"){
                wire_type[arr[2]] = "F";
                wire_type[arr[1]] = "H";
                wire_type[arr[0]] = "H";

            }else if(wire_type[arr[0]] == "H" || wire_type[arr[1]] == "H"){
                wire_type[arr[2]] = "B";
                if(wire_type[arr[0]] == "H"){
                    wire_type[arr[0]] == "S";
                }else{
                    wire_type[arr[1]] == "S";
                }
            }else if(wire_type[arr[0]] == "B" || wire_type[arr[1]] == "B"){
                wire_type[arr[2]] = "B";
            }else if(wire_type[arr[0]] == "S" || wire_type[arr[1]] == "S"){
                wire_type[arr[2]] = "B";
            }

            netlist.push_back(arr);

            // map<string, string>::iterator it = wire_type.begin();

            // while (it != wire_type.end()) {
            //     cout << "Key: " << it->first
            //         << ", Value1: " << it->second << endl;
            //     ++it;
            // } 

            // cout << tokens[0] << endl;

        }
    }

    for (int i = 0; i < netlist.size(); i++){
        if(wire_type[netlist[i][0]] == "h" && wire_type[netlist[i][1]] == "h"){
            wire_type[netlist[i][2]] = "F";
            wire_type[netlist[i][1]] = "H";
            wire_type[netlist[i][0]] = "H";

        }else if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][1]] == "H"){
            wire_type[netlist[i][2]] = "B";
            if(wire_type[netlist[i][0]] == "H"){
                wire_type[netlist[i][0]] == "S";
            }else{
                wire_type[netlist[i][1]] == "S";
            }
        }else if(wire_type[netlist[i][0]] == "B" || wire_type[netlist[i][1]] == "B"){
            wire_type[netlist[i][2]] = "B";
        }else if(wire_type[netlist[i][0]] == "S" || wire_type[netlist[i][1]] == "S"){
            wire_type[netlist[i][2]] = "B";
        }

    }
    cout << "Wire Type" << endl;
    map<string, string>::iterator it = wire_type.begin();

    while (it != wire_type.end()) {
        cout << "Key: " << it->first
             << ", Value: " << it->second << endl;
        ++it;
    } 

    cout << endl;
    cout << "Input Input Output Gate" << endl;

    for(int i = 0; i < netlist.size(); i++){
        cout << netlist[i][0] << " " << netlist[i][1] << " " << netlist[i][2] << " " << netlist[i][3] << endl;
    }

    fclose(textfile);


    map<string, string>::iterator it2 = wire_type.begin();

    while (it2 != wire_type.end()) {
        values_for_fault[it2->first] = 'x';
        ++it2;
    } 

    cout << endl;
    // Taking user input for which wire the fault is at - 
    cout << "Please mention wire: ";
    
    string faulty_wire;
    cin >> faulty_wire;

    cout << "Wire is stuck-at: ";
    char stuck_at;
    cin >> stuck_at;

    int value_required;
    if(stuck_at == '0')
        value_required = '1';
    else
        value_required = '0';
    
    cout << "Hence, to activate the fault at " << faulty_wire << " we need to set it to " << char(value_required) << endl;
    cout << endl;

    // cout << faulty_wire << stuck_at << endl;

    values_for_fault[faulty_wire] = value_required;

    // map<string, char>::iterator it3 = values_for_fault.begin();

    // while (it3 != values_for_fault.end()) {
    //     cout << "Key: " << it3->first
    //          << ", Value: " << it3->second << endl;
    //     ++it3;
    // } 
    // cout << endl;

    
    int b = back_propagate_to_head(faulty_wire, value_required);

    // initializing 
    cout << "This is the initial setup" << endl;
    map<string, char>::iterator it4 = values_for_fault.begin();

    while (it4 != values_for_fault.end()) {
        cout << "Key: " << it4->first
             << ", Value: " << it4->second << endl;
        ++it4;
    }

    // cout << "Hello" << endl;
    int c = front_propagate(faulty_wire, value_required); 
    cout << endl;

    cout << "This is post front propagation" << endl;
    map<string, char>::iterator it5 = values_for_fault.begin();

    while (it5 != values_for_fault.end()) {
        cout << "Key: " << it5->first
             << ", Value: " << it5->second << endl;
        ++it5;
    }

    map<string, string>::iterator it6 = wire_type.begin();

    while (it6 != wire_type.end()) {
        if(it6->second == "h" && values_for_fault[it6->first] == 'x'){
            justify_head1(it6->first);
        }
        ++it6;
    }

    it6 = wire_type.begin();
    while (it6 != wire_type.end()) {
        if(it6->second == "H" && values_for_fault[it6->first] == 'x'){
            justify_head1(it6->first);
        }
        ++it6;
    }

    cout << endl;
    cout << "Final Output for " << faulty_wire << " stuck at " <<  stuck_at << endl;

    it5 = values_for_fault.begin();
    while (it5 != values_for_fault.end()) {
        cout << "Key: " << it5->first
             << ", Value: " << it5->second << endl;
        ++it5;
    }

    

    return 0;

}

int back_propagate_to_head(string wire, char value_reqd){
    /*
    what all do I need
    1. The gate type
    2. The inputs to this gate
    3. The output we already have
    4. Can't think of anything else
    5. Some would require us to give both controlled values
        5.1 Then backpropagate until both reach head line
    */ 

    int n = netlist.size();

    for(int i = 0; i < n; i++){
        // checking the row which has this particular wire as output
        if(netlist[i][2] == wire){      

            if(netlist[i][3] == "nand"){
                if(value_reqd == '0'){
                    // cout << netlist[i][2] << endl;
                    // we need both inputs to be at 1,1 so that NAND output is 0
                    // and the fault is activated
                    
                    // setting first input
                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '1'; // commented out coz head line also justified otherwise
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '1';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '1'){

                    }else{
                        cout << "Detected an already set value of 0 at nand" << endl;
                    }

                    // setting second input
                    if(values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][1]] = '1';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] = '1';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][1]] == '1'){

                    }else{
                        cout << "Detected an already set value of 0 at nand" << endl;
                    }

                }else{
                    
                    /* now here we have stuck at 0, which gives 3 possible inputs
                    0,0 ; 1,0 and 0,1

                    So the approach is - 
                    1. If 2 'x', change the first one to 0 and relax
                    2. If first one is already 0, relax
                    3. If first one is 1
                        3.1 if the second one is 0, relax
                        3.2 If the second one is 'x', set to 0 and relax
                        3.3. If the second one is also already set to 1, hmm, dikkat hai
                    */

                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '0'){
                        // relax
                    }else{
                        if(values_for_fault[netlist[i][1]] == 'x'){
                            // values_for_fault[netlist[i][1]] = '0';
                            if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                            
                            }else{
                                values_for_fault[netlist[i][1]] = '0';
                                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                            }
                        }else if (values_for_fault[netlist[i][1]] == '0'){
                            // relax

                        }else{
                            cout << "Detected an already set value of 1 at both nand inputs" << endl;
                        }
                    }

                }
            }else if(netlist[i][3] == "and"){
                if(value_reqd == '1'){
                    // we need both inputs to be at 1,1 so that AND output is 1
                    // and the fault is activated
                    
                    // setting first input
                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '1';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '1';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '1'){

                    }else{
                        cout << "Detected an already set value of 0 at AND" << endl;
                    }

                    // setting second input
                    if(values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][1]] = '1';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] = '1';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][1]] == '1'){

                    }else{
                        cout << "Detected an already set value of 0 at AND" << endl;
                    }

                }else{
                    
                    /* now here we have stuck at 1, which gives 3 possible inputs
                    0,0 ; 1,0 and 0,1

                    So the approach is - 
                    1. If 2 'x', change the first one to 0 and relax
                    2. If first one is already 0, relax
                    3. If first one is 1
                        3.1 if the second one is 0, relax
                        3.2 If the second one is 'x', set to 0 and relax
                        3.3. If the second one is also already set to 1, hmm, dikkat hai
                    */

                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '0'){
                        // relax
                    }else{
                        if(values_for_fault[netlist[i][1]] == 'x'){
                            // values_for_fault[netlist[i][1]] = '0';
                            if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                            
                            }else{
                                values_for_fault[netlist[i][1]] = '0';
                                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                            }
                        }else if (values_for_fault[netlist[i][1]] == '0'){
                            // relax

                        }else{
                            cout << "Detected an already set value of 1 at both AND inputs" << endl;
                        }
                    }

                }

            }else if(netlist[i][3] == "or"){

                // cout << netlist[i][0] << netlist[i][1] << value_reqd << wire  << endl;

                if(value_reqd == '0'){
                    // we need both inputs to be at 0,0 so that OR output is 0
                    // and the fault is activated
                    
                    // setting first input
                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            
                            values_for_fault[netlist[i][0]] = '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '0'){

                    }else{
                        cout << "Detected an already set value of 1 at OR" << endl;
                    }

                    // setting second input
                    if(values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][1]] = '0';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] = '0';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][1]] == '0'){

                    }else{
                        cout << "Detected an already set value of 1 at OR" << endl;
                    }

                }else{
                    
                    /* now here we have stuck at 0, which gives 3 possible inputs
                    1,1 ; 1,0 and 0,1

                    So the approach is - 
                    1. If 2 'x', change the first one to 1 and relax
                    2. If first one is already 1, relax
                    3. If first one is 0
                        3.1 if the second one is 1, then relax
                        3.2 If the second one is 'x', set to 1 and relax
                        3.3. If the second one is also already set to 0, hmm, dikkat hai
                    */

                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '1';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                            values_for_fault[netlist[i][1]] = '1';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);

                        }else{

                            values_for_fault[netlist[i][0]] = '1';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '1'){
                        // relax
                    }else{
                        if(values_for_fault[netlist[i][1]] == 'x'){
                            // values_for_fault[netlist[i][1]] = '1';
                            if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                            
                            }else{
                                values_for_fault[netlist[i][1]] = '1';
                                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                            }
                        }else if (values_for_fault[netlist[i][1]] == '1'){
                            // relax

                        }else{
                            cout << "Detected an already set value of 0 at both OR inputs" << endl;
                        }
                    }

                }

            }else if(netlist[i][3] == "nor"){
                if(value_reqd == '1'){
                    // we need both inputs to be at 0,0 so that NOR output is 1
                    // and the fault is activated
                    
                    // setting first input
                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '0'){

                    }else{
                        cout << "Detected an already set value of 1 at NOR" << endl;
                    }

                    // setting second input
                    if(values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][1]] = '0';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] = '0';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][1]] == '0'){

                    }else{
                        cout << "Detected an already set value of 1 at NOR" << endl;
                    }

                }else{
                    
                    /* now here we have stuck at 1, which gives 3 possible inputs
                    1,1 ; 1,0 and 0,1

                    So the approach is - 
                    1. If 2 'x', change the first one to 1 and relax
                    2. If first one is already 1, relax
                    3. If first one is 0
                        3.1 if the second one is 1, then relax
                        3.2 If the second one is 'x', set to 1 and relax
                        3.3. If the second one is also already set to 0, hmm, dikkat hai
                    */

                    if(values_for_fault[netlist[i][0]] == 'x'){
                        // values_for_fault[netlist[i][0]] = '1';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = '1';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == '1'){
                        // relax
                    }else{
                        if(values_for_fault[netlist[i][1]] == 'x'){
                            // values_for_fault[netlist[i][1]] = '1';
                            if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                            
                            }else{
                                values_for_fault[netlist[i][1]] = '1';
                                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                            }
                        }else if (values_for_fault[netlist[i][1]] == '1'){
                            // relax

                        }else{
                            cout << "Detected an already set value of 0 at both NOR inputs" << endl;
                        }
                    }

                }

            }else if(netlist[i][3] == "xor"){
                if(value_reqd == '0'){

                    // we need both inputs to be 0,0 or 1,1 in order to have a 0 at XOR output
                    // and the fault is activated

                    // So, we need a unique approach here - 
                    // If both the inputs are X, set each to 0,0 for now
                    // If one is X, set to same value as other
                    // If none is X, ensure they are same, otherwise throw error

                    if(values_for_fault[netlist[i][0]] == 'x' && values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][0]] == '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] == '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }

                        // values_for_fault[netlist[i][1]] == '0';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] == '0';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == 'x' && values_for_fault[netlist[i][1]] != 'x'){
                        // values_for_fault[netlist[i][0]] = values_for_fault[netlist[i][1]];
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] = values_for_fault[netlist[i][1]];
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }

                    }else if(values_for_fault[netlist[i][0]] != 'x' && values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][1]] = values_for_fault[netlist[i][0]];
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] = values_for_fault[netlist[i][0]];
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }

                    }else{
                        if(values_for_fault[netlist[i][0]] == values_for_fault[netlist[i][1]]){
                            // Yipee, all going good!
                        }else{
                            cout << "Both the logic values are set to different values for XOR gate" << endl;
                        }
                    }   

                }else{
                    // This is stuck at 0

                    // we need inputs to be 0,1 or 1,0 in order to have a 1 at XOR output
                    // and the fault is activated

                    // So, we need a unique approach here - 
                    // If both the inputs are X, set each to 0,1 for now
                    // If one is X, set to different value as other
                    // If none is X, ensure they are different, otherwise throw error

                    if(values_for_fault[netlist[i][0]] == 'x' && values_for_fault[netlist[i][1]] == 'x'){
                        // values_for_fault[netlist[i][0]] == '0';
                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][0]] == '0';
                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }

                        // values_for_fault[netlist[i][1]] == '1';
                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{
                            values_for_fault[netlist[i][1]] == '1';
                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }
                    }else if(values_for_fault[netlist[i][0]] == 'x' && values_for_fault[netlist[i][1]] != 'x'){

                        // if(values_for_fault[netlist[i][1]] == '1')
                        //     values_for_fault[netlist[i][0]] = '0';
                        // else{
                        //     values_for_fault[netlist[i][0]] = '1';
                        // }

                        if(wire_type[netlist[i][0]] == "H" || wire_type[netlist[i][0]] == "h"){
                        
                        }else{

                            if(values_for_fault[netlist[i][1]] == '1')
                                values_for_fault[netlist[i][0]] = '0';
                            else{
                                values_for_fault[netlist[i][0]] = '1';
                            }

                            back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                        }

                    }else if(values_for_fault[netlist[i][0]] != 'x' && values_for_fault[netlist[i][1]] == 'x'){
                        
                        // if(values_for_fault[netlist[i][0]] == '0')
                        //     values_for_fault[netlist[i][1]] = '1';
                        // else{
                        //     values_for_fault[netlist[i][1]] = '0';
                        // }

                        if(wire_type[netlist[i][1]] == "H" || wire_type[netlist[i][1]] == "h"){
                        
                        }else{

                            if(values_for_fault[netlist[i][0]] == '0')
                                values_for_fault[netlist[i][1]] = '1';
                            else{
                                values_for_fault[netlist[i][1]] = '0';
                            }

                            back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                        }

                    }else{
                        if(values_for_fault[netlist[i][0]] != values_for_fault[netlist[i][1]]){
                            // Yipee, all going good!
                        }else{
                            cout << "Both the logic values are set to different values for XOR gate" << endl;
                        }
                    }   

                }
                    
            }else{

            }
        }
    }
    return 0;
}

int front_propagate(string wire, char value_Reqd){

    int n = netlist.size();
    int is_input = 0;

    for(int i = 0; i < n; i++){
        if(netlist[i][0] == wire && is_input != 1){
            is_input = 1;
            char output_val;

            if(netlist[i][3] == "nand"){
                // setting to NC
                values_for_fault[netlist[i][1]] = '1';
                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                
                if(values_for_fault[netlist[i][0]] == '1')
                    output_val = '0';
                else
                    output_val = '1';

            }else if(netlist[i][3] == "or"){
                // setting to NC
                values_for_fault[netlist[i][1]] = '0';
                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                if(values_for_fault[netlist[i][0]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else if(netlist[i][3] == "nor"){
                // setting to NC
                values_for_fault[netlist[i][1]] = '0';
                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                if(values_for_fault[netlist[i][0]] == '1')
                    output_val = '0';
                else
                    output_val = '1';

            }else if(netlist[i][3] == "and"){
                // setting to NC
                values_for_fault[netlist[i][1]] = '1';
                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                if(values_for_fault[netlist[i][0]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else if(netlist[i][3] == "xor"){
                // setting to NC - considering 0 for now
                values_for_fault[netlist[i][1]] = '0';
                back_propagate_to_head(netlist[i][1], values_for_fault[netlist[i][1]]);
                if(values_for_fault[netlist[i][0]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else{}
            
            values_for_fault[netlist[i][2]] = output_val;
            front_propagate(netlist[i][2], output_val);

        }else if(netlist[i][1] == wire && is_input !=1){
            is_input = 1;
            char output_val;

            if(netlist[i][3] == "nand"){
                // setting to NC
                values_for_fault[netlist[i][0]] = '1';
                back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                
                if(values_for_fault[netlist[i][1]] == '1')
                    output_val = '0';
                else
                    output_val = '1';

            }else if(netlist[i][3] == "or"){
                // setting to NC
                values_for_fault[netlist[i][0]] = '0';
                back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                if(values_for_fault[netlist[i][1]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else if(netlist[i][3] == "nor"){
                // setting to NC
                values_for_fault[netlist[i][0]] = '0';
                back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                if(values_for_fault[netlist[i][1]] == '1')
                    output_val = '0';
                else
                    output_val = '1';

            }else if(netlist[i][3] == "and"){
                // setting to NC
                values_for_fault[netlist[i][0]] = '1';
                back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                if(values_for_fault[netlist[i][1]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else if(netlist[i][3] == "xor"){
                // setting to NC - considering 0 for now
                values_for_fault[netlist[i][0]] = '0';
                back_propagate_to_head(netlist[i][0], values_for_fault[netlist[i][0]]);
                if(values_for_fault[netlist[i][1]] == '1')
                    output_val = '1';
                else
                    output_val = '0';

            }else{}
            
            values_for_fault[netlist[i][2]] = output_val;
            front_propagate(netlist[i][2], output_val);
        }
    }

    // if(is_input == 0){
    //     cout << "moye moye" << endl;
    //     back_propagate_to_head(wire, );
    // }

    return 0;
}

int justify_head1(string wire){

    int n = netlist.size();
    int is_input = 0;

    for(int i = 0; i < n; i++){
        if(netlist[i][0] == wire){
            // just give input of gate, output wire value, other input value
            values_for_fault[netlist[i][0]] = wire_val(netlist[i][3], values_for_fault[netlist[i][2]], values_for_fault[netlist[i][1]]);
        }

        if(netlist[i][1] == wire){
            values_for_fault[netlist[i][1]] = wire_val(netlist[i][3], values_for_fault[netlist[i][2]], values_for_fault[netlist[i][0]]);
        }
    }
    return 0;
}

char wire_val(string gate, char out, char in){
    char in2 = 'x';

    if(gate == "nand"){
        if(out == '1'){
            if(in == '1'){
                in2 = '0';
            }else if(in == '0'){
                in2 = 'x';
            } else if(in == 'x'){
                in2 = '0';
            }

        }else if(out == '0'){
            if(in == '1'){
                in2 = '1';
            }else if(in == '0'){
                cout << "requiring output of nand to be 0, with one output 0. Not possible" << endl;
            }else if(in == 'x'){
                in2 = '1';
            }

        }else if(out == 'x'){
            in2 = 'x';

        }else{}
    }else if(gate == "and"){
        if(out == '1'){
            if(in == '1'){
                in2 = '1';
            }else if(in == '0'){
                cout << "requiring output of and to be 1, with one output 0. Not possible" << endl;
            }

        }else if(out == '0'){
            if(in == '1'){
                in2 = '0';
            }else if(in == '0'){
                in2 = 'x';
            } else if(in == 'x'){
                in2 = '0';
            }

        }else if(out == 'x'){
            in2 = 'x';

        }else{}

    } else if(gate == "nor"){
        if(out == '1'){
            if(in == '1'){
                cout << "requiring output of nor to be 1, with one output 1. Not possible" << endl;
            }else if(in == '0'){
                in2 = '0';
            }

        }else if(out == '0'){
            if(in == '1'){
                in2 = 'x';
            }else if(in == '0'){
                in2 = '1';
            } else if(in == 'x'){
                in2 = '1';
            }

        }else if(out == 'x'){
            in2 = 'x';

        }else{}
    } else if(gate == "or"){
        if(out == '1'){
            if(in == '1'){
                in2 = 'x';
            }else if(in == '0'){
                in2 = '1';
            } else if(in == 'x'){
                in2 = '1';
            }

        }else if(out == '0'){
            if(in == '1'){
                cout << "requiring output of or to be 0, with one output 1. Not possible" << endl;

            }else if(in == '0'){
                in2 = '0';
            }

        }else if(out == 'x'){
            in2 = 'x';

        }else{}

    } else if(gate == "xor"){
        if(out == '1'){
            if(in == '1'){
                in2 = '0';
            }else if(in == '0'){
                in2 = '1';
            }else if(in == 'x'){
                in2 = '0';
            }

        }else if(out == '0'){
            if(in == '1'){
                in2 = '1';
            }else if(in == '0'){
                in2 = '0';
            }else if(in == 'x'){
                in2 = '0';
            }

        }else if(out == 'x'){
            in2 = 'x';

        }else{}
    }else{

    }
    return in2;
}
