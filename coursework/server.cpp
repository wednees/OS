#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <zmq.h>
#include <unistd.h>

using namespace std;

#define MAX_SIZE 100

int generate_random_number(){
    int start = 1;
    int end = 500;
    int x = rand() % (end - start + 1) + start;
    return x;
}

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

int main(){

    void* context = zmq_ctx_new();
	void* respond_socket = zmq_socket(context, ZMQ_REP);
    zmq_bind(respond_socket, "tcp://127.0.0.1:5555");

    map <string, int> game_nums;
    map <string, int> game_players;
    map <string, queue<int>> game_queue_1;
    map <string, queue<int>> game_queue_2;

    for(;;){

        int id;

        char request[MAX_SIZE];
        char reply[MAX_SIZE];

        get_request(respond_socket, request);
        // команды:
        // connect id
        // create id key amount
        // is id key num
        // leave id name

        string command(strtok(request, " "));
         
        if (command == "create") {

            id = atoi(strtok(NULL, " "));
            string name_for_creation(strtok(NULL, " "));
            int amount_of_players = atoi(strtok(NULL, " "));

            if (game_nums[name_for_creation] != 0){
                cout << "Error: game already exist" << endl;
            }
            else{
                int game_number = generate_random_number();
                game_nums[name_for_creation] = game_number;
                game_players[name_for_creation] = amount_of_players;
                game_queue_1[name_for_creation].push(id);
            }
            
            sprintf(reply,"%d ok", id);
            send_reply(respond_socket, reply);
        }

        else if (command == "connect"){
            id = atoi(strtok(NULL, " "));
            const char* name_for_connect;

            for(const auto& [name, value]: game_players){
                if ((game_queue_1[name].size() + game_queue_2[name].size()) <= value){
                    game_queue_1[name].push(id);
                    name_for_connect = name.c_str();
                    break;
                }
            }

            sprintf(reply,"%d ok %s", id, name_for_connect);
            send_reply(respond_socket, reply);
        }

        else if (command == "is"){
            id = atoi(strtok(NULL, " "));
            string name_for_play(strtok(NULL, " "));
            int number = atoi(strtok(NULL, " "));

            if (game_queue_1[name_for_play].empty()){
                int tmp = game_queue_2[name_for_play].size();
                for(int i = 0; i < tmp; ++i){
                    int id_tmp = game_queue_2[name_for_play].front();
                    game_queue_2[name_for_play].pop();
                    game_queue_1[name_for_play].push(id_tmp);
                }
            }

            if (game_queue_1[name_for_play].front() != id){
                sprintf(reply, "%d wrongtime", id);
                send_reply(respond_socket, reply);
            }
            else {
                game_queue_1[name_for_play].pop();
                game_queue_2[name_for_play].push(id);
                if (number == game_nums[name_for_play]){
                    
                    sprintf(reply, "%d win", id);
                    send_reply(respond_socket, reply);
                    
                    while (!game_queue_1[name_for_play].empty()){
                        sprintf(reply, "%d otherwin", game_queue_1[name_for_play].front());
                        game_queue_1[name_for_play].pop();
                        send_reply(respond_socket, reply);
                    }

                     while (!game_queue_2[name_for_play].empty()){
                        sprintf(reply, "%d otherwin", game_queue_2[name_for_play].front());
                        game_queue_2[name_for_play].pop();
                        send_reply(respond_socket, reply);
                    }
                    //pobeda
                }
                else if (number > game_nums[name_for_play]){
                    sprintf(reply, "%d incorrect less", id);
                    send_reply(respond_socket, reply);
                    // men'che
                }
                else {
                    sprintf(reply, "%d incorrect more", id);
                    send_reply(respond_socket, reply);
                    // bol'che
                }
            }
        }

        else if (command == "leave"){
            id = atoi(strtok(NULL, " "));
            string name_for_leave(strtok(NULL, " "));
            int size_1 = game_queue_1[name_for_leave].size();

            for(int i = 0; i < size_1; ++i){
                int leave_tmp = game_queue_1[name_for_leave].front();
                game_queue_1[name_for_leave].pop();
                if (leave_tmp != id){
                    game_queue_2[name_for_leave].push(leave_tmp);
                }
                else{
                    break;
                }
            }

            int size_2 = game_queue_2[name_for_leave].size();

            for(int i = 0; i < size_2; ++i){
                int leave_tmp = game_queue_2[name_for_leave].front();
                game_queue_2[name_for_leave].pop();
                if (leave_tmp != id){
                    game_queue_1[name_for_leave].push(leave_tmp);
                }
                else{
                    break;
                }
            }
            // sprintf(reply, "%d stop", id);
            // send_reply(respond_socket, reply);
            // stop game
        }
    }

    return 0;
}