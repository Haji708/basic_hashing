# basic_hashing
the hashing library used:"https://github.com/Cyan4973/xxHash".
install any C++ compiler of your choice(preferably GCC g++).
if you are using vscode(like I did), You can install the C/C++ runner extension to build,compile and run using the vscode UI.
Or use the following commands based on your OS and compiler once you are in the working directory.
Linux/MAC
g++ -std=c++17 -O2 main.cpp -o hash_buckets
./hash_buckets

Windows (MinGW / MSYS2 g++)
g++ -std=c++17 -O2 main.cpp -o hash_buckets.exe
.\hash_buckets.exe

Overflow_buckets are located by means of 'overflow_state.txt' state file, the file stores the id of the last overflow bucket used by each bucket. It is the read into a
vector of integers "overflow_state"(the ids are updated every time a new overflow bucket is created). if 'overflow_state.txt' is somehow deleted, the mechanism falls back to linear search by iterating through overflow buckets to find the last one.

Tampering with the contents of 'overflow_state.txt' may break the app, so please keep it intact.