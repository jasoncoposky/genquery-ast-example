default: example

example:
	g++ -Wall -Werror -std=c++11 example.cpp jsonify.cpp jsoncpp.cpp -o example

.PHONY: clean
clean:
	rm -f example
