#ifndef AST_TYPES_HPP
#define AST_TYPES_HPP

#include <string>
#include <utility>
#include <vector>

#include <boost/variant.hpp>


struct Column {
    explicit Column(std::string name)
        : name{std::move(name)} {}
    std::string name;
};

struct SelectFunction {
    SelectFunction(std::string name, Column column)
        : name{std::move(name)}, column{std::move(column)} {}
    std::string name;
    Column column;
};

struct ConditionLike {
    explicit ConditionLike(std::string string_literal)
        : string_literal{std::move(string_literal)} {}
    std::string string_literal;
};

struct ConditionIn {
    explicit ConditionIn(std::vector<std::string> list_of_string_literals)
        : list_of_string_literals{std::move(list_of_string_literals)} {}
    std::vector<std::string> list_of_string_literals;
};

struct ConditionBetween {
    ConditionBetween(std::string low, std::string high)
        : low{std::move(low)}, high{std::move(high)} {}
    std::string low;
    std::string high;
};

struct ConditionEqual {
    explicit ConditionEqual(std::string string_literal)
        : string_literal{std::move(string_literal)} {}
    std::string string_literal;
};

struct ConditionNotEqual {
    explicit ConditionNotEqual(std::string string_literal)
        : string_literal{std::move(string_literal)} {}
    std::string string_literal;
};

struct ConditionOperator_And;
struct ConditionOperator_Or;
template <typename OpTag> struct ConditionOperator;

typedef boost::variant <
    ConditionLike
    , ConditionIn
    , ConditionBetween
    , ConditionEqual
    , ConditionNotEqual
    , boost::recursive_wrapper<ConditionOperator<ConditionOperator_And> >
    , boost::recursive_wrapper<ConditionOperator<ConditionOperator_Or> >
    > ConditionExpression;

template <typename OpTag>
struct ConditionOperator {
    ConditionOperator(ConditionExpression left, ConditionExpression right)
        : left{std::move(left)}, right{std::move(right)} {}
    ConditionExpression left;
    ConditionExpression right;
};

struct Condition {
    Condition(Column column, ConditionExpression expression)
        : column{std::move(column)}, expression{std::move(expression)} {}
    Column column;
    ConditionExpression expression;
};

typedef boost::variant<SelectFunction, Column> Selection;
typedef std::vector<Selection> Selections;
typedef std::vector<Condition> Conditions;

struct Select {
    Select(Selections selections, Conditions conditions)
        : selections(std::move(selections)), conditions(std::move(conditions)) {}
    Selections selections;
    Conditions conditions;
};

#endif
