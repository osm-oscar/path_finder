#include <fstream>
#include <path_finder/storage/FileWriter.h>
namespace pathFinder{
void FileWriter::writeGraph(const pathFinder::RamGraph &graph, const std::string& graphName, const std::string& folder) {
  std::string command = "mkdir " + folder;
  system(command.c_str());
  pathFinder::GraphDataInfo dataConfig;
  dataConfig.graphName = graphName;
  dataConfig.timestamp = Static::getTimeStampString();
  dataConfig.nodes = {"nodes", graph.getNodes().size(), true};
  dataConfig.forwardEdges = {"forwardEdges", graph.getForwardEdges().size(), true};
  dataConfig.forwardOffset = {"forwardOffset", graph.getForwardOffset().size(), false};
  dataConfig.backwardEdges = {"backwardEdges", graph.getBackEdges().size(), true};
  dataConfig.backwardOffset = {"backwardOffset", graph.getBackOffset().size(), false};
  Static::writeVectorToFile(graph.getNodes(), (folder + dataConfig.nodes.path).c_str());
  Static::writeVectorToFile(graph.getForwardEdges(), (folder + dataConfig.forwardEdges.path).c_str());
  Static::writeVectorToFile(graph.getForwardOffset(), (folder + dataConfig.forwardOffset.path).c_str());
  Static::writeVectorToFile(graph.getBackEdges(), (folder + dataConfig.backwardEdges.path).c_str());
  Static::writeVectorToFile(graph.getBackOffset(), (folder + dataConfig.backwardOffset.path).c_str());
  //write config to file
  std::ofstream out(folder + "/config.json");
  nlohmann::json j;
  to_json(j, dataConfig);
  std::string jsonString = j.dump();
  out << j.dump();
  out.close();
}
void FileWriter::writeHubLabels(const RamHubLabelStore &hubLabelStore, const std::string& graphName, const std::string& folder) {
  std::string command = "mkdir " + folder;
  system(command.c_str());
  pathFinder::HubLabelDataInfo dataConfig;
  dataConfig.graphName = graphName;
  dataConfig.timestamp = Static::getTimeStampString();
  dataConfig.forwardHublabels = {"forwardHubLabels", hubLabelStore.getForwardLabelsSize(), true};
  dataConfig.backwardHublabels = { "backwardHubLabels", hubLabelStore.getBackwardLabelsSize(), true};
  dataConfig.forwardHublabelOffset = { "forwardHubLabelOffset", hubLabelStore.getForwardOffsetSize(), false};
  dataConfig.backwardHublabelOffset = {"backwardHubLabelOffset", hubLabelStore.getBackwardOffsetSize(), false};
  dataConfig.maxLevel = hubLabelStore.maxLevel;
  dataConfig.calculatedUntilLevel = hubLabelStore.calculatedUntilLevel;
  // write hub label files
  Static::writeVectorToFile(hubLabelStore.getForwardLabels(), hubLabelStore.getForwardLabelsSize(),
                            (folder + dataConfig.forwardHublabels.path).c_str());
  Static::writeVectorToFile(hubLabelStore.getBackwardLabels(), hubLabelStore.getBackwardLabelsSize(),
                            (folder + dataConfig.backwardHublabels.path).c_str());
  Static::writeVectorToFile(hubLabelStore.getForwardOffset(), hubLabelStore.getForwardOffsetSize(),
                            (folder + dataConfig.forwardHublabelOffset.path).c_str());
  Static::writeVectorToFile(hubLabelStore.getBackwardOffset(), hubLabelStore.getBackwardOffsetSize(),
                            (folder + dataConfig.backwardHublabelOffset.path).c_str());

  //write config to file
  std::ofstream out(folder + "/config.json");
  nlohmann::json j;
  to_json(j, dataConfig);
  std::string jsonString = j.dump();
  out << j.dump();
  out.close();
}
void FileWriter::writeCells(const RamCellIdStore &cellIdStore, const std::string &graphName,
                            const std::string &folder) {
  std::string command = "mkdir " + folder;
  system(command.c_str());
  pathFinder::CellDataInfo dataConfig;
  dataConfig.graphName = graphName;
  dataConfig.timestamp = Static::getTimeStampString();
  dataConfig.cellIds = {"cellIds", cellIdStore.cellIdSize()};
  dataConfig.cellIdsOffset = {"cellIdsOffset", cellIdStore.offsetSize()};
  Static::writeVectorToFile(cellIdStore.cellIdsVec(), cellIdStore.cellIdSize(),(folder + dataConfig.cellIds.path).c_str());
  Static::writeVectorToFile(cellIdStore.offsetVec(), cellIdStore.offsetSize(), (folder + dataConfig.cellIdsOffset.path).c_str());
  //write config to file
  std::ofstream out(folder + "/config.json");
  nlohmann::json j;
  to_json(j, dataConfig);
  std::string jsonString = j.dump();
  out << j.dump();
  out.close();
}
void FileWriter::writeAll(RamGraph* graph, RamHubLabelStore* hubLabelStore,
                          RamCellIdStore* cellIdStore, const std::string &folder) {
  std::string command = "mkdir " + folder;
  system(command.c_str());
  pathFinder::HybridPfDataInfo dataInfo;
  if(graph != nullptr) {
    dataInfo.graphFolder = "graph/";
    writeGraph(*graph, "stgt", folder + '/' + dataInfo.graphFolder);
  }

  if(hubLabelStore != nullptr) {
    dataInfo.hubLabelFolder = "hubLabels/";
    writeHubLabels(*hubLabelStore, "stgt", folder + '/' + dataInfo.hubLabelFolder);
    dataInfo.hubLabelsCalculated = true;
    dataInfo.labelsUntilLevel = hubLabelStore->calculatedUntilLevel;
  }

  if(cellIdStore != nullptr){
    dataInfo.cellIdFolder = "cellIds/";
    writeCells(*cellIdStore, "stgt", folder + '/' + dataInfo.cellIdFolder);
    dataInfo.cellIdsCalculated = true;
  }
  //write config to file
  std::ofstream out(folder + "/config.json");
  nlohmann::json j;
  to_json(j, dataInfo);
  std::string jsonString = j.dump();
  out << j.dump();
  out.close();
}
}

