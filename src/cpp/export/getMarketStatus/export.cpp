#include <nan.h>

#include "../../../include/Analytics.h"
#include "../../../include/utils/Utils.h"

NAN_METHOD(getMarketStatus) {
    // expecting 2 arguments
    //  1. indicator data
    //  2. configuration {strategy}

    if(info.Length() < 2) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    try {
        //TODO validate configuration
        v8::Handle<v8::Object> indicatorData    = v8::Handle<v8::Object>::Cast(info[0]);
        v8::Handle<v8::Object> configuration    = v8::Handle<v8::Object>::Cast(info[1]);
        v8::Handle<v8::Object> strategy         = v8::Handle<v8::Object>::Cast(configuration->Get(Nan::New<v8::String>("strategy").ToLocalChecked()));

        std::vector<std::string> indicatorNames;
        std::unordered_map<std::string, std::vector<double>> indicatorValues;
        std::unordered_map<std::string,double> indicatorMax;
        std::unordered_map<std::string,double> indicatorMin;

        PopulateIndicators(&indicatorNames, &indicatorValues, &indicatorMin, &indicatorMax, indicatorData);

        BinaryTreeChromosome *chromosome = BinaryTreeChromosome::FromJs(indicatorNames, indicatorMin, indicatorMax, strategy);

        std::vector<std::unordered_map<std::string, double>> remappedValues = RemapMapArray(&indicatorValues);

        bool shouldBuy = chromosome->shouldBuy(remappedValues[remappedValues.size() - 1]);
        bool shouldSell = chromosome->shouldSell(remappedValues[remappedValues.size() - 1]);

        v8::Local<v8::Object> output = Nan::New<v8::Object>();

        output->Set(Nan::New<v8::String>("shouldBuy").ToLocalChecked(), Nan::New<v8::Boolean>(shouldBuy));

        output->Set(Nan::New<v8::String>("shouldSell").ToLocalChecked(), Nan::New<v8::Boolean>(shouldSell));

        info.GetReturnValue().Set(output);

        delete chromosome;
    } catch (std::runtime_error e) {
        Nan::ThrowTypeError(e.what());
    }
} // getTrades
