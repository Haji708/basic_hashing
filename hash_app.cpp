#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#define XXH_INLINE_ALL
#include <xxhash.h>
#include <filesystem>
inline constexpr std::string_view DIR_PATH = "./basic_hasing";
using namespace std;



string folder_name;

void create_hash_folder(int n){
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
    
    int overflow_id = 1;
    file_name = to_string(bucket_id)+"_overflow" +to_string(overflow_id) + ".txt";
    path = filesystem::path(folder_name)/file_name;
    while(filesystem::exists(path)){
        overflow_id++;
        file_name = to_string(bucket_id)+"_overflow" +to_string(overflow_id) + ".txt";
        path = filesystem::path(folder_name)/file_name;
        
    }
    string file_1 = to_string(bucket_id)+"_overflow" +to_string(overflow_id-1) + ".txt";
    string file_2 = to_string(bucket_id)+"_overflow" +to_string(overflow_id) + ".txt";
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