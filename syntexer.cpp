#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector> 
using namespace std;
#include "parser.tab.h"

extern int yyparse();

queue<string> task_queue;
mutex mut;
condition_variable cv;
bool query_processing = false;
bool ready = false; 


// Flex-specific functions needed for string processing
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yyparse();


void query_input_handle() {
    while (true) {
        string query_buffer;
        cout << "Enter your query: ";
        // Use getline to allow spaces in queries 
        //first i used the  cin>>query_buffer;
        
        getline(cin, query_buffer);
        ready = false;
        unique_lock<mutex> lock(mut);
        task_queue.push(query_buffer);
        
        cv.notify_one();
        cout << "\n Query  input  done now procceding further \n"<<endl;

        // Wait until ready becomes true
        cv.wait(lock, [] { return ready; });
        cout<<"\n Last query processing is done \n "<<endl;



    }
}

void token_syn() {
    
    while (true) {
        //here using the unique_lock insted of lock_guard becaseu manual lock and unlock is required 
        unique_lock<mutex> lock(mut); 
        
        // Wait until the queue is not empty

        cv.wait(lock, [] { return !task_queue.empty(); });

         string query_to_parse = task_queue.front();
        task_queue.pop();
        lock.unlock(); 

        cout << "--- Validating SQL Syntax ---" << endl;

        // 1. Tell Flex to use the user's string buffer
        YY_BUFFER_STATE buffer = yy_scan_string(query_to_parse.c_str());

        // 2. Call Bison's parser
        if (yyparse() == 0) {
            cout << "[SUCCESS] Query is syntactically correct." << endl;
        } else {
            cout << "[FAILURE] Query has syntax errors." << endl;
        }

        // 3. Clean up Flex buffer memory
        yy_delete_buffer(buffer);

        // Notify input thread we are finished
        lock.lock();
        ready = true;
        cv.notify_one(); 
    }
}


int main() {
    //query input thread 
    thread query_input(query_input_handle);
    //tokenizer and  syntexer thread 
    
    thread tokenizer_syntexe(token_syn);

    query_input.join();
    tokenizer_syntexe.join();

    return 0;
}

