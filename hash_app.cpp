#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#define XXH_INLINE_ALL
#include <xxhash.h>
#include <csignal>
#include <filesystem>
#include <vector>
//inline constexpr std::string_view DIR_PATH = "./basic_hasing";
using namespace std;



string folder_name;
filesystem::path overflow_state_path;
vector<int> overflow_state;


void vector_to_overflow_state(vector<int> overflow_state){
    ofstream outfile(overflow_state_path);

    for (int i = 1; i < overflow_state.size(); i++) {
        outfile << overflow_state[i] << '\n';
    }
    outfile.close();
}

void read_overflow_state(){
    ifstream infile(overflow_state_path);
    string line;
    int i=1;
    while (std::getline(infile, line)) {
        overflow_state[i] = stoi(line);
        i++;
    }
    infile.close();
}
void create_hash_folder(int n){
    error_code ec;
    bool created = filesystem::create_directories(filesystem::path(folder_name));
    overflow_state.clear();
    overflow_state.resize(n+1,1);
    if (ec) {
        cout << "Filesystem error: " << ec.message() << '\n';
        
    } 
    else if (created) {
        for(int i = 1; i<=n ; i++){
            string file_name = to_string(i)+".txt";
            ofstream outfile(filesystem::path(folder_name)/file_name);
            outfile.close();
        }
        
        vector_to_overflow_state(overflow_state);
        cout << "Folder created successfully" << '\n';
    } 
    else {
        if(filesystem::exists(overflow_state_path) && filesystem::file_size(overflow_state_path)==n) read_overflow_state();
        cout << "Folder already exists. Existing contents were preserved.\n";
    }
}




string get_overflow_bucket_name(int bucket_id, int overflow_id){
    return  to_string(bucket_id)+"_overflow" +to_string(overflow_id) + ".txt";
}
uint64_t get_hash(string s){
    return XXH3_64bits(s.data(), s.size());
}
pair<filesystem::path , string> find_empty_bucket(string str, int n ,int s){
    uint64_t hashValue = get_hash(str);
    int string_length = str.size();
    uint64_t bucket_id = (hashValue%((uint64_t)n))+1;
    string file_name = to_string(bucket_id)+".txt";
    filesystem::path path = filesystem::path(folder_name)/file_name;
    int size = filesystem::file_size(path);
    if (size+string_length<=s){
        return {path,file_name};
    }
    
    if(filesystem::exists(overflow_state_path)){
        int overflow_id = overflow_state[bucket_id];
        string file = get_overflow_bucket_name(bucket_id,overflow_id);
        path = filesystem::path(folder_name)/file;
        if(!filesystem::exists(path) || filesystem::file_size(path)+string_length<=s){return {path, file};}
        overflow_state[bucket_id]++;
        file = get_overflow_bucket_name(bucket_id,overflow_state[bucket_id]);
        path = filesystem::path(folder_name)/file;
        vector_to_overflow_state(overflow_state);
        return {path, file};
    }
    //the below block runs if overflow_state.txt DNE
    int overflow_id = 1;
    file_name = get_overflow_bucket_name(bucket_id,overflow_id);
    path = filesystem::path(folder_name)/file_name;
    while(filesystem::exists(path)){
        overflow_id++;
        file_name = get_overflow_bucket_name(bucket_id,overflow_id);

        path = filesystem::path(folder_name)/file_name;
        
    }
    string file_1= get_overflow_bucket_name(bucket_id,overflow_id-1);

    string file_2 = get_overflow_bucket_name(bucket_id,overflow_id);
    filesystem::path path_1 = filesystem::path(folder_name)/file_1;
    filesystem::path path_2 = filesystem::path(folder_name)/file_2;
    if((filesystem::exists(path_1) && filesystem::file_size(path_1)+string_length<=s)){return {path_1, file_1};}
    return {path_2,file_2};
}
void put_string_into_bucket(string str, int n,int s){
    pair<filesystem::path , string> bucket= find_empty_bucket(str, n , s);
    ofstream outfile(bucket.first,ios::app);
    
    outfile <<str;
    outfile.close();
    cout<<str+" added to "+bucket.second<<'\n';
    
}



int main() {
    int n=0,s=0;
    cout<<"Enter number of hash buckets 'n' and bucket size 's':\n";
    while (!(cin >> n >> s) || n <= 0 || s <= 0) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Invalid n or s!\nEnter number of hash buckets 'n' and bucket size 's':\n";
    }
    folder_name = "hash(" + to_string(n) + "," + to_string(s) + ")";
    overflow_state_path = filesystem::path(folder_name)/"overflow_state.txt";
    create_hash_folder(n);
    string str;
    cout<<"Please enter the string:"<<'\n';
    while(cin>>str){
        if (str.size() > static_cast<size_t>(s)) {
            cout << "Too large to fit in any bucket!\nPlease enter the string:\n";
            continue;
        }
        
        put_string_into_bucket(str,n,s);
        cout<<"Please enter the string:"<<'\n';
        
    }
    
    return 0;
}