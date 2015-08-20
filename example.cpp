#include "ast_types.hpp"
#include "jsonify.hpp"
#include "stream_insertion.hpp"


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
    Conditions conditions{condition0, condition1, condition2};

    return {selections, conditions};
}

int
main(int, char**) {
    std::cout << makeExampleSimple() << std::endl;
    std::cout << jsonify(makeExampleSimple()) << std::endl;
    std::cout << std::endl;
    std::cout << makeExampleComplex() << std::endl;
    std::cout << jsonify(makeExampleComplex()) << std::endl;
}
