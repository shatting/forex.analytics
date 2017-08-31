#include <nan.h>
#include "../../../include/TradingSimulator.h"
#include "../../../include/utils/Utils.h"

NAN_METHOD(getTrades) {
// expecting two arguments 1. candlestick data 2. trade settings
    if(info.Length() < 3) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    if(!info[0]->IsArray()) {
        Nan::ThrowTypeError("Wrong first argument. Expecting close values array.");
        return;
    }

    if(!info[1]->IsObject()) {
        Nan::ThrowTypeError("Wrong second argument. Expecting object with indicator data");
        return;
    }

    if(!info[2]->IsObject() || info[2]->IsArray()) {
        Nan::ThrowTypeError("Wrong second argument. Expecting object with strategy configuration");
        return;
    }
    std::vector<double> closePrices = jsArrayToDoubleArray(getArrayFromArguments(info, 0));
    v8::Handle<v8::Object> indicatorData = v8::Handle<v8::Object>::Cast(info[1]);
    v8::Handle<v8::Object> configuration = v8::Handle<v8::Object>::Cast(info[2]);
    v8::Handle<v8::Object> strategy = v8::Handle<v8::Object>::Cast(configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));


    TradingSimulator simulator;
    v8::Local<v8::Array> output = Nan::New<v8::Array>();
    std::vector<std::string> indicatorNames;
    std::unordered_map<std::string, std::vector<double>> indicatorValues;
    std::unordered_map<std::string,double> indicatorMax;
    std::unordered_map<std::string,double> indicatorMin;

    PopulateIndicators(&indicatorNames, &indicatorValues, &indicatorMin, &indicatorMax, indicatorData);

    std::vector<std::unordered_map<std::string, double>> remappedValues = RemapMapArray(&indicatorValues);

    BinaryTreeChromosome *chromosome = BinaryTreeChromosome::FromJs(indicatorNames, indicatorMin, indicatorMax, strategy);

    std::vector<Trade> *trades = simulator.Simulate(chromosome, &closePrices, &remappedValues);

    Trade::ToArray(*trades, output);

    info.GetReturnValue().Set(output);

    delete trades;
    delete chromosome;
} // getTrades
