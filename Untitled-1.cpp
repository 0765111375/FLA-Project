
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<tuple>
#include<stdexcept>

using namespace std;


struct pair_hash{
    template <class ta,class tb>
     size_t operator()(const pair<ta,tb>&p) const{
        auto h1=hash<ta>{}(p.first);
        auto h2=hash<tb>{}(p.second);
        return h1^(h2<<1);
        }

};

string decode(const string &encodedString){
    unordered_map<string, char>mapping ={{"00",'0'},{"01",'1'},{"10",'#'},{"11",';'}};
    string decoded="";
    for(int i=0;i<encodedString.length();i+=2){
        decoded+=mapping[encodedString.substr(i,2)];
    }
    return decoded;

}

unordered_map<pair<int,int>,tuple<int,int,int>,pair_hash> decodedTransitions(const string &decodedString){
   unordered_map<pair<int,int>,tuple<int,int,int>,pair_hash> transitions;
   size_t semicolon=0;
    string s=decodedString;
    while((semicolon=s.find(';'))!=string::npos){//=decodedstring
        string s1 = s.substr(0,semicolon);
        s.erase(0,semicolon+1);

        vector<int>parts;
        size_t  equal=0;
        while((equal=s1.find('#'))!=string::npos){
            parts.push_back(stoi(s1.substr(0,equal),nullptr,2));
            s1.erase(0,equal+1);
        }
        if(!s1.empty()){
            parts.push_back(stoi(s1, nullptr,2));
        }
        if(parts.size()!=5){
            //pair<int,int>from_state(parts[0],parts[1]);
            //tuple<int,int,int>to_state(parts[2],parts[3], parts[4]);
            throw runtime_error("Invalid transition");
            
        }
        transitions[{parts[0],parts[1]}]={parts[2],parts[3],parts[4]};

    }
    return transitions;
}

string TuringMachine(unordered_map<pair<int,int>,tuple<int,int,int>,pair_hash>&transitions,const string &str){
    vector<int>tape(1,0);
    for(char i: str){
        if(!isdigit(i)){
            return "Error";
        }
        tape.push_back(i-'0'+1);
    }
    int head=1;
    int q_i=0;
    while(true){
        int s_k=tape[head];
        if(transitions.find({q_i,s_k})==transitions.end()){
            return "Error";
        }
        auto [q_j,s_l,D] = transitions[{q_i,s_k}];
        tape[head]=s_l;
        q_i=q_j;
        if(q_i==1){
            return "Accept";
        }
        if(q_i==2){
            return "Reject";
        }
        if(D==0){
            if(head==0){
                tape.insert(tape.begin(),0);
            }
            else {
                head--;
            }
        
        }
        else{
            head++;
            if(head>=tape.size()){
                tape.push_back(0);
            }
        }
    }
}

int main(){
    //create a Turing machine that accepts random binary strings.
    string str;
    string s;
    getline(cin,str);
    getline(cin,s);
    try{
        string decodedT=decode(str);
        auto trans = decodedTransitions(decodedT);
        cout<<TuringMachine(trans,s)<<endl;
    }
    catch (const exception &e){
        cout<<"Error"<<endl;
    }
    


    return 0;

}