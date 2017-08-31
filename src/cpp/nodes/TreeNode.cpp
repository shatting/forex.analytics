#include "nan.h"

#include "../../include/nodes/TreeNode.h"
#include "../../include/nodes/OperatorTreeNode.h"
#include "../../include/nodes/IndicatorTreeNode.h"

std::random_device TreeNode::r;

TreeNode::TreeNode() {
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    this->engine = std::mt19937(seed);

    this->left = nullptr;
    this->right = nullptr;
}

TreeNode::~TreeNode() {
    if(this->left != nullptr)
        delete this->left;

    if(this->right != nullptr)
        delete this->right;
}

std::string TreeNode::toString() const {
    std::string result = "";
    if(left != nullptr)
        result += "left: " + left->toString();

    if(right != nullptr)
        result += "right: " + right->toString();

    return result;
}

void TreeNode::ToJs(v8::Local<v8::Object> &input) const {

    if(this->left != nullptr) {
        v8::Local<v8::Object> left = Nan::New<v8::Object>();

        this->left->ToJs(left);

        input->Set(Nan::New<v8::String>("left").ToLocalChecked(), left);
    }

    if(this->right != nullptr) {
        v8::Local<v8::Object> right = Nan::New<v8::Object>();

        this->right->ToJs(right);

        input->Set(Nan::New<v8::String>("right").ToLocalChecked(), right);
    }
}

TreeNode *TreeNode::FromJs(const std::vector<std::string> &indicatorNames,
                           const std::unordered_map<std::string, double> &indicatorMin,
                           const std::unordered_map<std::string, double> &indicatorMax,
                           const v8::Local<v8::Object> &input) {

    //try to parse operator tree node first
    TreeNode *node = OperatorTreeNode::FromJs(input);

    if(node == nullptr) {
        node = IndicatorTreeNode::FromJs(indicatorNames, indicatorMin, indicatorMax, input);
    }

    if(input->Has(Nan::New<v8::String>("left").ToLocalChecked())) {

        v8::Handle<v8::Object> left = v8::Handle<v8::Object>::Cast(input->Get(Nan::New<v8::String>("left").ToLocalChecked()));

        node->left = TreeNode::FromJs(indicatorNames, indicatorMin, indicatorMax, left);
    }

    if(input->Has(Nan::New<v8::String>("right").ToLocalChecked())) {

        v8::Handle<v8::Object> right = v8::Handle<v8::Object>::Cast(input->Get(Nan::New<v8::String>("right").ToLocalChecked()));

        node->right = TreeNode::FromJs(indicatorNames, indicatorMin, indicatorMax, right);
    }

    return node;
}
