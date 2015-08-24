#include <iostream>
#include <string>

#include "genquery_wrapper.hpp"
#include "genquery_stream_insertion.hpp"
#include "genquery_jsonify.hpp"


int main(int, char**) {
    std::string line;
    while (std::getline(std::cin, line)) {
        auto select(Genquery::Wrapper::parse(line));
        std::cout << select << std::endl;
        std::cout << jsonify(select) << std::endl;
    }
}
