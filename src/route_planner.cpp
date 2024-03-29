#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &m_Model.FindClosestNode(start_x , start_y);
    end_node = &m_Model.FindClosestNode(end_x , end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
  return node->distance(*end_node);
}

// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
current_node->FindNeighbors();
for(RouteModel::Node *node : current_node->neighbors){
    node->parent = current_node;
    node->g_value = current_node->g_value + current_node->distance(*node));
    node->h_value = CalculateHValue(node);
    open_list.push_back(node);
    node->visited = true ;
}
}

// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.
bool compare(const RouteModel::Node *a , const RouteModel::Node *b){
    float sum1 = a->g_value + a->h_value;
    float sum2 = b->g_value + b->h_value;
    return sum1 > sum2 ;
}
RouteModel::Node *RoutePlanner::NextNode() {
 std::sort(open_list.begin() , open_list.end() , compare);
 RouteModel::Node *lowest_sum ;
 lowest_sum = open_list.back();
 open_list.pop_back();
 return lowest_sum ;
}

// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
     while(current_node->parent != nullptr){
         path_found.push_back(*current_node);
         distance+=current_node->distance(*(current_node->parent));
         current_node = current_node->parent;
     }
     path_found.push_back(*start_node);
     std::reverse(path_found.begin() , path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
    start_node->visited = true ;
    open_list.push_back(start_node);

    while (open_list.size() > 0)
    {
         current_node = NextNode();
        if(current_node->distance(*end_node) == 0){
            m_Model.path = ConstructFinalPath(current_node);
            break ;
        }
        AddNeighbors(current_node);
       
    }
    m_Model.path = ConstructFinalPath(current_node);
}
