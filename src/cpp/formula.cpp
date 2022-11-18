#include <iostream>
#include <formula.h>

using namespace theorem128;

Formula::Formula(Predicate predicate, std::vector<Term> subterms) {
    if(subterms.size() != predicate.get_valence()){
        std::cerr << "invalid predicate arguments" << std::endl;
        exit(1);
    }
    this->predicate = predicate;
    this->subterms = subterms;
    for(int i=0;i<subterms.size();++i) {
        variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
    }
    type = 1;
}

Formula::Formula(Quantifier quantifier, Variable quantified, Formula subformula) {
    this->quantifier = quantifier;
    this->quantified = quantified;
    this->subformula1 = new Formula(subformula);
    variables.insert(subformula.get_variables().begin(), subformula.get_variables().end());
    variables.erase(quantified);
    type = 2;
}

Formula::Formula(Connective connective, Formula subformula1, Formula subformula2) {
    this->connective = connective;
    this->subformula1 = new Formula(subformula1);
    this->subformula2 = new Formula(subformula2);
    variables.insert(subformula1.get_variables().begin(), subformula1.get_variables().end());
    variables.insert(subformula2.get_variables().begin(), subformula2.get_variables().end());
    type = 3;
}

Formula::Formula(Formula subformula, int) {
    this->subformula1 = new Formula(subformula);
    variables.insert(subformula.get_variables().begin(), subformula.get_variables().end());
    type = 4;
}

Formula::Formula(Term subterm1, Term subterm2) {
    subterms = {subterm1, subterm2};
    variables.insert(subterm1.get_variables().begin(), subterm1.get_variables().end());
    variables.insert(subterm2.get_variables().begin(), subterm2.get_variables().end());
    type = 5;
}

Formula::Formula(int) {
    type = 6;
}

Formula::Formula(const Formula& formula) {
    type = formula.get_type();
    predicate = formula.get_predicate();
    quantifier = formula.get_quantifier();
    connective = formula.get_connective();
    quantified = formula.get_quantified();
    if(formula.get_subformula(0)) {
        subformula1 = new Formula(*formula.get_subformula(0));
    }
    if(formula.get_subformula(1)) {
        subformula2 = new Formula(*formula.get_subformula(1));
    }
    subterms = formula.get_subterms();
    variables = formula.get_variables();
}

Predicate Formula::get_predicate() const {
    return predicate;
}

Quantifier Formula::get_quantifier() const {
    return quantifier;
}

Connective Formula::get_connective() const {
    return connective;
}

Variable Formula::get_quantified() const {
    return quantified;
}

Formula *Formula::get_subformula(int selection) const {
    if(!selection) {
        return subformula1;
    }
    return subformula2;
}

std::vector<Term> Formula::get_subterms() const {
    return subterms;
}

std::unordered_set<Variable> Formula::get_variables() const {
    return variables;
}

int Formula::get_type() const {
    return type;
}

std::string Formula::get_latex() const {
    std::string result;
    if(type == 1) {
        result = predicate.get_latex() + "(";
        if(subterms.size()) {
            result += subterms[0].get_latex();
        }
        for(int i=1;i<subterms.size();++i) {
            result += "," + subterms[i].get_latex();
        }
        result += ")";
    }
    if(type == 2) {
        if(quantifier == FORALL) {
            result = "\\forall";
        }
        if(quantifier == EXISTS) {
            result = "\\exists";
        }
        result += "{" + quantified.get_latex() + "}.(" + subformula1->get_latex() + ")";
    }
    if(type == 3) {
        result = "(" + subformula1->get_latex() + ")";
        if(connective == AND) {
            result += "\\land";
        }
        if(connective == OR) {
            result += "\\lor";
        }
        if(connective == IMPLIES) {
            result += "\\Rightarrow";
        }
        if(connective == EQUIVALENT) {
            result += "\\Leftrightarrow";
        }
        result += "(" + subformula2->get_latex() + ")";
    }
    if(type == 4) {
        result = "\\neg(" + subformula1->get_latex() + ")";
    }
    if(type == 5) {
        result = subterms[0].get_latex() + "=" + subterms[1].get_latex();
    }
    if(type == 6) {
        result = "\\bot";
    }
    return result;
}

void Formula::replace(Predicate dummy, Formula formula) {
    if(type==1 && predicate==dummy) {
        *this = formula;
    }
    if(type == 2) {
        subformula1->replace(dummy, formula);
        variables.clear();
        variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
        variables.erase(quantified);
    }
    if(type == 3) {
        subformula1->replace(dummy, formula);
        subformula2->replace(dummy, formula);
        variables.clear();
        variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
        variables.insert(subformula2->get_variables().begin(), subformula2->get_variables().end());
    }
    if(type == 4) {
        subformula1->replace(dummy, formula);
        variables.clear();
        variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
    }
}

int Formula::replace(Variable dummy, Term term) {
    int res = 0;
    if(type==1 || type==5) {
        variables.clear();
        for(int i=0;i<subterms.size();++i) {
            subterms[i].replace(dummy, term);
            variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
        }
    }
    if(type == 2) {
        if(variables.count(dummy)) {
            if(term.get_variables().count(quantified)) {
                res = 1;
            }
            if(subformula1->replace(dummy, term)) {
                res = 1;
            }
            variables.clear();
            variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
            variables.erase(quantified);
        }
    }
    if(type == 3) {
        if(subformula1->replace(dummy, term)) {
            res = 1;
        }
        if(subformula2->replace(dummy, term)) {
            res = 1;
        }
        variables.clear();
        variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
        variables.insert(subformula2->get_variables().begin(), subformula2->get_variables().end());
    }
    if(type == 4) {
        if(subformula1->replace(dummy, term)) {
            res = 1;
        }
        variables.clear();
        variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
    }
    return res;
}

void Formula::replace(Term dummy, Term term, std::vector<bool>& mask) {
    if(mask.size()) {
        if(type==1 || type==5) {
            variables.clear();
            for(int i=0;i<subterms.size();++i) {
                subterms[i].replace(dummy, term, mask);
                variables.insert(subterms[i].get_variables().begin(), subterms[i].get_variables().end());
            }
        }
        if(type == 2) {
            if(!dummy.get_variables().count(quantified) && !term.get_variables().count(quantified)) {
                subformula1->replace(dummy, term, mask);
                variables.clear();
                variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
                variables.erase(quantified);
            }
        }
        if(type == 3) {
            subformula1->replace(dummy, term, mask);
            subformula2->replace(dummy, term, mask);
            variables.clear();
            variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
            variables.insert(subformula2->get_variables().begin(), subformula2->get_variables().end());
        }
        if(type == 4) {
            subformula1->replace(dummy, term, mask);
            variables.clear();
            variables.insert(subformula1->get_variables().begin(), subformula1->get_variables().end());
        }
    }
}

Formula& Formula::operator=(const Formula& formula) {
    type = formula.get_type();
    predicate = formula.get_predicate();
    quantifier = formula.get_quantifier();
    connective = formula.get_connective();
    quantified = formula.get_quantified();
    if(formula.get_subformula(0)) {
        subformula1 = new Formula(*formula.get_subformula(0));
    }
    if(formula.get_subformula(1)) {
        subformula2 = new Formula(*formula.get_subformula(1));
    }
    subterms = formula.get_subterms();
    variables = formula.get_variables();
    return *this;
}

bool Formula::operator==(const Formula& formula) const {
    if(type != formula.get_type()) {
        return false;
    }
    if(predicate != formula.get_predicate()) {
        return false;
    }
    if(quantifier != formula.get_quantifier()) {
        return false;
    }
    if(connective != formula.get_connective()) {
        return false;
    }
    if(quantified != formula.get_quantified()) {
        return false;
    }
    if(subformula1 != formula.get_subformula(0)) {
        if(subformula1==NULL || formula.get_subformula(0)==NULL) {
            return false;
        }
        else if(*subformula1 != *formula.get_subformula(0)) {
            return false;
        }
    }
    if(subformula2 != formula.get_subformula(1)) {
        if(subformula2==NULL || formula.get_subformula(1)==NULL) {
            return false;
        }
        else if(*subformula2 != *formula.get_subformula(1)) {
            return false;
        }
    }
    if(subterms != formula.get_subterms()) {
        return false;
    }
    if(variables != formula.get_variables()) {
        return false;
    }
    return true;
}

bool Formula::operator!=(const Formula& formula) const {
    return !(*this == formula);
}

Formula::~Formula() {
    delete subformula1;
    delete subformula2;
}
