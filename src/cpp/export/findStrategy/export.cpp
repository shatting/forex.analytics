#include <nan.h>

#include "../../../include/export/findStrategy/FindStrategyBaton.h"
#include "../../../include/export/findStrategy/FindStrategyAsyncWorker.h"
#include "../../../include/utils/Utils.h"

bool findStrategyValidateInput(const Nan::FunctionCallbackInfo<v8::Value> &args);

bool validateConfiguration(v8::Handle<v8::Object> configuration);


NAN_METHOD(findStrategy) {
    if(findStrategyValidateInput(info)) {

        v8::Handle<v8::Array> closePrices = getArrayFromArguments(info, 0);
        v8::Handle<v8::Object> indicators = getObjectFromArguments(info, 1);
        v8::Handle<v8::Object> configuration = getObjectFromArguments(info, 2);
        Nan::Callback * update = new Nan::Callback(info[3].As<v8::Function>());
        Nan::Callback * callback = new Nan::Callback(info[4].As<v8::Function>());

        FindStrategyBaton *baton = new FindStrategyBaton(closePrices, indicators, configuration, update, callback);

        auto worker = new FindStrategyAsyncWorker(baton, baton->callback);

        Nan::AsyncQueueWorker(worker);
    }

    info.GetReturnValue().Set(Nan::Undefined());
} // findStrategy


bool findStrategyValidateInput(const Nan::FunctionCallbackInfo<v8::Value> &args) {
    if(args.Length() < 5) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return false;
    }

    if(args[0]->IsUndefined() ||!args[0]->IsArray()) {
        Nan::ThrowTypeError("Wrong first argument. Expecting array of close values");
        return false;
    }

    if(args[1]->IsUndefined() || !args[1]->IsObject()) {
        Nan::ThrowTypeError("Wrong second argument. Expecting object {'INDICATOR1':[x1,y1,..], 'INDICATOR2':[x2,y2,..]}");
        return false;
    }

    if(args[2]->IsUndefined() || !args[2]->IsObject() || args[2]->IsArray()) {
        Nan::ThrowTypeError("Wrong third argument. Expecting object with genetic algorithm configuration");
        return false;
    }

    if(!args[3]->IsUndefined() && (!args[3]->IsFunction())) {
        Nan::ThrowTypeError("Wrong fourth argument. Expecting and update callbac)");
        return false;
    }

    if(!args[4]->IsUndefined() && (!args[4]->IsFunction())) {
        Nan::ThrowTypeError("Wrong fifth argument. Expecting a callback");
        return false;
    }

    if(!validateConfiguration(getObjectFromArguments(args, 2))) {
        return false;
    }

    return true;
}

bool validateConfiguration(v8::Handle<v8::Object> configuration) {

    if(!configuration->Has(Nan::New<v8::String>("pipInDecimals").ToLocalChecked())) {
        Nan::ThrowTypeError("Expecting configuration object to have pipInDecimals value set");
        return false;
    }

    if(!configuration->Get(Nan::New<v8::String>("pipInDecimals").ToLocalChecked())->IsNumber()) {
        Nan::ThrowTypeError("Expecting pipInDecimals to be a number");
        return false;
    }

    if(getNumberOrDefault(configuration, "pipInDecimals", 0) <= 0) {
        Nan::ThrowTypeError("Expecting pipInDecimals to be larger than 0");
        return false;
    }

    return true;
}
