#ifndef _FIND_STRATEGY_BATON_H
#define _FIND_STRATEGY_BATON_H

#include <nan.h>
#include <node.h>

#include "../../BinaryTreeChromosome.h"

const int DEFAULT_POPULATION_COUNT = 100;
const int DEFAULT_GENERATION_COUNT = 100;
const int DEFAULT_SELECTION_AMOUNT = 10;

const double DEFAULT_LEAF_VALUE_MUTATION_PROBABILITY = 0.5;
const double DEFAULT_LEAF_SIGN_MUTATION_PROBABILITY = 0.3;
const double DEFAULT_LOGICAL_NODE_MUTATION_PROBABILITY = 0.3;
const double DEFAULT_LEAF_INDICATOR_MUTATION_PROBABILITY = 0.2;
const double DEFAULT_CROSSOVER_PROBABILITY = 0.03;
const double DEFAULT_SPREAD = 0.0;
const double DEFAULT_PIP_IN_DECIMALS = 0.001;
const double DEFAULT_TREE_HEIGHT = 3;

class FindStrategyBaton {
public:
    Nan::Callback *progress;
    Nan::Callback *callback;
    BinaryTreeChromosome *chromosome;
    BinaryTreeChromosome *startingChromosome;
    std::vector<std::unordered_map<std::string, double>> input;
    std::vector<std::string> indicatorNames;
    std::unordered_map<std::string,double> indicatorMin;
    std::unordered_map<std::string,double> indicatorMax;
    std::vector<double> closeValues;
    const char *errorMessage;
    unsigned populationCount;
    unsigned generationCount;
    unsigned selectionAmount;
    double leafValueMutationProbability;
    double leafSignMutationProbability;
    double logicalNodeMutationProbability;
    double leafIndicatorMutationProbability;
    double crossoverProbability;
    double spread;
    int treeHeight;
    double pipInDecimals;

    FindStrategyBaton(v8::Handle<v8::Array> closePrices, v8::Handle<v8::Object> indicatorToValues, v8::Handle<v8::Object> configuration, Nan::Callback * progress, Nan::Callback * callback);

};

#endif
