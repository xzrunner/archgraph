#pragma once

#include "archgraph/VarType.h"

#include <string>

namespace archgraph
{

class Variant
{
public:
    virtual VarType Type() const = 0;
}; // Variant

class BoolVar : public Variant
{
public:
    explicit BoolVar(bool b) : m_val(b) {}

    virtual VarType Type() const override {
        return VarType::Boolean;
    }

    auto& GetValue() const { return m_val; }

private:
    bool m_val;

}; // BoolVar

class FloatVar : public Variant
{
public:
    explicit FloatVar(float f) : m_val(f) {}

    virtual VarType Type() const override {
        return VarType::Float;
    }

    auto& GetValue() const { return m_val; }

private:
    float m_val;

}; // FloatVar

class StringVar : public Variant
{
public:
    explicit StringVar(const std::string& str) : m_val(str) {}

    virtual VarType Type() const override {
        return VarType::String;
    }

    auto& GetValue() const { return m_val; }

private:
    std::string m_val;

}; // StringVar

bool check_bool(const std::shared_ptr<Variant>& var);
float check_float(const std::shared_ptr<Variant>& var);
std::string check_string(const std::shared_ptr<Variant>& var);

}