#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<tuple>
#include<stdexcept>
using namespace std;

struct pair_hash{
    template<typename T1, typename T2> 
    size_t operator()(const pair<T1,T2>&p)const{
        return hash<T1>()(p.first)^(hash<T2>()(p.second)<<1);
    }
};

unordered_map<pair<int,int>, tuple<int,int,int>, pair_hash>TransitionTable( const string &encodedString){
    unordered_map<pair<int,int>, tuple<int, int, int>, pair_hash> transitions;
    unordered_map<string, char>mapping={{"00",'0'},{"01",'1'},{"10",'#'},{"11",';'}};
    string decodedString="";

    for(size_t i=0;i<encodedString.length();i+=2){
        decodedString+=mapping[encodedString.substr(i,2)];
    }
    
    
    size_t track = 0;
    while((track=decodedString.find(';'))!=string::npos){
        string transition = decodedString.substr(0,track);
        decodedString.erase(0,track+1);
        size_t t1=transition.find('#');
        size_t t2=transition.find('#',t1+1);
        size_t t3 = transition.find('#',t2+1);
        size_t t4= transition.find('#',t3+1);

        if(t1==string::npos || t2==string::npos || t3==string::npos || t4==string::npos){
            continue;
        }

         int qi=stoi(transition.substr(0,t1),nullptr,2);
         int sk=stoi(transition.substr(t1+1,t2-t1-1),nullptr,2);
         int qj=stoi(transition.substr(t2+1,t3-t2-1),nullptr,2);
         int sl=stoi(transition.substr(t3+1,t4-t3-1),nullptr,2);
         int D=stoi(transition.substr(t4+1),nullptr,10);
         
         transitions[{qi,sk}]={qj,sl,D};
    }
    return transitions;
}
string TuringMachine(const unordered_map<pair<int, int>, tuple<int,int,int>, pair_hash>&transitions, const string &inputString){
    vector<int>tape;
    for(size_t i=0;i< inputString.length();i++){
        if(inputString[i]<'0' || inputString[i]>'9'){
            return "Error";
        }
        tape.push_back(inputString[i]-'0'+1);
    }
    int head=0;
    int state=0;
    const int accept_state=1;
    const int reject_state=2;
    const int blank=10;//check
    tape.push_back(blank);
    while(true){
        int symbol;
        if(head>=0 && head<tape.size()){
            symbol=tape[head];
        }
        else{
            symbol=blank;
        }

        auto found = transitions.find({state,symbol});
        if(found==transitions.end()){
            return "Error";
        }

        auto [newState,newSymbol,direction]=found->second;
        tape[head]=newSymbol;
        state=newState;
        if(state==accept_state){
            return "Accept";
        }
        else if(state==reject_state){
            return "Reject";
        }

        if(direction ==0){
            head-=1;
        }
        else{
            head+=1;
        }
        
        if(head<0){
            tape.insert(tape.begin(),blank);
            head=0;
        }
        else if(head>=tape.size()){
            tape.push_back(blank);
        }

    }  
    
}
int main(){
    string encodedString, inputString;
    cin>>encodedString;
    cin>>inputString;
    //getline(cin,encodedString);
    //getline(cin,inputString);

    auto transitions = TransitionTable(encodedString);
    cout<<TuringMachine(transitions, inputString)<<endl;


}


