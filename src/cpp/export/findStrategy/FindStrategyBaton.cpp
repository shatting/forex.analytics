#include "../../../include/export/findStrategy/FindStrategyBaton.h"
#include "../../../include/utils/Utils.h"

FindStrategyBaton::FindStrategyBaton(v8::Handle<v8::Array> closePrices, v8::Handle<v8::Object> indicatorToValues, v8::Handle<v8::Object> configuration, Nan::Callback * progress, Nan::Callback * callback) {

    //printf("creating findStrategyBaton\n");
    this->startingChromosome = nullptr;
    this->progress = nullptr;
    this->chromosome = nullptr;
    this->startingChromosome = nullptr;
    this->errorMessage = "";

    this->populationCount = getIntOrDefault(configuration, "populationCount", DEFAULT_POPULATION_COUNT);
    this->generationCount = getIntOrDefault(configuration, "generationCount", DEFAULT_GENERATION_COUNT);
    this->selectionAmount = getIntOrDefault(configuration, "selectionAmount", DEFAULT_SELECTION_AMOUNT);
    this->leafValueMutationProbability = getNumberOrDefault(configuration, "leafValueMutationProbability", DEFAULT_LEAF_VALUE_MUTATION_PROBABILITY);
    this->leafSignMutationProbability = getNumberOrDefault(configuration, "leafSignMutationProbability", DEFAULT_LEAF_SIGN_MUTATION_PROBABILITY);
    this->logicalNodeMutationProbability = getNumberOrDefault(configuration, "logicalNodeMutationProbability", DEFAULT_LOGICAL_NODE_MUTATION_PROBABILITY);
    this->leafIndicatorMutationProbability = getNumberOrDefault(configuration, "leafIndicatorMutationProbability", DEFAULT_LEAF_INDICATOR_MUTATION_PROBABILITY);
    this->crossoverProbability = getNumberOrDefault(configuration, "crossoverProbability", DEFAULT_CROSSOVER_PROBABILITY);
    this->spread = getNumberOrDefault(configuration, "spread", DEFAULT_SPREAD);
    this->pipInDecimals = getNumberOrDefault(configuration, "pipInDecimals", DEFAULT_PIP_IN_DECIMALS);

    this->progress = progress;
    this->callback = callback;
    this->closeValues = jsArrayToDoubleArray(closePrices);

    std::unordered_map<std::string, std::vector<double>> indicatorValues;


    PopulateIndicators(&indicatorNames, &indicatorValues, &indicatorMin, &indicatorMax, indicatorToValues);

    input = RemapMapArray(&indicatorValues);


    if(configuration->Has(Nan::New<v8::String>("strategy").ToLocalChecked())) {
        startingChromosome = BinaryTreeChromosome::FromJs(indicatorNames, indicatorMin, indicatorMax, v8::Handle<v8::Object>::Cast(configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked())));
    }
    //printf("names: %i, input.size=%i, closeValues.size=%i, indicatorMin.size=%i, indicatorMax.size=%i\n",indicatorNames.size(), input.size(), closeValues.size(), indicatorMin.size(), indicatorMax.size());
}
