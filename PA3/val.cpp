/* Implementation of operator functions from "val.h"
 * val.cpp
 * Programming Assignment 3
 * Spring 2022
*/

#include "val.h"

// add op to this
Value Value::operator+(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp + op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp + op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() + op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() + (float)op.GetInt());
    }
    return Value();
}

// subtract op to this
Value Value::operator-(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp - op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp - op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() - op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() - (float)op.GetInt());
    }
    return Value();
}

// mult this with op
Value Value::operator*(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp * op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp * op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() * op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() * (float)op.GetInt());
    }
    return Value();
}

// div this by op
Value Value::operator/(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp / op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp / op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() / op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() / (float)op.GetInt());
    }
    return Value();
}

// greater than
Value Value::operator>(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp > op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp > op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() > op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() > (float)op.GetInt());
    }
    return Value();
}

// less than
Value Value::operator<(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp < op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp < op.GetReal());
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((float)GetInt() < op.GetReal());
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value(GetReal() < (float)op.GetInt());
    }
    return Value();
}

// equality
Value Value::operator==(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value((bool)(Itemp == op.GetInt()));
        }
        if (IsReal()){
            return Value((bool)(Rtemp == op.GetReal()));
        }
        if (IsString()){
            return Value((bool)(Stemp == op.GetString()));
        }
    }
    // both int and float, make result float
    else if (IsInt() && op.IsReal()){
        return Value((bool)(((float)GetInt()) == op.GetReal()));
    }
    // both int and float, make result float
    else if (IsReal() && op.IsInt()){
        return Value((bool)(GetReal() == ((float)op.GetInt())));
    }
    return Value();
}