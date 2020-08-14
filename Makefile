flex_input = genquery_flex_input.l
flex_output_source = genquery_flex_output.cpp

bison_input = genquery_bison_input.y
bison_output_source = genquery_parser_bison_generated.cpp
bison_output_header = genquery_parser_bison_generated.hpp
bison_output = $(bison_output_source) $(bison_output_header) location.hh position.hh stack.hh

default: example

example:
	flex -o $(flex_output_source) $(flex_input)
	bison -o $(bison_output_source) $(bison_input)
	#g++ -Wall -Werror -Wno-sign-compare -std=c++14 example.cpp $(flex_output_source) $(bison_output_source) genquery_wrapper.cpp genquery_jsonify.cpp jsoncpp.cpp -o example
	g++ -Wall -Werror -Wno-sign-compare -std=c++14 example.cpp $(flex_output_source) $(bison_output_source) genquery_wrapper.cpp genquery_stringify.cpp jsoncpp.cpp -o example

.PHONY: clean
clean:
	rm -rf example
	rm -rf $(flex_output_source)
	rm -rf $(bison_output)
