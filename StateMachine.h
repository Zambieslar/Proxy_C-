#include <iostream>
#include <iterator>
#include <ostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

#define LINE_FEED 0x0A
#define CARRIAGE_RETURN 0x0D
#define SPACE 0x20

namespace Parser {
class StateMachine {
private:
  enum States {
    Method,
    Path,
    Version,
    Host,
    Headers,
    Body,
  };

  struct HttpHeader {
    std::string Key;
    std::string Value;
  };

  struct Request {
    std::string Method;
    std::string Path;
    std::string Version;
    std::vector<HttpHeader> Headers;
  };

  struct Machine {
    States State = Method;
    int index = 0;
    std::string token;
    HttpHeader header;
  };

public:
  bool isLF(char c) {
    if (c == LINE_FEED) {
      return true;
    } else {
      return false;
    }
  }

  bool isCR(char c) {
    if (c == CARRIAGE_RETURN) {
      return true;
    } else {
      return false;
    }
  }

  bool isSP(char c) {
    if (c == SPACE) {
      return true;
    } else {
      return false;
    }
  }

  bool isHeaderDelim(char c) {
    if (c == ':') {
      return true;
    } else {
      return false;
    }
  }

  void acquireToken(char *buf, Machine *machine) {
    // Check for delimiter. If a delimiter is reached (CR, LF, SP) then itterate
    // the index past the delimiter in the char array.
    while ((isSP(buf[machine->index]) || isCR(buf[machine->index]) ||
            isLF(buf[machine->index]))) {
      machine->index++;
    }
    // Capture chars while delimiter is false.
    while (!(isSP(buf[machine->index]) || isCR(buf[machine->index]) ||
             isLF(buf[machine->index]))) {
      // If key-value delimiter is found then we capture they key and values in
      // the conditional statement.
      if (buf[machine->index] == ':') {
        HttpHeader header;
        machine->header.Key = machine->token;
        machine->token.clear();
        // Capture chars while delimiter is false.
        while (!(isSP(buf[machine->index]) || isCR(buf[machine->index]) ||
                 isLF(buf[machine->index]))) {
          machine->token.push_back(buf[machine->index]);
          machine->index++;
        }
        machine->header.Value = machine->token;
        machine->token.clear();
        if ((isCR(buf[machine->index] || isLF(buf[machine->index])))) {
        }
        return;
      }
      machine->token.push_back(buf[machine->index]);
      machine->index++;
    }
  }

  void run(char *buf, int bytes) {
    Request request;
    Machine machine;
    while (machine.index != bytes) {

      switch (machine.State) {

      case Method:
        acquireToken(buf, &machine);
        request.Method = machine.token;
        machine.State = Path;
        machine.token.clear();

      case Path:
        acquireToken(buf, &machine);
        request.Path = machine.token;
        machine.State = Version;
        machine.token.clear();

      case Version:
        acquireToken(buf, &machine);
        request.Version = machine.token;
        machine.State = Headers;
        machine.token.clear();

      case Headers:
        acquireToken(buf, &machine);
        request.Headers.push_back(machine.header);

      default:
        machine.token.clear();
      }
    };
  };
};
} // namespace Parser
  //      switch (machine.State) {
  //      case Method:
  //        while (buf[machine.index] != SPACE) {
  //          machine.token.push_back(buf[machine.index]);
  //          machine.index++;
  //        }
  //        machine.index++;
  //        machine.State = Path;
  //        request.Method =
  //            std::string(machine.token.begin(), machine.token.end());
  //        machine.token.clear();
  //
  //      case Path:
  //        while (buf[machine.index] != SPACE) {
  //          machine.token.push_back(buf[machine.index]);
  //          machine.index++;
  //        }
  //        machine.index++;
  //        machine.State = Version;
  //        request.Path = std::string(machine.token.begin(),
  //        machine.token.end()); machine.token.clear();
  //
  //      case Version:
  //        while (buf[machine.index] != CARRIAGE_RETURN) {
  //          machine.token.push_back(buf[machine.index]);
  //          machine.index++;
  //        }
  //        while (buf[machine.index] == (CARRIAGE_RETURN | LINE_FEED)) {
  //          machine.token.push_back(buf[machine.index]);
  //          machine.index++;
  //        }
  //        request.Version =
  //            std::string(machine.token.begin(), machine.token.end());
  //        machine.State = Headers;
  //        machine.token.clear();
  //
  //      case Headers:
  //        while (machine.index != bytes) {
  //          std::vector<std::string> lines;
  //          while (buf[machine.index] != (CARRIAGE_RETURN)) {
  //            machine.token.push_back(buf[machine.index]);
  //            machine.index++;
  //          }
  //          printf("%s\n", machine.token.data());
  //          return request;
  //        }
  //
  //      default:
  //        return request;
  //      }
  //    };
  //    return request;
  //  };
