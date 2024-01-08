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

void print_menu(){
    cout << "Добро пожаловать в игру 'Быки и Коровы'" << endl;
    cout << "Доступные команды:" << endl;
    cout << "  -create <NAME> <AMOUNT OF PLAYERS> - создать игру" << endl;
    cout << "  -connect - подключиться к игре" << endl;
    cout << "  -exit - выйти из игры" << endl;
    cout << "Команды внутри игры:" << endl;
    cout << "  -<VALUE> - попытаться угадать загаданное число" << endl;
    cout << "  -leave - покуинуть текущую игру" << endl;
}

void send_and_receive(char* message, char* reply) {
    void* context = zmq_ctx_new();
    void* request_socket = zmq_socket(context, ZMQ_REQ);

    zmq_connect(request_socket, "tcp://127.0.0.1:5555");

    zmq_msg_t req_msg;
    zmq_msg_init_size(&req_msg, strlen(message) + 1);
    memcpy(zmq_msg_data(&req_msg), message, strlen(message) + 1);
    zmq_msg_send(&req_msg, request_socket, 0);
    zmq_msg_close(&req_msg);

    // cout << "Control: sendind a message '" << message << "'" << endl;

    zmq_msg_t rep_msg;
    zmq_msg_init(&rep_msg);
    zmq_msg_recv(&rep_msg, request_socket, 0);
    memcpy(reply, zmq_msg_data(&rep_msg), zmq_msg_size(&rep_msg));
    zmq_msg_close(&req_msg);

    // cout << "Control: message from node '" << reply << "'" << endl;

    zmq_close(request_socket);
    zmq_ctx_destroy(context);
}

int generate_random_number(){
    int start = 1;
    int end = 40;
    int x = rand() % (end - start + 1) + start;
    return x;
}

bool check(char* reply, int id){
    if(atoi(strtok(reply, " ")) == id){
        return true;
    }
    return false;
}

int main(){

    print_menu();

    int id = generate_random_number();
    string command;
    char reply[MAX_SIZE];
    char message[MAX_SIZE];

    // команды:
    // create name amount
    // connect
    // игра
    // // number
    // // leave
    // exit

    while(true){
        cin >> command;

        if (command == "create"){
            string name_for_create;
            int amount_of_players;

            cin >> name_for_create >> amount_of_players;

            sprintf(message, "create %d %s %d", id, name_for_create.c_str(), amount_of_players);
            send_and_receive(message, reply);

            if(check(reply, id)){
                //strtok(reply, " ");
                string is_ok(strtok(NULL, " "));
                if (is_ok == "ok"){
                    cout << "Игра с именем '" << name_for_create << "' была успешно создана" << endl;
                }
                else{
                    cout << "Ошибка" << endl;
                }
            }
        }

        else if (command == "connect"){
            sprintf(message, "connect %d", id);
            send_and_receive(message, reply);

            if(check(reply, id)){
                //strtok(reply, " ");
                string is_ok(strtok(NULL, " "));
                string name(strtok(NULL, " "));

                if (is_ok == "ok"){
                    cout << "Выполнено подключение к игре '" << name << "'" << endl;
                    cout << "Игра началась, попробуйте отгадать число" << endl;

                    while(true){
                        string game_command;
                        cin >> game_command;

                        if (game_command == "leave"){
                            sprintf (message, "leave %d %s", id, name.c_str());
                            send_and_receive(message, reply);
                            cout << "Спасибо за игру!" << endl;
                            break;
                        }
                        else{
                            int t = stoi(game_command);
                            
                            sprintf(message, "is %d %s %d", id, name.c_str(), t);
                            send_and_receive(message, reply);
                            if (check(reply, id)){
                                //strtok(reply, " ");
                                string result(strtok(NULL, " "));
                                if (result == "wrongtime"){
                                    cout << "Не ваша очередь!" << endl;
                                    continue;
                                }

                                else if (result == "win"){
                                    cout << "Поздравляем! Вы угадали!" << endl;
                                    break;
                                }

                                else if (result == "otherwin"){
                                    cout << "Увы! Другой игрок угадал раньше вас" << endl;
                                    break;
                                }

                                else if (result == "incorrect"){
                                    string help(strtok(NULL, " "));
                                    if (help == "more"){
                                        cout << "Неверно! Загаданное число больше вашего" << endl;
                                        continue;
                                    }
                                    
                                    else if (help == "less"){
                                        cout << "Неверно! Загаданное число меньше вашего" << endl;
                                        continue;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        else if (command == "exit") {
            break;
        }

        else{
            cout << "Неизвестная команда" << endl;
        }
    }

    return 0;
}