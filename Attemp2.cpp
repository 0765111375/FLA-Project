#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<sstream>

using namespace std;

map<pair<int,int>,tuple<int,int,int>>DecodingTransitionTable(const string &encodedTransition){
    map<string,char>mapping={
        {"00",'0'},
        {"01",'1'},
        {"10",'#'},
        {"11",';'}
    };
    string decodedString;
    for(size_t i=0;i < encodedTransition.size();i+=2){
        decodedString+=mapping[encodedTransition.substr(i,2)];
    }
    stringstream ss(decodedString);
    string transitions;
    map<pair<int,int>,tuple<int,int,int>>TransitionTable;
    while(getline(ss,transitions,';')){
        if(transitions.empty()){
            continue;
        }
        vector<string>subStrings;
        stringstream tc(transitions);
        string subString;
        while(getline(tc,subString,'#')){
            subStrings.push_back(subString);
        }
        if(subStrings.size()!=5){
            continue;
        }
        try{
            int qi=stoi(subStrings[0],nullptr,2);
            int sk=stoi(subStrings[1],nullptr,2);
            int qj=stoi(subStrings[2],nullptr,2);
            int sl=stoi(subStrings[3],nullptr,2);
            char D;
            if(subString[0]=='0'){
                D=0;
            }else{
                D=1;
            }
            TransitionTable[{qi,sk}]={qj,sl,D};
        }
        catch(...){
            continue;
        }
    }
    return TransitionTable;
}

string TuringMachine(const string &encodedTransition, const string &inputString){
    auto TransitionTable = DecodingTransitionTable(encodedTransition);
    vector<int>tape;
    for(size_t i=0;i<inputString.length();i++){
        tape.push_back(inputString[i]-'0'+1);
    }
    int head=0;
    int qCurrent=0;
    const int acceptState=1;
    const int rejectState=2;
    const int blank=0;
    while(true){
        if(qCurrent==acceptState){
            return "Accept";
        }
        if(qCurrent==rejectState){
            return "Reject";
        }

        int sCurrent;
        if(head<0 || head>=tape.size()){
            sCurrent=0;
        }else{
            sCurrent=tape[head];
        }

        auto key=make_pair(qCurrent,sCurrent);
        if(TransitionTable.find(key)==TransitionTable.end()){
            return "Error";
        }

        auto [qNext,sNext,direction]=TransitionTable[key];
        if(head<0){
            tape.insert(tape.begin(),sCurrent);
            head=0;
        }
        else if(head>=tape.size()){
            tape.push_back(sCurrent);
            head=tape.size()-1;
        }
        else{
            tape[head]=sCurrent;
        }
        
        if(direction==1){
            head++;
        }
        else{
            head--;
        }
        
        qCurrent=qNext;
    }
}

int main(){
    string encodingTransition, x;
    cin>>encodingTransition;
    cin>>x;

    auto encodingString=DecodingTransitionTable(encodingTransition);
    cout<<TuringMachine(encodingTransition,x)<<endl;

    return 0;
}
