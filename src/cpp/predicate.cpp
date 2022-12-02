#include <predicate.h>

using namespace theorem128;

Predicate::Predicate(std::string latex, unsigned int valence) {
    this->latex = latex;
    this->valence = valence;
}

std::string Predicate::get_latex() const {
    return latex;
}

unsigned int Predicate::get_valence() const {
    return valence;
}

bool Predicate::operator==(const Predicate& predicate) const {
    if(latex != predicate.get_latex()) {
        return false;
    }
    if(valence != predicate.get_valence()) {
        return false;
    }
    return true;
}

bool Predicate::operator!=(const Predicate& predicate) const {
    return !(*this == predicate);
}
