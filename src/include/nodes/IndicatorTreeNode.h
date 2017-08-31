#pragma once

#include "TreeNode.h"

static const char *SignStrings[] = {">", "<"};

/**
 * A sign enum used for comparism of strategy required values
 * with real values
 */
enum Sign {
    Gt = 0, Lt = 1
};

class IndicatorTreeNode : public TreeNode {
private:
    const std::vector<std::string> &indicatorNames;
    const std::unordered_map<std::string, double> &indicatorMin;
    const std::unordered_map<std::string, double> &indicatorMax;

    std::uniform_int_distribution<int> sign_uniform_dist;
    std::uniform_int_distribution<int> indicator_name_uniform_dist;
    std::unordered_map<std::string,std::uniform_real_distribution<double>> indicator_value_uniform_dists;

public:
    std::string indicator;
    double value;
    Sign sign;


    IndicatorTreeNode(const std::vector<std::string> &indicatorNames, const std::unordered_map<std::string, double> &indicatorMin, const std::unordered_map<std::string, double> &indicatorMax);

    ~IndicatorTreeNode();

    // Inherited via TreeNode
    bool Evaluate(const std::unordered_map<std::string, double> &data) const override;

    virtual std::string toString() const override;

    void GenerateRandomValue() override;

    virtual void GenerateRandomSign();

    virtual void GenerateRandomIndicator();

    virtual void GenerateRandomNumbericValue();

    void CopyTo(TreeNode *destination) const override;

    virtual TreeNode *Copy() const override;

    void ToJs(v8::Local<v8::Object> &object) const override;

    static TreeNode *FromJs(const std::vector<std::string> &indicators,
                            const std::unordered_map<std::string, double> &indicatorMin,
                            const std::unordered_map<std::string, double> &indicatorMax,
                            const v8::Local<v8::Object> &input);
};
