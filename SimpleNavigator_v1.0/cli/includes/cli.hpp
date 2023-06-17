#pragma once

#include <iostream>
#include <stdexcept>

#include "timer.hpp"
#include "../../graph/includes/graph.hpp"
#include "../../graph/includes/graphAlgorithms.hpp"


namespace s21{

class CLI{
public:
    CLI() = default;
    CLI(const CLI&) = delete;
    CLI(CLI&&) = delete;
    ~CLI() = default;

    CLI& operator=(const CLI&) = delete;
    CLI& operator=(CLI&&) = delete;

    /**
     * Main method to start CLI.
    */
    void run();

private:
    typedef void (CLI::*FuncPtr)(void);

    const static std::string GREEN_COLOR_;
    const static std::string CYAN_COLOR_;
    const static std::string BLUE_COLOR_;
    const static std::string DEFAULT_COLOR_;
    const static std::string SN_PROMT_COLOR_;
    const static std::string USER_PROMT_COLOR_;
    const static std::string MSG_COLOR_;
    const static std::string IDX_COLOR_;
    const static std::string SN_PROMT_;
    const static std::string USER_PROMT_;
    const static std::string SEPARATOR_;
    const static int MENU_OPTIONS_COUNT_ = 8;
    const static int EXIT_INPUT_VALUE_ = -1;
    const static int CLS_INPUT_VALUE_ = -2;
    const static int BAD_INPUT_VALUE_ = -100;
    std::unique_ptr<Graph> graph_;
    GraphAlgorithms graph_algorithms_;
    const std::string menu_msg_[MENU_OPTIONS_COUNT_] = {
        "graph traversal in breadth",
        "graph traversal in depth",
        "searching for the shortest path between any two vertices",
        "searching for the shortest paths between all pairs of vertices",
        "searching for the minimal spanning tree",
        "solving the salesman problem (ant algorithm)",
        "solving the salesman problem (branch and bound method)",
        "solving the salesman problem (exhaustive search)"
    };
    const FuncPtr methods_[MENU_OPTIONS_COUNT_] = {
        &CLI::BreadthFirstSearch_,
        &CLI::DepthFirstSearch_,
        &CLI::GetShortestPathBetweenVertices_,
        &CLI::GetShortestPathsBetweenAllVertices_,
        &CLI::GetLeastSpanningTree_,
        &CLI::SolveTravelingSalesmanProblem_,
        &CLI::STSPBranchBoundMethodAlgorithm_,
        &CLI::ExhaustiveSearch_
    };

    void PrintMenu_() const;
    void PrintMsg_(const std::string& msg) const;
    int ReadInput_() const;

    /**
     * Load graph from given file.
     * Return true on success, false otherwise.
    */
    bool LoadGraphFromFile_();

    void BreadthFirstSearch_();
    void DepthFirstSearch_();
    void GetShortestPathBetweenVertices_();
    void GetShortestPathsBetweenAllVertices_();
    void GetLeastSpanningTree_();
    void SolveTravelingSalesmanProblem_();

    void STSPBranchBoundMethodAlgorithm_();
    void ExhaustiveSearch_();

    void TrimLine_(std::string& line) const;

};

}
