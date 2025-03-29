#include <iostream>
#include <string>
#include<vector>
#include<unordered_map>
using namespace std;



int main()
{
string table;
string s;
size_t track=0;
unordered_map<char,string>store;
//take in the user inputs
getline(cin,table);
getline(cin,s);
//split the string by a character
while(track<table.length()){
    size_t delimeter = table.find('|',track);
    if(delimeter==string::npos){
        break;
    }
    string entry = table.substr(track,delimeter-track);
    track=delimeter+1;
    if(entry.empty()){
        continue;
    }
    size_t equal = entry.find('=');
    if(equal==string::npos){
        continue;
    }
    char key=entry[0];
    string value = entry.substr(equal+1);
    store[key]=value;
}
string word;
for(char i:s){
    word+=store[i];
}
cout<<word<<endl;
    return 0;
}
