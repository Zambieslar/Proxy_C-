#include <string>
#include <vector>
#include "HttpStatusCodes.h"

#define NEW_LINE		0x0A
#define CARRIAGE_RETURN 	0x0D
#define SPACE			0x20

enum States 
{
	Method,
	Path,
	Version,
	Host,
	Headers,
};
namespace Parser {
	class StateMachine {
public:
	struct HttpHeader {
		std::string Key;
		std::string Value;
	};

	struct Request {
		std::string Method;
		std::string Path;
		std::string Version;
		std::string Host;
		std::vector<HttpHeader> Headers;
	};

	struct machine {
		States State;
		int index;
		std::vector<char> buf;
	};

	bool run(char* buf, machine machine) {
		Request request;
		while (true) {
			switch (machine.State) 
			{
				case Method:
					machine.buf.push_back(buf[machine.index]);
					machine.index++;
					if (buf[machine.index] == SPACE) {
						request.Method = machine.buf.data();
						machine.buf.clear();
						machine.State = Path;
						continue;
					}

				case Path:
					machine.buf.push_back(buf[machine.index]);
					machine.index++;
					if (buf[machine.index] == SPACE) {
						request.Method = machine.buf.data();
						machine.buf.clear();
						machine.State = Version;
						continue;
					}

				case Version:
					machine.buf.push_back(buf[machine.index]);
					machine.index++;
					if (buf[machine.index] == NEW_LINE || CARRIAGE_RETURN) {
						request.Method = machine.buf.data();
						machine.buf.clear();
						machine.State = Host;
						continue;
					}

				case Host:

				case Headers:
					
				default:

			}
		};
	};
	};
};
