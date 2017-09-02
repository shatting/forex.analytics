#include "nan.h"

#include "../include/BinaryTreeChromosome.h"
#include "../include/nodes/OperatorTreeNode.h"
#include "../include/nodes/IndicatorTreeNode.h"

BinaryTreeChromosome::BinaryTreeChromosome() {

    this->fitness = 0.0;
}

void BinaryTreeChromosome::copyTo(BinaryTreeChromosome *chromosome) const {
    this->buy->CopyTo(chromosome->buy);
    this->sell->CopyTo(chromosome->sell);
}

void BinaryTreeChromosome::Mutate(double leafValueMutationProbability, double leafValueSignMutationProbability, double logicalNodeMutationProbability, double crossoverProbability, double leafIndicatorMutationProbability) {

    this->Mutate(leafValueMutationProbability, leafValueSignMutationProbability, logicalNodeMutationProbability, crossoverProbability, leafIndicatorMutationProbability, this->buy);

    this->Mutate(leafValueMutationProbability, leafValueSignMutationProbability, logicalNodeMutationProbability, crossoverProbability, leafIndicatorMutationProbability, this->sell);
}

void BinaryTreeChromosome::Mutate(double leafValueMutationProbability, double leafValueSignMutationProbability, double logicalNodeMutationProbability, double crossoverProbability, double leafIndicatorMutationProbability, TreeNode *node) {
    if(node->left != nullptr && node->right != nullptr) {

        OperatorTreeNode *opNode = static_cast<OperatorTreeNode *>(node);

        double logicalNodeMutation = static_cast<double>(rand() % 100) / 100;

        if(logicalNodeMutation <= logicalNodeMutationProbability) {
            opNode->GenerateRandomValue();
        }

        this->Mutate(leafValueMutationProbability, leafValueSignMutationProbability, logicalNodeMutationProbability, crossoverProbability, leafIndicatorMutationProbability, opNode->left);

        this->Mutate(leafValueMutationProbability, leafValueSignMutationProbability, logicalNodeMutationProbability, crossoverProbability, leafIndicatorMutationProbability, opNode->right);
    } else {
        IndicatorTreeNode *inNode = static_cast<IndicatorTreeNode *>(node);

        double leafValueMutation = static_cast<double>(rand() % 100) / 100;
        double leafValueSignMutation = static_cast<double>(rand() % 100) / 100;
        double leafValueIndicatorMutation = static_cast<double>(rand() % 100) / 100;

        if(leafValueMutation <= leafValueMutationProbability) {
            inNode->GenerateRandomValue();
        }

        if(leafValueIndicatorMutation <= leafIndicatorMutationProbability) {
            inNode->GenerateRandomIndicator();
        }

        if(leafValueSignMutation <= leafValueSignMutationProbability) {
            inNode->GenerateRandomSign();
        }

    }
} // BinaryTreeGeneticAlgo::Mutate

BinaryTreeChromosome::BinaryTreeChromosome(const BinaryTreeChromosome *chromosome) {

    this->buy = chromosome->buy->Copy();
    this->sell = chromosome->sell->Copy();

    this->fitness = chromosome->fitness;
}

BinaryTreeChromosome::~BinaryTreeChromosome() {
    delete this->buy;
    delete this->sell;
}

void BinaryTreeChromosome::setFitness(double value) {
    this->fitness = value;
}

double BinaryTreeChromosome::getFitness() const {
    return this->fitness;
}

void BinaryTreeChromosome::createRandom(int height,
                                        const std::vector<std::string> &indicatorNames,
                                        const std::unordered_map<std::string, double> &indicatorMin,
                                        const std::unordered_map<std::string, double> &indicatorMax) {
    this->indicatorNames = &indicatorNames;
    this->indicatorMin = &indicatorMin;
    this->indicatorMax = &indicatorMax;

    this->buy = GenerateTree(0, height);
    this->sell = GenerateTree(0, height);
}

TreeNode *BinaryTreeChromosome::GenerateTree(int index, int height) const {

    TreeNode *ret;

    //printf("createRandom %i/%i\n", index, height);
    if(index < height - 1) {
        ret = new OperatorTreeNode();
        ret->GenerateRandomValue();
        ret->left = GenerateTree(index + 1, height);
        ret->right = GenerateTree(index + 1, height);
    } else {
        ret = new IndicatorTreeNode(*indicatorNames, *indicatorMin, *indicatorMax);

        ret->GenerateRandomValue();
    }

    return ret;
}

bool BinaryTreeChromosome::operator<(const BinaryTreeChromosome *chromosome) const {
    return this->fitness < chromosome->fitness;
}

bool BinaryTreeChromosome::operator>(const BinaryTreeChromosome *chromosome) const {
    return this->fitness > chromosome->fitness;
}

bool BinaryTreeChromosome::shouldBuy(const std::unordered_map<std::string, double> &data) const {
    return this->buy->Evaluate(data);
}

bool BinaryTreeChromosome::shouldSell(const std::unordered_map<std::string, double> &data) const {
    return this->sell->Evaluate(data);
}

void BinaryTreeChromosome::ToJs(v8::Handle<v8::Object> &input) const {
    v8::Local<v8::Object> buy = Nan::New<v8::Object>();
    v8::Local<v8::Object> sell = Nan::New<v8::Object>();

    this->buy->ToJs(buy);
    this->sell->ToJs(sell);

    Nan::Set(input, Nan::New<v8::String>("buy").ToLocalChecked(), buy);
    Nan::Set(input, Nan::New<v8::String>("sell").ToLocalChecked(), sell);
}

BinaryTreeChromosome *BinaryTreeChromosome::FromJs(const std::vector<std::string> &indicatorNames,
                                                   const std::unordered_map<std::string, double> &indicatorMin,
                                                   const std::unordered_map<std::string, double> &indicatorMax,
                                                   const v8::Local<v8::Object> &strategy)
{

    v8::Handle<v8::Object> buy = v8::Handle<v8::Object>::Cast(Nan::Get(strategy, Nan::New<v8::String>("buy").ToLocalChecked()).ToLocalChecked());

    v8::Handle<v8::Object> sell = v8::Handle<v8::Object>::Cast(Nan::Get(strategy, Nan::New<v8::String>("sell").ToLocalChecked()).ToLocalChecked());

    BinaryTreeChromosome *chromosome = new BinaryTreeChromosome();
    chromosome->indicatorNames = &indicatorNames;
    chromosome->indicatorMin = &indicatorMin;
    chromosome->indicatorMax = &indicatorMax;

    chromosome->buy = TreeNode::FromJs(indicatorNames, indicatorMin, indicatorMax, buy);
    chromosome->sell = TreeNode::FromJs(indicatorNames, indicatorMin, indicatorMax, sell);

    return chromosome;
}
