#include <arpa/inet.h>
#include <memory.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

template <typename T> class TCPClient {
  public:
    TCPClient(int nServerPort, const char *strServerIP);
    virtual ~TCPClient();
    int Run();
    virtual void HandleFunction(int client_sock);

  private:
    char *server_ip_;
    int server_port_;
};