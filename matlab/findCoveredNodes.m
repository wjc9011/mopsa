% find cover nodes for calculation
function res = findCoveredNodes(dP, position, sorted_node, node)
radius = dP / 2;
total = numel(sorted_node) / 3;
current_covered_nodes = 0;
nodeSet = [];
for i = 1: total
    
    if abs(position(1) - sorted_node(i,1)) < radius
        distance = ((position(1, 1) - sorted_node(i, 1)) ^2 + ...
            (position(1, 2) - sorted_node(i, 2)) ^2) ^ 0.5;
        if distance <= radius
            nodeSet = [nodeSet; sorted_node(i, 3)];
            current_covered_nodes = current_covered_nodes + 1;
        end
    end
end

if isempty(nodeSet)
    nodeSet = findNode(node, position);
end

res = nodeSet;
