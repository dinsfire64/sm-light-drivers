#include "socket.h"

bool Socket::Init(string server) {
  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed with error: %d\n", iResult);
    return false;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  iResult = getaddrinfo(server.c_str(), DEFAULT_PORT, &hints, &result);
  if (iResult != 0) {
    printf("getaddrinfo failed with error: %d\n", iResult);
    WSACleanup();
    return false;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
      printf("socket failed with error: %ld\n", WSAGetLastError());
      WSACleanup();
      return false;
    }

    // Connect to server.
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
      closesocket(ConnectSocket);
      ConnectSocket = INVALID_SOCKET;
      continue;
    }
    break;
  }

  freeaddrinfo(result);

  if (ConnectSocket == INVALID_SOCKET) {
#if DEBUG
    printf("ConnectSocket == INVALID_SOCKET\n");
#endif
    WSACleanup();
    return false;
  }

  string_buff = "";

  return true;
}

string Socket::WaitForNewData() {
  int new_line_pos = string_buff.find('\r');

  if (new_line_pos > 0) {
    string new_info = string_buff.substr(0, new_line_pos);
    string_buff.erase(0, new_line_pos + 1);

    return new_info;
  } else if (string_buff[0] == '\r') {
    string_buff.erase(0, 1);
  } else {
    // Receive until the peer closes the connection
    do {

      iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

      if (iResult > 0) {
        // printf("Bytes received: %d\n", iResult);

        string_buff += string(recvbuf);
        memset(recvbuf, 0, DEFAULT_BUFLEN);

        break;
      }
      /*else if (iResult == 0)
      {
              //printf("Connection closed\n");
              return ERROR_CHAR;
      }*/
      else {
        // printf("recv failed with error: %d\n", WSAGetLastError());
        return ERROR_CHAR;
      }

    } while (iResult > 0);
  }

  return "";
}

bool Socket::Close() {
  // cleanup
  closesocket(ConnectSocket);
  WSACleanup();

  return true;
}
