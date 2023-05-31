#include "../includes/antAlgorithmUtils.hpp"

namespace s21{

Ant::Ant(int start_node) : start_node_(start_node), best_way_count_(0),
        bad_way_count_(0), end_status_(0){
    current_way_.vertices.push_back(start_node_);
}

int Ant::ChooseNextNode(Ant::elem_of_graph_type& available_nodes,
            std::vector<double>& pheromones){
    int next_node;
    std::pair<std::vector<int>, std::vector<double>> node_and_probability =
        std::move(NodeSelectionProbability_(available_nodes, pheromones));

    next_node = (
        node_and_probability.first[
            std::distance(
                node_and_probability.second.begin(),
                std::max_element(
                    node_and_probability.second.begin(),
                    node_and_probability.second.end()
                )
            )
        ]
    );
    if (next_node == -1){
        best_way_count_ = 0;
        ResetCurrentWay_();
        bad_way_count_++;
        next_node = start_node_;
    } else {
        bad_way_count_ = 0;
        current_way_.vertices.push_back(next_node);
        current_way_.distance += available_nodes[next_node];
        if (next_node == start_node_){
            UpdateBestWay_();
            ResetCurrentWay_();
        }
    }
    UpdateEndStatus_();
    return next_node;
}

TsmResult& Ant::CurrentWay(){
    return current_way_;
}

int Ant::StartNode(){
    return start_node_;
}

int Ant::FromNode(){
    return current_way_.vertices.end()[-2];
}

int Ant::CurrentNode(){
    return current_way_.vertices.back();
}

int Ant::EndCodeStatus(){
    return end_status_;
}

int Ant::BestWayCount(){
    return best_way_count_;
}

int Ant::BadWayCount(){
    return bad_way_count_;
}

std::pair<std::vector<int>, std::vector<double>>&&
    Ant::NodeSelectionProbability_(Ant::elem_of_graph_type& available_nodes,
                                    std::vector<double>& pheromones){
    std::vector<int>& current_way = current_way_.vertices;
    auto is_permitted_node = [&current_way](int node) -> bool{
        auto nodeIt = std::find(current_way.begin(), current_way.end(), node);
        return nodeIt == current_way.end();
    };
    auto node_probability = [](double pheromone, double visible) -> double{
        return (std::pow(pheromone, TSM_ALPHA) * std::pow(visible, TSM_BETA));
    };
    std::pair<std::vector<int>, std::vector<double>> node_and_probability;

    for (size_t i = 0; i < available_nodes.size(); i++){
        int node = available_nodes[i];
        if ((node && is_permitted_node(node)) ||
            (node == start_node_ &&
                current_way.size() == (available_nodes.size() - 1))){
            node_and_probability.first.push_back(node);
            node_and_probability.second.push_back(
                node_probability(pheromones[i], ((double)1 / (double)node))
            );
        }
    }
    if (node_and_probability.first.empty()){
        node_and_probability.first.push_back(-1);
        node_and_probability.second.push_back(-1);
    } else {
        double probability_sum = std::reduce(
            node_and_probability.second.begin(),
            node_and_probability.second.end()
        );

        for (int i = 0; i < node_and_probability.first.size(); i++){
            node_and_probability.second[i] = (
                node_and_probability.second[i] / probability_sum
            );
        }
    }
    return std::move(node_and_probability);
}

void Ant::UpdateBestWay_(){
    if (best_way_.vertices.empty() ||
        current_way_.distance < best_way_.distance){
        best_way_ = current_way_;
        best_way_count_ = 1;
    } else {
        best_way_count_++;
    }
}

void Ant::UpdateEndStatus_(){
    if (best_way_count_ >= TSM_SAME_ROAD_LIMIT){
        end_status_ = 1;
    } else if (bad_way_count_ >= TSM_SAME_ROAD_LIMIT){
        end_status_ = 2;
    }
}

void Ant::ResetCurrentWay_(){
    current_way_.vertices.clear();
    current_way_.vertices.push_back(start_node_);
    current_way_.distance = 0;
}


double AntAlgorithmUtils::AverageDistance(const Graph& graph){
    int edges_count;
    int total_length_count;

    edges_count = 0;
    total_length_count = 0;
    for (Graph::const_iterator_type row = graph.Begin();
            row != graph.End(); ++row){
        for (auto weight : *row){
            if (weight){
                edges_count++;
                total_length_count++;
            }
        }
    }
    return (float)total_length_count / (float)edges_count;
}

std::vector<Ant>&& AntAlgorithmUtils::AntsColony(const Graph& graph){
    std::vector<Ant> ants;

    for(size_t i = 0; i < graph.Size(); i++){
        ants.push_back(Ant(i));
    }
    return std::move(ants);
}

TsmResult&& AntAlgorithmUtils::UpdateReturnedWay(TsmResult& new_way,
                                TsmResult& best_way){
    if (best_way.vertices.empty() ||
        new_way.distance < best_way.distance){
        return std::move(new_way);
    } else {
        return std::move(best_way);
    }
}

void AntAlgorithmUtils::RefreshPheromones(int from_node, int to_node,
        const Graph &graph, std::vector<std::vector<double>>& pheromones){
    double q_parameter;

    q_parameter = AverageDistance(graph);
    pheromones[from_node][to_node] +=
        q_parameter / (double)(graph[from_node][to_node]);
}

void AntAlgorithmUtils::PheromoneEvaporation(
        std::vector<std::vector<double>>& pheromones){
    for (auto& string_pheromones : pheromones){
        for (auto& node_pheromone : string_pheromones){
            node_pheromone = ((double)1 - TSM_PYRA) * node_pheromone;
        }
    }
}

}
