#include "../../include/utils/Utils.h"

int getIntOrDefault(v8::Handle<v8::Object> object, const char *name, int def) {
    if(!object->Has(Nan::New<v8::String>(name).ToLocalChecked()))
        return def;

    return object->Get(Nan::New<v8::String>(name).ToLocalChecked())->Int32Value();
}

double getNumberOrDefault(v8::Handle<v8::Object> object, const char *name, double def) {
    if(!object->Has(Nan::New<v8::String>(name).ToLocalChecked()))
        return def;

    return object->Get(Nan::New<v8::String>(name).ToLocalChecked())->NumberValue();
}

v8::Handle<v8::Object> getObjectFromArguments(const Nan::FunctionCallbackInfo<v8::Value> &args, int index) {
    if(args.Length() - 1 >= index && !args[index]->IsUndefined())
        return v8::Handle<v8::Object>::Cast(args[index]);

    return Nan::New<v8::Object>();
}

v8::Handle<v8::Array> getArrayFromArguments(const Nan::FunctionCallbackInfo<v8::Value> &args, int index) {
    if(args.Length() - 1 >= index && !args[index]->IsUndefined())
        return v8::Handle<v8::Array>::Cast(args[index]);

    return Nan::New<v8::Array>();
}

void PopulateIndicators(std::vector<std::string> * indicatorNames,
                        std::unordered_map<std::string, std::vector<double>> * input,
                        std::unordered_map<std::string, double> * indicatorMax,
                        std::unordered_map<std::string, double> * indicatorMin,
                        v8::Handle<v8::Object> indicators) {

    v8::Handle<v8::Array> keys = Nan::GetOwnPropertyNames(indicators).ToLocalChecked();
    //printf("populating %i indicators\n", keys->Length());

    for (unsigned i = 0; i < keys->Length(); ++i) {

        v8::Local<v8::String> key = v8::Local<v8::String>::Cast(keys->Get(i));
        std::string name = std::string(*v8::String::Utf8Value(key));

        indicatorNames->push_back(name);

        (*input)[name] = jsArrayToDoubleArray(v8::Handle<v8::Array>::Cast(indicators->Get(key)));

        auto smallest = std::min_element(std::begin((*input)[name]), std::end((*input)[name]));
        (*indicatorMin)[name] = *smallest;

        auto biggest = std::max_element(std::begin((*input)[name]), std::end((*input)[name]));
        (*indicatorMax)[name] = *biggest;
    }
}

std::vector<double> jsArrayToDoubleArray(v8::Handle<v8::Array> array) {

    std::vector<double> result = std::vector<double>();

    for (unsigned i = 0; i < array->Length(); ++i) {
        result.push_back(array->Get(i)->NumberValue());
    }
    return result;
}

std::vector<std::unordered_map<std::string, double>> RemapMapArray(std::unordered_map<std::string, std::vector<double>> *input) {
    std::vector<std::unordered_map<std::string, double>> result = std::vector<std::unordered_map<std::string, double>>();
    long i;

    for (auto ind : *input) {
        i = 0;
        for (auto v : ind.second) {
            if (result.size() < i+1)
                result.push_back(std::unordered_map<std::string, double>());

            result[i][ind.first] = v;
            //printf("result[%i][%s] = %f\n",i,ind.first.c_str(),v);
            i++;
        }
    }

    return result;
};