#include "../../../include/Analytics.h"
#include "StrategyUpdateBaton.h"

template<typename T>
static void chromosomeToObject(T *baton, v8::Local<v8::Object> &strategy) {
    strategy = Nan::New<v8::Object>();
    baton->chromosome->ToJs(strategy);
}

class FindStrategyAsyncWorker : public Nan::AsyncProgressWorker {
public:
    explicit FindStrategyAsyncWorker(FindStrategyBaton *baton_, Nan::Callback *callback_);

    void Execute(const ExecutionProgress &progress) override;

    void HandleOKCallback() override;

    void HandleErrorCallback() override;

    void HandleProgressCallback(const char *data, size_t size) override;

private:
    FindStrategyBaton *baton;
};
