//
// Created by sokol on 23.01.20.
//

#include "path_finder/storage/HubLabelStore.h"
#include <cstring>
#include <memory>
#include <path_finder/helper/Static.h>
#include <sstream>
#include <utility>

namespace pathFinder {
HubLabelStore::HubLabelStore(size_t numberOfLabels, bool mmap, std::string folder) {
  forwardOffset = (OffsetElement*) std::calloc( numberOfLabels , sizeof(OffsetElement));
  backwardOffset = (OffsetElement*) std::calloc( numberOfLabels , sizeof(OffsetElement));
  backwardLabels = nullptr;
  forwardLabels = nullptr;
  this->numberOfLabels = numberOfLabels;
  _mmap = mmap;
  _folder = std::move(folder);
  if(mmap) {
    std::string command = "mkdir " + _folder;
    system(command.c_str());
    command = "touch " + _folder + "/forwardHubLabels";
    system(command.c_str());
    command = "touch " + _folder + "/backwardHubLabels";
    system(command.c_str());
    command = "touch " + _folder + "/forwardHubLabelOffset";
    system(command.c_str());
    command = "touch " + _folder + "/backwardHubLabelOffset";
    system(command.c_str());
  }
}

void HubLabelStore::store(
    const std::vector<CostNode> &label, NodeId id, EdgeDirection direction) {
  if(!_mmap) {
    if (direction == EdgeDirection::FORWARD) {
      forwardOffset[id] =
          OffsetElement{forwardLabelSize, (uint32_t)label.size()};
      forwardLabels = (CostNode*) std::realloc(forwardLabels, sizeof(CostNode) * (forwardLabelSize + label.size()));
      //std::memcpy(forwardLabels + forwardLabelSize, label.data(), sizeof(CostNode) * label.size());
      for(const auto& entry : label)
        forwardLabels[forwardLabelSize++] = entry;
    } else {
      backwardOffset[id] =
          OffsetElement{backwardLabelSize, (uint32_t)label.size()};
      backwardLabels = (CostNode*) std::realloc(backwardLabels, sizeof(CostNode) * (backwardLabelSize + label.size()));
      //std::memcpy(backwardLabels + backwardLabelSize, label.data(), sizeof(CostNode) * label.size());
      for(const auto& entry : label)
        backwardLabels[backwardLabelSize++] = entry;
    }
  }
}

void HubLabelStore::retrieve(NodeId id, EdgeDirection direction, std::vector<CostNode>& storeVec) {
  if(id > numberOfLabels - 1)
    throw std::runtime_error("[HublabelStore] node id does not exist.");
  if (direction == EdgeDirection::FORWARD) {
    auto offsetElement = forwardOffset[id];
    storeVec.reserve(offsetElement.size);
    auto labelEnd = offsetElement.position + offsetElement.size;
    for(auto i = offsetElement.position; i < labelEnd; ++i) {
      CostNode costNode = forwardLabels[i];
      storeVec.emplace_back(costNode);
    }
  } else if(direction == EdgeDirection::BACKWARD) {
    auto offsetElement = backwardOffset[id];
    storeVec.reserve(offsetElement.size);
    auto labelEnd = offsetElement.position + offsetElement.size;
    for(auto i = offsetElement.position; i < labelEnd; ++i) {
      CostNode costNode = backwardLabels[i];
      storeVec.emplace_back(costNode);
    }
  }
}

size_t HubLabelStore::getNumberOfLabels() const  {
  return numberOfLabels;
}


HubLabelStore::HubLabelStore(HubLabelStoreInfo hubLabelStoreInfo) {
  this->forwardLabels = hubLabelStoreInfo.forwardLabels;
  this->backwardLabels = hubLabelStoreInfo.backwardLabels;
  this->forwardOffset = hubLabelStoreInfo.forwardOffset;
  this->backwardOffset = hubLabelStoreInfo.backwardOffset;
  this->numberOfLabels = hubLabelStoreInfo.numberOfLabels;
}


CostNode* HubLabelStore::getBackwardLabels() {
  return backwardLabels;
}

CostNode* HubLabelStore::getBackwardLabels() const{
  return backwardLabels;
}


CostNode* HubLabelStore::getForwardLabels() {
  return forwardLabels;
}

CostNode* HubLabelStore::getForwardLabels() const{
  return forwardLabels;
}


OffsetElement* HubLabelStore::getForwardOffset() {
  return forwardOffset;
}

OffsetElement* HubLabelStore::getForwardOffset() const{
  return forwardOffset;
}


OffsetElement* HubLabelStore::getBackwardOffset() {
  return backwardOffset;
}

OffsetElement* HubLabelStore::getBackwardOffset() const{
  return backwardOffset;
}


size_t HubLabelStore::getSpaceConsumption() {
  return forwardLabelSize * sizeof(CostNode) +
         backwardLabelSize * sizeof(CostNode);
}
HubLabelStore::~HubLabelStore() {
  /*std::cout << "[HubLabelStore] destruct" << '\n';
  free(forwardLabels);
  free(backwardLabels);
  free(forwardOffset);
  free(backwardOffset);
   */
}
size_t HubLabelStore::getForwardLabelsSize() const { return forwardLabelSize; }
size_t HubLabelStore::getBackwardLabelsSize() const { return backwardLabelSize; }
size_t HubLabelStore::getForwardOffsetSize() const { return numberOfLabels; }
size_t HubLabelStore::getBackwardOffsetSize() const { return numberOfLabels; }
std::string HubLabelStore::printStore() {
  std::stringstream ss;

  for(NodeId i = 0; i < numberOfLabels; ++i) {
    std::vector<CostNode> forwardVec;
    std::vector<CostNode> backwardVec;
    retrieve(i, EdgeDirection::FORWARD, forwardVec);
    retrieve(i, EdgeDirection::BACKWARD, backwardVec);
    ss << "label for id: " << i << '\n';
    ss << "forward:" << '\n';
    for(const auto& [id, cost , previousNode]: forwardVec) {
      ss << id << ' ' << cost << ' ' << previousNode << '\n';
    }
    ss << "backward:" << '\n';
    for(const auto& [id, cost , previousNode]: backwardVec) {
      ss << id << ' ' << cost << ' ' << previousNode << '\n';
    }
  }

  return ss.str();
}
std::shared_ptr<HubLabelStore> HubLabelStore::makeShared(size_t numberOfLabels) {
  return std::make_shared<HubLabelStore>(numberOfLabels);
}
}