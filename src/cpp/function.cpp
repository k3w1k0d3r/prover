#include <function.h>

using namespace theorem128;

Function::Function(std::string latex, unsigned int valence) {
    this->latex = latex;
    this->valence = valence;
}

std::string Function::get_latex() const {
    return latex;
}

unsigned int Function::get_valence() const {
    return valence;
}

bool Function::operator==(const Function& function) const {
    if(latex != function.get_latex()) {
        return false;
    }
    if(valence != function.get_valence()) {
        return false;
    }
    return true;
}

bool Function::operator!=(const Function& function) const {
    return !(*this == function);
}
