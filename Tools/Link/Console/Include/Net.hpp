
#pragma once
#include <bjs/brew.js>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <switch.h>
#include <iostream>
#include <string>
using namespace std;

namespace bjs::link
{
    void Initialize(u16 Port);
    void WaitForConnection();
    void Finalize();
    const char *WaitReceive();
    void SendResult(EvaluationResult JSResult);
    bool IsOK();
}