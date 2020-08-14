#ifndef GENQUERY_JSONIFY_HPP
#define GENQUERY_JSONIFY_HPP

#include "genquery_ast_types.hpp"
#include <iostream>

namespace Genquery {
    template <typename Iterable>
    std::string
    iterableToString(Iterable const& iterable) {
        std::string v{"( "};
        for (auto&& element: iterable) {
            v += "'";
            v += element;
            v += "', ";
        }
        v += " ) ";
        return v;
    }

    struct JsonifyVisitor: public boost::static_visitor<std::string> {
        template <typename T>
        std::string operator()(const T& arg) const {
            return stringify(arg);
        }
    };

    std::string
    stringify(const Column& column) {
        return column.name;
    }

    static uint8_t emit_second_paren{};

    std::string
    stringify(const SelectFunction& select_function) {
        std::string ret{};
        ret += select_function.name;
        ret += "(";
        emit_second_paren = 1;
        
        return ret;
    }

    std::string
    stringify(const Selections& selections) {
        std::string ret{};
        for (auto&& selection: selections) {
            ret.append(boost::apply_visitor(JsonifyVisitor{}, selection));
            if(emit_second_paren >= 2) {
                ret += "), ";
                emit_second_paren = 0;
            }
            else if(emit_second_paren > 0) {
                ++emit_second_paren;
            }
            else {
                ret += ", ";
            }
        }

        ret.erase(ret.find_last_of(","));

        return ret;
    }

    std::string
    stringify(const ConditionOperator_Or& op_or) {
        std::string ret{};
        ret += boost::apply_visitor(JsonifyVisitor(), op_or.left);
        ret += " || ";
        ret += boost::apply_visitor(JsonifyVisitor(), op_or.right);
        return ret;
    }

    std::string
    stringify(const ConditionOperator_And& op_and) {
        std::string ret = boost::apply_visitor(JsonifyVisitor(), op_and.left);
        ret += " && ";
        ret += boost::apply_visitor(JsonifyVisitor(), op_and.right);
        return ret;
    }

    std::string
    stringify(const ConditionOperator_Not& op_not) {
        std::string ret{" NOT "};
        ret += boost::apply_visitor(JsonifyVisitor(), op_not.expression);
        return ret;
    }

    std::string
    stringify(const ConditionNotEqual& not_equal) {
        std::string ret{" != "};
        ret += "'";
        ret += not_equal.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionEqual& equal) {
        std::string ret{" = "};
        ret += "'";
        ret += equal.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionLessThan& less_than) {
        std::string ret{" < "};
        ret += "'";
        ret += less_than.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionLessThanOrEqualTo& less_than_or_equal_to) {
        std::string ret{" <= "};
        ret += "'";
        ret += less_than_or_equal_to.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionGreaterThan& greater_than) {
        std::string ret{" > "};
        ret += "'";
        ret += greater_than.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionGreaterThanOrEqualTo& greater_than_or_equal_to) {
        std::string ret{" >= "};
        ret += greater_than_or_equal_to.string_literal;
        return ret;
    }

    std::string
    stringify(const ConditionBetween& between) {
        std::string ret{" BETWEEN '"};
        ret += between.low;
        ret += "' AND '";
        ret += between.high;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionIn& in) {
        std::string ret{" IN "};
        ret += iterableToString(in.list_of_string_literals);
        return ret;
    }

    std::string
    stringify(const ConditionLike& like) {
        std::string ret{" LIKE '"};
        ret += like.string_literal;
        ret += "'";
        return ret;
    }

    std::string
    stringify(const ConditionParentOf& parent_of) {
        std::string ret{"parent_of"};
        ret += parent_of.string_literal;
        return ret;
    }

    std::string
    stringify(const ConditionBeginningOf& beginning_of) {
        std::string ret{"beginning_of"};
        ret += beginning_of.string_literal;
        return ret;
    }

    std::string
    stringify(const Condition& condition) {
        std::string ret{stringify(condition.column)};
        ret += boost::apply_visitor(JsonifyVisitor(), condition.expression);
        return ret;
    }

    std::string
    stringify(const Conditions& conditions) {
        std::string ret{};

        int i{};
        for (auto&& condition: conditions) {
            ret += stringify(condition);
            if(i < conditions.size()-1) { ret += " AND "; }
            ++i;
        }
        return ret;
    }

    std::string
    stringify(const Select& select) {
        std::string root{"SELECT "};
        root += stringify(select.selections);
        root += " WHERE ";
        root += stringify(select.conditions);
        return root;
    }
}
#endif // GENQUERY_JSONIFY_HPP
