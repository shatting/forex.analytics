#include "nan.h"
#include "../../../include/export/findStrategy/FindStrategyBaton.h"
#include "../../../include/Analytics.h"
#include "../../../include/export/findStrategy/StrategyUpdateBaton.h"
#include "../../../include/export/findStrategy/FindStrategyAsyncWorker.h"

FindStrategyAsyncWorker::FindStrategyAsyncWorker(FindStrategyBaton *baton_, Nan::Callback *callback_) : AsyncProgressWorkerBase(callback_){
    //printf("FindStrategyAsyncWorker::create\n");
    baton = baton_;
}

void FindStrategyAsyncWorker::Execute(const ExecutionProgress &execProgress) {

    Analytics system;
    Nan::Callback *progress = baton->progress;
    ExecutionProgress *pp = &const_cast<ExecutionProgress &>(execProgress);

    //printf("FindStrategyAsyncWorker::Execute\n");

    auto update = [progress, pp](double fitness, BinaryTreeChromosome *chromosome, int generation) {
        if(progress != nullptr) {
            StrategyUpdateBaton updateBaton;
            updateBaton.fitness = fitness;
            updateBaton.generation = generation;
            updateBaton.chromosome = chromosome;

            pp->Send(reinterpret_cast<const char *>(&updateBaton), sizeof(StrategyUpdateBaton));
        }
    };

    try {
        baton->chromosome = system.PerformAnalysis(
                baton->closeValues,
                baton->indicatorNames,
                baton->indicatorMin,
                baton->indicatorMax,
                baton->input,
                baton->populationCount,
                baton->generationCount,
                baton->selectionAmount,
                baton->leafValueMutationProbability,
                baton->leafSignMutationProbability,
                baton->logicalNodeMutationProbability,
                baton->leafIndicatorMutationProbability,
                baton->crossoverProbability,
                baton->pipInDecimals,
                baton->spread,
                baton->startingChromosome,
                update
        );
    } catch (const char *error) {
        baton->errorMessage = error;
        baton->chromosome = nullptr;
    }
}

void FindStrategyAsyncWorker::HandleOKCallback() {
    Nan::HandleScope scope;

    if(baton->chromosome == nullptr) {

        v8::Handle<v8::Value> argv[] = {v8::Exception::TypeError(Nan::New<v8::String>(baton->errorMessage).ToLocalChecked()), Nan::Undefined()};

        this->callback->Call(2, argv);
    } else {
        v8::Local<v8::Object> strategy;

        chromosomeToObject(baton, strategy);

        v8::Handle<v8::Value> argv[] = {Nan::Undefined(), strategy};

        this->callback->Call(2, argv);
    }

    if(baton->progress != nullptr)
        delete baton->progress;

    if(baton->chromosome != nullptr)
        delete baton->chromosome;

    if(baton->startingChromosome != nullptr)
        delete baton->startingChromosome;

    delete baton;
}

void FindStrategyAsyncWorker::HandleErrorCallback() {
    Nan::HandleScope scope;

    v8::Handle<v8::Value> argv[] = {v8::Exception::TypeError(Nan::New<v8::String>(ErrorMessage()).ToLocalChecked()), Nan::Undefined()};

    this->callback->Call(2, argv);

    if(baton->progress != nullptr)
        delete baton->progress;

    if(baton->chromosome != nullptr)
        delete baton->chromosome;

    delete baton;
}

void FindStrategyAsyncWorker::HandleProgressCallback(const char *data, size_t size) {
    Nan::HandleScope scope;

    StrategyUpdateBaton *updateBaton = reinterpret_cast<StrategyUpdateBaton *>(const_cast<char *>(data));

    v8::Local<v8::Object> strategy;

    chromosomeToObject(updateBaton, strategy);

    v8::Handle<v8::Value> argv[] = {strategy, v8::Handle<v8::Value>(Nan::New<v8::Number>(updateBaton->fitness)), v8::Handle<v8::Value>(Nan::New<v8::Number>(updateBaton->generation)),};

    this->baton->progress->Call(3, argv);
}