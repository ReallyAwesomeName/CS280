/* val.cpp
 * RA8
 * Jason Geoghegan
 * CS280-002
 * Professor Bassel Arafeh
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
        if (IsString()){
            return Value(Stemp + op.GetString());
        }
    }
    else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())){
        if (op.IsReal()){
            return Value((float)GetInt() + op.GetReal());
        }
        else if (op.IsInt()){
            return Value(GetReal() + (float)op.GetInt());
        }
    }
    return Value();
}

// subtract op from this
Value Value::operator-(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp - op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp - op.GetReal());
        }
    }
    else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())){
        if (op.IsReal()){
            return Value((float)GetInt() - op.GetReal());
        }
        else if (op.IsInt()){
            return Value(GetReal() - (float)op.GetInt());
        }
    }
    return Value();
}

// multiply this by op
Value Value::operator*(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp * op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp * op.GetReal());
        }
    }
    else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())){
        if (op.IsReal()){
            return Value((float)GetInt() * op.GetReal());
        }
        else if (op.IsInt()){
            return Value(GetReal() * (float)op.GetInt());
        }
    }
    return Value();
}

// divide this by op
Value Value::operator/(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp / op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp / op.GetReal());
        }
    }
    else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())){
        if (op.IsReal()){
            return Value((float)GetInt() / op.GetReal());
        }
        else if (op.IsInt()){
            return Value(GetReal() / (float)op.GetInt());
        }
    }
    return Value();
}

// compare this to op
Value Value::operator==(const Value& op) const{
    if (GetType() == op.GetType()){
        if (IsInt()){
            return Value(Itemp == op.GetInt());
        }
        if (IsReal()){
            return Value(Rtemp == op.GetReal());
        }
        if (IsString()){
            return Value(Stemp == op.GetString());
        }
    }
    else if ((IsInt() && op.IsReal()) || (IsReal() && op.IsInt())){
        if (op.IsReal()){
            return Value((float)GetInt() == op.GetReal());
        }
        else if (op.IsInt()){
            return Value(GetReal() == (float)op.GetInt());
        }
    }
    return Value();
}
