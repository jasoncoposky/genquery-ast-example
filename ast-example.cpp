#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <boost/variant.hpp>

// Definition of AST types, read from bottom up

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

// Example navigating AST to print, read from bottom up
// Prints out valid iquest strings, except added parens to && and ||

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

// Example AST constructions

Select
makeExampleSimple() {
    Selection selection{Column{"DATA_NAME"}};
    Selections selections{selection};
    Condition condition{Column{"DATA_NAME"}, ConditionLike{"/tempZone/home/%"}};
    Conditions conditions{condition};
    return {selections, conditions};
}

Select
makeExampleComplex() {
    Selection selection0{SelectFunction{"order_desc", Column{"DATA_SIZE"}}};
    Selection selection1{Column{"DATA_NAME"}};
    Selection selection2{Column{"COLL_NAME"}};
    Selections selections{selection0, selection1, selection2};

    Condition condition0{Column{"DATA_NAME"}, ConditionLike{"/tempZone/home/%"}};
    Condition condition1{Column{"DATA_ID"}, ConditionBetween{"100", "1000"}};
    ConditionExpression expression0{ConditionBetween{"400", "500"}};
    ConditionExpression expression1{ConditionIn{{"100", "150", "797", "9999"}}};
    ConditionExpression expression2{ConditionOperator<ConditionOperator_Or>{expression0, expression1}};
    ConditionExpression expression3{ConditionEqual{"3792"}};
    ConditionExpression expression4{ConditionOperator<ConditionOperator_Or>{expression2, expression3}};
    ConditionExpression expression5{ConditionNotEqual{"12345"}};
    ConditionExpression expression6{ConditionOperator<ConditionOperator_And>{expression4, expression5}};
    Condition condition2{Column{"DATA_SIZE"}, expression6};
    std::vector<Condition> conditions {condition0, condition1, condition2};

    return {selections, conditions};
}

int
main(int, char**) {
    std::cout << makeExampleSimple() << std::endl;
    std::cout << makeExampleComplex() << std::endl;
}
