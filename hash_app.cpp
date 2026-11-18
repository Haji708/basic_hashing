#include <iostream>
#include <fstream>
#include <string>

#define XXH_INLINE_ALL
#include <xxhash.h>
#include <filesystem>
inline constexpr std::string_view DIR_PATH = "./basic_hasing";
using namespace std;

void create_hash_folder(int n , int s){
    string folder_name = "hash(" + std::to_string(n) + "," + to_string(s) + ")";
    error_code ec;
    bool created = filesystem::create_directories(filesystem::path(folder_name));

    if (ec) {
        cout << "Filesystem error: " << ec.message() << '\n';
        
    } 
    else if (created) {
        for(int i = 1; i<=n ; i++){
            string file_name = to_string(i)+".txt";
            ofstream outfile(filesystem::path(folder_name)/file_name);
            outfile.close();
        }
        cout << "Folder created successfully" << '\n';
    } 
    else {
        cout << "Folder already exists. Existing contents were preserved.\n";
    }
}
uint64_t get_hash(string s){
    return XXH3_64bits(s.data(), s.size());
}
void put_string_into_bucket(string str, int n,int s){
    uint64_t hashValue = get_hash(str);
    uint64_t bucket_id = (hashValue%((uint64_t)n))+1;
    string folder_name = "hash(" + to_string(n) + "," + to_string(s) + ")";
    string file_name = to_string(bucket_id)+".txt";

    ofstream outfile(filesystem::path(folder_name)/file_name,ios::app);

    if (!outfile) {
        std::cerr << "Error opening file for writing!\n";
        return ;
    }

    
    outfile <<str;
    outfile.close();
    cout<<str+" added to "+file_name<<'\n';
    
}



int main() {
    int n,s;
    cout<<"Enter number of hash buckets 'n' and bucket size 's'";
    cin>>n>>s;
    create_hash_folder(n,s);

    string str;
    cout<<"Please enter the string:"<<'\n';
    while(cin>>str){
        
        put_string_into_bucket(str,n,s);
        cout<<"Please enter the string:"<<'\n';
    }
        
    return 0;
}