#ifndef JSONIFY_HPP
#define JSONIFY_HPP

#include "ast_types.hpp"
#include "json/json.h"

Json::Value
jsonify(const Select&);

#endif
