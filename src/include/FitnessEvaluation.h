#ifndef _FITNESS_EVALUATION_H
#define _FITNESS_EVALUATION_H

#include "TradingSimulator.h"

/**
 * Fitness fucntion for chromosome quality evaluation
 *
 * @param  chromosome Pointer to evaluated chromosome
 * @param  data       input data containing generated indicator values
 * @return            fitness of the passed chromosome
 */
inline double EvaluateFitness(FitnessFunctionArgs args) {
    TradingSimulator simulator;

    //printf("eval %i, %i\n", args.closePrices->size(), args.data->size());
    std::vector<Trade> *trades = simulator.Simulate(args.chromosome, args.closePrices, args.data);

    if(trades->size() == 0) {
        return 0;
    }

    double points = 0;

    for (unsigned long i = 0; i < trades->size(); i++) {
        Trade * trade = &trades->at(i);
        int duration =trade->End - trade->Start;
        trade->revenue = trade->EndPrice*(1 - args.pipInDecimals) - trade->StartPrice*(1 + args.pipInDecimals);

        points += 100*trade->revenue;
        /*if(trade->revenue > 0) {
            points += 100*trade->revenue / duration;
        } else {
            points += 400*trade->revenue / duration;
        }*/
    }

    double fitness = points;
    delete trades;

    return fitness;
}

#endif
