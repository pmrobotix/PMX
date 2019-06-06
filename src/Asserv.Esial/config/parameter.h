#ifndef PARAMETER_H
#define PARAMETER_H

#include <string>
#include <sstream>

struct Parameter {
    enum Type {
        INT32,
        BOOL,
        FLOAT
    };

    Parameter(std::string name, int32_t *ptr) : name(name), ptr(ptr), type(INT32) { *int_ptr = 0; }
    Parameter(std::string name, bool *ptr) : name(name), ptr(ptr), type(BOOL) { *bool_ptr = false; }
    Parameter(std::string name, float *ptr) : name(name), ptr(ptr), type(FLOAT) { *float_ptr = 0; }

    template <typename T>
    void set(T val) const {
        *((T*) ptr) = val;
    }

    void setFromString(std::string value) const {
        switch (type)  {
            case INT32:
                *int_ptr = atoll(value.c_str());
                break;
            case BOOL:
                *bool_ptr = (value == "true" || value == "1");
                break;
            case FLOAT:
                *float_ptr = atof(value.c_str());
                break;
        }
    }

    std::string toString() const {
        std::stringstream out;
        switch (type) {
            case INT32:
                out << get<int32_t>();
                break;

            case BOOL:
                out << std::boolalpha << get<bool>();
                break;

            case FLOAT:
                out << get<float>();
                break;
        }
        return out.str();
    }

    const std::string& getName() const {
        return name;
    }

    template <typename T>
    T get(void) const {
        return *((T*) ptr);
    }

    bool is(const std::string &other) const {
        return other == name;
    }

    const std::string name;
    const union {
        void *ptr;
        int32_t *int_ptr;
        bool *bool_ptr;
        float *float_ptr;
    };

    const Type type;
};

#endif
