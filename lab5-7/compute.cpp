#include <iostream>
#include <cstdio>
#include <map>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

#include "topology.hpp"

using namespace std;

void get_request(void* socket, char* request) {
    zmq_msg_t request_msg;
    zmq_msg_init(&request_msg);
    zmq_msg_recv(&request_msg, socket, 0);
    memcpy(request, zmq_msg_data(&request_msg), zmq_msg_size(&request_msg));
    zmq_msg_close(&request_msg);
}

void send_reply(void* socket, char* message) {
    zmq_msg_t reply_msg;
    zmq_msg_init_size(&reply_msg, strlen(message)+1);
    memcpy(zmq_msg_data(&reply_msg), message, strlen(message)+1);
    zmq_msg_send(&reply_msg, socket, 0);
    zmq_msg_close(&reply_msg);
}

void print_map(const map<string, int>& m){
    for (const auto& [key, value] : m)
        cout << '[' << key << "] = " << value << "; " << endl;;
}



int main(int argc, char* argv[]) {

    printf("Ok: %d\n", getpid());

    int id = atoi(argv[1]);
    map<string, int> dictionary; 

    void* context = zmq_ctx_new();
	void* respond_socket = zmq_socket(context, ZMQ_REP);

    char adress[100];
    sprintf(adress, "tcp://127.0.0.1:%d", 5555 + id);
    zmq_bind(respond_socket, adress);

    for(;;) {

        char request[100];
        char reply[100];

        get_request(respond_socket, request);

        if (!strcmp(request, "print")) {
            cout << "Node " << id << ": content of the dictionary:" << endl;
            print_map(dictionary);
            sprintf(reply, "Ok:%d", id);

        } else {
            char* command = strtok(request, " ");
            char* key = strtok(NULL, " ");
            string key_fin(key);

            if (!strcmp(command, "save")) {
                int value = atoi(strtok(NULL, " "));
                dictionary[key_fin] = value;
                cout << "Node " << id << ": key " << key_fin << "was saved with value " << value << endl;
                sprintf(reply, "Ok: %d", id);
    
            } else if (!strcmp(command, "return")) {
                if (dictionary[key_fin] != 0){
                    cout << "Node " << id << ": value is " <<  dictionary[key_fin] << endl;
                }
                else{
                    cout << "There is no such value in the dictionary" << endl;
                }
                sprintf(reply, "Ok:%d:%d", id, dictionary[key_fin]);
            }
        }
        send_reply(respond_socket, reply);
    }
}