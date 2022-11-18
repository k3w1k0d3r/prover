#include <iostream>
#include <theory.h>

using namespace theorem128;

Schema::Schema(std::vector<Predicate> dummies, Formula generic, std::vector<std::unordered_set<Variable>> criteria) {
    if(dummies.size() != criteria.size()) {
        std::cerr << "invalid schema" << std::endl;
        exit(1);
    }
    this->dummies = dummies;
    this->generic = generic;
    this->criteria = criteria;
}

bool Schema::verify(Formula result, std::vector<Formula> substitutions) const {
    if(criteria.size() != substitutions.size()) {
        std::cerr << "invalid substitutions" << std::endl;
    }
    Formula base = generic;
    for(int i=0;i<criteria.size();++i) {
        for(auto variable : substitutions[i].get_variables()) {
            if(!criteria[i].count(variable)) {
                return false;
            }
        }
        base.replace(dummies[i], substitutions[i]);
    }
    return base == result;
}

Theory::Theory(std::vector<Formula> axioms, std::vector<Schema> schemas) {
    this->axioms = axioms;
    this->schemas = schemas;
}

Theory::Theory(func_t custom_check_axiom) {
    this->custom_check_axiom = custom_check_axiom;
}

bool Theory::check_axiom(Formula formula, void *signature) const {
    if(custom_check_axiom == NULL) {
        bool is_schema = *(bool*)signature;
        signature = (void*)((bool*)signature+1);
        unsigned int index = *(int*)signature;
        signature = (void*)((int*)signature+1);
        if(is_schema) {
            if(index >= schemas.size()) {
                return false;
            }
            return schemas[index].verify(Formula(formula), *(std::vector<Formula>*)signature);
        }
        if(index >= axioms.size()) {
            return false;
        }
        return axioms[index]==formula && formula.get_variables().empty();
    }
    return custom_check_axiom(Formula(formula), signature) && formula.get_variables().empty();
}
