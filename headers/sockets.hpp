#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#define PORT 8080

#include <chrono>
#include <thread>
bool login();
class server_client_socket {
	public:
	static const char msg_break = '\n';
	static const unsigned int buffer_size = 1024;
	char buffer[buffer_size+1];
	std::deque<std::string> messages;
	std::string readed;
	long unsigned int readedindex = 0;
	int socket_id;
	server_client_socket(){
		buffer[buffer_size] = '\0';
	}
	server_client_socket(int socket): server_client_socket() {
		socket_id = socket;
		int iMode = 1;
		ioctl(socket_id, FIONBIO, &iMode);  //non-blocking
	}
	ssize_t sendtoclient(std::string &str) {
		cout << "Socket Out: " << str;
		return send(socket_id, &str[0], str.length(), 0);
	}
	int next(){
		if(messages.empty()) return -1;
		messages.pop_front();
		return 0;
	}
	bool isempty(){
		return messages.empty();
	}
	std::string &get(){
		return messages.front();
	}
	ssize_t recvfromclient() {
		long int val;
		//memset(buffer, -1, 1024);
		val = read(socket_id, buffer, 1024);
		if (val == -1) {
			perror("socket error");
			return -1;
		} else {
			/* readedindex = 0;
			readed.clear(); */
			buffer[val] = '\0';//packet end
			long unsigned int msg_sz;
			char *find = strchr(buffer, msg_break);
			char* next = buffer;
			while (find <= &buffer[val] && find != NULL){
				msg_sz = (unsigned long)(find - next);
				//if (readed.capacity() < readedindex + msg_sz) readed.reserve(readedindex + msg_sz + 100);
				//memcpy(&readed[readedindex], next, msg_sz);
				readed.append(next, msg_sz);
				readedindex += (long unsigned)val;
				messages.push_back(readed);
				readedindex = 0;
				readed.clear();
				//if (readed.capacity() < readedindex + msg_sz) readed.reserve(readedindex + msg_sz + 100);
				next = find +1;
				find = strchr(next, msg_break);
				msg_sz = (unsigned long)(find - next);
				if(find == NULL){
					msg_sz = strlen(next);
				}
				//memcpy(&readed[readedindex], next, msg_sz);
				readed.append(next, msg_sz);
				readedindex += msg_sz;
			}
			if (!(find <= &buffer[val])) {
				msg_sz = (long unsigned)val;
				//if(readed.capacity() < readedindex + msg_sz)readed.reserve(readedindex + msg_sz + 100);
				//memcpy(&readed[readedindex], buffer, msg_sz);
				readed.append(next, msg_sz);
				readedindex += msg_sz;
			}
		}
		return val;
	}
	~server_client_socket(){
		std::string goodbye = "Closing connection";
		sendtoclient(goodbye);
		close(socket_id);
	}
};
server_client_socket *cliente;

void recv_loop(){
	while(1){
		std::chrono::steady_clock::time_point tp = std::chrono::steady_clock::now() + std::chrono::milliseconds(20);
		cliente->recvfromclient();
		std::this_thread::sleep_until(tp);
	}
}

void server() {
	int server_fd, new_socket, valread = 0;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	std::string hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
			 sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) > 0){
		cliente = new server_client_socket(new_socket);
		std::thread(recv_loop).detach();
		std::thread(login).detach();
	}
	perror("accept");
	exit(EXIT_FAILURE);

	//valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello.c_str(), hello.length(), 0);
	printf("Hello message sent %d\n", valread);
	return;
}