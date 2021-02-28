function res = findNode(node, point)
total = numel(node) / 2;
i = 1;
distance = 99999999999999;
while i <= total
    %tmp = node(i,:) - point;
    tmpDistance = ((node(i,1) - point(1,1))^2 + ((node(i,2) - point(1,2))^2 ))^0.5;
    if tmpDistance <= distance
        distance = tmpDistance;
        nodeNum = i;
    end
    i = i + 1;
end
res = nodeNum;