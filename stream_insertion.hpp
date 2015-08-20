#ifndef STREAM_INSERTION_HPP
#define STREAM_INSERTION_HPP

#include "ast_types.hpp"

template <typename T>
T&
operator<<(T& os, const Column& column) {
    os << column.name;
    return os;
}

template <typename T>
T&
operator<<(T& os, const SelectFunction& select_function) {
    os << select_function.name << "("
       << select_function.column << ")";
    return os;
}

template <typename T>
T&
operator<<(T& os, const Selections& selections) {
    os << "select ";
    for (auto it = selections.cbegin(); it != selections.cend(); ++it) {
        os << *it;
        if (it+1 != selections.cend()) {
            os << ", ";
        }
    }
    return os;
}

template <typename T>
T&
operator<<(T& os, const ConditionOperator<ConditionOperator_Or>& op_or) {
    return os << "(" << op_or.left << " || " << op_or.right << ")";
}

template <typename T>
T&
operator<<(T& os, const ConditionOperator<ConditionOperator_And>& op_and) {
    return os << "(" << op_and.left << " && " << op_and.right << ")";
}

template <typename T>
T&
operator<<(T& os, const ConditionNotEqual& not_equal) {
    return os << "!= '" << not_equal.string_literal << "'";
}

template <typename T>
T&
operator<<(T& os, const ConditionEqual& equal) {
    return os << "== '" << equal.string_literal << "'";
}

template <typename T>
T&
operator<<(T& os, const ConditionBetween& between) {
    os << "between "
       << "'" << between.low << "'"
       << " "
       << "'" << between.high << "'";
    return os;
}

template <typename T>
T&
operator<<(T& os, const ConditionIn& in) {
    os << "in (";
    for (auto it = in.list_of_string_literals.cbegin(); it != in.list_of_string_literals.cend(); ++it) {
        os << "'" << *it << "'";
        if (it+1 != in.list_of_string_literals.cend()) {
            os << ", ";
        }
    }
    os << ")";
    return os;
}

template <typename T>
T&
operator<<(T& os, const ConditionLike& like) {
    return os << "like '" << like.string_literal << "'";
}

template <typename T>
T&
operator<<(T& os, const Condition& condition) {
    return os << condition.column << " " << condition.expression;
}

template <typename T>
T&
operator<<(T& os, const Conditions& conditions) {
    for (auto it = conditions.cbegin(); it != conditions.cend(); ++it) {
        os << *it;
        if (it+1 != conditions.cend()) {
            os << " and ";
        }
    }
    return os;
}

template <typename T>
T&
operator<<(T& os, const Select& select) {
    os << select.selections;
    if (select.conditions.size() > 0) {
        os << " where ";
        os << select.conditions;
    }
    return os;
}

#endif
