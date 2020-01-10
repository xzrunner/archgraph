#pragma once

#include "cga/VarType.h"

#include <string>

namespace cga
{

class Variant
{
public:
    virtual VarType Type() const = 0;

    virtual bool ToBool() const = 0;
    virtual float ToFloat() const = 0;
    virtual const char* ToString() const = 0;

}; // Variant

class BoolVar : public Variant
{
public:
    explicit BoolVar(bool b) : m_val(b) {}

    virtual VarType Type() const override {
        return VarType::Boolean;
    }

    virtual bool ToBool() const override {
        return m_val;
    }
    virtual float ToFloat() const override {
        return m_val ? 1.0f : 0.0f;
    }
    virtual const char* ToString() const override;

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

    virtual bool ToBool() const override {
        return m_val == 0 ? false : true;
    }
    virtual float ToFloat() const override {
        return m_val;
    }
    virtual const char* ToString() const override;

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

    virtual bool ToBool() const override;
    virtual float ToFloat() const override;
    virtual const char* ToString() const override {
        return m_val.c_str();
    }

private:
    std::string m_val;

}; // StringVar

}