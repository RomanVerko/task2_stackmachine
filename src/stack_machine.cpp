////////////////////////////////////////////////////////////////////////////////
// Module Name:  stack_machine.h/cpp
// Authors:      Sergey Shershakov
// Version:      0.2.0
// Date:         23.01.2017
//
// This is a part of the course "Algorithms and Data Structures"
// provided by  the School of Software Engineering of the Faculty
// of Computer Science at the Higher School of Economics.
////////////////////////////////////////////////////////////////////////////////

#include "stack_machine.h"

#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>

namespace xi {

//==============================================================================
// Free functions -- helpers
//==============================================================================

// TODO: if you need any free functions, add their definitions here.

std::vector<std::string> split(std::string str)
{
    std::vector<std::string> v;
    std::string buffer = "";
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            v.push_back(buffer);
            buffer = "";
        }
        else
            buffer += str[i];
    }
    v.push_back(buffer);
    return v;
}

bool canBeNumber(std::string str)
{
    for (char i : str)
        if (i > '9' or i < '0')
            return false;
        return true;
}

char check(std::string str)
{
    if (str.length() != 1)
        return -1;
    return str[0];
}

//==============================================================================
// class PlusOp
//==============================================================================


int PlusOp::operation(char op, int a, int b, int /*c*/) // < just commented unused argument. This does not affect the code anyhow.
{
    if(op != '+')
        throw std::logic_error("Operation other than Plus (+) are not supported");

    // here we just ignore unused operands
    return a + b;
}

IOperation::Arity PlusOp::getArity() const
{
    return arDue;
}

//==============================================================================
// class MinusOp
//==============================================================================

int MinusOp::operation(char op, int a, int b, int c)
{
    if(op != '-')
        throw std::logic_error("Operation other than Minus (-) are not supported");

    return a - b;
}

IOperation::Arity MinusOp::getArity() const
{
    return arDue;
}

//==============================================================================
// class InverOp
//==============================================================================

int InverOp::operation(char op, int a, int b, int c)
{
    if(op != '~')
        throw std::logic_error("Operation other than Invention (~) are not supported");

    return ~a;
}
IOperation::Arity InverOp::getArity() const
{
    return arUno;
}

//==============================================================================
// class OrOp
//==============================================================================

int OrOp::operation(char op, int a, int b, int c)
{
    if(op != '|')
        throw std::logic_error("Operation other than Or (|) are not supported");

    return a|b;
}

IOperation::Arity OrOp::getArity() const
{
    return arDue;
}

//==============================================================================
// class StackMachine
//==============================================================================


// TODO: put StackMachine methods implementation here


    void StackMachine::registerOperation(char symb, IOperation *oper)
    {
        SymbolToOperMapConstIter it = _opers.find(symb);
        if (it!=_opers.end())
            throw std::logic_error("An operation ... is alr. reg...");

        _opers[symb] = oper;
    }

    IOperation* StackMachine::getOperation(char symb)
    {
        SymbolToOperMapConstIter it = _opers.find(symb);
        if (it == _opers.end())
            return nullptr;
        return _opers[symb];
    }

    int StackMachine::calculate(const std::string &expr, bool clearStack)
    {
        std::vector<std::string> elements = split(expr);
        for (int i = 0; i < elements.size(); i++)
        {
            if (canBeNumber(elements[i]))
            {
                _s.push(std::atoi((elements[i].c_str())));
            }
            else
            {
                IOperation* operation = nullptr;
                if (getOperation(check(elements[i])) == nullptr)
                    throw std::logic_error("Error");

                operation = getOperation(check(elements[i]));
                int amount = operation->getArity();
                int a,b,c;
                switch(amount)
                {
                    case IOperation::arUno:
                        {
                            a = _s.pop();
                            break;
                        }
                    case IOperation::arDue:
                    {
                        b = _s.pop();
                        a = _s.pop();
                        break;
                    }
                    case IOperation::arTre:
                    {
                        c = _s.pop();
                        b = _s.pop();
                        a = _s.pop();
                        break;
                    }
                }
                _s.push(operation->operation(check(elements[i]), a, b ,c));
            }
        }
        return _s.top();
    }
} // namespace xi
