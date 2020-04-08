#ifndef BINARYDATATHREAD_HPP
#define BINARYDATATHREAD_HPP

#include <string>
#include "binary_data_GiD.hpp"

class Binary_data_class : public Str_binary_data_GiD{
    public:
        Binary_data_class(std::string str);
        ~Binary_data_class();
        std::string getPath() const;
    private:
        std::string pathToFile ; 
    protected:
};


#endif 