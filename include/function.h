#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>

namespace theorem128 {
    class Function {
        public:
            Function(std::string, unsigned int);
            Function(){};
            std::string get_latex() const;
            unsigned int get_valence() const;
            bool operator==(const Function&) const;
            bool operator!=(const Function&) const;
        private:
            std::string latex;
            unsigned int valence = 0;
    };
}

#endif
