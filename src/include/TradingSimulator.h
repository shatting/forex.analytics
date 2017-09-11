#pragma once


#include "BinaryTreeFitness.h"
#include "BinaryTreeChromosome.h"

/**
 * Represents a model of one trade having a start and an end defined by
 * a specific candlestick.
 */
class Trade {
public:

/**
 * Candlestic defining entry of this specific trade
 */
    long Start;

    double StartPrice;

/**
 * Candlestic defining exit of this specific trade
 */
    long End;

    double EndPrice;

    double revenue;
/**
 * Converts a vector of trades to a v8 array
 * @param trades  The input vector if trades
 * @param output  The output v8 array
 */
    static void ToArray(const std::vector<Trade> &trades, v8::Local<v8::Array> &output);

/**
 * Converts an input trade to a v8 object
 * @param trade   The input trade object
 * @param output  The output v8 object
 */
    static void ToObject(const Trade &trade, v8::Local<v8::Object> &output);
};


/**
 * Simulator simulates trading processes and generates trades based on
 * a defined strategy
 */
class TradingSimulator {
public:

/**
 * Simulates a certain strategy on a set of test data and returns all performed \
 * trades within the boundaries of the test data
 * @param chromosome Chromosome containing trading strategy which is then
 *                   applied on each indicator value in the input data
 * @param data      Input data used for trade evaluation
 * @return          Vector of trades made with passed strategy and test data
 */
    std::vector<Trade> *Simulate(const BinaryTreeChromosome *chromosome, const std::vector<double> *closePrices, const std::vector<std::unordered_map<std::string, double>> *data) const;
};