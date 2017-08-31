// Copyright 2016 Marek Magdziak

#include <iostream>
#include <vector>
#include <exception>
#include "nan.h"

#include "../../include/nodes/IndicatorTreeNode.h"

IndicatorTreeNode::IndicatorTreeNode(const std::vector<std::string> &indicatorNames,
                                     const std::unordered_map<std::string, double> &indicatorMin,
                                     const std::unordered_map<std::string, double> &indicatorMax
) : indicatorNames(indicatorNames),
    indicatorMin(indicatorMin),
    indicatorMax(indicatorMax),
    value(0),
    sign(Sign::Lt)
{

    sign_uniform_dist = std::uniform_int_distribution<int>(0, 1);
    indicator_name_uniform_dist = std::uniform_int_distribution<int>(0, indicatorNames.size() - 1);

    std::string n;
    for (unsigned long i = 0; i < indicatorNames.size(); i++) {
        n = indicatorNames[i];
        indicator_value_uniform_dists[n]
                = std::uniform_real_distribution<double>(indicatorMin.at(n),indicatorMax.at(n));
    }
    //printf("created indicatortreenode names:%i, min: %i, max:%i\n", indicatorNames.size(), indicatorMin.size(), indicatorMax.size());

}

IndicatorTreeNode::~IndicatorTreeNode() {
}

bool IndicatorTreeNode::Evaluate(const std::unordered_map<std::string, double> &data) const {

    for (unsigned long i = 0; i < this->indicatorNames.size(); i++) {
        if(this->indicatorNames[i] == this->indicator) {
            switch (sign) {
                case Sign::Gt:
                    return value > data.at(this->indicator);
                case Sign::Lt:
                    return value < data.at(this->indicator);
            }

            return false;
        }
    }

    throw std::runtime_error("could not find strategy indicator '" + indicator + "' in indicator data");
}

std::string IndicatorTreeNode::toString() const {
    std::string opstr = "?";

    switch (sign) {
        case Sign::Gt:
            opstr = "<";
            break;
        case Sign::Lt:
            opstr = ">";
            break;
    }

    return "( " + indicator + " " + opstr + " " + std::to_string(value) + " )";
}


void IndicatorTreeNode::GenerateRandomValue() {
    GenerateRandomSign();
    GenerateRandomIndicator();
    //printf("created random indicatorTreeNode: %s\n",toString().c_str());
}

void IndicatorTreeNode::GenerateRandomSign() {
    sign = static_cast<Sign>(sign_uniform_dist(engine));
}

void IndicatorTreeNode::GenerateRandomIndicator() {
    indicator = indicatorNames[indicator_name_uniform_dist(engine)];

    GenerateRandomNumbericValue();
}

void IndicatorTreeNode::GenerateRandomNumbericValue() {
    for (unsigned long i = 0; i < indicatorNames.size(); i++) {
        if(indicatorNames[i] == indicator) {
            value = indicator_value_uniform_dists[indicatorNames[i]](engine);
            break;
        }
    }
}

void IndicatorTreeNode::CopyTo(TreeNode *destination) const {
    IndicatorTreeNode *dest = reinterpret_cast<IndicatorTreeNode *>(destination);

    dest->value = this->value;
    dest->indicator = this->indicator;
    dest->sign = this->sign;
}

TreeNode *IndicatorTreeNode::Copy() const {
    IndicatorTreeNode *dest = new IndicatorTreeNode(indicatorNames, indicatorMin, indicatorMax);

    dest->value = this->value;
    dest->indicator = this->indicator.c_str();
    dest->sign = this->sign;

    return dest;
}

void IndicatorTreeNode::ToJs(v8::Local<v8::Object> &object) const {
    object->Set(Nan::New<v8::String>("indicator").ToLocalChecked(), Nan::New<v8::String>(this->indicator.c_str()).ToLocalChecked());

    object->Set(Nan::New<v8::String>("sign").ToLocalChecked(), Nan::New<v8::String>(SignStrings[this->sign]).ToLocalChecked());

    object->Set(Nan::New<v8::String>("value").ToLocalChecked(), Nan::New<v8::Number>(value));

    TreeNode::ToJs(object);
}

TreeNode *IndicatorTreeNode::FromJs(const std::vector<std::string> &indicatorNames,
                                    const std::unordered_map<std::string, double> &indicatorMin,
                                    const std::unordered_map<std::string, double> &indicatorMax,
                                    const v8::Local<v8::Object> &input) {

    if(!input->Has(Nan::New<v8::String>("indicator").ToLocalChecked()) || !input->Has(Nan::New<v8::String>("sign").ToLocalChecked()) || !input->Has(Nan::New<v8::String>("value").ToLocalChecked()))
        return nullptr;

    std::string sign = std::string(*v8::String::Utf8Value(input->Get(Nan::New<v8::String>("sign").ToLocalChecked())->ToString()));

    Sign s = Sign::Gt;

    for (int i = 0; i < 2; i++) {
        if(sign == SignStrings[i]) {
            s = static_cast<Sign>(i);
        }
    }

    IndicatorTreeNode *node = new IndicatorTreeNode(indicatorNames, indicatorMin, indicatorMax);
    node->sign = s;
    node->indicator = std::string(*v8::String::Utf8Value(input->Get(Nan::New<v8::String>("indicator").ToLocalChecked())->ToString()));
    node->value = input->Get(Nan::New<v8::String>("value").ToLocalChecked())->NumberValue();

    return node;
}
