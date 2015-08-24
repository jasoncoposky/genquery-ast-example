#ifndef GENQUERY_JSONIFY_HPP
#define GENQUERY_JSONIFY_HPP

#include "genquery_ast_types.hpp"
#include "json/json.h"

namespace Genquery {
    Json::Value
    jsonify(const Select&);
}
#endif // GENQUERY_JSONIFY_HPP
