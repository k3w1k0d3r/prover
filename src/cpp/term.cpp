#include <iostream>
#include <term.h>

using namespace theorem128;

Term::Term(Function function, std::vector<Term> subterms) {
    if(subterms.size() != function.get_valence()){
        std::cerr << "invalid function arguments" << std::endl;
        exit(1);
    }
    this->function = function;
    this->subterms = subterms;
    for(int i=0;i<subterms.size();++i) {
        variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
    }
    is_variable = false;
}

Term::Term(Variable variable) {
    variables.insert(variable);
    is_variable = true;
}

Function Term::get_function() const {
    return function;
}

bool Term::get_is_variable() const {
    return is_variable;
}

std::vector<Term> Term::get_subterms() const {
    return subterms;
}

std::unordered_set<Variable> Term::get_variables() const {
    return variables;
}

std::string Term::get_latex() const {
    if(is_variable) {
        return (*variables.begin()).get_latex();
    }
    std::string result = function.get_latex() + "(";
    if(subterms.size()) {
        result += subterms[0].get_latex();
    }
    for(int i=1;i<subterms.size();++i) {
           result += "," + subterms[i].get_latex();
    }
    result += ")";
    return result;
}

void Term::replace(Variable dummy, Term term) {
    if(is_variable && variables.count(dummy)) {
        *this = term;
    }
    else if(!is_variable){
        variables.clear();
        for(int i=0;i<subterms.size();++i) {
            subterms[i].replace(dummy, term);
            variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
        }
    }
}

void Term::replace(Term dummy, Term term, std::vector<bool>& mask) {
    if(mask.size()) {
        if(*this == dummy) {
            if(mask[mask.size()-1]) {
                *this = term;
            }
            mask.pop_back();
        }
        else if(!is_variable){
            variables.clear();
            for(int i=0;i<subterms.size();++i) {
                subterms[i].replace(dummy, term, mask);
                variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
            }
        }
    }
}

bool Term::operator==(const Term& term) const {
    if(function != term.get_function()) {
        return false;
    }
    if(subterms != term.get_subterms()) {
        return false;
    }
    if(is_variable != term.get_is_variable()) {
        return false;
    }
    if(variables != term.get_variables()) {
        return false;
    }
    return true;
}

Variable::Variable(std::string latex) {
    this->latex = latex;
}

std::string Variable::get_latex() const {
    return latex;
}

bool Variable::operator==(const Variable& variable) const {
    return latex == variable.get_latex();
}

bool Variable::operator!=(const Variable& variable) const {
    return !(*this == variable);
}
