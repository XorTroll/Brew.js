#include "Net.hpp"

struct sockaddr_in server;
struct sockaddr_in client;
int sock;
int inew,i;
int sockaddr_len = sizeof(struct sockaddr_in);
int data_len = 1;
char srvdata[4096];
bool disc = false;

void _neterror(const char *errorstr)
{
    consoleExit(NULL);
    consoleInit(NULL);
    printf("\n\n   Brew.js link aborted execution because an error ocurred:\n   - ");
    printf("%s", errorstr);
    printf("\n\n   Press A to exit this application.");
    while(true)
    {
        hidScanInput();
        int k = hidKeysDown(CONTROLLER_P1_AUTO);
        if(k & KEY_A) break;
        consoleUpdate(NULL);
    }
    bjs::link::Finalize();
    consoleExit(NULL);
    exit(0);
}

namespace bjs::link
{
    void Initialize(u16 Port)
    {
        socketInitializeDefault();
        if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) _neterror("Error creating socket");
        server.sin_family = AF_INET;
        server.sin_port = ntohs(Port);
        server.sin_addr.s_addr = INADDR_ANY;
        bzero(&server.sin_zero, 8);
        if((bind(sock, (struct sockaddr*)&server, sockaddr_len)) == -1) _neterror("Error binding socket");
        if((listen(sock, 50)) == -1) _neterror("Error listening socket");
    }

    void WaitForConnection()
    {
        if((inew = accept(sock, (struct sockaddr*)&client, (socklen_t*)&sockaddr_len)) == -1) _neterror("Error accepting socket");
    }

    void Finalize()
    {
        close(sock);
        socketExit();
    }

    const char *WaitReceive()
    {
        if(!data_len)
        {
            disc = true;
            close(inew);
            return NULL;
        }
        srvdata[data_len] = '\0';
        data_len = recv(inew, srvdata, sizeof(srvdata), 0);
        if(data_len == -1) _neterror("Error receiving source code from PC client");
        srvdata[data_len] = '\0';
        if(data_len) return srvdata;
        else return NULL;
    }

    void SendResult(EvaluationResult JSResult)
    {
        string sres = JSResult.AsString();
        string succ = (JSResult.IsSuccess() ? "true" : "false");
        string ecode = to_string(JSResult.GetErrorCode());
        string type = "Undefined";
        string all = succ + "|" + sres + "|" + ecode + "|" + type;
        const char *call = all.c_str();
        if(send(inew, call, strlen(call), 0) == -1) _neterror("Error sending evaluation result to PC client");
    }

    bool IsOK()
    {
        return !disc;
    }
}