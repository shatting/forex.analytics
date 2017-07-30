#include "../include/BinaryTreeGeneticAlgo.h"

BinaryTreeGeneticAlgo::BinaryTreeGeneticAlgo(
	int selectNumber,
	double leafValueMutation,
	double leafValueSignMutation,
	double logicalNodeMutation,
	double leafValueIndicatorMutation,
	double crossover) {
	this->select = selectNumber;
	this->leafValueMutationProbability = leafValueMutation;
	this->leafValueSignMutationProbability = leafValueSignMutation;
	this->logicalNodeMutationProbability = logicalNodeMutation;
	this->crossoverProbability = crossover;
	this->leafIndicatorMutationProbability = leafValueIndicatorMutation;
}

BinaryTreeGeneticAlgo::~BinaryTreeGeneticAlgo() {
}

void BinaryTreeGeneticAlgo::Select(
	std::vector<BinaryTreeChromosome*>* newGeneration,
	std::vector<BinaryTreeChromosome*>* oldGeneration,
	unsigned size) {

	std::default_random_engine generator;
	
	std::vector<BinaryTreeChromosome*> listChromos;
	
	for (int i = size - 1; i >= 0; i--) {
		double comp = static_cast<double>(rand() % 100) / 100;
		if (comp < .8) {
			listChromos.push_back(oldGeneration->at(i));
		}
		if (listChromos.size() >= this->select)
			break;
	}

	int count = 0;
	if (listChromos.size() < this->select) {
		count = listChromos.size();
	}
	else {
		count = this->select;
	}
	std::uniform_int_distribution<unsigned> distribution(0, count);

	for (int i = 0; i < count; i++) {
		newGeneration->at(i)->setFitness(listChromos.at(i)->getFitness());
		listChromos.at(i)->copyTo(newGeneration->at(i));
	}

	for (unsigned i = count; i < size; i++) {
		unsigned index = distribution(generator);
		this->Mutate(newGeneration, index, newGeneration->at(i));
	}

	this->Crossover(newGeneration);
}

void BinaryTreeGeneticAlgo::Mutate(
	std::vector<BinaryTreeChromosome*>* generation,
	unsigned index,
	BinaryTreeChromosome * outputChromosome) {

	generation->at(index)->copyTo(outputChromosome);

	outputChromosome->Mutate(leafValueMutationProbability,
		leafValueSignMutationProbability,
		logicalNodeMutationProbability,
		crossoverProbability,
		leafIndicatorMutationProbability);
}

void BinaryTreeGeneticAlgo::Crossover(std::vector<BinaryTreeChromosome*>* generation) {
	for (unsigned long i = this->select + 1; i < generation->size(); i++) {
		double crossover = static_cast<double>(rand() % 100) / 100;

		if (crossover <= this->crossoverProbability) {
			unsigned long rnd = i;

			while (rnd == i || rnd <= this->select)
				rnd = rand() % generation->size();

			this->Crossover(generation->at(i), generation->at(rnd));
		}
	}
}


void BinaryTreeGeneticAlgo::Crossover(
	BinaryTreeChromosome * left,
	BinaryTreeChromosome * right) {
	TreeNode * leftBuy = left->buy;

	left->buy = right->buy;
	right->buy = leftBuy;
}
