#ifndef GENQUERY_JSONIFY_HPP
#define GENQUERY_JSONIFY_HPP

#include "genquery_ast_types.hpp"
#include "json/json.h"

namespace Genquery {
    template <typename Iterable>
    Json::Value
    iterableToJson(Iterable const& iterable) {
        Json::Value v{Json::arrayValue};
        for (auto&& element: iterable) {
            v.append(element);
        }
        return v;
    }

    struct JsonifyVisitor: public boost::static_visitor<Json::Value> {
        template <typename T>
        Json::Value
        operator()(const T& arg) const {
            return jsonify(arg);
        }
    };

    Json::Value
    jsonify(const Column& column) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "column";
        ret["name"] = column.name;
        return ret;
    }

    Json::Value
    jsonify(const SelectFunction& select_function) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "select_function";
        ret["name"] = select_function.name;
        ret["column"] = jsonify(select_function.column);
        return ret;
    }

    Json::Value
    jsonify(const Selections& selections) {
        Json::Value ret{Json::arrayValue};
        for (auto&& selection: selections) {
            ret.append(boost::apply_visitor(JsonifyVisitor{}, selection));
        }
        return ret;
    }

    Json::Value
    jsonify(const ConditionOperator_Or& op_or) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "or";
        ret["left"] = boost::apply_visitor(JsonifyVisitor(), op_or.left);
        ret["right"] = boost::apply_visitor(JsonifyVisitor(), op_or.right);
        return ret;
    }

    Json::Value
    jsonify(const ConditionOperator_And& op_and) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "and";
        ret["left"] = boost::apply_visitor(JsonifyVisitor(), op_and.left);
        ret["right"] = boost::apply_visitor(JsonifyVisitor(), op_and.right);
        return ret;
    }

    Json::Value
    jsonify(const ConditionOperator_Not& op_not) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "not";
        ret["expression"] = boost::apply_visitor(JsonifyVisitor(), op_not.expression);
        return ret;
    }

    Json::Value
    jsonify(const ConditionNotEqual& not_equal) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "not_equal";
        ret["string_literal"] = not_equal.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionEqual& equal) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "equal";
        ret["string_literal"] = equal.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionLessThan& less_than) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "less_than";
        ret["string_literal"] = less_than.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionLessThanOrEqualTo& less_than_or_equal_to) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "less_than_or_equal_to";
        ret["string_literal"] = less_than_or_equal_to.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionGreaterThan& greater_than) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "greater_than";
        ret["string_literal"] = greater_than.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionGreaterThanOrEqualTo& greater_than_or_equal_to) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "greater_than_or_equal_to";
        ret["string_literal"] = greater_than_or_equal_to.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionBetween& between) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "between";
        ret["low"] = between.low;
        ret["high"] = between.high;
        return ret;
    }

    Json::Value
    jsonify(const ConditionIn& in) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "in";
        ret["list_of_string_literals"] = iterableToJson(in.list_of_string_literals);
        return ret;
    }

    Json::Value
    jsonify(const ConditionLike& like) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "like";
        ret["string_literal"] = like.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionParentOf& parent_of) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "parent_of";
        ret["string_literal"] = parent_of.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const ConditionBeginningOf& beginning_of) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "beginning_of";
        ret["string_literal"] = beginning_of.string_literal;
        return ret;
    }

    Json::Value
    jsonify(const Condition& condition) {
        Json::Value ret{Json::objectValue};
        ret["type"] = "condition";
        ret["column"] = jsonify(condition.column);
        ret["expression"] = boost::apply_visitor(JsonifyVisitor(), condition.expression);
        return ret;
    }

    Json::Value
    jsonify(const Conditions& conditions) {
        Json::Value ret{Json::arrayValue};
        for (auto&& condition: conditions) {
            ret.append(jsonify(condition));
        }
        return ret;
    }

    Json::Value
    jsonify(const Select& select) {
        Json::Value root{Json::objectValue};
        root["type"] = "select";
        root["selections"] = jsonify(select.selections);
        root["conditions"] = jsonify(select.conditions);
        return root;
    }
}
#endif // GENQUERY_JSONIFY_HPP
