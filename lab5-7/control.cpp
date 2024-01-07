#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <zmq.h>
#include "topology.hpp"

void print_menu() {
    cout << "Menu:" << endl;
    cout << "\t-create <child_id> <parent_id> - create a new node" << endl;
    cout << "\t-exec <id> <KEY> <VALUE> - save value in the local dictionary or print saved value" << endl;
    cout << "\t-print <id> - print the contents of the local dictionary" << endl;
    cout << "\t-pingall - availability check" << endl;
    cout << "\t-exit - stop programm" << endl;
}

void send_and_receive(Node* root, int id, char* message, char* reply) {
    Node* node = search(root, id);
    node->is_available = false;
    void* context = zmq_ctx_new();
    void* request_socket = zmq_socket(context, ZMQ_REQ);

    char adress[100];
    sprintf(adress, "tcp://127.0.0.1:%d", 5555 + id);
    zmq_connect(request_socket, adress);

    zmq_msg_t req_msg;
    zmq_msg_init_size(&req_msg, strlen(message) + 1);
    memcpy(zmq_msg_data(&req_msg), message, strlen(message) + 1);
    zmq_msg_send(&req_msg, request_socket, 0);
    zmq_msg_close(&req_msg);

    // cout << "Control: sendind a message '" << message << "' to " << id << endl;

    zmq_msg_t rep_msg;
    zmq_msg_init(&rep_msg);
    zmq_msg_recv(&rep_msg, request_socket, 0);
    memcpy(reply, zmq_msg_data(&rep_msg), zmq_msg_size(&rep_msg));
    zmq_msg_close(&req_msg);

    // cout << "Control: message from node '" << reply << "'" << endl;

    zmq_close(request_socket);
    zmq_ctx_destroy(context);
    node->is_available = true; 
}

void pingall(Node* root, int* nodes, int* len) {
    if(!root->is_available && root->id != -1){
        nodes[*len] = root->id;
        *len += 1;
    }
    for (size_t i = 0; i < root->_children.size(); ++i){
        pingall(root->_children[i], nodes, len);
    }
}



int main() {
    int id;
    Node* root = create_node(-1);
    print_menu();
    cout << "Enter your command:" << endl;
    char command[100];
    char reply[100];
    while (scanf("%s", command) != EOF) {

        if (!strcmp(command, "create")) {
            int id_par, id_ch;
            cin >> id_ch >> id_par;
            if (exist(root, id_ch)) {
                printf("Error: Already exist\n");
                continue;
            }
            if (!exist(root, id_par)) {
                printf("Error: There is no such parent\n");
                continue;
            }

            Node* par = search(root, id_par);    
            add_node(par, id_ch);

            int pid = fork();
            if (pid < 0) {
                printf("Error: Fork error\n");
                exit(-1);
            } 
            if (pid == 0) {
                char str_id[100];
                sprintf(str_id, "%d", id_ch);
                int err = execl("compute", "compute", str_id, NULL);
                if (err == -1) {
                    printf("Error: Execl error");
                }
            }

        } else if (!strcmp(command,"exec")) {
            char key[100];
            char buffer[100];
            cin >> id >> key;
            fgets(buffer, sizeof(char) * 100, stdin);
            buffer[strlen(buffer)-1] = '\0';

            if (!exist(root, id)) {
                printf("Error: %d Not found\n", id);
                continue;
            }

            char message[1000];
            if (!strcmp(buffer, "")) {
                sprintf(message, "return %s", key);
            } else {
                sprintf(message, "save %s%s", key, buffer);
            }
            send_and_receive(root, id, message, reply);

        } else if (!strcmp(command, "print")) {
            cin >> id;
            if (!exist(root, id)) {
                printf("Error: Not found\n");
                continue;
            }
            send_and_receive(root, id, "print", reply);

        } else if (!strcmp(command, "pingall")) {
            int len = 0;
            int unvailable_nodes[100];
            pingall(root, unvailable_nodes, &len);
            cout << "Ok: ";
            if (len == 0) {
                cout << "-1" << endl;
            } else {
                for (int i = 0; i < len; i++) {
                    cout << unvailable_nodes[i] << ";";
                }
                cout << endl;
            }

        } else if (!strcmp(command,"exit")) {
            delete_topology(root);
            break;
        } else {
            cout << "Unknown command." << endl;
        }
    }
    return 0;
}