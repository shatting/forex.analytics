#include "nan.h"

#include "../include/TradingSimulator.h"
#include <iostream>

std::vector<Trade> *TradingSimulator::Simulate(const BinaryTreeChromosome *chromosome, const std::vector<double> *closePrices, const std::vector<std::unordered_map<std::string, double>> *data) const {

    std::vector<Trade> *trades = new std::vector<Trade>();

    int startIndex = -1;
    double startPrice = 0;

    bool shouldBuy, shouldSell;

    for (int i = 0; i < data->size(); i++) {
        shouldBuy = chromosome->shouldBuy(data->at(i));
        shouldSell = chromosome->shouldSell(data->at(i));

        if(shouldBuy && !shouldSell && startIndex == -1) {

            startIndex = i;
            startPrice = closePrices->at(i);

        } else if(!shouldBuy && shouldSell && startIndex >= 0) {

            Trade t;
            t.Start = startIndex;
            t.End = i;
            t.StartPrice = startPrice;
            t.EndPrice = closePrices->at(i);

            trades->push_back(t);

            startIndex = -1;
            startPrice = 0;
        }

    }

    return trades;
} // TradingSimulator::Simulate


/*double Trade::getRevenue() const {
    return this->Buy ? this->EndPrice - this->StartPrice : this->StartPrice - this->EndPrice;
}*/

void Trade::ToArray(const std::vector<Trade> &trades, v8::Local<v8::Array> &output) {

    size_t tradeCount = trades.size();

    for (size_t i = 0; i < tradeCount; i++) {

        v8::Local<v8::Object> object = Nan::New<v8::Object>();

        const Trade &trade = trades[i];

        Trade::ToObject(trade, object);

        output->Set(static_cast<uint32_t>(i), object);
    }
}

void Trade::ToObject(const Trade &trade, v8::Local<v8::Object> &output) {

    //output->Set(Nan::New<v8::String>("Buy").ToLocalChecked(), Nan::New<v8::Boolean>(trade.Buy));

    /*output->Set(Nan::New<v8::String>("MaximumLoss").ToLocalChecked(), Nan::New<v8::Number>(trade.MaximumLoss));

    output->Set(Nan::New<v8::String>("MaximumProfit").ToLocalChecked(), Nan::New<v8::Number>(trade.MaximumProfit));

    output->Set(Nan::New<v8::String>("ProfitBeforeLoss").ToLocalChecked(), Nan::New<v8::Number>(trade.ProfitBeforeLoss));

*/
    output->Set(Nan::New<v8::String>("revenue").ToLocalChecked(), Nan::New<v8::Number>(trade.revenue));
    output->Set(Nan::New<v8::String>("startPrice").ToLocalChecked(), Nan::New<v8::Number>(trade.StartPrice));
    output->Set(Nan::New<v8::String>("endPrice").ToLocalChecked(), Nan::New<v8::Number>(trade.EndPrice));
    output->Set(Nan::New<v8::String>("startIndex").ToLocalChecked(), Nan::New<v8::Number>(trade.Start));
    output->Set(Nan::New<v8::String>("endIndex").ToLocalChecked(), Nan::New<v8::Number>(trade.End));
}
