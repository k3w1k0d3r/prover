#include <proof.h>

using namespace theorem128;

std::vector<bool> bit_or(std::vector<bool> vec1, std::vector<bool> vec2) {
    if(vec1.size() > vec2.size()) {
        return bit_or(vec2, vec1);
    }
    for(int i=0;i<vec1.size();++i) {
        vec2[i] = vec2[i] || vec1[i];
    }
    return vec2;
}

std::vector<bool> reduce(std::vector<bool> vec) {
    while(vec.size() and !vec[vec.size()-1]) {
        vec.pop_back();
    }
    return vec;
}

Proof::Proof(Theory theory) {
    this->theory = theory;
    truths[{}] = {};
}

void Proof::axiom(Formula axiom, void* signature) {
    if(theory.check_axiom(axiom, signature)) {
        truths[{}].push_back(axiom);
    }
}

void Proof::hypothesis(Formula formula) {
    std::vector<bool> vec;
    for(int i=0;i<hypotheses.size();++i) {
        vec.push_back(false);
    }
    vec.push_back(true);
    truths[vec] = {formula};
    hypotheses.push_back(formula);
}

std::unordered_map<std::vector<bool>, std::vector<Formula>> Proof::get_truths() {
    return truths;
}

std::vector<Formula> Proof::get_hypotheses() {
    return hypotheses;
}

void Proof::conjunction(unsigned int f1, unsigned int f2, std::vector<bool> h1, std::vector<bool> h2) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    std::vector<bool> h3 = reduce(bit_or(h1, h2));
    if(truths.count(h1) && truths.count(h2)) {
        if(!truths.count(h3)) {
            truths[h3] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size()) {
            truths[h3].push_back(Formula(AND, truths[h1][f1], truths[h2][f2]));
        }
    }
}

void Proof::conjunction(unsigned int f, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            if(truths[h][f].get_type()==3 && truths[h][f].get_connective()==AND) {
                truths[h].push_back(*truths[h][f].get_subformula(0));
                truths[h].push_back(*truths[h][f].get_subformula(1));
            }
        }
    }
}

void Proof::disjunction(unsigned int f1, Formula f2, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f1 < truths[h].size()) {
            truths[h].push_back(Formula(OR, truths[h][f1], f2));
        }
    }
}

void Proof::disjunction(Formula f1, unsigned int f2, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f2 < truths[h].size()) {
            truths[h].push_back(Formula(OR, f1, truths[h][f2]));
        }
    }
}
            
void Proof::disjunction(unsigned int f1, unsigned int f2, unsigned int f3, std::vector<bool> h1, std::vector<bool> h2, std::vector<bool> h3) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    h3 = reduce(h3);
    std::vector<bool> h4 = reduce(bit_or(bit_or(h1, h2), h3));
    if(truths.count(h1) && truths.count(h2) && truths.count(h3)) {
        if(!truths.count(h4)) {
            truths[h4] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size() && f3<truths[h3].size()) {
            Formula formula1 = truths[h1][f1];
            Formula formula2 = truths[h2][f2];
            Formula formula3 = truths[h3][f3];
            if(formula1.get_type()==3 && formula2.get_type()==3 && formula3.get_type()==3) {
                if(formula1.get_connective()==OR && formula2.get_connective()==IMPLIES && formula3.get_connective()==IMPLIES) {
                    if(*formula1.get_subformula(0)==*formula2.get_subformula(0) && *formula1.get_subformula(1)==*formula3.get_subformula(0) && *formula2.get_subformula(1)==*formula3.get_subformula(1)) {
                        truths[h4].push_back(*formula2.get_subformula(1));
                    }
                }
            }
        }
    }
}

void Proof::modus_ponens(unsigned int f1, unsigned int f2, std::vector<bool> h1, std::vector<bool> h2) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    std::vector<bool> h3 = reduce(bit_or(h1, h2));
    if(truths.count(h1) && truths.count(h2)) {
        if(!truths.count(h3)) {
            truths[h3] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size()) {
            Formula formula1 = truths[h1][f1];
            Formula formula2 = truths[h2][f2];
            if(formula1.get_type()==3 && formula1.get_connective()==IMPLIES && *formula1.get_subformula(0)==formula2) {
                truths[h3].push_back(*formula1.get_subformula(1));
            }
        }
    }
}

void Proof::explosion(unsigned int f, std::vector<bool> h, Formula formula) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            if(truths[h][f].get_type() == 6) {
                truths[h].push_back(formula);
            }
        }
    }
}

void Proof::contradiction(unsigned int f, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            Formula formula = truths[h][f];
            if(formula.get_type()==3 && formula.get_connective()==AND) {
                if(formula.get_subformula(1)->get_type()==4) {
                    if(*formula.get_subformula(0) == *(formula.get_subformula(1)->get_subformula(0))) {
                        truths[h].push_back(Formula(0));
                    }
                }
            }
        }
    }
}

void Proof::negation(unsigned int f, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            Formula formula = truths[h][f];
            if(formula.get_type()==3 && formula.get_connective()==IMPLIES && *formula.get_subformula(1)==Formula(0)) {
                truths[h].push_back(Formula(*formula.get_subformula(0), 0));
            }
        }
    }
}

void Proof::equivalence(unsigned int f1, unsigned int f2, std::vector<bool> h1, std::vector<bool> h2) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    std::vector<bool> h3 = reduce(bit_or(h1, h2));
    if(truths.count(h1) && truths.count(h2)) {
        if(!truths.count(h3)) {
            truths[h3] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size()) {
            Formula formula1 = truths[h1][f1];
            Formula formula2 = truths[h2][f2];
            if(formula1.get_type()==3 && formula2.get_type()==3) {
                if(formula1.get_connective()==IMPLIES && formula2.get_connective()==IMPLIES) {
                    if(*formula1.get_subformula(0)==*formula2.get_subformula(1) && *formula1.get_subformula(1)==*formula2.get_subformula(0)) {
                        truths[h3].push_back(Formula(EQUIVALENT, *formula1.get_subformula(0), *formula1.get_subformula(1)));
                    }
                }
            }
        }
    }
}

void Proof::equivalence(unsigned int f, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            Formula formula = truths[h][f];
            if(formula.get_type()==3 && formula.get_connective()==EQUIVALENT) {
                truths[h].push_back(Formula(IMPLIES, *formula.get_subformula(0), *formula.get_subformula(1)));
                truths[h].push_back(Formula(IMPLIES, *formula.get_subformula(1), *formula.get_subformula(0)));
            }
        }
    }
}

void Proof::reductio_ad_absurdum(unsigned int f, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h)) {
        if(f < truths[h].size()) {
            Formula formula = truths[h][f];
            if(formula.get_type()==3 && formula.get_connective()==IMPLIES && *formula.get_subformula(1)==Formula(0)) {
                if(formula.get_subformula(0)->get_type() == 4) {
                    truths[h].push_back(*(formula.get_subformula(0)->get_subformula(0)));
                }
            }
        }
    }
}

void Proof::cancel_hypothesis(unsigned int f, unsigned int to_cancel, std::vector<bool> h) {
    h = reduce(h);
    if(truths.count(h) && f<truths[h].size()) {
        Formula formula = truths[h][f];
        while(to_cancel >= h.size()) {
            h.push_back(0);
        }
        h[to_cancel] = false;
        h = reduce(h);
        if(!truths.count(h)) {
            truths[h] = {};
        }
        truths[h].push_back(Formula(IMPLIES, hypotheses[to_cancel], formula));
    }
}

void Proof::identity(Term t) {
    truths[{}].push_back(Formula(t, t));
}

void Proof::identity(unsigned int f1, unsigned int f2, std::vector<bool> h1, std::vector<bool> h2, std::vector<bool> mask) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    std::vector<bool> h3 = reduce(bit_or(h1, h2));
    if(truths.count(h1) && truths.count(h2)) {
        if(!truths.count(h3)) {
            truths[h3] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size()) {
            Formula formula1 = truths[h1][f1];
            Formula formula2 = truths[h2][f2];
            if(formula1.get_type() == 5) {
                Term t1 = formula1.get_subterms()[0];
                Term t2 = formula1.get_subterms()[1];
                Formula formula3 = formula2;
                formula3.replace(t1, t2, mask);
                truths[h3].push_back(formula3);
            }
        }
    }
}

void Proof::universal(unsigned int f, std::vector<bool> h, Variable v) {
    h = reduce(h);
    if(h.size() > hypotheses.size()) {
        return;
    }
    for(int i=0;i<h.size();++i) {
        if(h[i] && hypotheses[i].get_variables().count(v)) {
            return;
        }
    }
    if(truths.count(h) && f<truths[h].size()) {
        truths[h].push_back(Formula(FORALL, v, truths[h][f]));
    }
}

void Proof::universal(unsigned int f, std::vector<bool> h, Term t) {
    h = reduce(h);
    if(truths.count(h) && f<truths[h].size()) {
        Formula formula = truths[h][f];
        if(formula.get_type()==2 && formula.get_quantifier()==FORALL) {
            Formula res = *formula.get_subformula(0);
            if(!res.replace(formula.get_quantified(), t)) {
                truths[h].push_back(res);
            }
        }
    }
}

void Proof::existential(unsigned int f1, unsigned int f2, std::vector<bool> h1, std::vector<bool> h2) {
    h1 = reduce(h1);
    h2 = reduce(h2);
    std::vector<bool> h3 = reduce(bit_or(h1, h2));
    if(truths.count(h1) && truths.count(h2)) {
        if(!truths.count(h3)) {
            truths[h3] = {};
        }
        if(f1<truths[h1].size() && f2<truths[h2].size()) {
            Formula formula1 = truths[h1][f1];
            Formula formula2 = truths[h2][f2];
            if(formula1.get_type()==2 && formula1.get_quantifier()==EXISTS) {
                Variable v = formula1.get_quantified();
                for(int i=0;i<h3.size();++i) {
                    if(h3[i] && hypotheses[i].get_variables().count(v)) {
                        return;
                    }
                }
                if(formula2.get_subformula(1)->get_variables().count(v)) {
                    return;
                }
                if(formula2.get_type()==3 && formula2.get_connective()==IMPLIES) {
                    if(*formula1.get_subformula(0) == *formula2.get_subformula(0)) {
                        truths[h3].push_back(*formula2.get_subformula(1));
                    }
                }
            }
        }
    }
}

void Proof::existential(unsigned int f1, std::vector<bool> h, Formula f2, Term t) {
    h = reduce(h);
    if(truths.count(h) && f1<truths[h].size()) {
        if(f2.get_type()==2 && f2.get_quantifier()==EXISTS) {
            Formula check = *f2.get_subformula(0);
            if(!check.replace(f2.get_quantified(), t)) {
                if(check == truths[h][f1]) {
                    truths[h].push_back(f2);
                }
            }
        }
    }
}
